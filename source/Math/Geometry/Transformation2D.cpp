// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Transformation2D.cpp
* @author       Clement Berthaud
* @brief        This file provides the Transformation2D class definition.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Math/Geometry/Transformation2D.h"

ULIS_NAMESPACE_BEGIN
FTransformation2D::~FTransformation2D() {
}

FTransformation2D::FTransformation2D()
    : mMatrix()
    , mInverseMatrix()
    , mInvalidMatrix( false )
{
}

FTransformation2D::FTransformation2D( const FMat3F& iMat )
    : mMatrix( iMat )
    , mInverseMatrix()
    , mInvalidMatrix( true )
{
}


const FMat3F&
FTransformation2D::Matrix() const
{
    return  mMatrix;
}

const FMat3F&
FTransformation2D::InverseMatrix() const
{
    if( mInvalidMatrix ) {
        mInverseMatrix = mMatrix.Inverse();
        mInvalidMatrix = false;
    }

    return  mInverseMatrix;
}

ULIS_NAMESPACE_END

