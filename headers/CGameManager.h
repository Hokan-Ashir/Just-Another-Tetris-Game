/* 
 * File:   CGameManager.h
 * Author: hokan
 *
 * Created on 1 Июль 2014 г., 13:28
 */

#ifndef CGAMEMANAGER_H
#define	CGAMEMANAGER_H

#include "headers/CGameFieldManager.h"
#include "headers/CGameFiguresFactory.h"
#include "headers/CFigure.h"
#include "CUserInputEventReceiver.h"

class CGameManager {
public:
    CGameManager(CGameFieldManager* gameFieldManager, CGameFiguresFactory* gameFiruresFactory, CUserInputEventReceiver* userInputReceiver, bool twoDimensionalMode, irr::core::vector3df fieldOrigin, irr::core::aabbox3df fieldAABB);
    virtual ~CGameManager();
    // thid method also affect 3d mode, so its naming is fine
    // return number of deleted lines
    irr::u32 deleteConstuctedLines();
    
    bool spawnNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize, irr::video::SColor colour);
    bool spawnNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize);
    bool spawnNewFigure(EGF_FIGURE_TYPE figureType);
    
    irr::f32 getGameSpeed() const;
    void setGameSpeed(irr::f32 newSpeed);
    
    irr::f32 getFigureSize() const;
    void setFigureSize(irr::f32 newFigureSize);
    
    irr::core::vector3di getSpawnPoint() const;
    void setSpawnPoint(irr::core::vector3di newSpawnPoint);
    
    irr::u32 getGameScore() const;
    void setGameScore(irr::u32 newGameScore);
    
    bool getTwoDimensionalMode() const;
    void setTwoDimensionalMode(bool twoDimensionalMode);
    
    irr::u32 calculateGameScore(irr::u32 numberOfDeletedLines);
    
    // TODO softly remove
    irr::core::vector3df getGridBoxPosition() const {
        return gridBoxPosition;
    }
    
    // game loop - delete constructed lines, calculate score, spawn new figure
    // returns true if game still proceeds, false if game is over
    bool runOneLoop();
    
    // TODO make private
    irr::core::array<CFigure*> figures;
private:
    CGameFieldManager* gameFieldManager;
    CGameFiguresFactory* gameFiruresFactory;
    CUserInputEventReceiver* userInputReceiver;
    bool twoDimensionalMode;   
    irr::f32 speed;
    irr::f32 figureSize;
    irr::core::vector3di spawnPoint;
    irr::u32 gameScore;
        
    // point in world coordinates where (0,0,0)-field point would be spawned
    irr::core::vector3df fieldOrigin;
        
    irr::core::aabbox3df aabb;
    irr::core::vector3df gridBoxPosition;
};

#endif	/* CGAMEMANAGER_H */

