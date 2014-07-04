/* 
 * File:   CGridBox.h
 * Author: hokan
 *
 * Created on 2 Июль 2014 г., 12:42
 */

#ifndef CGRIDBOX_H
#define	CGRIDBOX_H

#include "headers/CGridSceneNode.h"

class CGridBox {
public:
    CGridBox(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* smgr, irr::s32 idRight = -1, irr::s32 idLeft = -1, irr::s32 idDown = -1, irr::s32 idFront = -1, irr::s32 idBack = -1);
    virtual ~CGridBox();

    void setPosition(const irr::core::vector3df& newposition);
    const irr::core::vector3df& getPosition() const;

    void setColour(irr::video::SColor newcolour);

    void setGridsSize(const irr::core::dimension2df& newsize);
    irr::core::dimension2df getGridsSize() const;

    void setSpacing(irr::f32 newspacing);
    irr::f32 getSpacing() const;

    void turnOn3DMode();
    void turnOn2DMode();
private:
    CGridSceneNode* rightGrid;
    CGridSceneNode* leftGrid;
    CGridSceneNode* downGrid;
    CGridSceneNode* frontGrid;
    CGridSceneNode* backGrid;
};

#endif	/* CGRIDBOX_H */

