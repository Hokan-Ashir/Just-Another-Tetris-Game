/* 
 * File:   CGameApplication.h
 * Author: hokan
 *
 * Created on 2 Июль 2014 г., 16:54
 */

#ifndef CGAMEAPPLICATION_H
#define	CGAMEAPPLICATION_H

#include "CGameManager.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <IrrlichtDevice.h>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"

/**
 * Whole game application class - creates all game object and indicates of
 * game over
 */
class CGameApplication {
public:
    
    /**
     * Constructor, not implemented
     */
    CGameApplication();    
    
    /**
     * Create all game objects, initialize file system, game field, spawning figures factory, game manager etc.
     * 
     * @param device - pointer to Irrlicht device
     * @return true if initialization cause no errors, false otherwise
     */
    bool initialize(irr::IrrlichtDevice* device);
    
    /**
     * Iterate one game loop (check for constructed lines/planes, calculating scrore, spawning new figure)
     * @see CGameManager
     * 
     * @return 
     */
    bool gameLoopIteration();
    
    /**
     * Destructor, delete all created on heap objects     
     */
    virtual ~CGameApplication();
private:
    
    /**
     * game camera (static, for now)
     */
    irr::scene::ICameraSceneNode* cameraSceneNode;
    
    /**
     * pointer to Irrlicht device
     */
    irr::IrrlichtDevice* device;
    
    /**
     * game field manager, manage game field access, calculating created lines etc.
     */
    CGameFieldManager* gameFieldManager;
    
    /**
     * game figures factory, spawns new figures
     */
    CGameFiguresFactory* gameFiguresFactory;
    
    /**
     * game manager, control game - delete figures, calculate score, manage spawning figures
     */
    CGameManager* gameManager;
    
    /**
     * user input receiver, manage user input
     */
    CUserInputEventReceiver* userInputEventReceiver;
};

#endif	/* CGAMEAPPLICATION_H */

