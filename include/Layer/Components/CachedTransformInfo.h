// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CachedTransformInfo.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FCachedTransformInfo class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Matrix.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
// TODO: Check do we really need it to be called "cached" or to cache at all ?
/////////////////////////////////////////////////////
/// @class      FCachedTransformInfo
/// @brief      Simple CachedTransformInfo class.
class ULIS_API FCachedTransformInfo
{
public:
    // TODO: CTor

public:
    // TODO: Getters / Setters
    // TODO: Check, how do we chose to manipulate it ?
    // Obtain reference to matrix and manipulate it
    // Make APIs like Rotate() Translate() SetTranslation(), etc...
    // Is it gonna be easy to make this an AnimatedProperty in a 2D timeline ?

private:
    // TODO: Check, how do we chose to store info ?
    // A) only matrix, implicitely store all there, no real need for a cache then
    // B) Separate Translation, Rotation, Scale, on demand, build and cache matrix
    FMat3F mMatrix;
};

ULIS_NAMESPACE_END

