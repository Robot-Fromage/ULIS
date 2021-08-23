// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         SeparableBlendFuncF.h
* @author       Clement Berthaud
* @brief        This file provides the implementations for the float Separable Blending Modes functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Modes.h"
#include "Maths/Maths.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Compositing
ULIS3_FORCEINLINE ufloat ComposeF( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar, ufloat iCr ) {
    return ( 1.f - iVar ) * iCb + iVar * ( ( 1.f - iAb ) *iCs + iAb * iCr );
}

/////////////////////////////////////////////////////
// Standard Separable Blending Modes
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Normal
ULIS3_FORCEINLINE ufloat BlendNormalF( ufloat iCs, ufloat iCb ) {
    return iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS3_FORCEINLINE ufloat BlendTopF( ufloat iCs, ufloat iCb ) {
    return iCs;
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Back
ULIS3_FORCEINLINE ufloat BlendBackF( ufloat iCs, ufloat iCb ) {
    return iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Behind
ULIS3_FORCEINLINE ufloat BlendBehindF( ufloat iCs, ufloat iCb ) {
    return iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Darken
ULIS3_FORCEINLINE ufloat BlendDarkenF( ufloat iCs, ufloat iCb ) {
    return  FMaths::Min( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Multiply
ULIS3_FORCEINLINE ufloat BlendMultiplyF( ufloat iCs, ufloat iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- ColorBurn
ULIS3_FORCEINLINE ufloat BlendColorBurnF( ufloat iCs, ufloat iCb ) {
    return  iCb == 1.f ? 1.f : iCs == 0.f ? 0.f : 1.f - FMaths::Min( 1.f, ( 1.f - iCb ) / iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Lighten
ULIS3_FORCEINLINE ufloat BlendLightenF( ufloat iCs, ufloat iCb ) {
    return  FMaths::Max( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Average
ULIS3_FORCEINLINE ufloat BlendAverageF( ufloat iCs, ufloat iCb ) {
    return  ( iCs + iCb ) / 2.f;
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Negation
ULIS3_FORCEINLINE ufloat BlendNegationF( ufloat iCs, ufloat iCb ) {
    return  1.f - fabsf( 1.f - iCs - iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Screen
ULIS3_FORCEINLINE ufloat BlendScreenF( ufloat iCs, ufloat iCb ) {
    return  iCb + iCs - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- ColorDodge
ULIS3_FORCEINLINE ufloat BlendColorDodgeF( ufloat iCs, ufloat iCb ) {
    return  iCb == 0 ? 0.f : iCs == 1.f ? 1.f : FMaths::Min( 1.f, iCb / ( 1.f - iCs ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS3_FORCEINLINE ufloat BlendAddF( ufloat iCs, ufloat iCb ) {
    return  FMaths::Min( 1.f, iCs + iCb );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearDodge
ULIS3_FORCEINLINE ufloat BlendLinearDodgeF( ufloat iCs, ufloat iCb ) {
    return  BlendAddF( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- SoftLight
ULIS3_FORCEINLINE ufloat BlendSoftLightF( ufloat iCs, ufloat iCb ) {
    ufloat  q = iCb * iCb;
    ufloat  d = 2 * iCs;
    return  q + d * iCb - d * q;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- LinearBurn
ULIS3_FORCEINLINE ufloat BlendLinearBurnF( ufloat iCs, ufloat iCb ) {
    return  FMaths::Max( 0.f, iCs + iCb - 1.f );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- HardLight
ULIS3_FORCEINLINE ufloat BlendHardLightF( ufloat iCs, ufloat iCb ) {
    return  iCs <= 0.5f ? BlendMultiplyF( iCb, 2.f * iCs ) : BlendScreenF( iCb, 2 * iCs - 1.f );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Overlay
ULIS3_FORCEINLINE ufloat BlendOverlayF( ufloat iCs, ufloat iCb ) {
    return  BlendHardLightF( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- VividLight
ULIS3_FORCEINLINE ufloat BlendVividLightF( ufloat iCs, ufloat iCb ) {
    return  iCs <= 0.5f ? BlendColorBurnF( iCb, 2 * iCs ) : BlendColorDodgeF( iCb, 2 * ( iCs - 0.5f ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearLight
ULIS3_FORCEINLINE ufloat BlendLinearLightF( ufloat iCs, ufloat iCb ) {
    return  FMaths::Clamp( iCb + 2.f * iCs - 1.f, 0.f, 1.f );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- PinLight
ULIS3_FORCEINLINE ufloat BlendPinLightF( ufloat iCs, ufloat iCb ) {
    return  iCs > 0.f ? FMaths::Max( iCb, 2.f * iCs - 1.f ) : FMaths::Min( iCb, 2.f * iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ HardMix
ULIS3_FORCEINLINE ufloat BlendHardMixF( ufloat iCs, ufloat iCb ) {
    return  iCs + iCb < 0.999f ? 0 : iCs + iCb > 1.001f ? 1.f : iCb > iCs ? 1.f : 0.f;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Difference
ULIS3_FORCEINLINE ufloat BlendDifferenceF( ufloat iCs, ufloat iCb ) {
    return  abs( iCb - iCs );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Exclusion
ULIS3_FORCEINLINE ufloat BlendExclusionF( ufloat iCs, ufloat iCb ) {
    return  iCb + iCs - 2.f * iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Substract
ULIS3_FORCEINLINE ufloat BlendSubstractF( ufloat iCs, ufloat iCb ) {
    return  FMaths::Max( 0.f, iCb - iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Divide
ULIS3_FORCEINLINE ufloat BlendDivideF( ufloat iCs, ufloat iCb ) {
    return  iCs == 0.f && iCb == 0.f ? 0.f : iCs == 0.f && iCb != 0.f ? 1.f : FMaths::Clamp( iCb / iCs, 0.f, 1.f );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Phoenix
ULIS3_FORCEINLINE ufloat BlendPhoenixF( ufloat iCs, ufloat iCb ) {
    return  1.f - FMaths::Max( iCs, iCb ) + FMaths::Min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Reflect
ULIS3_FORCEINLINE ufloat BlendReflectF( ufloat iCs, ufloat iCb ) {
    return  iCb == 1.f ? 1.f : FMaths::Min( 1.f, iCs * iCs / ( 1.f - iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Glow
ULIS3_FORCEINLINE ufloat BlendGlowF( ufloat iCs, ufloat iCb ) {
    return  BlendReflectF( iCb, iCs );
}

/////////////////////////////////////////////////////
// SeparableCompOpF Template Selector
//--------------------------------------------------------------------------------------
//------------------------------------------- Generic SeparableCompOpF Template Selector
template< eBlendingMode _BM >
ULIS3_FORCEINLINE ufloat SeparableCompOpF( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) {
    ULIS3_ASSERT( false, "Blend Specialization Not Implemented" );
    return  0.f;
}

//--------------------------------------------------------------------------------------
//----------------------------------- SeparableCompOpF Template Selector Specializations
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_NORMAL         >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendNormalF(         iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_BEHIND         >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendBehindF(         iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_DARKEN         >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendDarkenF(         iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_MULTIPY        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendMultiplyF(       iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_COLORBURN      >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendColorBurnF(      iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_LINEARBURN     >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendLinearBurnF(     iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_LIGHTEN        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendLightenF(        iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_SCREEN         >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendScreenF(         iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_COLORDODGE     >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendColorDodgeF(     iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_LINEARDODGE    >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendLinearDodgeF(    iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_OVERLAY        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendOverlayF(        iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_SOFTLIGHT      >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendSoftLightF(      iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_HARDLIGHT      >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendHardLightF(      iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_VIVIDLIGHT     >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendVividLightF(     iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_LINEARLIGHT    >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendLinearLightF(    iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_PINLIGHT       >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendPinLightF(       iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_HARDMIX        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendHardMixF(        iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_PHOENIX        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendPhoenixF(        iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_REFLECT        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendReflectF(        iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_GLOW           >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendGlowF(           iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_DIFFERENCE     >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendDifferenceF(     iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_EXCLUSION      >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendExclusionF(      iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_ADD            >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendAddF(            iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_SUBSTRACT      >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendSubstractF(      iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_DIVIDE         >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendDivideF(         iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_AVERAGE        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendAverageF(        iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_NEGATION       >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendNegationF(       iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_TOP            >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  iCs; }
template<> ULIS3_FORCEINLINE ufloat SeparableCompOpF< BM_BACK           >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  iCb; }

ULIS3_NAMESPACE_END

