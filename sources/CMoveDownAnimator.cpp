/* 
 * File:   CMoveDownAnimator.cpp
 * Author: hokan
 * 
 * Created on 29 Июнь 2014 г., 19:21
 */

#include "headers/CMoveDownAnimator.h"
#include "headers/CFigure.h"

#include <iostream>

CMoveDownAnimator::CMoveDownAnimator(CGameFieldManager* gameFieldManager, bool twoDimensionalMode, irr::u32 step, irr::f32 delay, irr::core::aabbox3df aabb) :
delay(delay)
, aabb(aabb)
, gameFieldManager(gameFieldManager)
, twoDimensionalMode(twoDimensionalMode)
, noBoundaries(false)
, firstUpdate(true) {
#ifdef _DEBUG
    setDebugName("CMoveDownAnimator");
#endif

    if (aabb == irr::core::aabbox3df(irr::core::vector3df(0.0f, 0.0f, 0.0f))) {
        noBoundaries = true;
    }

    directionStep.set(0, -step, 0);
}

irr::u32 CMoveDownAnimator::getStep() const {
    return -directionStep.Y;
}

void CMoveDownAnimator::setStep(irr::u32 step) {
    directionStep.set(0, -step, 0);
}

irr::f32 CMoveDownAnimator::getDelay() const {
    return delay;
}

void CMoveDownAnimator::setDelay(irr::f32 delay) {
    this->delay = delay;
}

bool CMoveDownAnimator::outOfFieldBorders(irr::core::vector3di position) {
    irr::core::vector3di fieldSize = gameFieldManager->getFieldSize();
    return ((position.X < 0 || position.Y < 0 || position.Z < 0) || ((position.X > (fieldSize.X - 1)) || (position.Y > (fieldSize.Y - 1)) || (position.Z > (fieldSize.Z - 1))));
}

bool CMoveDownAnimator::outOfBottomFieldBorders(irr::core::vector3di position) {
    return (position.Y < 0);
}

void CMoveDownAnimator::animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs) {
    if (!node) {
        return;
    }

    if (firstUpdate) {
        loopTime = timeMs;
        firstUpdate = false;
    }

    if ((timeMs - loopTime) >= delay) {
        CFigure* figure = static_cast<CFigure*> (node);
        if (figure->g()) {
            irr::s32 direction = 1;
            // check in which direction nodes gone
            for (irr::u32 i = 0; i < figure->getFieldPositions().size(); ++i) {
                if ((figure->getFieldPositions()[i] + irr::core::vector3di(0, 1, 0)) == figure->parentNodeFieldPosition) {
                    // so parent is most upper node
                    direction = -1;
                    break;
                }
            }
            
            for (irr::u32 i = 0; i < figure->getFieldPositions().size(); ++i) {
                if (figure->getFieldPositions()[i] != figure->parentNodeFieldPosition) {
                    gameFieldManager->setFieldValue(EGF_EMPTY, figure->getFieldPositions()[i] + irr::core::vector3di(0, direction, 0));
                    gameFieldManager->setFieldValue(EGF_CUBE, figure->getFieldPositions()[i]);
                }
            }
            figure->setFieldPositionChanged(true);
            gameFieldManager->printField();
        } else {


            // check if it's possible to make step
            irr::core::array<irr::core::vector3di> extremePositions;
            figure->getExtremePositions(directionStep, extremePositions);
            bool canMakeStep = true;
            bool outOfBorders = false;
            irr::u32 i = 0;
            for (i; i < extremePositions.size(); ++i) {
                if (outOfFieldBorders(extremePositions[i] + directionStep)) {
                    outOfBorders = true;
                    break;
                }
                if (gameFieldManager->getFieldValue(extremePositions[i] + directionStep) != EGF_EMPTY) {
                    canMakeStep = false;
                    break;
                }
            }
            if (canMakeStep && !outOfBorders) {
                // go in negative direction - fill field from up/right/front to bottom/left/back
                // and vice versa
                if (directionStep.X > 0 || directionStep.Y > 0 || directionStep.Z > 0) {
                    for (irr::s32 i = figure->fieldPositions.size() - 1; i > -1; --i) {
                        // set empty to current position, make step, fill field to new position
                        gameFieldManager->setFieldValue(EGF_EMPTY, figure->fieldPositions[i]);
                        figure->fieldPositions[i] += directionStep;
                        gameFieldManager->setFieldValue(figure->getFieldFigureType(), figure->fieldPositions[i]);
                    }
                } else {
                    for (irr::u32 i = 0; i < figure->fieldPositions.size(); ++i) {
                        gameFieldManager->setFieldValue(EGF_EMPTY, figure->fieldPositions[i]);
                        figure->fieldPositions[i] += directionStep;
                        gameFieldManager->setFieldValue(figure->getFieldFigureType(), figure->fieldPositions[i]);
                    }
                }
                figure->parentNodeFieldPosition += directionStep;

                irr::core::vector3df newPosition = figure->getPosition();
                newPosition.Y += (directionStep.Y * figure->getScale().Y * figure->getScale().Y);
                figure->setPosition(newPosition);
                figure->setFieldPositionChanged(true);
            } else if (!canMakeStep || outOfBottomFieldBorders(extremePositions[i] + directionStep)) {
                figure->setFieldPositionChanged(false);
            }
        }
        loopTime = timeMs;
    }
}

irr::scene::ISceneNodeAnimator* CMoveDownAnimator::createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager) {
    irr::scene::ISceneNodeAnimator* animator = new CMoveDownAnimator(gameFieldManager, twoDimensionalMode, -directionStep.Y, delay, aabb);
    return animator;
}

CMoveDownAnimator::~CMoveDownAnimator() {
}

