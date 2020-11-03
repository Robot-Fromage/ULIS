// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         SeparableBlendFuncAVXF.ipp
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Vec8f Separable Blending Modes functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Modes.h"
#include "Maths/Maths.h"
#include <vectorclass.h>

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Compositing
ULIS3_FORCEINLINE Vec8f ComposeAVXF( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar, Vec8f iCr ) {
    return ( 1.f - iVar ) * iCb + iVar * ( ( 1.f - iAb ) * iCs + iAb * iCr );
}

/////////////////////////////////////////////////////
// Standard Separable Blending Modes
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Normal
ULIS3_FORCEINLINE Vec8f BlendNormalAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS3_FORCEINLINE Vec8f BlendTopAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCs;
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Back
ULIS3_FORCEINLINE Vec8f BlendBackAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Behind
ULIS3_FORCEINLINE Vec8f BlendBehindAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Darken
ULIS3_FORCEINLINE Vec8f BlendDarkenAVXF( Vec8f iCs, Vec8f iCb ) {
    return  min( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Multiply
ULIS3_FORCEINLINE Vec8f BlendMultiplyAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- ColorBurn
ULIS3_FORCEINLINE Vec8f BlendColorBurnAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCb == 1.f, 1.f, select( iCs == 0.f, 0.f, 1.f - min( 1.f, ( 1.f - iCb ) / iCs ) ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Lighten
ULIS3_FORCEINLINE Vec8f BlendLightenAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Average
ULIS3_FORCEINLINE Vec8f BlendAverageAVXF( Vec8f iCs, Vec8f iCb ) {
    return  ( iCs + iCb ) / 2.f;
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Negation
ULIS3_FORCEINLINE Vec8f BlendNegationAVXF( Vec8f iCs, Vec8f iCb ) {
    return  1.f - abs( 1.f - iCs - iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Screen
ULIS3_FORCEINLINE Vec8f BlendScreenAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb + iCs - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- ColorDodge
ULIS3_FORCEINLINE Vec8f BlendColorDodgeAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCb == 0.f, 0.f, select( iCs ==1.f, 1.f, min( 1.f, iCb / ( 1.f - iCs ) ) ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS3_FORCEINLINE Vec8f BlendAddAVXF( Vec8f iCs, Vec8f iCb ) {
    return  min( 1.f, iCs + iCb );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearDodge
ULIS3_FORCEINLINE Vec8f BlendLinearDodgeAVXF( Vec8f iCs, Vec8f iCb ) {
    return  BlendAddAVXF( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- SoftLight
ULIS3_FORCEINLINE Vec8f BlendSoftLightAVXF( Vec8f iCs, Vec8f iCb ) {
    Vec8f  q = iCb * iCb;
    Vec8f  d = 2 * iCs;
    return  q + d * iCb - d * q;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- LinearBurn
ULIS3_FORCEINLINE Vec8f BlendLinearBurnAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( 0.f, iCs + iCb - 1.f );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- HardLight
ULIS3_FORCEINLINE Vec8f BlendHardLightAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCs <= 0.5f, BlendMultiplyAVXF( iCb, 2.f * iCs ), BlendScreenAVXF( iCb, 2 * iCs - 1.f ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Overlay
ULIS3_FORCEINLINE Vec8f BlendOverlayAVXF( Vec8f iCs, Vec8f iCb ) {
    return  BlendHardLightAVXF( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- VividLight
ULIS3_FORCEINLINE Vec8f BlendVividLightAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCs <= 0.5f, BlendColorBurnAVXF( iCb, 2.f * iCs ), BlendColorDodgeAVXF( iCb, 2 * ( iCs - 0.5f ) ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearLight
ULIS3_FORCEINLINE Vec8f BlendLinearLightAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( 0.f, min( iCb + 2.f * iCs - 1.f, 1.f ) );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- PinLight
ULIS3_FORCEINLINE Vec8f BlendPinLightAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCs > 0.f, max( iCb, 2.f * iCs - 1.f ), min( iCs, 2.f * iCs ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ HardMix
ULIS3_FORCEINLINE Vec8f BlendHardMixAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCs + iCb < 0.999f, 0.f, select( iCs + iCb > 1.001f, 1.f, select( iCb > iCs, 1.f, 0.f ) ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Difference
ULIS3_FORCEINLINE Vec8f BlendDifferenceAVXF( Vec8f iCs, Vec8f iCb ) {
    return  abs( iCb - iCs );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Exclusion
ULIS3_FORCEINLINE Vec8f BlendExclusionAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb + iCs - 2.f * iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Substract
ULIS3_FORCEINLINE Vec8f BlendSubstractAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( 0.f, iCb - iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Divide
ULIS3_FORCEINLINE Vec8f BlendDivideAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCs == 0.f && iCb == 0.f, 0.f, select( iCs == 0.f && iCb != 0.f, 1.f, max( 0.f, min( iCb / iCs, 1.f ) ) ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Phoenix
ULIS3_FORCEINLINE Vec8f BlendPhoenixAVXF( Vec8f iCs, Vec8f iCb ) {
    return  1.f - max( iCs, iCb ) + min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Reflect
ULIS3_FORCEINLINE Vec8f BlendReflectAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCb == 1.f, 1.f, min( 1.f, iCs * iCs / ( 1.f - iCb ) ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Glow
ULIS3_FORCEINLINE Vec8f BlendGlowAVXF( Vec8f iCs, Vec8f iCb ) {
    return  BlendReflectAVXF( iCb, iCs );
}

/////////////////////////////////////////////////////
// SeparableCompOpAVXF Template Selector
//--------------------------------------------------------------------------------------
//------------------------------------------- Generic SeparableCompOpAVXF Template Selector
template< eBlendingMode _BM >
ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) {
    ULIS3_ASSERT( false, "Blend Specialization Not Implemented" );
    return  0.f;
}

//--------------------------------------------------------------------------------------
//----------------------------------- SeparableCompOpAVXF Template Selector Specializations
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_NORMAL      >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendNormalAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_BEHIND      >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendBehindAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_DARKEN      >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendDarkenAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_MULTIPY     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendMultiplyAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_COLORBURN   >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendColorBurnAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_LINEARBURN  >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendLinearBurnAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_LIGHTEN     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendLightenAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_SCREEN      >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendScreenAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_COLORDODGE  >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendColorDodgeAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_LINEARDODGE >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendLinearDodgeAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_OVERLAY     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendOverlayAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_SOFTLIGHT   >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendSoftLightAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_HARDLIGHT   >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendHardLightAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_VIVIDLIGHT  >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendVividLightAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_LINEARLIGHT >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendLinearLightAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_PINLIGHT    >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendPinLightAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_HARDMIX     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendHardMixAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_PHOENIX     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendPhoenixAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_REFLECT     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendReflectAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_GLOW        >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendGlowAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_DIFFERENCE  >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendDifferenceAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_EXCLUSION   >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendExclusionAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_ADD         >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendAddAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_SUBSTRACT   >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendSubstractAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_DIVIDE      >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendDivideAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_AVERAGE     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendAverageAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_NEGATION    >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendNegationAVXF( iCs, iCb ) ); }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_TOP         >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  iCs; }
template<> ULIS3_FORCEINLINE Vec8f SeparableCompOpAVXF< BM_BACK        >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  iCb; }

ULIS3_NAMESPACE_END

