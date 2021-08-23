// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         NonSeparableBlendFuncF.h
* @author       Clement Berthaud
* @brief        This file provides the implementations for the float Non Separable Blending Modes functions.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Vector.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
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
static ULIS_FORCEINLINE ufloat LumF( const FRGBF& iC ) {
    return  0.3f * iC.m[0] + 0.59f * iC.m[1] + 0.11f * iC.m[2];
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- ClipColorF
static ULIS_FORCEINLINE FRGBF ClipColorF( FRGBF iC ) {
    ufloat l = LumF( iC );
    ufloat n = FMath::Min3( iC.m[0], iC.m[1], iC.m[2] );
    ufloat x = FMath::Max3( iC.m[0], iC.m[1], iC.m[2] );
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
static ULIS_FORCEINLINE FRGBF SetLumF( const FRGBF& iC, ufloat iL ) {
    ufloat d = iL - LumF( iC );
    FRGBF C;
    C.m[0] = iC.m[0] + d;
    C.m[1] = iC.m[1] + d;
    C.m[2] = iC.m[2] + d;
    return  ClipColorF( C );
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- SatF
static ULIS_FORCEINLINE ufloat SatF( const FRGBF& iC ) {
    return  FMath::Max3( iC.m[0], iC.m[1], iC.m[2] ) - FMath::Min3( iC.m[0], iC.m[1], iC.m[2] );
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ SetSatF
static ULIS_FORCEINLINE FRGBF SetSatF( const FRGBF& iC, ufloat iS ) {
    #pragma warning(push)
    // Disable warning about potential out of bounds access
    // I know what i'm doing here and midIndex is valid.
    #pragma warning(disable : 6386)
    #pragma warning(disable : 6385)
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
    #pragma warning(pop)
}

/////////////////////////////////////////////////////
// Stantard Non Separable FRGBF Blending Modes Function
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- DarkerColor
ULIS_FORCEINLINE FRGBF BlendDarkerColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  LumF( iCb ) < LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS_FORCEINLINE FRGBF BlendLighterColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  LumF( iCb ) > LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS_FORCEINLINE FRGBF BlendHueF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( SetSatF( iCs, SatF( iCb ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS_FORCEINLINE FRGBF BlendSaturationF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( SetSatF( iCb, SatF( iCs ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS_FORCEINLINE FRGBF BlendColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( iCs, LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS_FORCEINLINE FRGBF BlendLuminosityF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( iCb, LumF( iCs ) );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Partial Derivative
ULIS_FORCEINLINE FRGBF BlendPartialDerivativeF( const FRGBF& iCs, const FRGBF& iCb ) {
    FVec3F ns( iCs.m[0], iCs.m[1], iCs.m[2] );
    FVec3F nb( iCb.m[0], iCb.m[1], iCb.m[2] );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = FVec3F( ns.xy() * nb.z + nb.xy() * ns.z, ns.z * nb.z ).Normalized() * 0.5f + 0.5f;
    return  FRGBF{ res.x, res.y, res.z };
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Whiteout
ULIS_FORCEINLINE FRGBF BlendWhiteoutF( const FRGBF& iCs, const FRGBF& iCb ) {
    FVec3F ns( iCs.m[0], iCs.m[1], iCs.m[2] );
    FVec3F nb( iCb.m[0], iCb.m[1], iCb.m[2] );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = FVec3F( ns.xy() + nb.xy(), ns.z * nb.z ).Normalized() * 0.5f + 0.5f;
    return  FRGBF{ res.x, res.y, res.z };
}
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- AngleCorrected
ULIS_FORCEINLINE FRGBF BlendAngleCorrectedF( const FRGBF& iCs, const FRGBF& iCb ) {
    FVec3F ns( iCs.m[0], iCs.m[1], iCs.m[2] );
    FVec3F nb( iCb.m[0], iCb.m[1], iCb.m[2] );
    ns = ns * 2.f - 1.f;
    nb = nb * 2.f - 1.f;
    auto res = FVec3F( ns.xy() + nb.xy(), ns.z ).Normalized() * 0.5f + 0.5f;
    return  FRGBF{ res.x, res.y, res.z };
}

/////////////////////////////////////////////////////
// NonSeparableOpF Template Selector
//--------------------------------------------------------------------------------------
//-------------------------------------------- Generic NonSeparableOpF Template Selector
template< eBlendMode _BM >
ULIS_FORCEINLINE FRGBF NonSeparableOpF( const FRGBF& iCs, const FRGBF& iCb ) {
    ULIS_ASSERT( false, "Blend Specialization Not Implemented" );
    return  {};
}

//--------------------------------------------------------------------------------------
//------------------------------------ NonSeparableOpF Template Selector Specializations
template<> ULIS_FORCEINLINE FRGBF NonSeparableOpF< Blend_DarkerColor          >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendDarkerColorF( iCs, iCb ); }
template<> ULIS_FORCEINLINE FRGBF NonSeparableOpF< Blend_LighterColor         >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendLighterColorF( iCs, iCb ); }
template<> ULIS_FORCEINLINE FRGBF NonSeparableOpF< Blend_Hue                  >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendHueF( iCs, iCb ); }
template<> ULIS_FORCEINLINE FRGBF NonSeparableOpF< Blend_Saturation           >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendSaturationF( iCs, iCb ); }
template<> ULIS_FORCEINLINE FRGBF NonSeparableOpF< Blend_Color                >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendColorF( iCs, iCb ); }
template<> ULIS_FORCEINLINE FRGBF NonSeparableOpF< Blend_Luminosity           >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendLuminosityF( iCs, iCb ); }
template<> ULIS_FORCEINLINE FRGBF NonSeparableOpF< Blend_PartialDerivative    >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendPartialDerivativeF( iCs, iCb ); }
template<> ULIS_FORCEINLINE FRGBF NonSeparableOpF< Blend_Whiteout             >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendWhiteoutF( iCs, iCb ); }
template<> ULIS_FORCEINLINE FRGBF NonSeparableOpF< Blend_AngleCorrected       >( const FRGBF& iCs, const FRGBF& iCb ) { return  BlendAngleCorrectedF( iCs, iCb ); }

ULIS_NAMESPACE_END

