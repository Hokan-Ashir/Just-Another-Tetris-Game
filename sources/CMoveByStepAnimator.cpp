/* 
 * File:   CMoveByStepAnimator.cpp
 * Author: hokan
 * 
 * Created on 29 Июнь 2014 г., 17:38
 */

#include "headers/CMoveByStepAnimator.h"

#include <iostream>

CMoveByStepAnimator::CMoveByStepAnimator(const irr::core::vector3df& directionStep, irr::u32 delay, irr::core::aabbox3df aabb) :
directionStep(directionStep)
, delay(delay)
, aabb(aabb)
, noBoundaries(false)
, firstUpdate(true) {
#ifdef _DEBUG
    setDebugName("CMoveByStepAnimator");
#endif

    if (aabb == irr::core::aabbox3df(irr::core::vector3df(0.0f, 0.0f, 0.0f))) {
        noBoundaries = true;
    }
}

irr::scene::ISceneNodeAnimator* CMoveByStepAnimator::createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager) {
    irr::scene::ISceneNodeAnimator* animator = new CMoveByStepAnimator(directionStep, delay, aabb);
    return animator;
}

void CMoveByStepAnimator::animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs) {
    if (!node) {
        return;
    }

    if (firstUpdate) {
        loopTime = timeMs;
        firstUpdate = false;
    }

    if ((timeMs - loopTime) >= delay) {
        /*std::cout << "- - -" << std::endl;
         std::cout << node->getTransformedBoundingBox().MinEdge.X << std::endl;
        std::cout << node->getTransformedBoundingBox().MinEdge.Y << std::endl;
        std::cout << node->getTransformedBoundingBox().MinEdge.Z << std::endl;
        
        std::cout << node->getTransformedBoundingBox().MaxEdge.X << std::endl;
        std::cout << node->getTransformedBoundingBox().MaxEdge.Y << std::endl;
        std::cout << node->getTransformedBoundingBox().MaxEdge.Z << std::endl;*/
        if (noBoundaries || !node->getTransformedBoundingBox().intersectsWithBox(aabb)) {
            node->setPosition(node->getPosition() + directionStep);
        }
        loopTime = timeMs;
    }
}

CMoveByStepAnimator::~CMoveByStepAnimator() {
}

