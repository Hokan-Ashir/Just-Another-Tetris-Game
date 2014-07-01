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

    EGF_SPHERE
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

    void setFieldSize(irr::s32 x, irr::s32 y, irr::s32 z);

    void deleteNodes(bool twoDimensionalMode);

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

    bool hasLineSameValues(irr::s32 lineNumber);

    // check if plane has same values (used for getting list of node to delete)
    // player construct whole plane
    // used for 3d-mode
    bool hasPlaneSameValues(irr::s32 planeNumber);

    // player has constructed plane (3d-mode) or line (2d-mode) of same elemental blocks
    // so we get list of nodes which must be deleted
    void getNodesToDelete(bool twoDimensionalMode, irr::core::array<irr::scene::ISceneNode*>& outNodes);
private:
    irr::core::array<irr::u32> field;

    irr::s32 currentSizeX;
    irr::s32 currentSizeY;
    irr::s32 currentSizeZ;

    // TODO get via terrain->getPosition().Y
    irr::s32 fieldYPosition;

    irr::scene::ISceneManager* sceneManager;

    // TODO get via aabb
    irr::s32 elementalBlockSize;
    irr::core::stringc elementalBlockName;
};

#endif	/* CGAMEFIELDMANAGER_H */

