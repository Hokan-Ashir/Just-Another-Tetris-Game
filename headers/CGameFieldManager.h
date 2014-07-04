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

enum EGF_FIGURE_TYPE {
    EGF_EMPTY = 0,
    EGF_CUBE,
    EGF_SPHERE,
    EGF_NOT_EMPTY
};

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
    CGameFieldManager(irr::s32 x, irr::s32 y, irr::s32 z);
    CGameFieldManager(irr::core::vector3di fieldSize);
    virtual ~CGameFieldManager();

    // TODO round field size, so it can be divided by 2
    void setFieldSize(irr::s32 x, irr::s32 y, irr::s32 z);

    // player has constructed plane (3d-mode) or line (2d-mode) of same elemental blocks
    // so we get list of nodes which must be deleted
    void getLinesToDelete(bool twoDimensionalMode, EGF_FIGURE_TYPE figureType, irr::core::array<irr::s32>& linesToDelete);

    irr::u8 getFieldValue(irr::s32 x, irr::s32 y, irr::s32 z) const;

    void setFieldValue(irr::u8 newValue, irr::s32 x, irr::s32 y, irr::s32 z);

    irr::u8 getFieldValue(irr::core::vector3di position) const;

    void setFieldValue(irr::u8 newValue, irr::core::vector3di position);

    void clearField(irr::u8 value = EGF_EMPTY);

    irr::core::vector3di getFieldSize() const;

    // TODO remove in production
    void printField();

private:

    //  |y
    //  |    /z
    //  |   /
    //  |  /
    //  | /
    //  |/_ _ _ _ _ x


    // check if line has same values (used for getting list of node to delete)
    // player construct whole line
    // used for 2d-mode

    bool hasLineSameValues(irr::s32 lineNumber, EGF_FIGURE_TYPE value);

    // check if plane has same values (used for getting list of node to delete)
    // player construct whole plane
    // used for 3d-mode
    bool hasPlaneSameValues(irr::s32 planeNumber, EGF_FIGURE_TYPE value);

    bool hasLineNonEmptyValues(irr::s32 lineNumber);
    bool hasPlaneNonEmptyValues(irr::s32 planeNumber);
private:
    irr::core::array<irr::u32> field;

    irr::s32 currentSizeX;
    irr::s32 currentSizeY;
    irr::s32 currentSizeZ;
};

#endif	/* CGAMEFIELDMANAGER_H */

