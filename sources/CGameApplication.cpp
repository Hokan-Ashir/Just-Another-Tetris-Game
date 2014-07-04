/* 
 * File:   CGameApplication.cpp
 * Author: hokan
 * 
 * Created on 2 Июль 2014 г., 16:55
 */

#include "headers/CGameApplication.h"

#include "headers/CMoveDownAnimator.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <ICameraSceneNode.h>
#include <ITerrainSceneNode.h>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"

#include <iostream>

CGameApplication::CGameApplication() {
}

CGameApplication::~CGameApplication() {
    delete gameFieldManager;
    delete gameFiguresFactory;
    delete gameManager;

    delete userInputEventReceiver;
}

bool CGameApplication::initialize(irr::IrrlichtDevice* device) {
    this->device = device;
    irr::scene::ISceneManager *sceneManager = device->getSceneManager();
    irr::video::IVideoDriver *videoDriver = device->getVideoDriver();

    videoDriver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);

    cameraSceneNode = sceneManager->addCameraSceneNode();
    cameraSceneNode->setPosition(irr::core::vector3df(2700 * 2, 255 * 2, 3250 * 2));
    cameraSceneNode->setFarValue(42000.0f);

    irr::io::IFileSystem* fileSystem = device->getFileSystem();
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
        return false;
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
        return false;
    }
    
    // add terrain scene node
    irr::scene::ITerrainSceneNode* terrain = sceneManager->addTerrainSceneNode(
            "terrain-heightmap.bmp",
            0, // parent node
            -1, // node id
            irr::core::vector3df(0.f, 0.f, 0.f),
            irr::core::vector3df(0.f, 0.f, 0.f),
            irr::core::vector3df(40.f, 4.4f, 40.f),
            irr::video::SColor(255, 255, 255, 255),
            5,
            irr::scene::ETPS_17,
            4
            );

    terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    
    terrain->setMaterialTexture(0, videoDriver->getTexture("terrain-texture.jpg"));
    terrain->setMaterialTexture(1, videoDriver->getTexture("detailmap3.jpg"));

    terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);

    terrain->scaleTexture(1.0f, 20.0f);

    // create skybox and skydome
    videoDriver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);

    sceneManager->addSkyBoxSceneNode(
            videoDriver->getTexture("irrlicht2_up.jpg"),
            videoDriver->getTexture("irrlicht2_dn.jpg"),
            videoDriver->getTexture("irrlicht2_lf.jpg"),
            videoDriver->getTexture("irrlicht2_rt.jpg"),
            videoDriver->getTexture("irrlicht2_ft.jpg"),
            videoDriver->getTexture("irrlicht2_bk.jpg"));
    
    fileList->drop();
    fileSystem->changeWorkingDirectoryTo(defaultDirectory);
    
    videoDriver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);

    irr::core::vector3df cameraPosition = cameraSceneNode->getPosition();

    irr::IRandomizer* randomizer = device->getRandomizer();
    randomizer->reset(device->getTimer()->getRealTime());
    gameFiguresFactory = new CGameFiguresFactory(randomizer, sceneManager->getRootSceneNode(), sceneManager, fileSystem);

    irr::core::vector3df gameOrigin(cameraPosition.X, terrain->getPosition().Y, cameraPosition.Z);

    bool twoDimensionalMode = true;
    gameFieldManager = new CGameFieldManager(10, 10, 10);

    // create event receiver
    userInputEventReceiver = new CUserInputEventReceiver(gameFieldManager, twoDimensionalMode);
    device->setEventReceiver(userInputEventReceiver);

    gameManager = new CGameManager(device->getGUIEnvironment(), gameFieldManager, gameFiguresFactory, userInputEventReceiver, twoDimensionalMode, gameOrigin, terrain->getBoundingBox());
    irr::core::vector3df cameraTarget(gameManager->getGridBoxPosition());
    cameraTarget.Z += gameFieldManager->getFieldSize().Z / 2 * gameManager->getFigureSize() * gameManager->getFigureSize();
    cameraTarget.Y += gameFieldManager->getFieldSize().Y / 2 * gameManager->getFigureSize() * gameManager->getFigureSize();
    cameraSceneNode->setTarget(cameraTarget);
    cameraTarget.Z = gameManager->getGridBoxPosition().Z - 1300;
    cameraSceneNode->setPosition(cameraTarget);       
    
    return true;
}

bool CGameApplication::gameLoopIteration() {    
    bool result = gameManager->runOneLoop();
    if (gameManager->isGameOver() && device->getEventReceiver() != NULL) {        
        device->setEventReceiver(NULL);
        delete userInputEventReceiver;
    }
    return result;
}

