// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         NonSeparableBlendFuncF.h
* @author       Clement Berthaud
* @brief        This file provides the implementations for the float Non Separable Blending Modes functions.
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
// Helper for Computing Non Separable Blending functions
struct FRGBF {
    union {
        struct { float R, G, B; } rgb;
        float m[3];
    };
};

/////////////////////////////////////////////////////
// Helpers functions for Non Separable FRGBF Blending Modes Function
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- LumF
static ULIS3_FORCEINLINE ufloat LumF( const FRGBF& iC ) {
    return  0.3f * iC.m[0] + 0.59f * iC.m[1] + 0.11f * iC.m[2];
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- ClipColorF
static ULIS3_FORCEINLINE FRGBF ClipColorF( FRGBF iC ) {
    ufloat l = LumF( iC );
    ufloat n = FMaths::Min3( iC.m[0], iC.m[1], iC.m[2] );
    ufloat x = FMaths::Max3( iC.m[0], iC.m[1], iC.m[2] );
    if( n < 0.0f )
    {
        ufloat ln = l - n;
        iC.m[0] = l + ( ( ( iC.m[0] - l ) * l ) / ( ln ) );
        iC.m[1] = l + ( ( ( iC.m[1] - l ) * l ) / ( ln ) );
        iC.m[2] = l + ( ( ( iC.m[2] - l ) * l ) / ( ln ) );
    }

    if( x > 1.0f )
    {
        ufloat xl = x - l;
        ufloat ml = 1.f - l;
        iC.m[0] = l + ( ( ( iC.m[0] - l ) * ( ml ) ) / ( xl ) );
        iC.m[1] = l + ( ( ( iC.m[1] - l ) * ( ml ) ) / ( xl ) );
        iC.m[2] = l + ( ( ( iC.m[2] - l ) * ( ml ) ) / ( xl ) );
    }

    return  iC;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ SetLumF
static ULIS3_FORCEINLINE FRGBF SetLumF( const FRGBF& iC, ufloat iL ) {
    ufloat d = iL - LumF( iC );
    FRGBF C;
    C.m[0] = iC.m[0] + d;
    C.m[1] = iC.m[1] + d;
    C.m[2] = iC.m[2] + d;
    return  ClipColorF( C );
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- SatF
static ULIS3_FORCEINLINE ufloat SatF( const FRGBF& iC ) {
    return  FMaths::Max3( iC.m[0], iC.m[1], iC.m[2] ) - FMaths::Min3( iC.m[0], iC.m[1], iC.m[2] );
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ SetSatF
static ULIS3_FORCEINLINE FRGBF SetSatF( const FRGBF& iC, ufloat iS ) {
    uint8 maxIndex = iC.m[0] > iC.m[1] ? ( iC.m[0] > iC.m[2] ? 0 : 2 ) : ( iC.m[1] > iC.m[2] ? 1 : 2 );
    uint8 minIndex = iC.m[0] < iC.m[1] ? ( iC.m[0] < iC.m[2] ? 0 : 2 ) : ( iC.m[1] < iC.m[2] ? 1 : 2 );
    uint8 midIndex = 3 - maxIndex - minIndex;
    ufloat Cmax = iC.m[maxIndex];
    ufloat Cmin = iC.m[minIndex];
    ufloat Cmid = iC.m[midIndex];
    if( Cmax > Cmin )
    {
        Cmid = ( ( ( Cmid - Cmin ) * iS ) / ( Cmax - Cmin ) );
        Cmax = iS;
    }
    else
    {
        Cmid = Cmax = 0.f;
    }
    Cmin = 0.f;
    FRGBF ret;
    ret.m[maxIndex] = Cmax;
    ret.m[minIndex] = Cmin;
    ret.m[midIndex] = Cmid;
    return  ret;
}

/////////////////////////////////////////////////////
// Stantard Non Separable FRGBF Blending Modes Function
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- DarkerColor
ULIS3_FORCEINLINE FRGBF BlendDarkerColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  LumF( iCb ) < LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS3_FORCEINLINE FRGBF BlendLighterColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  LumF( iCb ) > LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS3_FORCEINLINE FRGBF BlendHueF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( SetSatF( iCs, SatF( iCb ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS3_FORCEINLINE FRGBF BlendSaturationF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( SetSatF( iCb, SatF( iCs ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS3_FORCEINLINE FRGBF BlendColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( iCs, LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS3_FORCEINLINE FRGBF BlendLuminosityF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( iCb, LumF( iCs ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Partial Derivative
ULIS3_FORCEINLINE FRGBF BlendPartialDerivativeF( const FRGBF& iCs, const FRGBF& iCb ) {
    glm::vec3 ns( iCs.m[0], iCs.m[1], iCs.m[2] );
    glm::vec3 nb( iCb.m[0], iCb.m[1], iCb.m[2] );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = glm::normalize( glm::vec3( ns.xy() * nb.z + nb.xy() * ns.z, ns.z * nb.z ) ) * 0.5f + 0.5f;
    return  FRGBF{ res.x, res.y, res.z };
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Whiteout
ULIS3_FORCEINLINE FRGBF BlendWhiteoutF( const FRGBF& iCs, const FRGBF& iCb ) {
    glm::vec3 ns( iCs.m[0], iCs.m[1], iCs.m[2] );
    glm::vec3 nb( iCb.m[0], iCb.m[1], iCb.m[2] );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = glm::normalize( glm::vec3( ns.xy() + nb.xy(), ns.z * nb.z ) ) * 0.5f + 0.5f;
    return  FRGBF{ res.x, res.y, res.z };
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- AngleCorrected
ULIS3_FORCEINLINE FRGBF BlendAngleCorrectedF( const FRGBF& iCs, const FRGBF& iCb ) {
    glm::vec3 ns( iCs.m[0], iCs.m[1], iCs.m[2] );
    glm::vec3 nb( iCb.m[0], iCb.m[1], iCb.m[2] );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = glm::normalize( glm::vec3( ns.xy() + nb.xy(), ns.z ) ) * 0.5f + 0.5f;
    return  FRGBF{ res.x, res.y, res.z };
}

/////////////////////////////////////////////////////
// NonSeparableOpF Template Selector
//--------------------------------------------------------------------------------------
//-------------------------------------------- Generic NonSeparableOpF Template Selector
template< eBlendingMode _BM >
ULIS3_FORCEINLINE FRGBF NonSeparableOpF( const FRGBF& iCs, const FRGBF& iCb ) {
    ULIS3_ASSERT( false, "Blend Specialization Not Implemented" );
    return  {};
}

//--------------------------------------------------------------------------------------
//------------------------------------ NonSeparableOpF Template Selector Specializations
template<> ULIS3_FORCEINLINE FRGBF NonSeparableOpF< BM_DARKERCOLOR          >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendDarkerColorF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE FRGBF NonSeparableOpF< BM_LIGHTERCOLOR         >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendLighterColorF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE FRGBF NonSeparableOpF< BM_HUE                  >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendHueF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE FRGBF NonSeparableOpF< BM_SATURATION           >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendSaturationF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE FRGBF NonSeparableOpF< BM_COLOR                >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendColorF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE FRGBF NonSeparableOpF< BM_LUMINOSITY           >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendLuminosityF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE FRGBF NonSeparableOpF< BM_PARTIALDERIVATIVE    >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendPartialDerivativeF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE FRGBF NonSeparableOpF< BM_WHITEOUT             >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendWhiteoutF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE FRGBF NonSeparableOpF< BM_ANGLECORRECTED       >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendAngleCorrectedF( iCs, iCb ); }

ULIS3_NAMESPACE_END

