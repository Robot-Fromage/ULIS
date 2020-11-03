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
static ULIS2_FORCEINLINE ufloat LumF( const FRGBF& iC ) {
    return  0.3f * iC.R + 0.59f * iC.G + 0.11f * iC.B;
}

static ULIS2_FORCEINLINE FRGBF ClipColorF( FRGBF iC ) {
    ufloat l = LumF( iC );
    ufloat n = FMaths::Min3( iC.R, iC.G, iC.B );
    ufloat x = FMaths::Max3( iC.R, iC.G, iC.B );
    if( n < 0.0f )
    {
        ufloat ln = l - n;
        iC.R = l + ( ( ( iC.R - l ) * l ) / ( ln ) );
        iC.G = l + ( ( ( iC.G - l ) * l ) / ( ln ) );
        iC.B = l + ( ( ( iC.B - l ) * l ) / ( ln ) );
    }

    if( x > 1.0f )
    {
        ufloat xl = x - l;
        ufloat ml = 1.f - l;
        iC.R = l + ( ( ( iC.R - l ) * ( ml ) ) / ( xl ) );
        iC.G = l + ( ( ( iC.G - l ) * ( ml ) ) / ( xl ) );
        iC.B = l + ( ( ( iC.B - l ) * ( ml ) ) / ( xl ) );
    }

    return  iC;
}

static ULIS2_FORCEINLINE FRGBF SetLumF( const FRGBF& iC, ufloat iL ) {
    ufloat d = iL - LumF( iC );
    FRGBF C;
    C.R = iC.R + d;
    C.G = iC.G + d;
    C.B = iC.B + d;
    return  ClipColorF( C );
}

static ULIS2_FORCEINLINE ufloat SatF( const FRGBF& iC ) {
    return  FMaths::Max3( iC.R, iC.G, iC.B ) - FMaths::Min3( iC.R, iC.G, iC.B );
}

static ULIS2_FORCEINLINE FRGBF SetSatF( const FRGBF& iC, ufloat iS ) {
    uint8 maxIndex = iC.R > iC.G ? ( iC.R > iC.B ? 0 : 2 ) : ( iC.G > iC.B ? 1 : 2 );
    uint8 minIndex = iC.R < iC.G ? ( iC.R < iC.B ? 0 : 2 ) : ( iC.G < iC.B ? 1 : 2 );
    uint8 midIndex = 3 - maxIndex - minIndex;
    ufloat Cmax = *( ( (ufloat*)( &iC ) ) + maxIndex );
    ufloat Cmin = *( ( (ufloat*)( &iC ) ) + minIndex );
    ufloat Cmid = *( ( (ufloat*)( &iC ) ) + midIndex );
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
    *( ( (ufloat*)( &ret ) ) + maxIndex ) = Cmax;
    *( ( (ufloat*)( &ret ) ) + minIndex ) = Cmin;
    *( ( (ufloat*)( &ret ) ) + midIndex ) = Cmid;
    return  ret;
}

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- DarkerColor
ULIS2_FORCEINLINE FRGBF BlendDarkerColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  LumF( iCb ) < LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS2_FORCEINLINE FRGBF BlendLighterColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  LumF( iCb ) > LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS2_FORCEINLINE FRGBF BlendHueF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( SetSatF( iCs, SatF( iCb ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS2_FORCEINLINE FRGBF BlendSaturationF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( SetSatF( iCb, SatF( iCs ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS2_FORCEINLINE FRGBF BlendColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( iCs, LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS2_FORCEINLINE FRGBF BlendLuminosityF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( iCb, LumF( iCs ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Partial Derivative
ULIS2_FORCEINLINE FRGBF BlendPartialDerivativeF( const FRGBF& iCs, const FRGBF& iCb ) {
    glm::vec3 ns( iCs.R, iCs.G, iCs.B );
    glm::vec3 nb( iCb.R, iCb.G, iCb.B );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = glm::normalize( glm::vec3( ns.xy * nb.z + nb.xy * ns.z, ns.z * nb.z ) ) * 0.5f + 0.5f;
    return  FRGBF{ res.x, res.y, res.z };
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Whiteout
ULIS2_FORCEINLINE FRGBF BlendWhiteoutF( const FRGBF& iCs, const FRGBF& iCb ) {
    glm::vec3 ns( iCs.R, iCs.G, iCs.B );
    glm::vec3 nb( iCb.R, iCb.G, iCb.B );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = glm::normalize( glm::vec3( ns.xy + nb.xy, ns.z * nb.z ) ) * 0.5f + 0.5f;
    return  FRGBF{ res.x, res.y, res.z };
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- AngleCorrected
ULIS2_FORCEINLINE FRGBF BlendAngleCorrectedF( const FRGBF& iCs, const FRGBF& iCb ) {
    glm::vec3 ns( iCs.R, iCs.G, iCs.B );
    glm::vec3 nb( iCb.R, iCb.G, iCb.B );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = glm::normalize( glm::vec3( ns.xy + nb.xy, ns.z ) ) * 0.5f + 0.5f;
    return  FRGBF{ res.x, res.y, res.z };
}

/////////////////////////////////////////////////////
// Non Separable Blending Modes for FRGBF_arr
static ULIS2_FORCEINLINE ufloat LumF( const FRGBF_arr& iC ) {
    return  0.3f * iC.m[0] + 0.59f * iC.m[1] + 0.11f * iC.m[2];
}

static ULIS2_FORCEINLINE FRGBF_arr ClipColorF( FRGBF_arr iC ) {
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

static ULIS2_FORCEINLINE FRGBF_arr SetLumF( const FRGBF_arr& iC, ufloat iL ) {
    ufloat d = iL - LumF( iC );
    FRGBF_arr C;
    C.m[0] = iC.m[0] + d;
    C.m[1] = iC.m[1] + d;
    C.m[2] = iC.m[2] + d;
    return  ClipColorF( C );
}

static ULIS2_FORCEINLINE ufloat SatF( const FRGBF_arr& iC ) {
    return  FMaths::Max3( iC.m[0], iC.m[1], iC.m[2] ) - FMaths::Min3( iC.m[0], iC.m[1], iC.m[2] );
}

static ULIS2_FORCEINLINE FRGBF_arr SetSatF( const FRGBF_arr& iC, ufloat iS ) {
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
    FRGBF_arr ret;
    ret.m[maxIndex] = Cmax;
    ret.m[minIndex] = Cmin;
    ret.m[midIndex] = Cmid;
    return  ret;
}

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- DarkerColor
ULIS2_FORCEINLINE FRGBF_arr BlendDarkerColorF( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) {
    return  LumF( iCb ) < LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS2_FORCEINLINE FRGBF_arr BlendLighterColorF( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) {
    return  LumF( iCb ) > LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS2_FORCEINLINE FRGBF_arr BlendHueF( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) {
    return  SetLumF( SetSatF( iCs, SatF( iCb ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS2_FORCEINLINE FRGBF_arr BlendSaturationF( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) {
    return  SetLumF( SetSatF( iCb, SatF( iCs ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS2_FORCEINLINE FRGBF_arr BlendColorF( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) {
    return  SetLumF( iCs, LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS2_FORCEINLINE FRGBF_arr BlendLuminosityF( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) {
    return  SetLumF( iCb, LumF( iCs ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Partial Derivative
ULIS2_FORCEINLINE FRGBF_arr BlendPartialDerivativeF( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) {
    glm::vec3 ns( iCs.m[0], iCs.m[1], iCs.m[2] );
    glm::vec3 nb( iCb.m[0], iCb.m[1], iCb.m[2] );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = glm::normalize( glm::vec3( ns.xy * nb.z + nb.xy * ns.z, ns.z * nb.z ) ) * 0.5f + 0.5f;
    return  FRGBF_arr{ res.x, res.y, res.z };
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Whiteout
ULIS2_FORCEINLINE FRGBF_arr BlendWhiteoutF( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) {
    glm::vec3 ns( iCs.m[0], iCs.m[1], iCs.m[2] );
    glm::vec3 nb( iCb.m[0], iCb.m[1], iCb.m[2] );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = glm::normalize( glm::vec3( ns.xy + nb.xy, ns.z * nb.z ) ) * 0.5f + 0.5f;
    return  FRGBF_arr{ res.x, res.y, res.z };
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- AngleCorrected
ULIS2_FORCEINLINE FRGBF_arr BlendAngleCorrectedF( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) {
    glm::vec3 ns( iCs.m[0], iCs.m[1], iCs.m[2] );
    glm::vec3 nb( iCb.m[0], iCb.m[1], iCb.m[2] );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = glm::normalize( glm::vec3( ns.xy + nb.xy, ns.z ) ) * 0.5f + 0.5f;
    return  FRGBF_arr{ res.x, res.y, res.z };
}

/////////////////////////////////////////////////////
// Non Separable Blending Modes for Grey, these are actually separable
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- DarkerColor
ULIS2_FORCEINLINE ufloat BlendDarkerColorF( ufloat iCs, ufloat iCb ) {
    return  iCb < iCs ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS2_FORCEINLINE ufloat BlendLighterColorF( ufloat iCs, ufloat iCb ) {
    return  iCb > iCs ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS2_FORCEINLINE ufloat BlendHueF( ufloat iCs, ufloat iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS2_FORCEINLINE ufloat BlendSaturationF( ufloat iCs, ufloat iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS2_FORCEINLINE ufloat BlendColorF( ufloat iCs, ufloat iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS2_FORCEINLINE ufloat BlendLuminosityF( ufloat iCs, ufloat iCb ) {
    return  iCs;
}
/////////////////////////////////////////////////////
// Non Separable Blending Modes for Lab LCh
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- DarkerColor
ULIS2_FORCEINLINE FLChF BlendDarkerColorF( const FLChF& iCs, const FLChF& iCb ) {
    return  iCb.L < iCs.L ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS2_FORCEINLINE FLChF BlendLighterColorF( const FLChF& iCs, const FLChF& iCb ) {
    return  iCb.L > iCs.L ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS2_FORCEINLINE FLChF BlendHueF( const FLChF& iCs, const FLChF& iCb ) {
    return  { iCb.L, iCb.C, iCs.h };
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS2_FORCEINLINE FLChF BlendSaturationF( const FLChF& iCs, const FLChF& iCb ) {
    return  { iCb.L, iCs.C, iCb.h };
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS2_FORCEINLINE FLChF BlendColorF( const FLChF& iCs, const FLChF& iCb ) {
    return  { iCb.L, iCs.C, iCs.h };
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS2_FORCEINLINE FLChF BlendLuminosityF( const FLChF& iCs, const FLChF& iCb ) {
    return  { iCs.L, iCb.C, iCb.h };
}

//--------------------------------------------------------------------------------------
//-------------------------------------------- NonSeparableOpF Template Selector for RGB
template< eBlendingMode _BM >
ULIS2_FORCEINLINE FRGBF NonSeparableOpF( const FRGBF& iCs, const FRGBF& iCb ) {
    ULIS2_ASSERT( false, "Blend Specialization Not Implemented" );
    return  {};
}

template<> ULIS2_FORCEINLINE FRGBF NonSeparableOpF< BM_DARKERCOLOR          >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendDarkerColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF NonSeparableOpF< BM_LIGHTERCOLOR         >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendLighterColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF NonSeparableOpF< BM_HUE                  >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendHueF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF NonSeparableOpF< BM_SATURATION           >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendSaturationF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF NonSeparableOpF< BM_COLOR                >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF NonSeparableOpF< BM_LUMINOSITY           >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendLuminosityF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF NonSeparableOpF< BM_PARTIALDERIVATIVE    >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendPartialDerivativeF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF NonSeparableOpF< BM_WHITEOUT             >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendWhiteoutF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF NonSeparableOpF< BM_ANGLECORRECTED       >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendAngleCorrectedF( iCs, iCb ); }

//--------------------------------------------------------------------------------------
//-------------------------------------------- NonSeparableOpF Template Selector for FRGBF_arr
template< eBlendingMode _BM >
ULIS2_FORCEINLINE FRGBF_arr NonSeparableOpF( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) {
    ULIS2_ASSERT( false, "Blend Specialization Not Implemented" );
    return  {};
}

template<> ULIS2_FORCEINLINE FRGBF_arr NonSeparableOpF< BM_DARKERCOLOR          >( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) { return  BlendDarkerColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF_arr NonSeparableOpF< BM_LIGHTERCOLOR         >( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) { return  BlendLighterColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF_arr NonSeparableOpF< BM_HUE                  >( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) { return  BlendHueF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF_arr NonSeparableOpF< BM_SATURATION           >( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) { return  BlendSaturationF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF_arr NonSeparableOpF< BM_COLOR                >( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) { return  BlendColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF_arr NonSeparableOpF< BM_LUMINOSITY           >( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) { return  BlendLuminosityF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF_arr NonSeparableOpF< BM_PARTIALDERIVATIVE    >( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) { return  BlendPartialDerivativeF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF_arr NonSeparableOpF< BM_WHITEOUT             >( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) { return  BlendWhiteoutF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FRGBF_arr NonSeparableOpF< BM_ANGLECORRECTED       >( const FRGBF_arr& iCs, const FRGBF_arr& iCb ) { return  BlendAngleCorrectedF( iCs, iCb ); }


//--------------------------------------------------------------------------------------
//------------------------------------------- NonSeparableOpF Template Selector for Grey
template< eBlendingMode _BM >
ULIS2_FORCEINLINE ufloat NonSeparableOpF( ufloat iCs, ufloat iCb ) {
    ULIS2_ASSERT( false, "Blend Specialization Not Implemented" );
    return  0;
}

template<> ULIS2_FORCEINLINE float NonSeparableOpF< BM_DARKERCOLOR  >( float iCs, float iCb ) { return  BlendDarkerColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE float NonSeparableOpF< BM_LIGHTERCOLOR >( float iCs, float iCb ) { return  BlendLighterColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE float NonSeparableOpF< BM_HUE          >( float iCs, float iCb ) { return  BlendHueF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE float NonSeparableOpF< BM_SATURATION   >( float iCs, float iCb ) { return  BlendSaturationF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE float NonSeparableOpF< BM_COLOR        >( float iCs, float iCb ) { return  BlendColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE float NonSeparableOpF< BM_LUMINOSITY   >( float iCs, float iCb ) { return  BlendLuminosityF( iCs, iCb ); }
//--------------------------------------------------------------------------------------
//---------------------------------------- NonSeparableOpF Template Selector for Lab LCh
template< eBlendingMode _BM >
ULIS2_FORCEINLINE FLChF NonSeparableOpF( const FLChF& iCs, const FLChF& iCb ) {
    ULIS2_ASSERT( false, "Blend Specialization Not Implemented" );
    return  {};
}

template<> ULIS2_FORCEINLINE FLChF NonSeparableOpF< BM_DARKERCOLOR  >( const FLChF& iCs, const FLChF& iCb ) { return  BlendDarkerColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FLChF NonSeparableOpF< BM_LIGHTERCOLOR >( const FLChF& iCs, const FLChF& iCb ) { return  BlendLighterColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FLChF NonSeparableOpF< BM_HUE          >( const FLChF& iCs, const FLChF& iCb ) { return  BlendHueF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FLChF NonSeparableOpF< BM_SATURATION   >( const FLChF& iCs, const FLChF& iCb ) { return  BlendSaturationF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FLChF NonSeparableOpF< BM_COLOR        >( const FLChF& iCs, const FLChF& iCb ) { return  BlendColorF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE FLChF NonSeparableOpF< BM_LUMINOSITY   >( const FLChF& iCs, const FLChF& iCb ) { return  BlendLuminosityF( iCs, iCb ); }

ULIS2_NAMESPACE_END

