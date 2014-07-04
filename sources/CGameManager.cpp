/* 
 * File:   CGameManager.cpp
 * Author: hokan
 * 
 * Created on 1 Июль 2014 г., 13:28
 */

#include "headers/CGameManager.h"
#include "headers/CMoveDownAnimator.h"
#include "headers/CGridBox.h"
#include "headers/Manipulators.h"

#include <iostream>

CGameManager::CGameManager(irr::gui::IGUIEnvironment* guiEnvironment, CGameFieldManager* gameFieldManager, CGameFiguresFactory* gameFiruresFactory, CUserInputEventReceiver* userInputReceiver, bool twoDimensionalMode, irr::core::vector3df fieldOrigin, irr::core::aabbox3df fieldAABB)
: gameFieldManager(gameFieldManager)
, gameFiruresFactory(gameFiruresFactory)
, userInputReceiver(userInputReceiver)
, gameScore(0) {
    speed = 1000;
    figureSize = 10.0f;
    setTwoDimensionalMode(twoDimensionalMode);
    this->fieldOrigin = fieldOrigin;
    aabb = fieldAABB;

    CGridBox gridBox(gameFiruresFactory->getFiguresParentNode(), gameFiruresFactory->getSceneManager());
    gridBox.setGridsSize(irr::core::dimension2df(figureSize * figureSize * gameFieldManager->getFieldSize().X, figureSize * figureSize * gameFieldManager->getFieldSize().Y));
    gridBox.setSpacing(figureSize * figureSize);
    gridBox.setColour(irr::video::SColor(255, 100, 255, 100));
    irr::core::vector3df gridBoxPosition(fieldOrigin);
    gridBoxPosition.X += gridBox.getGridsSize().Height / 2 - gridBox.getSpacing() / 2;
    gridBoxPosition.Z += gridBox.getGridsSize().Width / 2 - gridBox.getSpacing() / 2;
    gridBox.setPosition(gridBoxPosition);
    this->gridBoxPosition = gridBoxPosition;
    gameOver = false;
    animatorsStopped = false;

    irr::io::IFileSystem* fileSystem = gameFiruresFactory->getFileSystem();
    irr::io::path defaultDirectory = fileSystem->getWorkingDirectory();
    irr::io::IFileList* fileList = fileSystem->createFileList();
    bool directoryFound = false;
    for (irr::u32 i = 0; i < fileList->getFileCount(); ++i) {
        if (fileList->getFileName(i).equals_ignore_case("misc") && fileList->isDirectory(i)) {
            fileSystem->changeWorkingDirectoryTo(fileList->getFullFileName(i));
            directoryFound = true;
            break;
        }
    }

    if (!directoryFound) {
        fileList->drop();
    } else {
        this->guiEnvironment = guiEnvironment;
        irr::gui::IGUIFont* font = guiEnvironment->getFont("battlefield.bmp");
        fileSystem->changeWorkingDirectoryTo(defaultDirectory);
        guiEnvironment->getSkin()->setFont(font);
        text = guiEnvironment->addStaticText(L"Your scrore is 0",
                irr::core::rect<irr::s32>(10, 10, 600, 120), false, true, 0, -1, false);
        text->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
    }
}

irr::f32 CGameManager::getGameSpeed() const {
    return speed;
}

void CGameManager::setGameSpeed(irr::f32 newSpeed) {
    speed = newSpeed;
    for (irr::u32 i = 0; i < figures.size(); ++i) {
        irr::scene::ISceneNodeAnimatorList animators = figures[i]->getAnimators();
        for (irr::scene::ISceneNodeAnimatorList::Iterator it = animators.begin(); it != animators.end(); ++it) {
            CMoveDownAnimator* moveDownAnimator = dynamic_cast<CMoveDownAnimator*> (*it);
            if (moveDownAnimator == NULL) {
                continue;
            } else {
                moveDownAnimator->setDelay(speed);
            }
        }
    }
}

irr::f32 CGameManager::getFigureSize() const {
    return figureSize;
}

void CGameManager::setFigureSize(irr::f32 newFigureSize) {
    figureSize = newFigureSize;
}

irr::core::vector3di CGameManager::getSpawnPoint() const {
    return spawnPoint;
}

void CGameManager::setSpawnPoint(irr::core::vector3di newSpawnPoint) {
    spawnPoint = newSpawnPoint;
}

irr::u32 CGameManager::getGameScore() const {
    return gameScore;
}

void CGameManager::setGameScore(irr::u32 newGameScore) {
    gameScore = newGameScore;
}

irr::u32 CGameManager::calculateGameScore(irr::u32 numberOfDeletedLines) {
    // 600 - is default speed (1000) + speed increase (100)
    gameScore += (numberOfDeletedLines * 300 - 200) * ((1100 - speed) / 100);
    if ((gameScore - 10000 * ((1100 - speed) / 100)) > 0) {
        setGameSpeed(getGameSpeed() - 100);
    }
    return gameScore;
}

bool CGameManager::getTwoDimensionalMode() const {
    return twoDimensionalMode;
}

bool CGameManager::isPause() const {
    return userInputReceiver->isPause();
}

void CGameManager::setPauseValue(bool newPauseValue) {
    userInputReceiver->setPause(newPauseValue);
}

bool CGameManager::isGameOver() const {
    return gameOver;
}

void CGameManager::setGameOverValue(bool newGameOverValue) {
    gameOver = newGameOverValue;
}

// TODO optimize via delay

bool CGameManager::runOneLoop() {
    if (gameOver) {
        return false;
    }

    if (userInputReceiver->isPause()) {
        // stop all animators
        if (!animatorsStopped) {
            for (irr::u32 i = 0; i < figures.size(); ++i) {
                irr::scene::ISceneNodeAnimatorList animators = figures[i]->getAnimators();
                for (irr::scene::ISceneNodeAnimatorList::Iterator it = animators.begin(); it != animators.end(); ++it) {
                    CMoveDownAnimator* moveDownAnimator = dynamic_cast<CMoveDownAnimator*> (*it);
                    if (moveDownAnimator == NULL) {
                        continue;
                    } else {
                        moveDownAnimator->setStoppedValue(true);
                    }
                }
            }
            animatorsStopped = true;
        }

        text->remove();
        irr::core::stringw scoreString(L"Pause");
        text = guiEnvironment->addStaticText(scoreString.c_str(),
                irr::core::rect<irr::s32>(350, 250, 800, 620), false, true, 0, -1, false);
        text->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

        return true;
    } else {
        // run all animators
        if (animatorsStopped) {
            for (irr::u32 i = 0; i < figures.size(); ++i) {
                irr::scene::ISceneNodeAnimatorList animators = figures[i]->getAnimators();
                for (irr::scene::ISceneNodeAnimatorList::Iterator it = animators.begin(); it != animators.end(); ++it) {
                    CMoveDownAnimator* moveDownAnimator = dynamic_cast<CMoveDownAnimator*> (*it);
                    if (moveDownAnimator == NULL) {
                        continue;
                    } else {
                        moveDownAnimator->setStoppedValue(false);
                    }
                }
            }
            animatorsStopped = false;
        }

        text->remove();
        irr::core::stringw scoreString(L"Your scrore is ");
        scoreString += gameScore;
        text = guiEnvironment->addStaticText(scoreString.c_str(),
                irr::core::rect<irr::s32>(10, 10, 600, 120), false, true, 0, -1, false);
        text->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
    }

    // check if all animators stopped
    for (irr::u32 i = 0; i < figures.size(); ++i) {
        if (figures[i]->getFieldPositionChanged()) {
            return true;
        }
    }

    irr::u32 numberOfDeletedLines = deleteConstuctedLines();
    if (numberOfDeletedLines != 0) {
        userInputReceiver->setControllingFigure(NULL);
        gameScore = calculateGameScore(numberOfDeletedLines);
        text->remove();
        irr::core::stringw scoreString(L"Your scrore is ");
        scoreString += gameScore;
        text = guiEnvironment->addStaticText(scoreString.c_str(),
                irr::core::rect<irr::s32>(10, 10, 600, 120), false, true, 0, -1, false);
        text->setOverrideColor(irr::video::SColor(255, 255, 255, 255));

        for (irr::u32 i = 0; i < figures.size(); ++i) {
            if (figures[i] == NULL) {
                figures.erase(i);
                --i;
            }
        }

        // indicate figures, that they possibly can move
        for (irr::u32 i = 0; i < figures.size(); ++i) {
            figures[i]->setFieldPositionChanged(true);
        }
    }

    // check if there is no movement and all lines that can be deleted already deleted
    // if so - spawn new figure
    for (irr::u32 i = 0; i < figures.size(); ++i) {
        if (figures[i]->getFieldPositionChanged()) {
            return true;
        }
    }

    if (!spawnNewFigure(EGF_CUBE)) {
        gameOver = true;
        // stopping all animators
        if (!animatorsStopped) {
            for (irr::u32 i = 0; i < figures.size(); ++i) {
                irr::scene::ISceneNodeAnimatorList animators = figures[i]->getAnimators();
                for (irr::scene::ISceneNodeAnimatorList::Iterator it = animators.begin(); it != animators.end(); ++it) {
                    CMoveDownAnimator* moveDownAnimator = dynamic_cast<CMoveDownAnimator*> (*it);
                    if (moveDownAnimator == NULL) {
                        continue;
                    } else {
                        moveDownAnimator->setStoppedValue(true);
                    }
                }
            }
            animatorsStopped = true;
        }

        // set gameOver text
        text->remove();
        irr::core::stringw scoreString(L"Game over! Your scrore is ");
        scoreString += gameScore;
        text = guiEnvironment->addStaticText(scoreString.c_str(),
                irr::core::rect<irr::s32>(250, 250, 800, 620), false, true, 0, -1, false);
        text->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
        return false;
    } else {
        userInputReceiver->setControllingFigure(figures.getLast());
    }
    return true;
}

void CGameManager::setTwoDimensionalMode(bool twoDimensionalMode) {
    this->twoDimensionalMode = twoDimensionalMode;
    irr::core::vector3di fieldSize = gameFieldManager->getFieldSize();
    if (twoDimensionalMode) {
        spawnPoint.set(fieldSize.X / 2, fieldSize.Y - 1, 0);
    } else {
        spawnPoint.set(fieldSize.X / 2, fieldSize.Y - 1, fieldSize.Z / 2);
    }
}

bool CGameManager::spawnNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize) {
    CFigure* newFigure = gameFiruresFactory->createNewFigure(figureType, figureSize);
    if (newFigure == NULL) {
        return false;
    }

    newFigure->setParentNodeFieldPosition(spawnPoint);
    // make figure offset based on its height
    /* example:
      |     * *
     9|     x *
     8|     5 6
     ...
     we can't spawn square figure with root position 9,5,0 just because some other figure's nodes will be off field
     */
    irr::s32 upperBound = 0;
    for (irr::u32 i = 0; i < newFigure->getFieldPositions().size(); ++i) {
        if (newFigure->getFieldPositions()[i].Y > (gameFieldManager->getFieldSize().Y - 1)) {
            upperBound = newFigure->getFieldPositions()[i].Y - (gameFieldManager->getFieldSize().Y - 1);
        }
    }
    if (upperBound != 0) {
        spawnPoint.Y -= upperBound;
        // can be optimized via this call (but you have also set parent field position, without reconstruction)
        //newFigure->affectFieldPositions(irr::core::vector3di(0, -upperBound, 0));  
        newFigure->setParentNodeFieldPosition(spawnPoint);
    }

    // check if figure can be spawned - field is empty
    irr::core::array<irr::core::vector3di> fieldPositions = newFigure->getFieldPositions();
    for (irr::u32 i = 0; i < fieldPositions.size(); ++i) {
        if (gameFieldManager->getFieldValue(fieldPositions[i]) != EGF_EMPTY) {
            // no empty space for figure nodes - figure can't be spawned - game ends                                   
            newFigure->remove();
            return false;
        } else {
            gameFieldManager->setFieldValue(figureType, fieldPositions[i]);
        }
    }

    irr::core::vector3df spawnPosition;
    spawnPosition.X = spawnPoint.X * figureSize * figureSize;
    spawnPosition.Y = spawnPoint.Y * figureSize * figureSize + (figureSize * figureSize / 2);
    spawnPosition.Z = spawnPoint.Z * figureSize * figureSize;
    spawnPosition += fieldOrigin;
    newFigure->setPosition(spawnPosition);

    irr::scene::ISceneNodeAnimator* nodesAnimator = new CMoveDownAnimator(gameFieldManager, twoDimensionalMode, 1, speed, aabb);
    newFigure->addAnimator(nodesAnimator);
    nodesAnimator->drop();
    figures.push_back(newFigure);
    return true;
}

bool CGameManager::spawnNewFigure(EGF_FIGURE_TYPE figureType) {
    return spawnNewFigure(figureType, figureSize);
}

irr::u32 CGameManager::deleteConstuctedLines() {
    irr::core::array<irr::s32> linesToDelete;
    gameFieldManager->getLinesToDelete(twoDimensionalMode, EGF_CUBE, linesToDelete);
    if (linesToDelete.size() == 0) {
        return 0;
    }

    irr::u32 numberOfNodesToDelete = (linesToDelete.size() * gameFieldManager->getFieldSize().Y);
    irr::u32 deletedNodes = 0;
    for (irr::u32 i = 0; i < linesToDelete.size(); ++i) {
        for (irr::u32 j = 0; j < figures.size(); ++j) {
            // cause fieldPosition of figure recalculates each time figure change position or delete its nodes, we make copy
            irr::core::array<irr::core::vector3di> figureFieldPositions(figures[j]->getFieldPositions());
            for (irr::u32 k = 0; k < figureFieldPositions.size(); ++k) {
                if (figureFieldPositions[k].Y != linesToDelete[i]) {
                    continue;
                }

                irr::core::vector3di nodeOffset;
                rotateVectorByCoordinate(figureFieldPositions[k] - figures[j]->parentNodeFieldPosition, figures[j]->getRotation(), nodeOffset, true);

                nodeOffset.X *= figures[j]->getScale().X;
                nodeOffset.Y *= figures[j]->getScale().Y;
                nodeOffset.Z *= figures[j]->getScale().Z;
                figures[j]->removeNodeByOffset(nodeOffset);
                gameFieldManager->setFieldValue(EGF_EMPTY, nodeOffset);
                deletedNodes++;

                // check if figure empty - remove it
                if (figures[j]->isFigureEmpty()) {
                    figures[j]->remove();
                    figures[j] = NULL;
                    break;
                }

                if (deletedNodes == numberOfNodesToDelete) {
                    return linesToDelete.size();
                }

            }

        }
    }
    return linesToDelete.size();
}

CGameManager::~CGameManager() {
}