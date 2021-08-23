// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         KernelSSE.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FKernelSSE class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include <vectorclass.h>

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FConvolutionKernelSSE
/// @brief      The FKernelSSE class provides a mean of storing and manipulating convolution kernels optimized for sse.
/// @details    The KernelSSE is actually a 2D array of __m128 wrapped in Vec4f.
class ULIS3_API FKernelSSE
{
public:
    // Construction / Destruction
    virtual ~FKernelSSE();
    FKernelSSE( const FVec2I& iSize, float iValue = 0.f );
    FKernelSSE( const FVec2I& iSize, std::initializer_list< float > iNums );
    FKernelSSE( const FKernelSSE& );
    FKernelSSE( const FKernel& );
    FKernelSSE( FKernelSSE&& );
    FKernelSSE& operator=( const FKernelSSE& );
    FKernelSSE& operator=( FKernelSSE&& );

public:
    // Public API
    void  Set( std::initializer_list< float > iNums );
    const Vec4f* Data() const;
    float At( int iX, int iY ) const;
    float At( FVec2I iPoint ) const;
    void SetAt( int iX, int iY, float iValue );
    void SetAt( FVec2I iPoint, float iValue );
    void Clear();
    void Fill( float iValue );
    void SetZeroes();
    void SetOnes();
    float Sum() const;
    void Add( float iValue );
    void Mul( float iValue );
    void Normalize();
    bool IsNormalized() const;
    void FlipX();
    void FlipY();
    void Rotate90CW();
    void Rotate90CCW();
    void Rotate180();
    FKernelSSE Normalized() const;
    FKernelSSE FlippedX() const;
    FKernelSSE FlippedY() const;
    FKernelSSE Rotated90CW() const;
    FKernelSSE Rotated90CCW() const;
    FKernelSSE Rotated180() const;
    const FVec2I& Size() const;
    int Area() const;
    const FVec2I& Pivot() const;
    void SetPivot( const FVec2I& iPivot );
    int Width() const;
    int Height() const;

private:
    // Private Data Members
    FVec2I      mPivot;
    FVec2I      mSize;
    Vec4f*      mData;
};

ULIS3_NAMESPACE_END

