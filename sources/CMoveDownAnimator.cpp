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

bool CMoveDownAnimator::outOfFieldBorders(irr::core::vector3di position) {
    irr::core::vector3di fieldSize = gameFieldManager->getFieldSize();
    return ((position.X < 0 || position.Y < 0 || position.Z < 0) || ((position.X > fieldSize.X) || (position.Y > fieldSize.Y) || (position.Z > fieldSize.Z)));
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

        // check if it's possible to make step
        irr::core::array<irr::core::vector3di> extremePositions;
        figure->getExtremePositions(directionStep, extremePositions);
        bool canMakeStep = true;
        for (irr::u32 i = 0; i < extremePositions.size(); ++i) {
            if (outOfFieldBorders(extremePositions[i] + directionStep) || gameFieldManager->getFieldValue(extremePositions[i] + directionStep) != EGF_EMPTY) {
                canMakeStep = false;
                break;
            }
        }
        if (canMakeStep) {
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
            
            /*std::cout << "*******" << std::endl;
            gameFieldManager->printField();*/

            irr::core::vector3df newPosition = figure->getPosition();
            newPosition.Y += (directionStep.Y * figure->getScale().Y);
            figure->setPosition(newPosition);
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

