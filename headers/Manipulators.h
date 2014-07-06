/* 
 * File:   Manipulators.h
 * Author: hokan
 *
 * Created on 4 Июль 2014 г., 14:38
 */

#ifndef MANIPULATORS_H
#define	MANIPULATORS_H

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <vector3d.h>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"

/**
 * Rotates input vector with rotation vector by coordinates
 * 
 * @param inputVector - input vector to rotate
 * @param rotationVector - rotation vector
 * @param outputVector - output vector
 * @param inverted - it true, input vector rotated using inverted rotation values
 */
void rotateVectorByCoordinate(irr::core::vector3di inputVector, irr::core::vector3df rotationVector, irr::core::vector3di& outputVector, bool inverted = false);

#endif	/* MANIPULATORS_H */

