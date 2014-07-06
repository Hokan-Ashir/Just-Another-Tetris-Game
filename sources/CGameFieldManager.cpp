/* 
 * File:   CGameFieldManager.cpp
 * Author: hokan
 * 
 * Created on 29 Июнь 2014 г., 19:22
 */

#include "headers/CGameFieldManager.h"

#include <iostream>

CGameFieldManager::CGameFieldManager(irr::s32 x, irr::s32 y, irr::s32 z) {
    x = (x < 0) ? 10 : x;
    y = (x < 0) ? 10 : y;
    z = (x < 0) ? 10 : z;

    setFieldSize(x, y, z);
    clearField();
}

CGameFieldManager::CGameFieldManager(irr::core::vector3di fieldSize) {
    fieldSize.X = (fieldSize.X < 0) ? 10 : fieldSize.X;
    fieldSize.Y = (fieldSize.Y < 0) ? 10 : fieldSize.Y;
    fieldSize.Z = (fieldSize.Z < 0) ? 10 : fieldSize.Z;

    setFieldSize(fieldSize.X, fieldSize.Y, fieldSize.Z);
    clearField();
}

CGameFieldManager::~CGameFieldManager() {
}

void CGameFieldManager::clearField(irr::u8 value) {
    for (irr::u8 i = 0; i < currentSizeX; ++i) {
        for (irr::u8 j = 0; j < currentSizeY; ++j) {
            for (irr::u8 k = 0; k < currentSizeZ; ++k) {
                setFieldValue(value, i, j, k);
            }
        }
    }
}

void CGameFieldManager::setFieldSize(irr::s32 x, irr::s32 y, irr::s32 z) {
    x = (x < 0) ? 10 : x;
    y = (x < 0) ? 10 : y;
    z = (x < 0) ? 10 : z;
    field.reallocate(x * y * z);
    currentSizeX = x;
    currentSizeY = y;
    currentSizeZ = z;
}

irr::core::vector3di CGameFieldManager::getFieldSize() const {
    return irr::core::vector3di(currentSizeX, currentSizeY, currentSizeZ);
}

irr::u8 CGameFieldManager::getFieldValue(irr::s32 x, irr::s32 y, irr::s32 z) const {
    return field[x * currentSizeY * currentSizeZ + y * currentSizeZ + z];
}

void CGameFieldManager::setFieldValue(irr::u8 newValue, irr::s32 x, irr::s32 y, irr::s32 z) {
    field[x * currentSizeY * currentSizeZ + y * currentSizeZ + z] = newValue;
}

irr::u8 CGameFieldManager::getFieldValue(irr::core::vector3di position) const {
    return field[position.X * currentSizeY * currentSizeZ + position.Y * currentSizeZ + position.Z];
}

void CGameFieldManager::setFieldValue(irr::u8 newValue, irr::core::vector3di position) {
    field[position.X * currentSizeY * currentSizeZ + position.Y * currentSizeZ + position.Z] = newValue;
}

bool CGameFieldManager::hasLineSameValues(irr::s32 lineNumber, EGF_FIGURE_TYPE value) {
    for (irr::s32 i = 0; i < currentSizeX; ++i) {
        if (getFieldValue(i, lineNumber, 0) != value) {
            return false;
        }
    }

    return true;
}

bool CGameFieldManager::hasPlaneSameValues(irr::s32 planeNumber, EGF_FIGURE_TYPE value) {    
    for (irr::s32 i = 0; i < currentSizeX; ++i) {
        for (irr::s32 j = 0; j < currentSizeZ; ++j) {
            if (getFieldValue(i, planeNumber, j) != value) {
                return false;
            }
        }
    }

    return true;
}

bool CGameFieldManager::hasLineNonEmptyValues(irr::s32 lineNumber) {
    for (irr::s32 i = 0; i < currentSizeX; ++i) {
        if (getFieldValue(i, lineNumber, 0) == EGF_EMPTY) {
            return false;
        }
    }

    return true;
}

bool CGameFieldManager::hasPlaneNonEmptyValues(irr::s32 planeNumber) {
    for (irr::s32 i = 0; i < currentSizeX; ++i) {
        for (irr::s32 j = 0; j < currentSizeZ; ++j) {
            if (getFieldValue(i, planeNumber, j) == EGF_EMPTY) {
                return false;
            }
        }
    }

    return true;
}

void CGameFieldManager::getLinesToDelete(bool twoDimensionalMode, EGF_FIGURE_TYPE figureType, irr::core::array<irr::s32>& linesToDelete) {
    // get number of lines that already constructed and which nodes must be removed
    linesToDelete.reallocate(currentSizeY);
    for (irr::s32 i = 0; i < currentSizeY; ++i) {
        if (twoDimensionalMode) {
            // search for any not empty value
            if (figureType == EGF_NOT_EMPTY) {
                if (hasLineNonEmptyValues(i)) {
                    linesToDelete.push_back(i);
                }
            } else {
                if (hasLineSameValues(i, figureType)) {
                    linesToDelete.push_back(i);
                }
            }
        } else {
            if (figureType == EGF_NOT_EMPTY) {
                if (hasPlaneNonEmptyValues(i)) {
                    linesToDelete.push_back(i);
                }
            } else {
                if (hasPlaneSameValues(i, figureType)) {
                    linesToDelete.push_back(i);
                }
            }
        }
    }
}

