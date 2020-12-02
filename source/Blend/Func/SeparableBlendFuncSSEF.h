// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         SeparableBlendFuncSSEF.h
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Vec4f Separable Blending Modes functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Compositing
ULIS_FORCEINLINE Vec4f ComposeSSEF( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar, Vec4f iCr ) {
    return ( 1.f - iVar ) * iCb + iVar * ( ( 1.f - iAb ) * iCs + iAb * iCr );
}

/////////////////////////////////////////////////////
// Standard Separable Blending Modes
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Normal
ULIS_FORCEINLINE Vec4f BlendNormalSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS_FORCEINLINE Vec4f BlendTopSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCs;
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Back
ULIS_FORCEINLINE Vec4f BlendBackSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Behind
ULIS_FORCEINLINE Vec4f BlendBehindSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Darken
ULIS_FORCEINLINE Vec4f BlendDarkenSSEF( Vec4f iCs, Vec4f iCb ) {
    return  min( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Multiply
ULIS_FORCEINLINE Vec4f BlendMultiplySSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- ColorBurn
ULIS_FORCEINLINE Vec4f BlendColorBurnSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCb == 1.f, 1.f, select( iCs == 0.f, 0.f, 1.f - min( 1.f, ( 1.f - iCb ) / iCs ) ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Lighten
ULIS_FORCEINLINE Vec4f BlendLightenSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Average
ULIS_FORCEINLINE Vec4f BlendAverageSSEF( Vec4f iCs, Vec4f iCb ) {
    return  ( iCs + iCb ) / 2.f;
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Negation
ULIS_FORCEINLINE Vec4f BlendNegationSSEF( Vec4f iCs, Vec4f iCb ) {
    return  1.f - abs( 1.f - iCs - iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Screen
ULIS_FORCEINLINE Vec4f BlendScreenSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb + iCs - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- ColorDodge
ULIS_FORCEINLINE Vec4f BlendColorDodgeSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCb == 0.f, 0.f, select( iCs ==1.f, 1.f, min( 1.f, iCb / ( 1.f - iCs ) ) ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS_FORCEINLINE Vec4f BlendAddSSEF( Vec4f iCs, Vec4f iCb ) {
    return  min( 1.f, iCs + iCb );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearDodge
ULIS_FORCEINLINE Vec4f BlendLinearDodgeSSEF( Vec4f iCs, Vec4f iCb ) {
    return  BlendAddSSEF( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- SoftLight
ULIS_FORCEINLINE Vec4f BlendSoftLightSSEF( Vec4f iCs, Vec4f iCb ) {
    Vec4f  q = iCb * iCb;
    Vec4f  d = 2 * iCs;
    return  q + d * iCb - d * q;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- LinearBurn
ULIS_FORCEINLINE Vec4f BlendLinearBurnSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( 0.f, iCs + iCb - 1.f );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- HardLight
ULIS_FORCEINLINE Vec4f BlendHardLightSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCs <= 0.5f, BlendMultiplySSEF( iCb, 2.f * iCs ), BlendScreenSSEF( iCb, 2 * iCs - 1.f ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Overlay
ULIS_FORCEINLINE Vec4f BlendOverlaySSEF( Vec4f iCs, Vec4f iCb ) {
    return  BlendHardLightSSEF( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- VividLight
ULIS_FORCEINLINE Vec4f BlendVividLightSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCs <= 0.5f, BlendColorBurnSSEF( iCb, 2.f * iCs ), BlendColorDodgeSSEF( iCb, 2 * ( iCs - 0.5f ) ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearLight
ULIS_FORCEINLINE Vec4f BlendLinearLightSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( 0.f, min( iCb + 2.f * iCs - 1.f, 1.f ) );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- PinLight
ULIS_FORCEINLINE Vec4f BlendPinLightSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCs > 0.f, max( iCb, 2.f * iCs - 1.f ), min( iCs, 2.f * iCs ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ HardMix
ULIS_FORCEINLINE Vec4f BlendHardMixSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCs + iCb < 0.999f, 0.f, select( iCs + iCb > 1.001f, 1.f, select( iCb > iCs, 1.f, 0.f ) ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Difference
ULIS_FORCEINLINE Vec4f BlendDifferenceSSEF( Vec4f iCs, Vec4f iCb ) {
    return  abs( iCb - iCs );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Exclusion
ULIS_FORCEINLINE Vec4f BlendExclusionSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb + iCs - 2.f * iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Substract
ULIS_FORCEINLINE Vec4f BlendSubstractSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( 0.f, iCb - iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Divide
ULIS_FORCEINLINE Vec4f BlendDivideSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCs == 0.f && iCb == 0.f, 0.f, select( iCs == 0.f && iCb != 0.f, 1.f, max( 0.f, min( iCb / iCs, 1.f ) ) ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Phoenix
ULIS_FORCEINLINE Vec4f BlendPhoenixSSEF( Vec4f iCs, Vec4f iCb ) {
    return  1.f - max( iCs, iCb ) + min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Reflect
ULIS_FORCEINLINE Vec4f BlendReflectSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCb == 1.f, 1.f, min( 1.f, iCs * iCs / ( 1.f - iCb ) ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Glow
ULIS_FORCEINLINE Vec4f BlendGlowSSEF( Vec4f iCs, Vec4f iCb ) {
    return  BlendReflectSSEF( iCb, iCs );
}

/////////////////////////////////////////////////////
// SeparableCompOpSSEF Template Selector
//--------------------------------------------------------------------------------------
//------------------------------------------- Generic SeparableCompOpSSEF Template Selector
template< eBlendMode _BM >
ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) {
    ULIS_ASSERT( false, "Blend Specialization Not Implemented" );
    return  0.f;
}

//--------------------------------------------------------------------------------------
//----------------------------------- SeparableCompOpSSEF Template Selector Specializations
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Normal      >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendNormalSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Behind      >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendBehindSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Darken      >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendDarkenSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Multiply    >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendMultiplySSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_ColorBurn   >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendColorBurnSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_LinearBurn  >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendLinearBurnSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Lighten     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendLightenSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Screen      >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendScreenSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_ColorDodge  >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendColorDodgeSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_LinearDodge >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendLinearDodgeSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Overlay     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendOverlaySSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_SoftLight   >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendSoftLightSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_HardLight   >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendHardLightSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_VividLight  >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendVividLightSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_LinearLight >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendLinearLightSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_PinLight    >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendPinLightSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_HardMix     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendHardMixSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Phoenix     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendPhoenixSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Reflect     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendReflectSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Glow        >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendGlowSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Difference  >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendDifferenceSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Exclusion   >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendExclusionSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Add         >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendAddSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Substract   >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendSubstractSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Divide      >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendDivideSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Average     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendAverageSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Negation    >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendNegationSSEF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Top         >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  iCs; }
template<> ULIS_FORCEINLINE Vec4f SeparableCompOpSSEF< Blend_Back        >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  iCb; }

ULIS_NAMESPACE_END

