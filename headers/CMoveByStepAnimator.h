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

/**
 * Animator that manages moving node by step (with some delay) in any direction
 */
class CMoveByStepAnimator : public irr::scene::ISceneNodeAnimator {
public:    
    /**
     * Constructor
     * 
     * @param directionStep - direction in which node will be moved (not ending point)
     * @param delay - delay between steps, in milliseconds
     * @param aabb - axis aligned bounding box that indicated borders for animator, if no aabb set, animator has no boundaries
     */
    CMoveByStepAnimator(const irr::core::vector3df& directionStep, irr::u32 delay, irr::core::aabbox3df aabb = irr::core::aabbox3df(irr::core::vector3df(0.0f, 0.0f, 0.0f)));

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual irr::scene::ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager);

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs);

    /**
     * Destructor, not implemented
     */
    virtual ~CMoveByStepAnimator();
protected:
    
    /**
     * direction in which node will move each step
     */
    irr::core::vector3df directionStep;
    
    /**
     * delay between steps, in milliseconds
     */
    irr::u32 delay;    
    
    /**
     * axis aligned bounding box that indicated borders for animator
     */
    irr::core::aabbox3df aabb;
    
private:
    
    /**
     * true, if animator has no boundaries, false otherwise
     */
    bool noBoundaries;
    
    /**
     * true, if animator hasn't lauched yet, false otherwise
     */
    bool firstUpdate;
    
    /**
     * time since last animator step
     */
    irr::u32 loopTime;
};

#endif	/* CMOVEBYSTEPANIMATOR_H */

