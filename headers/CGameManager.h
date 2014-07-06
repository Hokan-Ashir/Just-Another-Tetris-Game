/* 
 * File:   CGameManager.h
 * Author: hokan
 *
 * Created on 1 Июль 2014 г., 13:28
 */

#ifndef CGAMEMANAGER_H
#define	CGAMEMANAGER_H

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <IGUIStaticText.h>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"

#include "headers/CGameFieldManager.h"
#include "headers/CGameFiguresFactory.h"
#include "headers/CFigure.h"
#include "CUserInputEventReceiver.h"

/**
 * Control all game - manage spawning new figures on field, manage game speed, score, end, etc.
 */
class CGameManager {
public:
    /**
     * Constructor
     * 
     * @param guiEnvironment - Irrlicht gui environment
     * @param gameFieldManager - game field manager
     * @param gameFiruresFactory - figures factory
     * @param userInputReceiver - user input receiver
     * @param twoDimensionalMode - true if game will proceed in two dimensions, false otherwise
     * @param fieldOrigin - world position of field 0,0,0-coordinates
     * @param fieldAABB - field axis aligned bounding box, with which all figures will collide
     */
    CGameManager(irr::gui::IGUIEnvironment* guiEnvironment, CGameFieldManager* gameFieldManager, CGameFiguresFactory* gameFiruresFactory, CUserInputEventReceiver* userInputReceiver, bool twoDimensionalMode, irr::core::vector3df fieldOrigin, irr::core::aabbox3df fieldAABB);
    
    /**
     * Destructor, not implemented
     */
    virtual ~CGameManager();

    /**
     * Deletes already constructed lines
     * This method also affect 3d mode, so its naming is fine
     * 
     * @return number of deleted lines 
     */
    irr::u32 deleteConstuctedLines();

    /**
     * Spawns new figure on field
     * 
     * @param figureType - figure game field type
     * @param figureSize - figure size
     * @return true, if figure is spawned, false otherwise
     */
    bool spawnNewFigure(EGF_FIGURE_TYPE figureType, irr::f32 figureSize);
    
    /**
     * Spawns new figure on field
     * 
     * @param figureType - figure game field type
     * @return true, if figure is spawned, false otherwise
     */
    bool spawnNewFigure(EGF_FIGURE_TYPE figureType);
    
    /**
     * Getter
     * 
     * @return current game speed (delay in milliseconds, between animator steps)
     */
    irr::f32 getGameSpeed() const;
    
    /**
     * Setter
     * 
     * @param newSpeed - new game speed (delay in milliseconds, between animator steps)
     */
    void setGameSpeed(irr::f32 newSpeed);
    
    /**
     * Getter
     * 
     * @return new spawning figures size
     */
    irr::f32 getFigureSize() const;
    
    /**
     * Setter
     * 
     * @param newFigureSize - new spawning figures size
     */
    void setFigureSize(irr::f32 newFigureSize);
    
    /**
     * Getter
     * 
     * @return spawn point of new figures
     */
    irr::core::vector3di getSpawnPoint() const;
    
    /**
     * Setter
     * 
     * @param newSpawnPoint - new figures spawn point
     */
    void setSpawnPoint(irr::core::vector3di newSpawnPoint);
    
    /**
     * Getter
     * 
     * @return current game score
     */
    irr::u32 getGameScore() const;
    
    /**
     * Setter
     * 
     * @param newGameScore - new game scrore value
     */
    void setGameScore(irr::u32 newGameScore);
    
    /**
     * Getter
     * 
     * @return true, if game in two dimensional mode
     */
    bool getTwoDimensionalMode() const;
    
    /**
     * Setter
     * 
     * @param twoDimensionalMode - new two dimensional mode
     */
    void setTwoDimensionalMode(bool twoDimensionalMode);
    
    /**
     * Getter
     * 
     * @return true, if game on pause, false otherwise
     */
    bool isPause() const;
    
    /**
     * Setter
     * 
     * @param newPauseValue - new pause value
     */
    void setPauseValue(bool newPauseValue);
    
    /**
     * Getter
     * 
     * @return true, if game is over, false otherwise
     */
    bool isGameOver() const;
    
    /**
     * Setter
     * 
     * @param newGameOverValue - new game over value
     */
    void setGameOverValue(bool newGameOverValue);
    
    /**
     * Calculates game scrore based on lines to remove and game speed
     * 
     * @param numberOfDeletedLines - lines that have been deleted
     * @return new game score
     */
    irr::u32 calculateGameScore(irr::u32 numberOfDeletedLines);
    
    // TODO softly remove
    /**
     * Getter
     * 
     * @return grid box position
     */
    irr::core::vector3df getGridBoxPosition() const {
        return gridBoxPosition;
    }

    /**
     * game loop - delete constructed lines, calculate score, spawn new figure
     * 
     * @return true if game still proceeds, false if game is over
     */
    bool runOneLoop();
        
private:
    /**
     * game field manager
     */
    CGameFieldManager* gameFieldManager;
    
    /**
     * new figures factory
     */
    CGameFiguresFactory* gameFiruresFactory;
    
    /**
     * user input receiver
     */
    CUserInputEventReceiver* userInputReceiver;
    
    /**
     * array of figures, currently presents on field
     */
    irr::core::array<CFigure*> figures;
    
    /**
     * true, if game proceeds in two dimensions, false otherwise
     */
    bool twoDimensionalMode;   
    
    /**
     * current game speed (delay in milliseconds, between animator steps
     */
    irr::f32 speed;
    
    /**
     * new spawning figures size
     */
    irr::f32 figureSize;
    
    /**
     * game field coordinates of new spawning figures
     */
    irr::core::vector3di spawnPoint;
    
    /**
     * current game score
     */
    irr::u32 gameScore;
            
    /**
     * point in world coordinates where (0,0,0)-field point would be spawned
     */
    irr::core::vector3df fieldOrigin;  
    
    /**
     * field axis aligned bounding box, with which figures interacts
     */
    irr::core::aabbox3df aabb;
    
    /**
     * grid box position
     */
    irr::core::vector3df gridBoxPosition;
    
    /**
     * Irrlicht gui environment
     */
    irr::gui::IGUIEnvironment* guiEnvironment;
    
    /**
     * victory or scrore text
     */
    irr::gui::IGUIStaticText* text;
    
    /**
     * true, if game is over, false otherwise
     */
    bool gameOver;
    
    /**
     * true, if all animatros of all existing figures on field stopped
     */
    bool animatorsStopped;
};

#endif	/* CGAMEMANAGER_H */

