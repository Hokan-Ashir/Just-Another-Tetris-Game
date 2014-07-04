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
    // TODO make platform independent
    texturePath = "media/textures";
    modelsPath = "media/models";

    // SCOL_BLACK
    colours.push_back(irr::video::SColor(255, 0, 0, 0));
    // SCOL_BLUE
    colours.push_back(irr::video::SColor(255, 0, 0, 255));
    // SCOL_CYAN
    colours.push_back(irr::video::SColor(255, 0, 255, 255));
    // SCOL_GRAY
    colours.push_back(irr::video::SColor(255, 128, 128, 128));
    // SCOL_GREEN
    colours.push_back(irr::video::SColor(255, 0, 255, 0));
    // SCOL_MAGENTA
    colours.push_back(irr::video::SColor(255, 255, 0, 255));
    // SCOL_RED
    colours.push_back(irr::video::SColor(255, 255, 0, 0));
    // SCOL_YELLOW
    colours.push_back(irr::video::SColor(255, 255, 255, 0));
    // SCOL_WHITE
    colours.push_back(irr::video::SColor(255, 255, 255, 255));
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

    irr::io::path workingDirectory = fileSystem->getWorkingDirectory();

    // get random model
    fileSystem->changeWorkingDirectoryTo(modelsPath);
    irr::io::IFileList* fileList = fileSystem->createFileList();
    /*std::cout << fileList->getFileCount() << std::endl;
    for (irr::u32 i = 0; i < fileList->getFileCount(); ++i) {
        std::cout << fileList->isDirectory(i) << std::endl;
        std::cout << fileList->getFileName(i).c_str() << std::endl;
    }*/

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
        fileSystem->changeWorkingDirectoryTo(workingDirectory);
        fileList->drop();
        return NULL;
    }

    fileSystem->changeWorkingDirectoryTo(workingDirectory);
    // get model texture
    fileSystem->changeWorkingDirectoryTo(texturePath);
    fileList = fileSystem->createFileList();
    /*std::cout << fileList->getFileCount() << std::endl;
    for (irr::u32 i = 0; i < fileList->getFileCount(); ++i) {
        std::cout << fileList->isDirectory(i) << std::endl;
        std::cout << fileList->getFileName(i).c_str() << std::endl;
    }*/
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
        fileSystem->changeWorkingDirectoryTo(workingDirectory);
        fileList->drop();
        return NULL;
    }

    std::cout << modelFilePath.c_str() << std::endl;
    std::cout << texturePath.c_str() << std::endl;
    CFigure* newFigure = new CFigure(figuresParent, sceneManager, -1, fileSystem, figureSize, modelFilePath, nodeType);
    newFigure->applyTextureToFigure(texturePath, colour);
    fileSystem->changeWorkingDirectoryTo(workingDirectory);
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
        figureColour = colours[irr::core::abs_(randomizer->rand()) % 255];
    }
    return createNewFigure(figureType, figureSize, figureColour);
}

irr::scene::ISceneManager* CGameFiguresFactory::getSceneManager() const {
    return sceneManager;
}

irr::scene::ISceneNode* CGameFiguresFactory::getFiguresParentNode() const {
    return figuresParent;
}

CGameFiguresFactory::~CGameFiguresFactory() {
}

