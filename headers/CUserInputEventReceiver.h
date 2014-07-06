/* 
 * File:   CUserInputReceiver.h
 * Author: hokan
 *
 * Created on 2 Июль 2014 г., 16:37
 */

#ifndef CUSERINPUTRECEIVER_H
#define	CUSERINPUTRECEIVER_H

#include "CGameFieldManager.h"
#include "CFigure.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <IEventReceiver.h>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"

class CUserInputEventReceiver : public irr::IEventReceiver {
public:
    /**
     * Constructor
     * 
     * @param gameFieldManager - game field manager
     * @param twoDimensionalMode - true, if game proceeds in two dimensions, false otherwise
     * @param figure - figure to manipulate
     */
    CUserInputEventReceiver(CGameFieldManager* gameFieldManager, bool twoDimensionalMode = true, CFigure* figure = NULL);  
    
    /**
     * Destructor, not implemented
     */
    virtual ~CUserInputEventReceiver();
    
    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual bool OnEvent(const irr::SEvent& event);
    
    /**
     * Getter
     * 
     * @return pointer to controlling figure
     */
    CFigure* getControllingFigure() const;

    /**
     * Setter
     * 
     * @param newFigure - pointer to new controlling figure
     */
    void setControllingFigure(CFigure* newFigure);

    /**
     * Getter
     * 
     * @return true, if game proceeds in two dimensions, false otherwise
     */
    bool getTwoDimensionalMode() const;

    /**
     * Setter
     * 
     * @param newValue - new two dimensions mode value
     */
    void setTwoDimensionalMode(bool newValue);
    
    /**
     * Getter
     * 
     * @return true, if game is paused, false otherwise
     */
    bool isPause() const;
    
    /**
     * Setter
     * 
     * @param newPauseValue - new value for paused flag
     */
    void setPause(bool newPauseValue);

private:
    /**
     * Checks if game field position if out of bottom field borders
     * 
     * @param position - position to check
     * @return true, if game field position out of borders, false otherwise
     */
    bool outOfBottomFieldBorders(irr::core::vector3di position);
    
    /**
     * Checks if game field position if out of field borders
     * 
     * @param position - position to check
     * @return true, if game field position out of borders, false otherwise
     */
    bool outOfFieldBorders(irr::core::vector3di position);
    
    /**
     * Moves figure in specific direction, inclued game field interaction
     * 
     * @param directionStep - direction to move figure (not end point)
     */
    void moveFigureInDirection(irr::core::vector3di directionStep);
    
    /**
     * Clips figure rotation if it greated 360 or less -360, in any axis
     * 
     * @param figureRotation - figure rotation to clip (also out value)
     */
    void clipFigureRotation(irr::core::vector3df& figureRotation);
    
    /**
     * Rotate figure in specific diretcion
     * 
     * @param directionStep - direction to rotate figure (not end point)
     */
    void rotateFigureInDirection(irr::core::vector3df directionStep);

private:
    /**
     * game manager
     */
    CGameFieldManager* gameFieldManager;
    
    /**
     * true, if game proceeds in two dimensions, false otherwise
     */
    bool twoDimensionalMode;
    
    /**
     * pointer to controlling figure
     */
    CFigure* figure;
    
    /**
     * true, if game is paused, false otherwise
     */
    bool pause;
};

#endif	/* CUSERINPUTRECEIVER_H */

