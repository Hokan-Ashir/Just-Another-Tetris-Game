/* 
 * File:   CGameManager.cpp
 * Author: hokan
 * 
 * Created on 1 Июль 2014 г., 13:28
 */

#include "headers/CGameManager.h"
#include "headers/CMoveDownAnimator.h"
#include "headers/CGridBox.h"

#include <iostream>

CGameManager::CGameManager(CGameFieldManager* gameFieldManager, CGameFiguresFactory* gameFiruresFactory, CUserInputEventReceiver* userInputReceiver, bool twoDimensionalMode, irr::core::vector3df fieldOrigin, irr::core::aabbox3df fieldAABB)
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
}

irr::f32 CGameManager::getGameSpeed() const {
    return speed;
}

void CGameManager::setGameSpeed(irr::f32 newSpeed) {
    speed = newSpeed;
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
    // TODO speed also must participate
    return gameScore + (numberOfDeletedLines * 100);
}

bool CGameManager::getTwoDimensionalMode() const {
    return twoDimensionalMode;
}

// TODO optimize via delay

bool CGameManager::runOneLoop() {   
    for (irr::u32 i = 0; i < figures.size(); ++i) {
        if (figures[i]->getFieldPositionChanged()) {
            return true;
        }
    }
    
    irr::u32 numberOfDeletedLines = deleteConstuctedLines();
    //gameFieldManager->printField();
    //std::cout << "******************" << std::endl;
    if (numberOfDeletedLines != 0) {
        userInputReceiver->setControllingFigure(NULL);
        gameScore = calculateGameScore(numberOfDeletedLines);
        std::cout << "Your game score is: " << gameScore << std::endl;
        
        for (irr::u32 i = 0; i < figures.size(); ++i) {
            if (figures[i] == NULL) {
                figures.erase(i);
                --i;
            }
        }
        std::cout << "array size: " << figures.size() << std::endl;

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
        return false;
    } else {
        userInputReceiver->setControllingFigure(figures.getLast());
    }    
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

bool CGameManager::spawnNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize, irr::video::SColor colour) {
    CFigure* newFigure = gameFiruresFactory->createNewFigure(figureType, figureSize, colour);
    if (newFigure == NULL) {
        return false;
    }

    newFigure->setParentNodeFieldPosition(spawnPoint);
    // check if node can be spawned - field is empty
    irr::core::array<irr::core::vector3di> fieldPositions = newFigure->getFieldPositions();
    for (irr::u32 i = 0; i < fieldPositions.size(); ++i) {
        if (gameFieldManager->getFieldValue(fieldPositions[i]) != EGF_EMPTY) {
            // no empty space for figure nodes - figure can't be spawned
            // TODO check if children deleted
            newFigure->remove();
            return false;
        } else {
            gameFieldManager->setFieldValue(figureType, fieldPositions[i]);
        }
    }

    irr::core::vector3df spawnPosition;
    spawnPosition.X = spawnPoint.X;
    spawnPosition.Y = spawnPoint.Y + (figureSize * figureSize / 2);
    spawnPosition.Z = spawnPoint.Z;
    spawnPosition *= irr::core::vector3df(figureSize * figureSize, figureSize * figureSize, figureSize * figureSize);
    spawnPosition += fieldOrigin;
    newFigure->setPosition(spawnPosition);

    irr::scene::ISceneNodeAnimator* nodesAnimator = new CMoveDownAnimator(gameFieldManager, twoDimensionalMode, 1, speed, aabb);
    newFigure->addAnimator(nodesAnimator);
    nodesAnimator->drop();
    figures.push_back(newFigure);
    return true;
}

bool CGameManager::spawnNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize) {
    CFigure* newFigure = gameFiruresFactory->createNewFigure(figureType, figureSize);
    if (newFigure == NULL) {
        return false;
    }

    newFigure->setParentNodeFieldPosition(spawnPoint);
    for (irr::u32 i = 0; i < newFigure->getFieldPositions().size(); ++i) {
        std::cout << "(" << newFigure->getFieldPositions()[i].X << ", " << newFigure->getFieldPositions()[i].Y << ", " << newFigure->getFieldPositions()[i].Z << ")" << std::endl;
    }
    // make figure offset based on its height
    /* example:
     * *
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

    std::cout << "after" << std::endl;
    for (irr::u32 i = 0; i < newFigure->getFieldPositions().size(); ++i) {
        std::cout << "(" << newFigure->getFieldPositions()[i].X << ", " << newFigure->getFieldPositions()[i].Y << ", " << newFigure->getFieldPositions()[i].Z << ")" << std::endl;
    }
    // check if figure can be spawned - field is empty
    irr::core::array<irr::core::vector3di> fieldPositions = newFigure->getFieldPositions();
    for (irr::u32 i = 0; i < fieldPositions.size(); ++i) {
        if (gameFieldManager->getFieldValue(fieldPositions[i]) != EGF_EMPTY) {
            // no empty space for figure nodes - figure can't be spawned - game ends            
            // TODO check if children deleted
            // TODO check memory leak            
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
            irr::core::vector3di parentNodeFieldPosition = figures[j]->getParentNodeFieldPosition();
            for (irr::u32 k = 0; k < figureFieldPositions.size(); ++k) {
                std::wcout << "name: " << figures[j]->getFigureName().c_str() << std::endl;
                std::cout << "figPos: " << figureFieldPositions[k].X << ", " << figureFieldPositions[k].Y << ", " << figureFieldPositions[k].Z << std::endl;
                if (figureFieldPositions[k].Y != linesToDelete[i]) {
                    continue;
                }

                std::cout << "parent position: " << figures[j]->parentNodeFieldPosition.X << ", " << figures[j]->parentNodeFieldPosition.Y << ", " << figures[j]->parentNodeFieldPosition.Z << std::endl;
                irr::core::vector3di nodeOffset; // = (figureFieldPositions[k] - figures[j]->parentNodeFieldPosition);                
                //std::cout << "offset: " << nodeOffset.X << ", " << nodeOffset.Y << ", " << nodeOffset.Z << std::endl;
                std::cout << "rotation: " << figures[j]->getRotation().X << ", " << figures[j]->getRotation().Y << ", " << figures[j]->getRotation().Z << std::endl;

                if (figures[j]->getRotation() == irr::core::vector3df()) {
                    nodeOffset.X = (figureFieldPositions[k] - figures[j]->parentNodeFieldPosition).X;
                    nodeOffset.Y = (figureFieldPositions[k] - figures[j]->parentNodeFieldPosition).Y;
                    nodeOffset.Z = (figureFieldPositions[k] - figures[j]->parentNodeFieldPosition).Z;
                } else {
                irr::core::vector3di tempVector;
                if (figures[j]->getRotation().X != 0) {
                    tempVector.X = 0;
                    tempVector.Y = (figureFieldPositions[k] - figures[j]->parentNodeFieldPosition).Y;
                    tempVector.Z = 0;
                    tempVector.rotateYZBy(-figures[j]->getRotation().X, irr::core::vector3di(0, 0, 0));
                    nodeOffset += tempVector;

                    tempVector.X = 0;
                    tempVector.Y = 0;
                    tempVector.Z = (figureFieldPositions[k] - figures[j]->parentNodeFieldPosition).Z;
                    tempVector.rotateYZBy(-figures[j]->getRotation().X, irr::core::vector3di(0, 0, 0));
                    nodeOffset += tempVector;
                }

                if (figures[j]->getRotation().Y != 0) {
                    tempVector.X = (figureFieldPositions[k] - figures[j]->parentNodeFieldPosition).X;
                    tempVector.Y = 0;
                    tempVector.Z = 0;
                    tempVector.rotateXZBy(-figures[j]->getRotation().Y, irr::core::vector3di(0, 0, 0));
                    nodeOffset += tempVector;

                    tempVector.X = 0;
                    tempVector.Y = 0;
                    tempVector.Z = (figureFieldPositions[k] - figures[j]->parentNodeFieldPosition).Z;
                    tempVector.rotateXZBy(-figures[j]->getRotation().Y, irr::core::vector3di(0, 0, 0));
                    nodeOffset += tempVector;
                }

                if (figures[j]->getRotation().Z != 0) {
                    tempVector.X = (figureFieldPositions[k] - figures[j]->parentNodeFieldPosition).X;
                    tempVector.Y = 0;
                    tempVector.Z = 0;
                    tempVector.rotateXYBy(-figures[j]->getRotation().Z, irr::core::vector3di(0, 0, 0));
                    nodeOffset += tempVector;

                    tempVector.X = 0;
                    tempVector.Y = (figureFieldPositions[k] - figures[j]->parentNodeFieldPosition).Y;
                    tempVector.Z = 0;
                    tempVector.rotateXYBy(-figures[j]->getRotation().Z, irr::core::vector3di(0, 0, 0));
                    nodeOffset += tempVector;
                }
                }

                nodeOffset.X *= figures[j]->getScale().X;
                nodeOffset.Y *= figures[j]->getScale().Y;
                nodeOffset.Z *= figures[j]->getScale().Z;
                std::wcout << "name: " << figures[j]->getFigureName().c_str() << std::endl;
                std::cout << "offset: " << nodeOffset.X << ", " << nodeOffset.Y << ", " << nodeOffset.Z << std::endl;
                figures[j]->removeNodeByOffset(nodeOffset);
                gameFieldManager->setFieldValue(EGF_EMPTY, nodeOffset);
                deletedNodes++;

                // TODO check this stuff
                // check if figure empty
                if (figures[j]->isFigureEmpty()) {
                    std::wcout << "name: " << figures[j]->getFigureName().c_str() << std::endl;
                    std::cout << "figure is empty" << std::endl;
                    figures[j]->remove();
                    figures[j] = NULL;
                    break;
                }

                if (deletedNodes == numberOfNodesToDelete) {
                    std::cout << "array size: " << figures.size() << std::endl;                    
                    gameFieldManager->printField();                    
                    return linesToDelete.size();
                }

            }

        }
    }
    std::cout << "array size: " << figures.size() << std::endl;
    return linesToDelete.size();
}

CGameManager::~CGameManager() {
}

