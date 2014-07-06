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

/**
 * Animator that manages moving node down by step (with some delay), with field interaction
 */
class CMoveDownAnimator : public irr::scene::ISceneNodeAnimator {
public:
    /**
     * Constructor
     * 
     * @param gameFieldManager - game field manager
     * @param twoDimensionalMode - true, if game proceeds in two dimensions, false otherwise
     * @param step - step which indicates speed of descending
     * @param delay - delay between steps, in milliseconds
     * @param aabb - axis aligned bounding box that indicated borders for animator, if no aabb set, animator has no boundaries
     */
    CMoveDownAnimator(CGameFieldManager* gameFieldManager, bool twoDimensionalMode, irr::u32 step, irr::f32 delay, irr::core::aabbox3df aabb = irr::core::aabbox3df(irr::core::vector3df(0.0f, 0.0f, 0.0f)));

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs);

    /**
     * Documentation for all virtual overriden methods see in Irrlicht include files
     */
    virtual irr::scene::ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager);

    /**
     * Destructor, not implemented
     */
    virtual ~CMoveDownAnimator();

    /**
     * Getter
     * 
     * @return current animator step
     */
    irr::u32 getStep() const;

    /**
     * Setter
     * 
     * @param step - new step for animator
     */
    void setStep(irr::u32 step);

    /**
     * Getter
     * 
     * @return current delay between steps, in milliseconds
     */
    irr::f32 getDelay() const;

    /**
     * Setter
     * 
     * @param delay - new delay between steps, in milliseconds
     */
    void setDelay(irr::f32 delay);

    /**
     * Getter
     * 
     * @return true, if animator stopped and don't affect on node
     */
    bool isStopped() const;

    /**
     * Setter
     * 
     * @param newStoppedValue - new value for stopped flag
     */
    void setStoppedValue(bool newStoppedValue);

protected:

    /**
     * direction in which node will move each step
     */
    irr::core::vector3di directionStep;

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
     * Checks if game field position if out of field borders
     * 
     * @param position - position to check
     * @return true, if game field position out of borders, false otherwise
     */
    bool outOfFieldBorders(irr::core::vector3di position);
    
    /**
     * Checks if game field position if out of bottom field borders
     * 
     * @param position - position to check
     * @return true, if game field position out of borders, false otherwise
     */
    bool outOfBottomFieldBorders(irr::core::vector3di position);
private:
    /**
     * game field manager
     */
    CGameFieldManager* gameFieldManager;
    
    /**
     * true, if game proceeds in two dimensions, false otherwise
     */
    bool twoDimensionalMode;
    
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
    
    /**
     * true, if animator is stopped, false otherwise
     */
    bool stopped;

};

#endif	/* CMOVEDOWNANIMATOR_H */

