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

void CGameApplication::initialize(irr::IrrlichtDevice* device) {    
    irr::scene::ISceneManager *smgr = device->getSceneManager();
    irr::video::IVideoDriver *driver = device->getVideoDriver();
    
    driver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);

    camera =
            smgr->addCameraSceneNodeFPS(0, 100.0f, 1.2f);    

    camera->setPosition(irr::core::vector3df(2700 * 2, 255 * 2, 2600 * 2));    
    camera->setFarValue(42000.0f);    

    // add terrain scene node
    irr::scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
            "/home/hokan/Irrlicht/irrlicht-1.8/media/terrain-heightmap.bmp",
            0, // parent node
            -1, // node id
            irr::core::vector3df(0.f, 0.f, 0.f), // position
            irr::core::vector3df(0.f, 0.f, 0.f), // rotation
            irr::core::vector3df(40.f, 4.4f, 40.f), // scale
            irr::video::SColor(255, 255, 255, 255), // vertexColor
            5, // maxLOD
            irr::scene::ETPS_17, // patchSize
            4 // smoothFactor
            );

    terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);

    terrain->setMaterialTexture(0,
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/terrain-texture.jpg"));
    terrain->setMaterialTexture(1,
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/detailmap3.jpg"));

    terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);

    terrain->scaleTexture(1.0f, 20.0f);

    // create skybox and skydome
    driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);

    /*smgr->addSkyBoxSceneNode(
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_up.jpg"),
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_dn.jpg"),
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_lf.jpg"),
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_rt.jpg"),
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_ft.jpg"),
            driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/irrlicht2_bk.jpg"));*/
    smgr->addSkyDomeSceneNode(driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/skydome.jpg"), 16, 8, 0.95f, 2.0f);

    driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);

    irr::core::vector3df cameraPosition = camera->getPosition();

    //createFugureXmlFile(device);
    irr::IRandomizer* randomizer = device->getRandomizer();
    randomizer->reset(device->getTimer()->getRealTime());
    gameFiguresFactory = new CGameFiguresFactory(randomizer, smgr->getRootSceneNode(), smgr, smgr->getFileSystem());

    //CFigure* figure = new CFigure(smgr->getRootSceneNode(), smgr, -1, smgr->getFileSystem(), 10.0f, "media/models/tblock.xml", irr::scene::ESNT_CUBE);//gameFiguresFactory->createNewFigure(EGF_CUBE, 10.0f, true);
    //figure->applyTextureToFigure("media/textures/cube.png", irr::video::SColor(255, 100, 255, 100));
    irr::core::vector3df gameOrigin(cameraPosition.X, terrain->getPosition().Y, cameraPosition.Z);


    //figure->setDebugDataVisible(irr::scene::EDS_BBOX);

    bool twoDimensionalMode = true;
    gameFieldManager = new CGameFieldManager(10, 10, 10);

    // create event receiver
    userInputEventReceiver = new CUserInputEventReceiver(gameFieldManager, twoDimensionalMode);
    device->setEventReceiver(userInputEventReceiver);

    /*irr::core::vector3di fieldPosition(5, 5, 0);
    figure->setParentNodeFieldPosition(fieldPosition);
    for (irr::u32 i = 0; i < figure->getFieldPositions().size(); ++i) {
        std::cout << figure->getFieldPositions()[i].X << ", " << figure->getFieldPositions()[i].Y << ", " << figure->getFieldPositions()[i].Z << std::endl;
    }
    std::cout << "********" << std::endl;
    figure->setRotation(irr::core::vector3df(0, 0, -90));
    figure->constructNodesFieldPositions(fieldPosition);    
    figure->removeNodeFromFigure(figure->figureParentNode);    
    std::cout << "root pos, befor rot. " << figure->getParentNodeFieldPosition().X << ", " << figure->getParentNodeFieldPosition().Y << ", " << figure->getParentNodeFieldPosition().Z << std::endl;        
    for (irr::u32 i = 0; i < figure->getFieldPositions().size(); ++i) {
        std::cout << figure->getFieldPositions()[i].X << ", " << figure->getFieldPositions()[i].Y << ", " << figure->getFieldPositions()[i].Z << std::endl;
    }
    std::cout << (*figure->figureParentNode->getChildren().begin())->getPosition().X << ", " << (*figure->figureParentNode->getChildren().begin())->getPosition().Y << ", " << (*figure->figureParentNode->getChildren().begin())->getPosition().Z << std::endl;
    //std::cout << (*(*figure->figureParentNode->getChildren().begin())->getChildren().begin())->getPosition().X << ", " << (*(*figure->figureParentNode->getChildren().begin())->getChildren().begin())->getPosition().Y << ", " << (*(*figure->figureParentNode->getChildren().begin())->getChildren().begin())->getPosition().Z << std::endl;
    irr::core::vector3di offset(1, -2, 0);    
    //check offset like (X, Y, 0)
    //offset.rotateYZBy(-90, irr::core::vector3df(0, 0, 0));
    offset.rotateXYBy(180, irr::core::vector3di(0, 0, 0));
    
    std::cout << "new offset"<< std::endl;
    std::cout << offset.X << ", " << offset.Y << ", " << offset.Z << std::endl;
    irr::scene::ISceneNode* node;// = figure->getNodeByOffset(offset);
    if (node != NULL) {
        std::cout << "f" << std::endl;
    }
    if (offset.X < 0) {
        std::cout << "lz" << std::endl;
    } else if (offset.X == 0) {
        std::cout << "z" << std::endl;
    } else {
        std::cout << "gz" << std::endl;
    }
    
    irr::core::vector3df figurePosition(gameOrigin);
    figurePosition.X += 5 * 10.0f * 10.0f;
    figurePosition.Y += 5 * 10.0f * 10.0f + (10 * 10 / 2);
    figurePosition.Z += 0 * 10.0f * 10.0f;
    figure->setPosition(figurePosition);
    userInputEventReceiver->setControllingFigure(figure);*/
    terrain->setMaterialFlag(irr::video::EMF_WIREFRAME, true);

    gameManager = new CGameManager(gameFieldManager, gameFiguresFactory, userInputEventReceiver, twoDimensionalMode, gameOrigin, terrain->getBoundingBox());    
    irr::core::vector3df cameraTarget(gameManager->getGridBoxPosition());
    cameraTarget.Z += gameFieldManager->getFieldSize().Z / 2 * gameManager->getFigureSize() * gameManager->getFigureSize();
    cameraTarget.Y += gameFieldManager->getFieldSize().Y / 2 * gameManager->getFigureSize() * gameManager->getFigureSize();
    camera->setTarget(cameraTarget);
    cameraTarget.Z = gameManager->getGridBoxPosition().Z - 1200;
    camera->setPosition(cameraTarget);
    /*irr::scene::ISceneNodeAnimator* animator = new CMoveDownAnimator(gameFieldManager, twoDimensionalMode, 1, 500, terrain->getBoundingBox());
    figure->addAnimator(animator);
    animator->drop();*/
}

bool CGameApplication::gameLoopIteration() {
    //std::cout << "(" << camera->getPosition().X << ", " << camera->getPosition().Y << ", " << camera->getPosition().Z << ")" << std::endl;
    return gameManager->runOneLoop();
}

