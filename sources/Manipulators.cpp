#include "headers/Manipulators.h"

void rotateVectorByCoordinate(irr::core::vector3di inputVector, irr::core::vector3df rotationVector, irr::core::vector3di& outputVector, bool inverted) {
    irr::s32 invertedDirection = inverted ? -1 : 1;
    irr::core::vector3di tempVector;    
    if (rotationVector == irr::core::vector3df()) {
        outputVector.X = inputVector.X;
        outputVector.Y = inputVector.Y;
        outputVector.Z = inputVector.Z;
    } else {
        // calculate child rotation offset
        if (rotationVector.X != 0) {
            tempVector.X = 0;
            tempVector.Y = inputVector.Y;
            tempVector.Z = 0;
            tempVector.rotateYZBy(invertedDirection * rotationVector.X, irr::core::vector3di(0, 0, 0));
            outputVector += tempVector;

            tempVector.X = 0;
            tempVector.Y = 0;
            tempVector.Z = inputVector.Z;
            tempVector.rotateYZBy(invertedDirection * rotationVector.X, irr::core::vector3di(0, 0, 0));
            outputVector += tempVector;
        }

        if (rotationVector.Y != 0) {
            tempVector.X = inputVector.X;
            tempVector.Y = 0;
            tempVector.Z = 0;
            tempVector.rotateXZBy(invertedDirection * rotationVector.Y, irr::core::vector3di(0, 0, 0));
            outputVector += tempVector;

            tempVector.X = 0;
            tempVector.Y = 0;
            tempVector.Z = inputVector.Z;
            tempVector.rotateXZBy(invertedDirection * rotationVector.Y, irr::core::vector3di(0, 0, 0));
            outputVector += tempVector;
        }

        if (rotationVector.Z != 0) {
            tempVector.X = inputVector.X;
            tempVector.Y = 0;
            tempVector.Z = 0;
            tempVector.rotateXYBy(invertedDirection * rotationVector.Z, irr::core::vector3di(0, 0, 0));
            outputVector += tempVector;

            tempVector.X = 0;
            tempVector.Y = inputVector.Y;
            tempVector.Z = 0;
            tempVector.rotateXYBy(invertedDirection * rotationVector.Z, irr::core::vector3di(0, 0, 0));
            outputVector += tempVector;
        }
    }
}