// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         NonSeparableBlendFuncF.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the ufloat Blending functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#define GLM_FORCE_SWIZZLE
#include "Core/Core.h"
#include "Blend/Modes.h"
#include "Color/ModelStructs.h"
#include "Maths/Maths.h"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Non Separable Blending Modes for RGB
static Vec4f gLum( 0.3f, 0.59f, 0.11f, 0.0f );
static Vec4f gFixMin( 0.f, 0.f, 0.f, FLT_MAX );
static Vec4f gFixFind( 1.0e-20F, 1.0e-21F, 1.0e-22F, 0.f );

ULIS2_FORCEINLINE ufloat myHorizontalMin( Vec4f iC ) {
    Vec4f a = permute4<2, 3, V_DC, V_DC>( iC );
    Vec4f b = min( a, iC );
    a = permute4< 1, V_DC, V_DC, V_DC >( b );
    b = min(a, b);
    return  b[0];
}

ULIS2_FORCEINLINE ufloat myHorizontalMax( Vec4f iC ) {
    Vec4f a = permute4< 2, 3, V_DC, V_DC >( iC );
    Vec4f b = max( a, iC );
    a = permute4< 1, V_DC, V_DC, V_DC >( b );
    b = max(a, b);
    return  b[0];
}

ULIS2_FORCEINLINE ufloat LumF( Vec4f iC ) {
    return  horizontal_add( iC * gLum );
}

ULIS2_FORCEINLINE ufloat SatF( Vec4f iC ) {
    return  myHorizontalMax( iC ) - myHorizontalMin( iC + gFixMin );
}

ULIS2_FORCEINLINE Vec4f ClipColorF( Vec4f iC ) {
    iC += gFixFind;
    ufloat l = LumF( iC );
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

ULIS2_FORCEINLINE Vec4f SetLumF( Vec4f iC, ufloat iL ) {
    return  ClipColorF( iC + ( iL - LumF( iC ) ) );
}

ULIS2_FORCEINLINE Vec4f SetSatF( Vec4f iC, ufloat iS ) {
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

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- DarkerColor
ULIS2_FORCEINLINE Vec4f BlendDarkerColorSSEF( Vec4f iCs, Vec4f iCb ) {
    return  LumF( iCb ) < LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS2_FORCEINLINE Vec4f BlendLighterColorSSEF( Vec4f iCs, Vec4f iCb ) {
    return  LumF( iCb ) > LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS2_FORCEINLINE Vec4f BlendHueSSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumF( SetSatF( iCs, SatF( iCb ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS2_FORCEINLINE Vec4f BlendSaturationSSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumF( SetSatF( iCb, SatF( iCs ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS2_FORCEINLINE Vec4f BlendColorSSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumF( iCs, LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS2_FORCEINLINE Vec4f BlendLuminositySSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumF( iCb, LumF( iCs ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Partial Derivative
ULIS2_FORCEINLINE Vec4f BlendPartialDerivativeSSEF( Vec4f iCs, Vec4f iCb ) {
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
ULIS2_FORCEINLINE Vec4f BlendWhiteoutSSEF( Vec4f iCs, Vec4f iCb ) {
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
ULIS2_FORCEINLINE Vec4f BlendAngleCorrectedSSEF( Vec4f iCs, Vec4f iCb ) {
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

//--------------------------------------------------------------------------------------
//-------------------------------------------- NonSeparableOpF Template Selector for RGB
template< eBlendingMode _BM >
ULIS2_FORCEINLINE Vec4f NonSeparableOpF( Vec4f iCs, Vec4f iCb ) {
    ULIS2_ASSERT( false, "Blend Specialization Not Implemented" );
    return  {};
}

template<> ULIS2_FORCEINLINE Vec4f NonSeparableOpF< BM_DARKERCOLOR          >( Vec4f iCs, Vec4f iCb ) { return  BlendDarkerColorSSEF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f NonSeparableOpF< BM_LIGHTERCOLOR         >( Vec4f iCs, Vec4f iCb ) { return  BlendLighterColorSSEF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f NonSeparableOpF< BM_HUE                  >( Vec4f iCs, Vec4f iCb ) { return  BlendHueSSEF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f NonSeparableOpF< BM_SATURATION           >( Vec4f iCs, Vec4f iCb ) { return  BlendSaturationSSEF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f NonSeparableOpF< BM_COLOR                >( Vec4f iCs, Vec4f iCb ) { return  BlendColorSSEF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f NonSeparableOpF< BM_LUMINOSITY           >( Vec4f iCs, Vec4f iCb ) { return  BlendLuminositySSEF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f NonSeparableOpF< BM_PARTIALDERIVATIVE    >( Vec4f iCs, Vec4f iCb ) { return  BlendPartialDerivativeSSEF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f NonSeparableOpF< BM_WHITEOUT             >( Vec4f iCs, Vec4f iCb ) { return  BlendWhiteoutSSEF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f NonSeparableOpF< BM_ANGLECORRECTED       >( Vec4f iCs, Vec4f iCb ) { return  BlendAngleCorrectedSSEF( iCs, iCb ); }

template< eBlendingMode _BM > ULIS2_FORCEINLINE Vec4f NonSeparableCompOpSSEF( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, NonSeparableOpF< _BM >( iCs, iCb ) ); }




ULIS2_NAMESPACE_END

