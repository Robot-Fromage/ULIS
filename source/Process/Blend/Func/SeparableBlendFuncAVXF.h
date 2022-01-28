// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SeparableBlendFuncAVXF.h
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Vec8f Separable Blending Modes functions.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Compositing
ULIS_FORCEINLINE Vec8f ComposeAVXF( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar, Vec8f iCr ) {
    return ( 1.f - iVar ) * iCb + iVar * ( ( 1.f - iAb ) * iCs + iAb * iCr );
}

/////////////////////////////////////////////////////
// Standard Separable Blending Modes
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Normal
ULIS_FORCEINLINE Vec8f BlendNormalAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS_FORCEINLINE Vec8f BlendTopAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCs;
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Back
ULIS_FORCEINLINE Vec8f BlendBackAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Behind
ULIS_FORCEINLINE Vec8f BlendBehindAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Darken
ULIS_FORCEINLINE Vec8f BlendDarkenAVXF( Vec8f iCs, Vec8f iCb ) {
    return  min( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Multiply
ULIS_FORCEINLINE Vec8f BlendMultiplyAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- ColorBurn
ULIS_FORCEINLINE Vec8f BlendColorBurnAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCb == 1.f, 1.f, select( iCs == 0.f, 0.f, 1.f - min( 1.f, ( 1.f - iCb ) / iCs ) ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Lighten
ULIS_FORCEINLINE Vec8f BlendLightenAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Average
ULIS_FORCEINLINE Vec8f BlendAverageAVXF( Vec8f iCs, Vec8f iCb ) {
    return  ( iCs + iCb ) / 2.f;
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Negation
ULIS_FORCEINLINE Vec8f BlendNegationAVXF( Vec8f iCs, Vec8f iCb ) {
    return  1.f - abs( 1.f - iCs - iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Screen
ULIS_FORCEINLINE Vec8f BlendScreenAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb + iCs - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- ColorDodge
ULIS_FORCEINLINE Vec8f BlendColorDodgeAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCb == 0.f, 0.f, select( iCs ==1.f, 1.f, min( 1.f, iCb / ( 1.f - iCs ) ) ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS_FORCEINLINE Vec8f BlendAddAVXF( Vec8f iCs, Vec8f iCb ) {
    return  min( 1.f, iCs + iCb );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearDodge
ULIS_FORCEINLINE Vec8f BlendLinearDodgeAVXF( Vec8f iCs, Vec8f iCb ) {
    return  BlendAddAVXF( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- SoftLight
ULIS_FORCEINLINE Vec8f BlendSoftLightAVXF( Vec8f iCs, Vec8f iCb ) {
    Vec8f  q = iCb * iCb;
    Vec8f  d = 2 * iCs;
    return  q + d * iCb - d * q;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- LinearBurn
ULIS_FORCEINLINE Vec8f BlendLinearBurnAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( 0.f, iCs + iCb - 1.f );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- HardLight
ULIS_FORCEINLINE Vec8f BlendHardLightAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCs <= 0.5f, BlendMultiplyAVXF( iCb, 2.f * iCs ), BlendScreenAVXF( iCb, 2 * iCs - 1.f ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Overlay
ULIS_FORCEINLINE Vec8f BlendOverlayAVXF( Vec8f iCs, Vec8f iCb ) {
    return  BlendHardLightAVXF( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- VividLight
ULIS_FORCEINLINE Vec8f BlendVividLightAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCs <= 0.5f, BlendColorBurnAVXF( iCb, 2.f * iCs ), BlendColorDodgeAVXF( iCb, 2 * ( iCs - 0.5f ) ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearLight
ULIS_FORCEINLINE Vec8f BlendLinearLightAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( 0.f, min( iCb + 2.f * iCs - 1.f, 1.f ) );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- PinLight
ULIS_FORCEINLINE Vec8f BlendPinLightAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCs > 0.f, max( iCb, 2.f * iCs - 1.f ), min( iCs, 2.f * iCs ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ HardMix
ULIS_FORCEINLINE Vec8f BlendHardMixAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCs + iCb < 0.999f, 0.f, select( iCs + iCb > 1.001f, 1.f, select( iCb > iCs, 1.f, 0.f ) ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Difference
ULIS_FORCEINLINE Vec8f BlendDifferenceAVXF( Vec8f iCs, Vec8f iCb ) {
    return  abs( iCb - iCs );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Exclusion
ULIS_FORCEINLINE Vec8f BlendExclusionAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb + iCs - 2.f * iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Substract
ULIS_FORCEINLINE Vec8f BlendSubstractAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( 0.f, iCb - iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Divide
ULIS_FORCEINLINE Vec8f BlendDivideAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCs == 0.f && iCb == 0.f, 0.f, select( iCs == 0.f && iCb != 0.f, 1.f, max( 0.f, min( iCb / iCs, 1.f ) ) ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Phoenix
ULIS_FORCEINLINE Vec8f BlendPhoenixAVXF( Vec8f iCs, Vec8f iCb ) {
    return  1.f - max( iCs, iCb ) + min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Reflect
ULIS_FORCEINLINE Vec8f BlendReflectAVXF( Vec8f iCs, Vec8f iCb ) {
    return  select( iCb == 1.f, 1.f, min( 1.f, iCs * iCs / ( 1.f - iCb ) ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Glow
ULIS_FORCEINLINE Vec8f BlendGlowAVXF( Vec8f iCs, Vec8f iCb ) {
    return  BlendReflectAVXF( iCb, iCs );
}

/////////////////////////////////////////////////////
// SeparableCompOpAVXF Template Selector
//--------------------------------------------------------------------------------------
//------------------------------------------- Generic SeparableCompOpAVXF Template Selector
template< eBlendMode _BM >
ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) {
    ULIS_ASSERT( false, "Blend Specialization Not Implemented" );
    return  0.f;
}

//--------------------------------------------------------------------------------------
//----------------------------------- SeparableCompOpAVXF Template Selector Specializations
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Normal      >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendNormalAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Behind      >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendBehindAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Darken      >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendDarkenAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Multiply    >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendMultiplyAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_ColorBurn   >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendColorBurnAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_LinearBurn  >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendLinearBurnAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Lighten     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendLightenAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Screen      >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendScreenAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_ColorDodge  >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendColorDodgeAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_LinearDodge >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendLinearDodgeAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Overlay     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendOverlayAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_SoftLight   >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendSoftLightAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_HardLight   >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendHardLightAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_VividLight  >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendVividLightAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_LinearLight >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendLinearLightAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_PinLight    >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendPinLightAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_HardMix     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendHardMixAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Phoenix     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendPhoenixAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Reflect     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendReflectAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Glow        >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendGlowAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Difference  >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendDifferenceAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Exclusion   >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendExclusionAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Add         >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendAddAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Substract   >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendSubstractAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Divide      >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendDivideAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Average     >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendAverageAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Negation    >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  ComposeAVXF( iCs, iCb, iAb, iVar, BlendNegationAVXF( iCs, iCb ) ); }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Top         >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  iCs; }
template<> ULIS_FORCEINLINE Vec8f SeparableCompOpAVXF< Blend_Back        >( Vec8f iCs, Vec8f iCb, Vec8f iAb, Vec8f iVar ) { return  iCb; }

ULIS_NAMESPACE_END

