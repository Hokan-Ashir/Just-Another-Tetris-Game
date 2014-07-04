/* 
 * File:   CUserInputReceiver.cpp
 * Author: hokan
 * 
 * Created on 2 Июль 2014 г., 16:37
 */

#include "headers/CUserInputEventReceiver.h"

#include <iostream>

CUserInputEventReceiver::CUserInputEventReceiver(CGameFieldManager* gameFieldManager, bool twoDimensionalMode, CFigure* figure)
: gameFieldManager(gameFieldManager)
, twoDimensionalMode(twoDimensionalMode)
, figure(figure)
, pause(false) {
}

CUserInputEventReceiver::~CUserInputEventReceiver() {

}

// TODO make key map

bool CUserInputEventReceiver::OnEvent(const irr::SEvent& event) {
    if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown) {
        switch (event.KeyInput.Key) {
            case irr::KEY_KEY_P:
                pause = !pause;
                return true;
        }
    }

    if (figure == NULL) {
        return false;
    }

    if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown) {
        switch (event.KeyInput.Key) {

                // direction buttons
            case irr::KEY_KEY_W:
                if (!twoDimensionalMode) {
                    moveFigureInDirection(irr::core::vector3di(0, 0, 1));
                }
                return true;
            case irr::KEY_KEY_A:
                moveFigureInDirection(irr::core::vector3di(-1, 0, 0));
                return true;
            case irr::KEY_KEY_D:
                moveFigureInDirection(irr::core::vector3di(1, 0, 0));
                return true;
            case irr::KEY_KEY_S:
                if (!twoDimensionalMode) {
                    moveFigureInDirection(irr::core::vector3di(0, 0, -1));
                }
                return true;
            case irr::KEY_SPACE:
                moveFigureInDirection(irr::core::vector3di(0, -1, 0));
                return true;

                // rotation buttons, maybe a little unusual
                // buy you don't need number keyboard, so you can play on notebooks too =)
            case irr::KEY_KEY_I:
                if (!twoDimensionalMode) {
                    rotateFigureInDirection(irr::core::vector3df(1, 0, 0));
                }
                return true;
            case irr::KEY_KEY_K:
                if (!twoDimensionalMode) {
                    rotateFigureInDirection(irr::core::vector3df(-1, 0, 0));
                }
                return true;
            case irr::KEY_KEY_J:
                rotateFigureInDirection(irr::core::vector3df(0, 0, 1));
                return true;
            case irr::KEY_KEY_L:
                rotateFigureInDirection(irr::core::vector3df(0, 0, -1));
                return true;
            case irr::KEY_KEY_U:
                if (!twoDimensionalMode) {
                    rotateFigureInDirection(irr::core::vector3df(0, 1, 0));
                }
                return true;
            case irr::KEY_KEY_O:
                if (!twoDimensionalMode) {
                    rotateFigureInDirection(irr::core::vector3df(0, -1, 0));
                }
                return true;

            default:
                break;
        }
    }

    return false;
}

CFigure* CUserInputEventReceiver::getControllingFigure() const {
    return figure;
}

void CUserInputEventReceiver::setControllingFigure(CFigure* newFigure) {
    figure = newFigure;
}

bool CUserInputEventReceiver::getTwoDimensionalMode() const {
    return twoDimensionalMode;
}

void CUserInputEventReceiver::setTwoDimensionalMode(bool newValue) {
    twoDimensionalMode = newValue;
}

bool CUserInputEventReceiver::isPause() const {
    return pause;
}

void CUserInputEventReceiver::setPause(bool newPauseValue) {
    pause = newPauseValue;
}

bool CUserInputEventReceiver::outOfFieldBorders(irr::core::vector3di position) {
    irr::core::vector3di fieldSize = gameFieldManager->getFieldSize();
    return ((position.X < 0 || position.Y < 0 || position.Z < 0) || ((position.X > (fieldSize.X - 1)) || (position.Y > (fieldSize.Y - 1)) || (position.Z > (fieldSize.Z - 1))));
}

bool CUserInputEventReceiver::outOfBottomFieldBorders(irr::core::vector3di position) {
    return (position.Y < 0);
}

void CUserInputEventReceiver::moveFigureInDirection(irr::core::vector3di directionStep) {
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


        irr::core::vector3df newPosition;
        newPosition.X = directionStep.X;
        newPosition.Y = directionStep.Y;
        newPosition.Z = directionStep.Z;
        newPosition *= figure->getScale() * figure->getScale();
        newPosition += figure->getPosition();
        figure->setPosition(newPosition);
        figure->setFieldPositionChanged(true);
    } else if ((!canMakeStep && directionStep.Y != 0) || outOfBottomFieldBorders(extremePositions[i] + directionStep)) {
        figure->setFieldPositionChanged(false);
    }
}

void CUserInputEventReceiver::clipFigureRotation(irr::core::vector3df& figureRotation) {
    if (figureRotation.X > 360) {
        figureRotation.X -= 360;
    } else if (figureRotation.X < -360) {
        figureRotation.X += 360;
    }

    if (figureRotation.Y > 360) {
        figureRotation.Y -= 360;
    } else if (figureRotation.Y < -360) {
        figureRotation.Y += 360;
    }

    if (figureRotation.Z > 360) {
        figureRotation.Z -= 360;
    } else if (figureRotation.Z < -360) {
        figureRotation.Z += 360;
    }
}

// very slow, i know

void CUserInputEventReceiver::rotateFigureInDirection(irr::core::vector3df directionStep) {
    // get new field position of rotated figure
    irr::core::array<irr::core::vector3di> oldFieldPositions = figure->getFieldPositions();
    directionStep *= 90;
    irr::core::vector3df figureRotation = figure->getRotation() + directionStep;
    clipFigureRotation(figureRotation);
    figure->setRotation(figureRotation);
    figure->constructNodesFieldPositions(figure->getParentNodeFieldPosition());
    irr::core::array<irr::core::vector3di> newFieldPositions = figure->getFieldPositions();

    irr::core::array<irr::core::vector3di> intersectedPositions;
    // get only new field positions
    for (irr::u32 i = 0; i < newFieldPositions.size(); ++i) {
        for (irr::u32 j = 0; j < oldFieldPositions.size(); ++j) {
            if (newFieldPositions[i] == oldFieldPositions[j]) {
                intersectedPositions.push_back(newFieldPositions[i]);
                newFieldPositions.erase(i);
                --i;
                break;
            }
        }
    }

    // check if field is empty on this positions
    for (irr::u32 i = 0; i < newFieldPositions.size(); ++i) {
        if (outOfFieldBorders(newFieldPositions[i]) || gameFieldManager->getFieldValue(newFieldPositions[i]) != EGF_EMPTY) {
            // can't rotate, restore old rotation values, rotate figure, return
            figure->setFieldPositions(oldFieldPositions);
            directionStep = directionStep.invert();
            figure->setRotation(figure->getRotation() + directionStep);
            if (outOfBottomFieldBorders(newFieldPositions[i])) {
                figure->setFieldPositionChanged(false);
            }
            return;
        }
    }

    // can rotate in direction - we've already rotated, just place non-empty values on field
    for (irr::u32 i = 0; i < oldFieldPositions.size(); ++i) {
        gameFieldManager->setFieldValue(EGF_EMPTY, oldFieldPositions[i]);
    }
    for (irr::u32 i = 0; i < newFieldPositions.size(); ++i) {
        gameFieldManager->setFieldValue(figure->getFieldFigureType(), newFieldPositions[i]);
    }
    for (irr::u32 i = 0; i < intersectedPositions.size(); ++i) {
        gameFieldManager->setFieldValue(figure->getFieldFigureType(), intersectedPositions[i]);
    }
    figure->setFieldPositionChanged(true);
}