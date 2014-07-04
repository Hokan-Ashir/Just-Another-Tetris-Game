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

class CGameApplication {
public:
    CGameApplication();    
    void initialize(irr::IrrlichtDevice* device);
    bool gameLoopIteration();
    virtual ~CGameApplication();
private:
    irr::scene::ICameraSceneNode* camera;
    
    CGameFieldManager* gameFieldManager;
    CGameFiguresFactory* gameFiguresFactory;
    CGameManager* gameManager;
    
    CUserInputEventReceiver* userInputEventReceiver;
};

#endif	/* CGAMEAPPLICATION_H */

