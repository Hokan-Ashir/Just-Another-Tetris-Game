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

class CGameFiguresFactory {
public:
    CGameFiguresFactory(irr::IRandomizer* randomizer, irr::scene::ISceneNode* figuresParent, irr::scene::ISceneManager* sceneManager, irr::io::IFileSystem* fileSystem);
    virtual ~CGameFiguresFactory();
    
    CFigure* createNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize, irr::video::SColor colour);
    CFigure* createNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize, bool randomColour = false);    
    
    irr::scene::ISceneManager* getSceneManager() const;
    irr::scene::ISceneNode* getFiguresParentNode() const;
    irr::io::IFileSystem* getFileSystem() const;
private:
    irr::core::array<irr::video::SColor> colours;    
    irr::IRandomizer* randomizer;
    irr::scene::ISceneNode* figuresParent;
    irr::scene::ISceneManager* sceneManager;
    irr::io::IFileSystem* fileSystem;
    
    irr::io::path texturePath;
    irr::io::path modelsPath;
};

#endif	/* CGAMEFIGURESFACTORY_H */

