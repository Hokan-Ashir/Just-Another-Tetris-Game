/* 
 * File:   CGameFiguresFactory.cpp
 * Author: hokan
 * 
 * Created on 1 Июль 2014 г., 16:45
 */

#include "headers/CGameFiguresFactory.h"

#include <iostream>

CGameFiguresFactory::CGameFiguresFactory(irr::IRandomizer* randomizer, irr::scene::ISceneNode* figuresParent, irr::scene::ISceneManager* sceneManager, irr::io::IFileSystem* fileSystem)
: randomizer(randomizer)
, figuresParent(figuresParent)
, sceneManager(sceneManager)
, fileSystem(fileSystem) {
    // SCOL_BLUE
    colours.push_back(irr::video::SColor(255, 100, 100, 255));
    // SCOL_CYAN
    colours.push_back(irr::video::SColor(255, 100, 255, 255));    
    // SCOL_GREEN
    colours.push_back(irr::video::SColor(255, 100, 255, 100));
    // SCOL_MAGENTA
    colours.push_back(irr::video::SColor(255, 255, 100, 255));
    // SCOL_RED
    colours.push_back(irr::video::SColor(255, 255, 100, 100));
    // SCOL_YELLOW
    colours.push_back(irr::video::SColor(255, 255, 255, 100));
}

CFigure* CGameFiguresFactory::createNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize, irr::video::SColor colour) {
    irr::io::path figurePath;
    irr::scene::ESCENE_NODE_TYPE nodeType;
    if (figureType == EGF_CUBE) {
        nodeType = irr::scene::ESNT_CUBE;
        figurePath = "cube";
    } else if (figureType == EGF_SPHERE) {
        nodeType = irr::scene::ESNT_SPHERE;
        figurePath = "sphere";
    } else {
        nodeType = irr::scene::ESNT_CUBE;
        figurePath = "cube";
    }

    irr::io::path defaultDirectory = fileSystem->getWorkingDirectory();         
    irr::io::IFileList* fileList = fileSystem->createFileList();
    bool directoryFound = false;
    for (irr::u32 i = 0; i < fileList->getFileCount(); ++i) {
        if (fileList->getFileName(i).equals_ignore_case("media") && fileList->isDirectory(i)) {
            fileSystem->changeWorkingDirectoryTo(fileList->getFullFileName(i));
            directoryFound = true;
            break;
        }
    } 
    
    if (!directoryFound) {
        fileList->drop();
        return NULL;
    }
    
    directoryFound = false;    
    fileList = fileSystem->createFileList();
    for (irr::u32 i = 0; i < fileList->getFileCount(); ++i) {
        if (fileList->getFileName(i).equals_ignore_case("models") && fileList->isDirectory(i)) {
            fileSystem->changeWorkingDirectoryTo(fileList->getFullFileName(i));
            directoryFound = true;
            break;
        }
    }  
    
    if (!directoryFound) {
        fileList->drop();
        return NULL;
    }

    // get random model
    fileList = fileSystem->createFileList();
    irr::io::path modelFilePath;
    while (true) {
        irr::u32 randomIndex = irr::core::abs_(randomizer->rand()) % fileList->getFileCount();        
        if (fileList->isDirectory(randomIndex)) {
            continue;
        } else {
            modelFilePath = fileList->getFullFileName(randomIndex);
            break;
        }

    }

    // model not found
    if (modelFilePath == "") {
        fileSystem->changeWorkingDirectoryTo(defaultDirectory);
        fileList->drop();
        return NULL;
    }        

    fileSystem->changeWorkingDirectoryTo(defaultDirectory);
    fileList = fileSystem->createFileList();
    directoryFound = false;
    for (irr::u32 i = 0; i < fileList->getFileCount(); ++i) {
        if (fileList->getFileName(i).equals_ignore_case("media") && fileList->isDirectory(i)) {
            fileSystem->changeWorkingDirectoryTo(fileList->getFullFileName(i));
            directoryFound = true;
            break;
        }
    } 
    
    if (!directoryFound) {
        fileList->drop();
        return NULL;
    }
    directoryFound = false;
    
    fileList = fileSystem->createFileList();
    for (irr::u32 i = 0; i < fileList->getFileCount(); ++i) {
        if (fileList->getFileName(i).equals_ignore_case("textures") && fileList->isDirectory(i)) {
            fileSystem->changeWorkingDirectoryTo(fileList->getFullFileName(i));
            directoryFound = true;
            break;
        }
    }  
    
    if (!directoryFound) {
        fileList->drop();
        return NULL;
    }
    
    // get model texture
    fileList = fileSystem->createFileList();
    irr::io::path texturePath;
    for (irr::u32 i = 0; i < fileList->getFileCount(); ++i) {
        if (fileList->isDirectory(i)) {
            continue;
        }

        if (fileList->getFileName(i).equals_ignore_case(irr::io::path(figurePath + ".png"))) {
            texturePath = fileList->getFullFileName(i);
            break;
        }
    }

    // texture not found
    if (texturePath == "") {
        fileSystem->changeWorkingDirectoryTo(defaultDirectory);
        fileList->drop();
        return NULL;
    }

    CFigure* newFigure = new CFigure(figuresParent, sceneManager, -1, fileSystem, figureSize, modelFilePath, nodeType);
    newFigure->applyTextureToFigure(texturePath, colour);
    fileSystem->changeWorkingDirectoryTo(defaultDirectory);
    fileList->drop();
    return newFigure;
}

CFigure* CGameFiguresFactory::createNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize, bool randomColour) {
    irr::video::SColor figureColour;
    if (randomColour) {
        // SColor set method will grab least 0xFF bits of generated number
        figureColour.set(255, (irr::u32)irr::core::abs_(randomizer->rand()),
                (irr::u32)irr::core::abs_(randomizer->rand()),
                (irr::u32)irr::core::abs_(randomizer->rand()));
    } else {
        figureColour = colours[irr::core::abs_(randomizer->rand()) % colours.size()];        
    }
    return createNewFigure(figureType, figureSize, figureColour);
}

irr::scene::ISceneManager* CGameFiguresFactory::getSceneManager() const {
    return sceneManager;
}

irr::scene::ISceneNode* CGameFiguresFactory::getFiguresParentNode() const {
    return figuresParent;
}

irr::io::IFileSystem* CGameFiguresFactory::getFileSystem() const {
    return fileSystem;
}

CGameFiguresFactory::~CGameFiguresFactory() {
}

