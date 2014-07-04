/* 
 * File:   CMoveDownAnimator.h
 * Author: hokan
 *
 * Created on 29 Июнь 2014 г., 19:21
 */

#ifndef CMOVEDOWNANIMATOR_H
#define	CMOVEDOWNANIMATOR_H

#include "headers/CGameFieldManager.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <ISceneNodeAnimator.h>
#include <ISceneNode.h>
#include <aabbox3d.h>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"

class CMoveDownAnimator : public irr::scene::ISceneNodeAnimator {
public:
    CMoveDownAnimator(CGameFieldManager* gameFieldManager, bool twoDimensionalMode, irr::u32 step, irr::f32 delay, irr::core::aabbox3df aabb = irr::core::aabbox3df(irr::core::vector3df(0.0f, 0.0f, 0.0f)));

    virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs);

    virtual irr::scene::ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager);


    virtual ~CMoveDownAnimator();
    
    irr::u32 getStep() const;
    void setStep(irr::u32 step);
    
    irr::f32 getDelay() const;
    void setDelay(irr::f32 delay);
    
protected:
    irr::core::vector3di directionStep;
    irr::u32 delay;
    irr::core::aabbox3df aabb;

private:
    bool outOfFieldBorders(irr::core::vector3di position);
    bool outOfBottomFieldBorders(irr::core::vector3di position);
    private:
    CGameFieldManager* gameFieldManager;
    bool twoDimensionalMode;
    bool noBoundaries;
    bool firstUpdate;
    irr::u32 loopTime;

};

#endif	/* CMOVEDOWNANIMATOR_H */

