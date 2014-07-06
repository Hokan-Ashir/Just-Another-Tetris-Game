/* 
 * File:   CGameFiguresFactory.h
 * Author: hokan
 *
 * Created on 1 Июль 2014 г., 16:45
 */

#ifndef CGAMEFIGURESFACTORY_H
#define	CGAMEFIGURESFACTORY_H

#include "headers/CFigure.h"
#include <IRandomizer.h>

/**
 * Spawn new figures (no game field interaction), no created figures time live control
 */
class CGameFiguresFactory {
public:
    
    /**
     * Constructor
     * 
     * @param randomizer - pointer to Irrlicht randomizer, for selecting random figures and colours
     * @param figuresParent - all figures parent node
     * @param sceneManager - scene manager of scene
     * @param fileSystem - Irrlicht file system
     */
    CGameFiguresFactory(irr::IRandomizer* randomizer, irr::scene::ISceneNode* figuresParent, irr::scene::ISceneManager* sceneManager, irr::io::IFileSystem* fileSystem);
    
    /**
     * Destructor, not implemented
     */
    virtual ~CGameFiguresFactory();
    
    /**
     * Creates new figure
     * 
     * @param figureType - game field figure's elemental block type
     * @param figureSize - figure size
     * @param colour - figure colour
     * @return pointer to created figure, NULL if creation failed
     */
    CFigure* createNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize, irr::video::SColor colour);
    
    /**
     * Created new figure
     * 
     * @param figureType - game field figure's elemental block type
     * @param figureSize - figure size
     * @param randomColour - if set to true, factory creates random colour with randomizer and attach it to figure
     * @return pointer to created figure, NULL if creation failed
     */
    CFigure* createNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize, bool randomColour = false);    
    
    /**
     * Getter
     * 
     * @return pointer to scene manager
     */
    irr::scene::ISceneManager* getSceneManager() const;
    
    /**
     * Getter
     * 
     * @return pointer to all figures parent node
     */
    irr::scene::ISceneNode* getFiguresParentNode() const;
    
    /**
     * Getter
     * 
     * @return pointer to Irrlicht file system
     */
    irr::io::IFileSystem* getFileSystem() const;
private:
    
    /**
     * array of predefined colours
     */
    irr::core::array<irr::video::SColor> colours;    
    
    /**
     * randomizer interface, used to select random figures and colours
     */
    irr::IRandomizer* randomizer;
    
    /**
     * parent to all figures
     */
    irr::scene::ISceneNode* figuresParent;
    
    /**
     * scene manager
     */
    irr::scene::ISceneManager* sceneManager;
    
    /**
     * Irrlicht file system
     */
    irr::io::IFileSystem* fileSystem;   
};

#endif	/* CGAMEFIGURESFACTORY_H */

