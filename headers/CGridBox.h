/* 
 * File:   CGridBox.h
 * Author: hokan
 *
 * Created on 2 Июль 2014 г., 12:42
 */

#ifndef CGRIDBOX_H
#define	CGRIDBOX_H

#include "headers/CGridSceneNode.h"

/**
 * Wrapped around CGridSceneNode class, which stores 5 instances in box-like style
 */
class CGridBox {
public:
    
    /**
     * Constructor
     * 
     * @param parent - parent of all grid nodes
     * @param smgr - scene manager
     * @param idRight - id of right grid node
     * @param idLeft - id of left grid node
     * @param idDown - id of down grid node
     * @param idFront - id of front grid node
     * @param idBack - id of back grid node
     */
    CGridBox(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* smgr, irr::s32 idRight = -1, irr::s32 idLeft = -1, irr::s32 idDown = -1, irr::s32 idFront = -1, irr::s32 idBack = -1);
    
    /**
     * Destructor, deletes all created on heap grid nodes
     */
    virtual ~CGridBox();

    /**
     * Setter
     * 
     * @param newposition - new position of grid box center
     */
    void setPosition(const irr::core::vector3df& newposition);
    
    /**
     * Getter
     * 
     * @return current position of grid box center
     */
    const irr::core::vector3df& getPosition() const;

    /**
     * Setter
     * 
     * @param newcolour - new colour of all grid nodes
     */
    void setColour(irr::video::SColor newcolour);

    /**
     * Setter
     * 
     * @param newsize - new grids size
     */
    void setGridsSize(const irr::core::dimension2df& newsize);
    
    /**
     * Getter
     * 
     * @return current grids size
     */
    irr::core::dimension2df getGridsSize() const;

    /**
     * Setter
     * 
     * @param newspacing - new value of all grids spacing
     */
    void setSpacing(irr::f32 newspacing);
    
    /**
     * Getter
     * 
     * @return current spacing of all grid nodes
     */
    irr::f32 getSpacing() const;

    // TODO implement
    /**
     * makes all grids visible, not implemented yet
     */
    void turnOn3DMode();
    
    // TODO implement
    /**
     * makes all grids, except right one invisible, not implemented yet
     */
    void turnOn2DMode();
private:
    
    /**
     * right grid scene node
     */
    CGridSceneNode* rightGrid;
    
    /**
     * left grid scene node
     */
    CGridSceneNode* leftGrid;
    
    /**
     * down grid scene node
     */
    CGridSceneNode* downGrid;
    
    /**
     * front grid scene node
     */
    CGridSceneNode* frontGrid;
    
    /**
     * back grid scene node
     */
    CGridSceneNode* backGrid;
};

#endif	/* CGRIDBOX_H */

