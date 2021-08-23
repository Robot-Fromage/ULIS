// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Samplers.ipp
* @author       Clement Berthaud
* @brief        This file provides the implementation for transform samplers
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Transform/Methods.h"
#include "Maths/Geometry.h"

ULIS2_NAMESPACE_BEGIN
template< typename T > ULIS2_FORCEINLINE void
SampleBilinear( tByte* iDst, const tByte* iCA, const tByte* iCB, const FFormatInfo& iFMT, const float iT, const float iU ) {
    float alphaA, alphaB, alphaC;
    alphaA = alphaB = alphaC = 1.f;
    if( iFMT.HEA ) {
        alphaA = TYPE2FLOAT( iCA, iFMT.AID );
        alphaB = TYPE2FLOAT( iCB, iFMT.AID );
        alphaC = ( alphaA * iU + alphaB * iT );
        FLOAT2TYPE( iDst, iFMT.AID, alphaC );
    }
    for( int i = 0; i < iFMT.NCC; ++i ) {
        uint8 r = iFMT.IDT[i];
        *( reinterpret_cast< T* >( iDst ) + r ) = static_cast< T >( ( iCA[r] * alphaA * iU + iCB[r] * alphaB * iT ) / alphaC );
    }
}

float InterpCubic( float iA, float iB, float iC, float iD, float iT ) {
    float a = -iA / 2.0f + (3.0f*iB) / 2.0f - (3.0f*iC) / 2.0f + iD / 2.0f;
    float b = iA - (5.0f*iB) / 2.0f + 2.0f*iC - iD / 2.0f;
    float c = -iA / 2.0f + iC / 2.0f;
    float d = iB;
 
    return a * iT * iT * iT + b * iT * iT + c * iT + d;
}

template< typename T > ULIS2_FORCEINLINE void
SampleBicubicH( float* iDst, const tByte* iA, const tByte* iB, const tByte* iC, const tByte* iD, const FFormatInfo& iFMT, const float iT ) {
    float alphaA, alphaB, alphaC, alphaD, alphaR;
    alphaA = alphaB = alphaC = alphaD = alphaR = 1.f;
    if( iFMT.HEA ) {
        alphaA = TYPE2FLOAT( iA, iFMT.AID );
        alphaB = TYPE2FLOAT( iB, iFMT.AID );
        alphaC = TYPE2FLOAT( iC, iFMT.AID );
        alphaD = TYPE2FLOAT( iD, iFMT.AID );
        alphaR = InterpCubic( alphaA, alphaB, alphaC, alphaD, iT );
        iDst[iFMT.AID] = alphaR;
        alphaR = alphaR == 0.f ? 0.f : 1.f / alphaR;
    }
    for( int i = 0; i < iFMT.NCC; ++i ) {
        uint8 r = iFMT.IDT[i];
        iDst[r] = InterpCubic( ConvType< T, float >( iA[r] ) * alphaA
                             , ConvType< T, float >( iB[r] ) * alphaB
                             , ConvType< T, float >( iC[r] ) * alphaC
                             , ConvType< T, float >( iD[r] ) * alphaD
                             , iT ) * alphaR;
    }
}

template< typename T > ULIS2_FORCEINLINE void
SampleBicubicV( tByte* iDst, const float* iA, const float* iB, const float* iC, const float* iD, const FFormatInfo& iFMT, const float iT ) {
    float alphaA, alphaB, alphaC, alphaD, alphaR;
    alphaA = alphaB = alphaC = alphaD = alphaR = 1.f;
    if( iFMT.HEA ) {
        alphaA = iA[iFMT.AID];
        alphaB = iB[iFMT.AID];
        alphaC = iC[iFMT.AID];
        alphaD = iD[iFMT.AID];
        alphaR = InterpCubic( alphaA, alphaB, alphaC, alphaD, iT );
        FLOAT2TYPE( iDst, iFMT.AID, FMaths::Clamp( alphaR, 0.f, 1.f ) );
        alphaR = alphaR == 0.f ? 0.f : 1.f / alphaR;
    }
    for( int i = 0; i < iFMT.NCC; ++i ) {
        uint8 r = iFMT.IDT[i];
        FLOAT2TYPE( iDst, r, FMaths::Clamp( InterpCubic( iA[r] * alphaA, iB[r] * alphaB, iC[r] * alphaC, iD[r] * alphaD, iT ) * alphaR, 0.f, 1.f ) );

    }
}

ULIS2_NAMESPACE_END

