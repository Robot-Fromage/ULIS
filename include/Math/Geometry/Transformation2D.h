// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Transformation2D.h
* @author       Clement Berthaud
* @brief        This file provides the FTransformation2D class declaration.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Geometry/Matrix.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FTransformation2D
/// @brief      The FTransformation2D class provides a simple 2D transformation
///             class geometry applications.
/// @details    The FTransformation2D wraps a matrix and provides mechanisms to
///             cache the inverse matrix.
///
///             It can either represent affine transforms or homographies for
///             2D perspective transform.
class ULIS_API FTransformation2D
{
public:

public:
    // Construction / Destruction
    ~FTransformation2D();
    FTransformation2D();
    FTransformation2D( const FMat3F& iMat );

public:
    // Public API
    const FMat3F& Matrix() const;
    const FMat3F& InverseMatrix() const;

private:
    // Private Data Members
    FMat3F          mMatrix;
    mutable FMat3F  mInverseMatrix;
    mutable bool    mInvalidMatrix;
};

ULIS_NAMESPACE_END

