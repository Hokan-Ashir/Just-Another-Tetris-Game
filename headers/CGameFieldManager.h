/* 
 * File:   CGameFieldManager.h
 * Author: hokan
 *
 * Created on 29 Июнь 2014 г., 19:22
 */

#ifndef CGAMEFIELDMANAGER_H
#define	CGAMEFIELDMANAGER_H

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <irrArray.h>
#include <ISceneNode.h>
#include <ISceneManager.h>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"

/**
 * enumeration for game figures on field abstraction
 */
enum EGF_FIGURE_TYPE {
    EGF_EMPTY = 0,
    EGF_CUBE,
    EGF_SPHERE,
    EGF_NOT_EMPTY
};

/**
 * Manage field creation
 */
class CGameFieldManager {
public:
    /*
     10 |
        |    / 10
        |   /
        |  /
        | /
     0  |/_ _ _ _ _ _ 10
          
     */
    
    /**
     * Constructor, create game field with specific size
     * 
     * @param x - size of game field in x-axis
     * @param y - size of game field in y-axis
     * @param z - size of game field in z-axis
     */
    CGameFieldManager(irr::s32 x, irr::s32 y, irr::s32 z);
    
    /**
     * Constructor, create game field with specific size
     * 
     * @param fieldSize - vector of field size by axis
     */
    CGameFieldManager(irr::core::vector3di fieldSize);
    
    /**
     * Destructor, not implemented
     */
    virtual ~CGameFieldManager();

    // TODO round field size, so it can be divided by 2
    /**
     * Setter, also reallocate field memory and values
     * 
     * @param x - new value of field x-axis size
     * @param y - new value of field y-axis size
     * @param z - new value of field z-axis size
     */
    void setFieldSize(irr::s32 x, irr::s32 y, irr::s32 z);
     
    /**
     * player has constructed plane (3d-mode) or line (2d-mode) of same elemental blocks \
     * so we get list of lines which must be deleted
     * 
     * @param twoDimensionalMode - true if game proceeds in two dimensions, false otherwise
     * @param figureType - game field figures which existence checked
     * @param linesToDelete - out array of lines to delete
     */
    void getLinesToDelete(bool twoDimensionalMode, EGF_FIGURE_TYPE figureType, irr::core::array<irr::s32>& linesToDelete);

    /**
     * Getter
     * 
     * @param x - x-axis game field coordinate
     * @param y - y-axis game field coordinate
     * @param z - z-axis game field coordinate
     * @return game field value
     */
    irr::u8 getFieldValue(irr::s32 x, irr::s32 y, irr::s32 z) const;

    /**
     * Setter
     * 
     * @param newValue - new value of game field
     * @param x - x-axis game field coordinate
     * @param y - y-axis game field coordinate
     * @param z - z-axis game field coordinate
     */
    void setFieldValue(irr::u8 newValue, irr::s32 x, irr::s32 y, irr::s32 z);

    /**
     * Getter
     * 
     * @param position - vector of axis positions of game field
     * @return game field value in this position
     */
    irr::u8 getFieldValue(irr::core::vector3di position) const;

    /**
     * Setter
     * 
     * @param newValue - new value of game field
     * @param position - vector of axis positions of game field
     */
    void setFieldValue(irr::u8 newValue, irr::core::vector3di position);

    /**
     * Fills field with specific value
     * 
     * @param value - value to fill field
     */
    void clearField(irr::u8 value = EGF_EMPTY);

    /**
     * Getter
     * 
     * @return vector of field size by coordinate
     */
    irr::core::vector3di getFieldSize() const;

private:

    //  |y
    //  |    /z
    //  |   /
    //  |  /
    //  | /
    //  |/_ _ _ _ _ x     

    /**
     * Checks if line has same values (used for getting list of node to delete)
     * As if player constructs whole line
     * used for 2d-mode
     * 
     * @param lineNumber - which line to check
     * @param value - which value to search
     * @return true, if line has same values, false otherwise
     */
    bool hasLineSameValues(irr::s32 lineNumber, EGF_FIGURE_TYPE value);

    /**
     * Check if plane has same values (used for getting list of node to delete)
     * As if player constructs whole plane
     * used for 3d-mode
     * 
     * @param planeNumber - which plane to check
     * @param value - which value to search
     * @return true, if plane has same values, false otherwise
     */
    bool hasPlaneSameValues(irr::s32 planeNumber, EGF_FIGURE_TYPE value);

    /**
     * Checks if line has any non values (used for getting list of node to delete)
     * As if player constructs whole line
     * used for 2d-mode
     * 
     * @param lineNumber - which line to check
     * @return true, if line has only non empty values
     */
    bool hasLineNonEmptyValues(irr::s32 lineNumber);
    
    /**
     * Check if plane has any non values (used for getting list of node to delete)
     * As if player constructs whole plane
     * used for 3d-mode
     * 
     * @param planeNumber - which plane to check
     * @return true, if plane has only non empty values
     */
    bool hasPlaneNonEmptyValues(irr::s32 planeNumber);
private:
    /**
     * 3d-array representing field
     */
    irr::core::array<irr::u32> field;

    /**
     * current size of field in x-axis
     */
    irr::s32 currentSizeX;
    
    /**
     * current size of field in y-axis
     */
    irr::s32 currentSizeY;
    
    /**
     * current size of field in z-axis
     */
    irr::s32 currentSizeZ;
};

#endif	/* CGAMEFIELDMANAGER_H */

