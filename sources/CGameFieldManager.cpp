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

void CGameFieldManager::printField() {
    for (irr::u8 i = 0; i < currentSizeX; ++i) {
        std::cout << "i: " << (irr::u32)i << std::endl;
        for (irr::u8 j = 0; j < currentSizeY; ++j) {
            for (irr::u8 k = 0; k < currentSizeZ; ++k) {                
                std::cout << (irr::u32)getFieldValue(i, j, k);
            }
            std::cout << std::endl;
        }
    }
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

void CGameFieldManager::deleteNodes(bool twoDimensionalMode) {
    irr::core::array<irr::scene::ISceneNode*> nodesToDelete;
    getNodesToDelete(twoDimensionalMode, nodesToDelete);

    for (irr::u32 i = 0; i < nodesToDelete.size(); ++i) {
        // TODO make figure interface
        /*Figure* figure = dynamic_cast<Fugure*>(nodesToDelete[i]);
        figure->deleteAction();
        figure->remove();*/
        nodesToDelete[i]->remove();
    }
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

bool CGameFieldManager::hasLineSameValues(irr::s32 lineNumber) {
    irr::u8 searchValue = getFieldValue(0, lineNumber, 0);
    for (irr::s32 i = 1; i < currentSizeX; ++i) {
        if (searchValue != getFieldValue(i, lineNumber, 0)) {
            return false;
        }
    }

    return true;
}

bool CGameFieldManager::hasPlaneSameValues(irr::s32 planeNumber) {
    irr::u8 searchValue = getFieldValue(0, planeNumber, 0);
    for (irr::s32 i = 1; i < currentSizeX; ++i) {
        for (irr::s32 j = 1; j < currentSizeZ; ++j) {
            if (searchValue != getFieldValue(i, planeNumber, j)) {
                return false;
            }
        }
    }

    return true;
}

void CGameFieldManager::getNodesToDelete(bool twoDimensionalMode, irr::core::array<irr::scene::ISceneNode*>& outNodes) {
    // get number of lines that already constructed and shich nodes must be removed
    irr::core::array<irr::s32> linesToDeleteArray(currentSizeY);
    for (irr::s32 i = 0; i < currentSizeY; ++i) {
        if (twoDimensionalMode) {
            if (hasLineSameValues(i)) {
                linesToDeleteArray.push_back(i);
            }
        } else {
            if (hasPlaneSameValues(i)) {
                linesToDeleteArray.push_back(i);
            }
        }
    }

    // get array of nodes with specific type        
    // TODO here must be correct SceneNode type
    sceneManager->getSceneNodesFromType(irr::scene::ESNT_CUBE, outNodes);

    // check which nodes must be removed
    for (irr::u32 i = 0; i < outNodes.size(); ++i) {
        irr::u32 j = 0;
        for (j; j < linesToDeleteArray.size(); ++j) {
            if (outNodes[i]->getPosition().Y == (fieldYPosition * j)) {
                break;
            }
        }

        // node must NOT be removed, cause its Y-position not in array of lines to deletion
        if (j == linesToDeleteArray.size()) {
            outNodes.erase(i);
        }
    }
}

