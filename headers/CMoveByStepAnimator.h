/* 
 * File:   CMoveByStepAnimator.h
 * Author: hokan
 *
 * Created on 29 Июнь 2014 г., 17:38
 */

#ifndef CMOVEBYSTEPANIMATOR_H
#define	CMOVEBYSTEPANIMATOR_H

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <ISceneNodeAnimator.h>
#include <ISceneNode.h>
#include <aabbox3d.h>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"

class CMoveByStepAnimator : public irr::scene::ISceneNodeAnimator {
public:
    // if no aabb set, animator has no boundaries
    CMoveByStepAnimator(const irr::core::vector3df& directionStep, irr::u32 delay, irr::core::aabbox3df aabb = irr::core::aabbox3df(irr::core::vector3df(0.0f, 0.0f, 0.0f)));

    virtual irr::scene::ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager);

    virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs);


    virtual ~CMoveByStepAnimator();
protected:
    irr::core::vector3df directionStep;
    irr::u32 delay;    
    irr::core::aabbox3df aabb;
    
private:
    bool noBoundaries;
    bool firstUpdate;
    irr::u32 loopTime;
};

#endif	/* CMOVEBYSTEPANIMATOR_H */

