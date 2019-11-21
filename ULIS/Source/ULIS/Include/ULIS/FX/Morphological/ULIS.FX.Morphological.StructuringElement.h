/**
*
*   ULIS
*__________________
*
* @file     ULIS.FX.Morphological.StructuringElement.h
* @author   Clement Berthaud
* @brief    This file provides the declaration for the Structuring Elements tools in a Mathematical Morphological Operator Context.
*/
#pragma once
#include "ULIS/Base/ULIS.Base.BaseTypes.h"
#include "ULIS/Maths/ULIS.Maths.Geometry.h"
#include "ULIS/Maths/ULIS.Maths.Utility.h"
#include <initializer_list>
#include <vector>
#include <memory>

namespace ULIS {
/////////////////////////////////////////////////////
// Kernel Edge Handling
enum class eStructuringElementValue : uint8
{
      kZero     = 0
    , kOne      = 1
    , kEither   = 2
};


/////////////////////////////////////////////////////
// FKernel
class  FStructuringElement
{
public:
    // Construction / Destruction
    ~FKernel();
    FKernel( const FSize& iSize, float iValue = 0.f, const FPoint& iAnchor = FPoint( -1, -1 ), eKernelEdgeMode iKernelEdgeMode = eKernelEdgeMode::kExtend );
    FKernel( const FSize& iSize, std::initializer_list< float > iNums, const FPoint& iAnchor = FPoint( -1, -1 ), eKernelEdgeMode iKernelEdgeMode = eKernelEdgeMode::kExtend );
    FKernel( const FKernel& iRhs );             // Copy Constructor
    FKernel& operator=( const FKernel& iRhs );  // Copy Assignment Operator
    FKernel( FKernel&& iRhs );                  // Move Constructor
    FKernel& operator=( FKernel&& iRhs );       // Move Assignment Operator

public:
    // Public API
    void  Set( std::initializer_list< float > iNums );
    float At( uint iX, uint iY ) const;
    float At( FPoint iPoint ) const;
    void SetAt( uint iX, uint iY, float iValue );
    void SetAt( FPoint iPoint, float iValue );
    float& operator[] ( uint iIndex );
    const float& operator[] ( uint iIndex ) const;
    void Clear();
    void Fill( float iValue );
    void SetZeroes();
    void SetOnes();
    float Sum() const;
    void Shift( float iValue );
    void Mul( float iValue );
    void Normalize();
    bool IsNormalized() const;
    void FlipX();
    void FlipY();
    void Rotate90CW();
    void Rotate90CCW();
    void Rotate180();
    FKernel Normalized() const;
    FKernel FlippedX() const;
    FKernel FlippedY() const;
    FKernel Rotated90CW() const;
    FKernel Rotated90CCW() const;
    FKernel Rotated180() const;
    int Width() const;
    int Height() const;
    int Length() const;
    const FSize& Size() const;
    const FPoint& Anchor() const;
    eKernelEdgeMode KernelEdgeMode() const;

private:
    // Private API
    void Init();
    void ComputeAnchor();
    void AssertValid() const;
    void Allocate();
    void Deallocate();

private:
    // Private member data
    FSize mSize;
    FPoint mAnchor;
    eKernelEdgeMode mKernelEdgeMode;
    std::unique_ptr< float[] > mData;
};

typedef  std::vector< FKernel > FKernelChain;

} // namespace ULIS

