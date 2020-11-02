// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
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
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Compositing
ULIS_FORCEINLINE ufloat ComposeF( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar, ufloat iCr ) {
    return ( 1.f - iVar ) * iCb + iVar * ( ( 1.f - iAb ) *iCs + iAb * iCr );
}

/////////////////////////////////////////////////////
// Standard Separable Blending Modes
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Normal
ULIS_FORCEINLINE ufloat BlendNormalF( ufloat iCs, ufloat iCb ) {
    return iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS_FORCEINLINE ufloat BlendTopF( ufloat iCs, ufloat iCb ) {
    return iCs;
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Back
ULIS_FORCEINLINE ufloat BlendBackF( ufloat iCs, ufloat iCb ) {
    return iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Behind
ULIS_FORCEINLINE ufloat BlendBehindF( ufloat iCs, ufloat iCb ) {
    return iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Darken
ULIS_FORCEINLINE ufloat BlendDarkenF( ufloat iCs, ufloat iCb ) {
    return  FMath::Min( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Multiply
ULIS_FORCEINLINE ufloat BlendMultiplyF( ufloat iCs, ufloat iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- ColorBurn
ULIS_FORCEINLINE ufloat BlendColorBurnF( ufloat iCs, ufloat iCb ) {
    return  iCb == 1.f ? 1.f : iCs == 0.f ? 0.f : 1.f - FMath::Min( 1.f, ( 1.f - iCb ) / iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Lighten
ULIS_FORCEINLINE ufloat BlendLightenF( ufloat iCs, ufloat iCb ) {
    return  FMath::Max( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Average
ULIS_FORCEINLINE ufloat BlendAverageF( ufloat iCs, ufloat iCb ) {
    return  ( iCs + iCb ) / 2.f;
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Negation
ULIS_FORCEINLINE ufloat BlendNegationF( ufloat iCs, ufloat iCb ) {
    return  1.f - fabsf( 1.f - iCs - iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Screen
ULIS_FORCEINLINE ufloat BlendScreenF( ufloat iCs, ufloat iCb ) {
    return  iCb + iCs - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- ColorDodge
ULIS_FORCEINLINE ufloat BlendColorDodgeF( ufloat iCs, ufloat iCb ) {
    return  iCb == 0 ? 0.f : iCs == 1.f ? 1.f : FMath::Min( 1.f, iCb / ( 1.f - iCs ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS_FORCEINLINE ufloat BlendAddF( ufloat iCs, ufloat iCb ) {
    return  FMath::Min( 1.f, iCs + iCb );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearDodge
ULIS_FORCEINLINE ufloat BlendLinearDodgeF( ufloat iCs, ufloat iCb ) {
    return  BlendAddF( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- SoftLight
ULIS_FORCEINLINE ufloat BlendSoftLightF( ufloat iCs, ufloat iCb ) {
    ufloat  q = iCb * iCb;
    ufloat  d = 2 * iCs;
    return  q + d * iCb - d * q;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- LinearBurn
ULIS_FORCEINLINE ufloat BlendLinearBurnF( ufloat iCs, ufloat iCb ) {
    return  FMath::Max( 0.f, iCs + iCb - 1.f );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- HardLight
ULIS_FORCEINLINE ufloat BlendHardLightF( ufloat iCs, ufloat iCb ) {
    return  iCs <= 0.5f ? BlendMultiplyF( iCb, 2.f * iCs ) : BlendScreenF( iCb, 2 * iCs - 1.f );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Overlay
ULIS_FORCEINLINE ufloat BlendOverlayF( ufloat iCs, ufloat iCb ) {
    return  BlendHardLightF( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- VividLight
ULIS_FORCEINLINE ufloat BlendVividLightF( ufloat iCs, ufloat iCb ) {
    return  iCs <= 0.5f ? BlendColorBurnF( iCb, 2 * iCs ) : BlendColorDodgeF( iCb, 2 * ( iCs - 0.5f ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearLight
ULIS_FORCEINLINE ufloat BlendLinearLightF( ufloat iCs, ufloat iCb ) {
    return  FMath::Clamp( iCb + 2.f * iCs - 1.f, 0.f, 1.f );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- PinLight
ULIS_FORCEINLINE ufloat BlendPinLightF( ufloat iCs, ufloat iCb ) {
    return  iCs > 0.f ? FMath::Max( iCb, 2.f * iCs - 1.f ) : FMath::Min( iCb, 2.f * iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ HardMix
ULIS_FORCEINLINE ufloat BlendHardMixF( ufloat iCs, ufloat iCb ) {
    return  iCs + iCb < 0.999f ? 0 : iCs + iCb > 1.001f ? 1.f : iCb > iCs ? 1.f : 0.f;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Difference
ULIS_FORCEINLINE ufloat BlendDifferenceF( ufloat iCs, ufloat iCb ) {
    return  abs( iCb - iCs );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Exclusion
ULIS_FORCEINLINE ufloat BlendExclusionF( ufloat iCs, ufloat iCb ) {
    return  iCb + iCs - 2.f * iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Substract
ULIS_FORCEINLINE ufloat BlendSubstractF( ufloat iCs, ufloat iCb ) {
    return  FMath::Max( 0.f, iCb - iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Divide
ULIS_FORCEINLINE ufloat BlendDivideF( ufloat iCs, ufloat iCb ) {
    return  iCs == 0.f && iCb == 0.f ? 0.f : iCs == 0.f && iCb != 0.f ? 1.f : FMath::Clamp( iCb / iCs, 0.f, 1.f );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Phoenix
ULIS_FORCEINLINE ufloat BlendPhoenixF( ufloat iCs, ufloat iCb ) {
    return  1.f - FMath::Max( iCs, iCb ) + FMath::Min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Reflect
ULIS_FORCEINLINE ufloat BlendReflectF( ufloat iCs, ufloat iCb ) {
    return  iCb == 1.f ? 1.f : FMath::Min( 1.f, iCs * iCs / ( 1.f - iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Glow
ULIS_FORCEINLINE ufloat BlendGlowF( ufloat iCs, ufloat iCb ) {
    return  BlendReflectF( iCb, iCs );
}

/////////////////////////////////////////////////////
// SeparableCompOpF Template Selector
//--------------------------------------------------------------------------------------
//------------------------------------------- Generic SeparableCompOpF Template Selector
template< eBlendMode _BM >
ULIS_FORCEINLINE ufloat SeparableCompOpF( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) {
    ULIS_ASSERT( false, "Blend Specialization Not Implemented" );
    return  0.f;
}

//--------------------------------------------------------------------------------------
//----------------------------------- SeparableCompOpF Template Selector Specializations
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Normal         >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendNormalF(         iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Behind         >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendBehindF(         iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Darken         >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendDarkenF(         iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Multiply       >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendMultiplyF(       iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_ColorBurn      >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendColorBurnF(      iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_LinearBurn     >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendLinearBurnF(     iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Lighten        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendLightenF(        iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Screen         >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendScreenF(         iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_ColorDodge     >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendColorDodgeF(     iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_LinearDodge    >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendLinearDodgeF(    iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Overlay        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendOverlayF(        iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_SoftLight      >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendSoftLightF(      iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_HardLight      >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendHardLightF(      iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_VividLight     >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendVividLightF(     iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_LinearLight    >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendLinearLightF(    iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_PinLight       >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendPinLightF(       iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_HardMix        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendHardMixF(        iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Phoenix        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendPhoenixF(        iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Reflect        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendReflectF(        iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Glow           >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendGlowF(           iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Difference     >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendDifferenceF(     iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Exclusion      >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendExclusionF(      iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Add            >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendAddF(            iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Substract      >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendSubstractF(      iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Divide         >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendDivideF(         iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Average        >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendAverageF(        iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Negation       >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  ComposeF( iCs, iCb, iAb, iVar, BlendNegationF(       iCs, iCb ) ); }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Top            >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  iCs; }
template<> ULIS_FORCEINLINE ufloat SeparableCompOpF< Blend_Back           >( ufloat iCs, ufloat iCb, ufloat iAb, ufloat iVar ) { return  iCb; }

ULIS_NAMESPACE_END

