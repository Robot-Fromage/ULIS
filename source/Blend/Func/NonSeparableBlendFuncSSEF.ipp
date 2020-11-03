// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         NonSeparableBlendFuncSSEF.h
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Vec4f Non Separable Blending Modes functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Modes.h"
#include "Maths/Maths.h"

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Compositing
ULIS3_FORCEINLINE Vec4f ComposeNonSeparableSSEF( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar, Vec4f iCr ) {
    return ( 1.f - iVar ) * iCb + iVar * ( ( 1.f - iAb ) * iCs + iAb * iCr );
}

/////////////////////////////////////////////////////
// Helper static values for Computing Non Separable Blending functions
static Vec4f gLum( 0.3f, 0.59f, 0.11f, 0.0f );
static Vec4f gFixMin( 0.f, 0.f, 0.f, FLT_MAX );
static Vec4f gFixFind( 1.0e-20F, 1.0e-21F, 1.0e-22F, 0.f );

/////////////////////////////////////////////////////
// Helpers functions for Non Separable FRGBF Blending Modes Function
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------- myHorizontalMin
ULIS3_FORCEINLINE ufloat myHorizontalMin( Vec4f iC ) {
    Vec4f a = permute4<2, 3, V_DC, V_DC>( iC );
    Vec4f b = min( a, iC );
    a = permute4< 1, V_DC, V_DC, V_DC >( b );
    b = min(a, b);
    return  b[0];
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------- myHorizontalMax
ULIS3_FORCEINLINE ufloat myHorizontalMax( Vec4f iC ) {
    Vec4f a = permute4< 2, 3, V_DC, V_DC >( iC );
    Vec4f b = max( a, iC );
    a = permute4< 1, V_DC, V_DC, V_DC >( b );
    b = max(a, b);
    return  b[0];
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ LumSSEF
ULIS3_FORCEINLINE ufloat LumSSEF( Vec4f iC ) {
    return  horizontal_add( iC * gLum );
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ SatSSEF
ULIS3_FORCEINLINE ufloat SatSSEF( Vec4f iC ) {
    return  myHorizontalMax( iC ) - myHorizontalMin( iC + gFixMin );
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ ClipColorSSEF
ULIS3_FORCEINLINE Vec4f ClipColorSSEF( Vec4f iC ) {
    iC += gFixFind;
    ufloat l = LumSSEF( iC );
    ufloat n = myHorizontalMax( iC );
    ufloat x = myHorizontalMin( iC + gFixMin );
    if( n < 0.0f ) {
        ufloat ln = l - n;
        iC = l + ( ( ( iC - l ) * l ) / ( ln ) );
    }

    if( x > 1.0f )
    {
        ufloat xl = x - l;
        ufloat ml = 1.f - l;
        iC = l + ( ( ( iC - l ) * ( ml ) ) / ( xl ) );
    }

    return  iC;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- SetLumSSEF
ULIS3_FORCEINLINE Vec4f SetLumSSEF( Vec4f iC, ufloat iL ) {
    return  ClipColorSSEF( iC + ( iL - LumSSEF( iC ) ) );
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- SetSatSSEF
ULIS3_FORCEINLINE Vec4f SetSatSSEF( Vec4f iC, ufloat iS ) {
    iC += gFixFind;
    ufloat hmax = myHorizontalMax( iC );
    ufloat hmin = myHorizontalMin( iC + gFixMin );
    int imax = horizontal_find_first( iC == hmax );
    int imin = horizontal_find_first( iC == hmin );
    int imid = 3 - imax - imin;
    ufloat Cmax = iC[imax];
    ufloat Cmin = iC[imin];
    ufloat Cmid = iC[imid];
    if( Cmax > Cmin ) {
        Cmid = ( ( ( Cmid - Cmin ) * iS ) / ( Cmax - Cmin ) );
        Cmax = iS;
    } else {
        Cmid = Cmax = 0.f;
    }
    Cmin = 0.f;
    Vec4f ret;
    ret.insert( imax, Cmax );
    ret.insert( imin, Cmin );
    ret.insert( imid, Cmid );
    return  ret;
}

/////////////////////////////////////////////////////
// Stantard Non Separable SSEF Blending Modes Function
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- DarkerColor
ULIS3_FORCEINLINE Vec4f BlendDarkerColorSSEF( Vec4f iCs, Vec4f iCb ) {
    return  LumSSEF( iCb ) < LumSSEF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS3_FORCEINLINE Vec4f BlendLighterColorSSEF( Vec4f iCs, Vec4f iCb ) {
    return  LumSSEF( iCb ) > LumSSEF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS3_FORCEINLINE Vec4f BlendHueSSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumSSEF( SetSatSSEF( iCs, SatSSEF( iCb ) ), LumSSEF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS3_FORCEINLINE Vec4f BlendSaturationSSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumSSEF( SetSatSSEF( iCb, SatSSEF( iCs ) ), LumSSEF( iCb ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS3_FORCEINLINE Vec4f BlendColorSSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumSSEF( iCs, LumSSEF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS3_FORCEINLINE Vec4f BlendLuminositySSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumSSEF( iCb, LumSSEF( iCs ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Partial Derivative
ULIS3_FORCEINLINE Vec4f BlendPartialDerivativeSSEF( Vec4f iCs, Vec4f iCb ) {
    iCs = iCs * 2.f - 1.f;
    iCb = iCb * 2.f - 1.f;
    float csz = iCs[2];
    float cbz = iCb[2];
    auto tmp = iCs * cbz + iCb * csz;
    tmp.insert( 2, csz * cbz );
    tmp.insert( 3, 0.f );
    tmp = ( tmp / sqrt( horizontal_add( tmp * tmp ) ) ) * 0.5f + 0.5f;
    return  tmp;
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Whiteout
ULIS3_FORCEINLINE Vec4f BlendWhiteoutSSEF( Vec4f iCs, Vec4f iCb ) {
    iCs = iCs * 2.f - 1.f;
    iCb = iCb * 2.f - 1.f;
    float csz = iCs[2];
    float cbz = iCb[2];
    auto tmp = iCs + iCb;
    tmp.insert( 2, csz * cbz );
    tmp.insert( 3, 0.f );
    tmp = ( tmp / sqrt( horizontal_add( tmp * tmp ) ) ) * 0.5f + 0.5f;
    return  tmp;
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- AngleCorrected
ULIS3_FORCEINLINE Vec4f BlendAngleCorrectedSSEF( Vec4f iCs, Vec4f iCb ) {
    iCs = iCs * 2.f - 1.f;
    iCb = iCb * 2.f - 1.f;
    auto csxy = permute4< 0, 1, V_DC, V_DC >( iCs );
    auto cbxy = permute4< 0, 1, V_DC, V_DC >( iCb );
    float csz = iCs[2];
    auto tmp = iCs + iCb;
    tmp.insert( 2, csz );
    tmp.insert( 3, 0.f );
    tmp = ( tmp / sqrt( horizontal_add( tmp * tmp ) ) ) * 0.5f + 0.5f;
    return  tmp;
}

/////////////////////////////////////////////////////
// NonSeparableOpSSEF Template Selector
//--------------------------------------------------------------------------------------
//-------------------------------------------- Generic NonSeparableOpSSEF Template Selector
template< eBlendingMode _BM >
ULIS3_FORCEINLINE Vec4f NonSeparableOpSSEF( Vec4f iCs, Vec4f iCb ) {
    ULIS3_ASSERT( false, "Blend Specialization Not Implemented" );
    return  {};
}

//--------------------------------------------------------------------------------------
//------------------------------------ NonSeparableOpSSEF Template Selector Specializations
template<> ULIS3_FORCEINLINE Vec4f NonSeparableOpSSEF< BM_DARKERCOLOR          >( Vec4f iCs, Vec4f iCb ) { return  BlendDarkerColorSSEF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec4f NonSeparableOpSSEF< BM_LIGHTERCOLOR         >( Vec4f iCs, Vec4f iCb ) { return  BlendLighterColorSSEF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec4f NonSeparableOpSSEF< BM_HUE                  >( Vec4f iCs, Vec4f iCb ) { return  BlendHueSSEF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec4f NonSeparableOpSSEF< BM_SATURATION           >( Vec4f iCs, Vec4f iCb ) { return  BlendSaturationSSEF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec4f NonSeparableOpSSEF< BM_COLOR                >( Vec4f iCs, Vec4f iCb ) { return  BlendColorSSEF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec4f NonSeparableOpSSEF< BM_LUMINOSITY           >( Vec4f iCs, Vec4f iCb ) { return  BlendLuminositySSEF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec4f NonSeparableOpSSEF< BM_PARTIALDERIVATIVE    >( Vec4f iCs, Vec4f iCb ) { return  BlendPartialDerivativeSSEF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec4f NonSeparableOpSSEF< BM_WHITEOUT             >( Vec4f iCs, Vec4f iCb ) { return  BlendWhiteoutSSEF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec4f NonSeparableOpSSEF< BM_ANGLECORRECTED       >( Vec4f iCs, Vec4f iCb ) { return  BlendAngleCorrectedSSEF( iCs, iCb ); }

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- CompOp Version
template< eBlendingMode _BM > ULIS3_FORCEINLINE Vec4f NonSeparableCompOpSSEF( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeNonSeparableSSEF( iCs, iCb, iAb, iVar, NonSeparableOpSSEF< _BM >( iCs, iCb ) ); }

ULIS3_NAMESPACE_END

