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
    CUserInputEventReceiver(CGameFieldManager* gameFieldManager, bool twoDimensionalMode = true, CFigure* figure = NULL);    
    virtual ~CUserInputEventReceiver();
    

    virtual bool OnEvent(const irr::SEvent& event);
    
    CFigure* getControllingFigure() const;

    void setControllingFigure(CFigure* newFigure);

    bool getTwoDimensionalMode() const;

    void setTwoDimensionalMode(bool newValue);
    
    bool isPause() const;
    void setPause(bool newPauseValue);

private:
    bool outOfBottomFieldBorders(irr::core::vector3di position);
    bool outOfFieldBorders(irr::core::vector3di position);
    void moveFigureInDirection(irr::core::vector3di directionStep);
    void clipFigureRotation(irr::core::vector3df& figureRotation);
    void rotateFigureInDirection(irr::core::vector3df directionStep);

private:
    CGameFieldManager* gameFieldManager;
    bool twoDimensionalMode;
    CFigure* figure;
    bool pause;
};

#endif	/* CUSERINPUTRECEIVER_H */

