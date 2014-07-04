/* 
 * File:   CGridBox.cpp
 * Author: hokan
 * 
 * Created on 2 Июль 2014 г., 12:42
 */

#include "headers/CGridBox.h"

CGridBox::CGridBox(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* smgr, irr::s32 idRight, irr::s32 idLeft, irr::s32 idDown, irr::s32 idFront, irr::s32 idBack) {
    rightGrid = new CGridSceneNode(parent, smgr, idRight, 1);
    leftGrid = new CGridSceneNode(parent, smgr, idLeft, 1);
    downGrid = new CGridSceneNode(parent, smgr, idDown, 1);
    frontGrid = new CGridSceneNode(parent, smgr, idFront, 1);
    backGrid = new CGridSceneNode(parent, smgr, idBack, 1);

    rightGrid->setRotation(irr::core::vector3df(0.0f, 0.0f, 90.0f));
    leftGrid->setRotation(irr::core::vector3df(0.0f, 0.0f, 90.0f));
    frontGrid->setRotation(irr::core::vector3df(90.0f, 0.0f, 0.0f));
    backGrid->setRotation(irr::core::vector3df(90.0f, 0.0f, 0.0f));

    // set right, left, back & front grid nodes as children for down grid node
    // so changing position will be easier
    downGrid->addChild(rightGrid);
    downGrid->addChild(leftGrid);
    downGrid->addChild(frontGrid);
    downGrid->addChild(backGrid);
}

CGridBox::~CGridBox() {
}

void CGridBox::setPosition(const irr::core::vector3df& newposition) {
    downGrid->setPosition(newposition);
}

const irr::core::vector3df& CGridBox::getPosition() const {
    return downGrid->getPosition();
}

void CGridBox::setColour(irr::video::SColor newcolour) {
    rightGrid->getGrid(0).setGridColor(newcolour);
    leftGrid->getGrid(0).setGridColor(newcolour);
    downGrid->getGrid(0).setGridColor(newcolour);
    frontGrid->getGrid(0).setGridColor(newcolour);
    backGrid->getGrid(0).setGridColor(newcolour);
}

void CGridBox::setGridsSize(const irr::core::dimension2df& newsize) {
    downGrid->setGridsSize(newsize);
    if (newsize.Height < newsize.Width) {
        rightGrid->setGridsSize(irr::core::dimension2df(newsize.Height, newsize.Height));
        rightGrid->setPosition(irr::core::vector3df(newsize.Width / 2, newsize.Height / 2, 0.0f));

        leftGrid->setGridsSize(irr::core::dimension2df(newsize.Height, newsize.Height));
        leftGrid->setPosition(irr::core::vector3df(-newsize.Width / 2, newsize.Height / 2, 0.0f));

        frontGrid->setGridsSize(newsize);
        frontGrid->setPosition(irr::core::vector3df(0.0f, newsize.Height / 2, newsize.Height / 2));

        backGrid->setGridsSize(newsize);
        backGrid->setPosition(irr::core::vector3df(0.0f, newsize.Height / 2, -newsize.Height / 2));
    } else if (newsize.Height > newsize.Width) {
        rightGrid->setGridsSize(newsize);
        rightGrid->setPosition(irr::core::vector3df(newsize.Width / 2, newsize.Width / 2, 0.0f));

        leftGrid->setGridsSize(newsize);
        leftGrid->setPosition(irr::core::vector3df(-newsize.Width / 2, newsize.Width / 2, 0.0f));

        frontGrid->setGridsSize(irr::core::dimension2df(newsize.Width, newsize.Width));
        frontGrid->setPosition(irr::core::vector3df(0.0f, newsize.Width / 2, newsize.Height / 2));

        backGrid->setGridsSize(irr::core::dimension2df(newsize.Width, newsize.Width));
        backGrid->setPosition(irr::core::vector3df(0.0f, newsize.Width / 2, -newsize.Height / 2));
    } else {
        rightGrid->setGridsSize(newsize);
        rightGrid->setPosition(irr::core::vector3df(newsize.Width / 2, newsize.Height / 2, 0.0f));

        leftGrid->setGridsSize(newsize);
        leftGrid->setPosition(irr::core::vector3df(-newsize.Width / 2, newsize.Height / 2, 0.0f));

        frontGrid->setGridsSize(newsize);
        frontGrid->setPosition(irr::core::vector3df(0.0f, newsize.Height / 2, newsize.Width / 2));

        backGrid->setGridsSize(newsize);
        backGrid->setPosition(irr::core::vector3df(0.0f, newsize.Height / 2, -newsize.Width / 2));
    }

    //leftGrid->setRotation(core::vector3df(0, 0, 0));
}

irr::core::dimension2df CGridBox::getGridsSize() const {
    return downGrid->getGridsSize();
}

void CGridBox::setSpacing(irr::f32 newspacing) {
    rightGrid->getGrid(0).setSpacing(newspacing);
    leftGrid->getGrid(0).setSpacing(newspacing);
    downGrid->getGrid(0).setSpacing(newspacing);
    frontGrid->getGrid(0).setSpacing(newspacing);
    backGrid->getGrid(0).setSpacing(newspacing);
}

irr::f32 CGridBox::getSpacing() const {
    return downGrid->getGrid(0).getSpacing();
}

// so all grids are visible

void CGridBox::turnOn3DMode() {
    /* downGrid->setVisible(true);
     rightGrid->setVisible(true);
     frontGrid->setVisible(true);
     backGrid->setVisible(true);*/
}

// so visible only left grid

void CGridBox::turnOn2DMode() {
    /*downGrid->setVisible(false);
    rightGrid->setVisible(false);
    frontGrid->setVisible(false);
    backGrid->setVisible(false);*/

    /*downGrid->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);       
    downGrid->setMaterialFlag(video::EMF_LIGHTING, false);
    smgr->getMeshManipulator()->setVertexColorAlpha(downGrid->getGrid(0).g(), 0);*/
}
