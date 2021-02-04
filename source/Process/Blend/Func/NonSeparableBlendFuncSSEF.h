// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         NonSeparableBlendFuncSSEF.h
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Vec4f Non Separable Blending Modes functions.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Compositing
ULIS_FORCEINLINE Vec4f ComposeNonSeparableSSEF( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar, Vec4f iCr ) {
    return ( 1.f - iVar ) * iCb + iVar * ( ( 1.f - iAb ) * iCs + iAb * iCr );
}

/////////////////////////////////////////////////////
// Helper static values for Computing Non Separable Blending functions
static Vec4f gLum( 0.3f, 0.59f, 0.11f, 0.0f );
static Vec4f gFixMin( 0.f, 0.f, 0.f, ULIS_FLOAT_MAX );
static Vec4f gFixFind( 1.0e-20F, 1.0e-21F, 1.0e-22F, 0.f );

/////////////////////////////////////////////////////
// Helpers functions for Non Separable FRGBF Blending Modes Function
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------- myHorizontalMin
ULIS_FORCEINLINE ufloat myHorizontalMin( Vec4f iC ) {
    Vec4f a = permute4<2, 3, V_DC, V_DC>( iC );
    Vec4f b = min( a, iC );
    a = permute4< 1, V_DC, V_DC, V_DC >( b );
    b = min(a, b);
    return  b[0];
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------- myHorizontalMax
ULIS_FORCEINLINE ufloat myHorizontalMax( Vec4f iC ) {
    Vec4f a = permute4< 2, 3, V_DC, V_DC >( iC );
    Vec4f b = max( a, iC );
    a = permute4< 1, V_DC, V_DC, V_DC >( b );
    b = max(a, b);
    return  b[0];
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ LumSSEF
ULIS_FORCEINLINE ufloat LumSSEF( Vec4f iC ) {
    return  horizontal_add( iC * gLum );
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ SatSSEF
ULIS_FORCEINLINE ufloat SatSSEF( Vec4f iC ) {
    return  myHorizontalMax( iC ) - myHorizontalMin( iC + gFixMin );
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ ClipColorSSEF
ULIS_FORCEINLINE Vec4f ClipColorSSEF( Vec4f iC ) {
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
ULIS_FORCEINLINE Vec4f SetLumSSEF( Vec4f iC, ufloat iL ) {
    return  ClipColorSSEF( iC + ( iL - LumSSEF( iC ) ) );
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- SetSatSSEF
ULIS_FORCEINLINE Vec4f SetSatSSEF( Vec4f iC, ufloat iS ) {
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
ULIS_FORCEINLINE Vec4f BlendDarkerColorSSEF( Vec4f iCs, Vec4f iCb ) {
    return  LumSSEF( iCb ) < LumSSEF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS_FORCEINLINE Vec4f BlendLighterColorSSEF( Vec4f iCs, Vec4f iCb ) {
    return  LumSSEF( iCb ) > LumSSEF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS_FORCEINLINE Vec4f BlendHueSSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumSSEF( SetSatSSEF( iCs, SatSSEF( iCb ) ), LumSSEF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS_FORCEINLINE Vec4f BlendSaturationSSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumSSEF( SetSatSSEF( iCb, SatSSEF( iCs ) ), LumSSEF( iCb ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS_FORCEINLINE Vec4f BlendColorSSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumSSEF( iCs, LumSSEF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS_FORCEINLINE Vec4f BlendLuminositySSEF( Vec4f iCs, Vec4f iCb ) {
    return  SetLumSSEF( iCb, LumSSEF( iCs ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Partial Derivative
ULIS_FORCEINLINE Vec4f BlendPartialDerivativeSSEF( Vec4f iCs, Vec4f iCb ) {
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
ULIS_FORCEINLINE Vec4f BlendWhiteoutSSEF( Vec4f iCs, Vec4f iCb ) {
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
ULIS_FORCEINLINE Vec4f BlendAngleCorrectedSSEF( Vec4f iCs, Vec4f iCb ) {
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
template< eBlendMode _BM >
ULIS_FORCEINLINE Vec4f NonSeparableOpSSEF( Vec4f iCs, Vec4f iCb ) {
    ULIS_ASSERT( false, "Blend Specialization Not Implemented" );
    return  {};
}

//--------------------------------------------------------------------------------------
//------------------------------------ NonSeparableOpSSEF Template Selector Specializations
template<> ULIS_FORCEINLINE Vec4f NonSeparableOpSSEF< Blend_DarkerColor          >( Vec4f iCs, Vec4f iCb ) { return  BlendDarkerColorSSEF( iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f NonSeparableOpSSEF< Blend_LighterColor         >( Vec4f iCs, Vec4f iCb ) { return  BlendLighterColorSSEF( iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f NonSeparableOpSSEF< Blend_Hue                  >( Vec4f iCs, Vec4f iCb ) { return  BlendHueSSEF( iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f NonSeparableOpSSEF< Blend_Saturation           >( Vec4f iCs, Vec4f iCb ) { return  BlendSaturationSSEF( iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f NonSeparableOpSSEF< Blend_Color                >( Vec4f iCs, Vec4f iCb ) { return  BlendColorSSEF( iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f NonSeparableOpSSEF< Blend_Luminosity           >( Vec4f iCs, Vec4f iCb ) { return  BlendLuminositySSEF( iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f NonSeparableOpSSEF< Blend_PartialDerivative    >( Vec4f iCs, Vec4f iCb ) { return  BlendPartialDerivativeSSEF( iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f NonSeparableOpSSEF< Blend_Whiteout             >( Vec4f iCs, Vec4f iCb ) { return  BlendWhiteoutSSEF( iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f NonSeparableOpSSEF< Blend_AngleCorrected       >( Vec4f iCs, Vec4f iCb ) { return  BlendAngleCorrectedSSEF( iCs, iCb ); }

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- CompOp Version
template< eBlendMode _BM > ULIS_FORCEINLINE Vec4f NonSeparableCompOpSSEF( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeNonSeparableSSEF( iCs, iCb, iAb, iVar, NonSeparableOpSSEF< _BM >( iCs, iCb ) ); }

ULIS_NAMESPACE_END

