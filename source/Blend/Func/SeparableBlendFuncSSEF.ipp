// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         SeparableBlendFuncSSEF.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Vec4f Blending functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Modes.h"
#include "Color/ModelStructs.h"
#include "Maths/Maths.h"
#include <vectorclass.h>

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Compositing
ULIS2_FORCEINLINE Vec4f ComposeSSEF( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar, Vec4f iCr ) {
    return ( 1.f - iVar ) * iCb + iVar * ( ( 1.f - iAb ) * iCs + iAb * iCr );
}

/////////////////////////////////////////////////////
// Standard Separable Blending Modes
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Normal
ULIS2_FORCEINLINE Vec4f BlendNormalSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS2_FORCEINLINE Vec4f BlendTopSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCs;
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Back
ULIS2_FORCEINLINE Vec4f BlendBackSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Behind
ULIS2_FORCEINLINE Vec4f BlendBehindSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Darken
ULIS2_FORCEINLINE Vec4f BlendDarkenSSEF( Vec4f iCs, Vec4f iCb ) {
    return  min( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Multiply
ULIS2_FORCEINLINE Vec4f BlendMultiplySSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- ColorBurn
ULIS2_FORCEINLINE Vec4f BlendColorBurnSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCb == 1.f, 1.f, select( iCs == 0.f, 0.f, 1.f - min( 1.f, ( 1.f - iCb ) / iCs ) ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Lighten
ULIS2_FORCEINLINE Vec4f BlendLightenSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Average
ULIS2_FORCEINLINE Vec4f BlendAverageSSEF( Vec4f iCs, Vec4f iCb ) {
    return  ( iCs + iCb ) / 2.f;
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Negation
ULIS2_FORCEINLINE Vec4f BlendNegationSSEF( Vec4f iCs, Vec4f iCb ) {
    return  1.f - abs( 1.f - iCs - iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Screen
ULIS2_FORCEINLINE Vec4f BlendScreenSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb + iCs - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- ColorDodge
ULIS2_FORCEINLINE Vec4f BlendColorDodgeSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCb == 0.f, 0.f, select( iCs ==1.f, 1.f, min( 1.f, iCb / ( 1.f - iCs ) ) ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS2_FORCEINLINE Vec4f BlendAddSSEF( Vec4f iCs, Vec4f iCb ) {
    return  min( 1.f, iCs + iCb );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearDodge
ULIS2_FORCEINLINE Vec4f BlendLinearDodgeSSEF( Vec4f iCs, Vec4f iCb ) {
    return  BlendAddSSEF( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- SoftLight
//ULIS2_FORCEINLINE Vec4f _softlight_D( Vec4f iX ) {
//    return  iX <= 0.25f ? ( ( 16.f * iX - 12.f ) * iX + 4 ) * iX : sqrtf( iX );
//}
ULIS2_FORCEINLINE Vec4f BlendSoftLightSSEF( Vec4f iCs, Vec4f iCb ) {
    //return  iCs <= 0.5f ? iCb - ( 1.f - 2.f * iCs ) * iCb * ( 1.f - iCb ) : iCb + ( 2.f * iCs - 1.f ) * ( _softlight_D( iCb ) - iCb );
    Vec4f  q = iCb * iCb;
    Vec4f  d = 2 * iCs;
    return  q + d * iCb - d * q;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- LinearBurn
ULIS2_FORCEINLINE Vec4f BlendLinearBurnSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( 0.f, iCs + iCb - 1.f );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- HardLight
ULIS2_FORCEINLINE Vec4f BlendHardLightSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCs <= 0.5f, BlendMultiplySSEF( iCb, 2.f * iCs ), BlendScreenSSEF( iCb, 2 * iCs - 1.f ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Overlay
ULIS2_FORCEINLINE Vec4f BlendOverlaySSEF( Vec4f iCs, Vec4f iCb ) {
    return  BlendHardLightSSEF( iCb, iCs );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- VividLight
ULIS2_FORCEINLINE Vec4f BlendVividLightSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCs <= 0.5f, BlendColorBurnSSEF( iCb, 2.f * iCs ), BlendColorDodgeSSEF( iCb, 2 * ( iCs - 0.5f ) ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- LinearLight
ULIS2_FORCEINLINE Vec4f BlendLinearLightSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( 0.f, min( iCb + 2.f * iCs - 1.f, 1.f ) );
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- PinLight
ULIS2_FORCEINLINE Vec4f BlendPinLightSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCs > 0.f, max( iCb, 2.f * iCs - 1.f ), min( iCs, 2.f * iCs ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ HardMix
ULIS2_FORCEINLINE Vec4f BlendHardMixSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCs + iCb < 0.999f, 0.f, select( iCs + iCb > 1.001f, 1.f, select( iCb > iCs, 1.f, 0.f ) ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Difference
ULIS2_FORCEINLINE Vec4f BlendDifferenceSSEF( Vec4f iCs, Vec4f iCb ) {
    return  abs( iCb - iCs );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Exclusion
ULIS2_FORCEINLINE Vec4f BlendExclusionSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb + iCs - 2.f * iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Substract
ULIS2_FORCEINLINE Vec4f BlendSubstractSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( 0.f, iCb - iCs );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Divide
ULIS2_FORCEINLINE Vec4f BlendDivideSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCs == 0.f && iCb == 0.f, 0.f, select( iCs == 0.f && iCb != 0.f, 1.f, max( 0.f, min( iCb / iCs, 1.f ) ) ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Phoenix
ULIS2_FORCEINLINE Vec4f BlendPhoenixSSEF( Vec4f iCs, Vec4f iCb ) {
    return  1.f - max( iCs, iCb ) + min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Reflect
ULIS2_FORCEINLINE Vec4f BlendReflectSSEF( Vec4f iCs, Vec4f iCb ) {
    return  select( iCb == 1.f, 1.f, min( 1.f, iCs * iCs / ( 1.f - iCb ) ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Glow
ULIS2_FORCEINLINE Vec4f BlendGlowSSEF( Vec4f iCs, Vec4f iCb ) {
    return  BlendReflectSSEF( iCb, iCs );
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------- SeparableCompOpSSEF Template Selector
template< eBlendingMode _BM >
ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) {
    ULIS2_ASSERT( false, "Blend Specialization Not Implemented" );
    return  0.f;
}

template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_NORMAL      >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendNormalSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_BEHIND      >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendBehindSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_DARKEN      >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendDarkenSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_MULTIPY     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendMultiplySSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_COLORBURN   >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendColorBurnSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_LINEARBURN  >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendLinearBurnSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_LIGHTEN     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendLightenSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_SCREEN      >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendScreenSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_COLORDODGE  >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendColorDodgeSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_LINEARDODGE >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendLinearDodgeSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_OVERLAY     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendOverlaySSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_SOFTLIGHT   >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendSoftLightSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_HARDLIGHT   >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendHardLightSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_VIVIDLIGHT  >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendVividLightSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_LINEARLIGHT >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendLinearLightSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_PINLIGHT    >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendPinLightSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_HARDMIX     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendHardMixSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_PHOENIX     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendPhoenixSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_REFLECT     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendReflectSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_GLOW        >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendGlowSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_DIFFERENCE  >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendDifferenceSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_EXCLUSION   >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendExclusionSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_ADD         >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendAddSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_SUBSTRACT   >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendSubstractSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_DIVIDE      >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendDivideSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_AVERAGE     >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendAverageSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_NEGATION    >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  ComposeSSEF( iCs, iCb, iAb, iVar, BlendNegationSSEF( iCs, iCb ) ); }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_TOP         >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  iCs; }
template<> ULIS2_FORCEINLINE Vec4f SeparableCompOpSSEF< BM_BACK        >( Vec4f iCs, Vec4f iCb, Vec4f iAb, Vec4f iVar ) { return  iCb; }

ULIS2_NAMESPACE_END

