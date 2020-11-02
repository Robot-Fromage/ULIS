// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         TransformHelpers.h
* @author       Clement Berthaud
* @brief        This file provides some helpers for transform tasks.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Format.h"

#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
#include <vectorclass.h>
#endif

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Helpers functions for Redundant Transform Operations
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- SampleBilinear
template< typename T > ULIS_FORCEINLINE void
SampleBilinear( uint8* iDst, const uint8* iCA, const uint8* iCB, const FFormatMetrics& iFMT, const float iT, const float iU ) {
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
        *( reinterpret_cast< T* >( iDst ) + r ) = alphaC == 0.f ? static_cast< T >( 0.f ) : static_cast< T >( ( reinterpret_cast< const T* >( iCA )[r] * alphaA * iU + reinterpret_cast< const T* >( iCB )[r] * alphaB * iT ) / alphaC );
    }
}

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- SampleBilinearSAT
template< typename T > ULIS_FORCEINLINE void
SampleBilinearSAT( uint8* iDst, const uint8* iCA, const uint8* iCB, const FFormatMetrics& iFMT, const float iT, const float iU ) {
    for( int i = 0; i < iFMT.SPP; ++i ) {
        uint8 r = iFMT.IDT[i];
        *( reinterpret_cast< T* >( iDst ) + r ) = static_cast< T >( ( reinterpret_cast< const T* >( iCA )[r] * iU + reinterpret_cast< const T* >( iCB )[r] * iT ) );
    }
}

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- InterpCubic
float InterpCubic( float iA, float iB, float iC, float iD, float iT ) {
    float a = -iA / 2.0f + (3.0f*iB) / 2.0f - (3.0f*iC) / 2.0f + iD / 2.0f;
    float b = iA - (5.0f*iB) / 2.0f + 2.0f*iC - iD / 2.0f;
    float c = -iA / 2.0f + iC / 2.0f;
    float d = iB;
 
    return a * iT * iT * iT + b * iT * iT + c * iT + d;
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- SampleBicubicH
template< typename T > ULIS_FORCEINLINE void
SampleBicubicH( float* iDst, const uint8* iA, const uint8* iB, const uint8* iC, const uint8* iD, const FFormatMetrics& iFMT, const float iT ) {
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

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- SampleBicubicV
template< typename T > ULIS_FORCEINLINE void
SampleBicubicV( uint8* iDst, const float* iA, const float* iB, const float* iC, const float* iD, const FFormatMetrics& iFMT, const float iT ) {
    float alphaA, alphaB, alphaC, alphaD, alphaR;
    alphaA = alphaB = alphaC = alphaD = alphaR = 1.f;
    if( iFMT.HEA ) {
        alphaA = iA[iFMT.AID];
        alphaB = iB[iFMT.AID];
        alphaC = iC[iFMT.AID];
        alphaD = iD[iFMT.AID];
        alphaR = InterpCubic( alphaA, alphaB, alphaC, alphaD, iT );
        FLOAT2TYPE( iDst, iFMT.AID, FMath::Clamp( alphaR, 0.f, 1.f ) );
        alphaR = alphaR == 0.f ? 0.f : 1.f / alphaR;
    }
    for( int i = 0; i < iFMT.NCC; ++i ) {
        uint8 r = iFMT.IDT[i];
        FLOAT2TYPE( iDst, r, FMath::Clamp( InterpCubic( iA[r] * alphaA, iB[r] * alphaB, iC[r] * alphaC, iD[r] * alphaD, iT ) * alphaR, 0.f, 1.f ) );
    }
}

/////////////////////////////////////////////////////
// SSE Helpers functions for specialized Transform Operations
#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- InterpCubic
ULIS_FORCEINLINE Vec4f InterpCubic( Vec4f iA, Vec4f iB, Vec4f iC, Vec4f iD, Vec4f iT ) {
    Vec4f a = -iA / 2.0f + (3.0f*iB) / 2.0f - (3.0f*iC) / 2.0f + iD / 2.0f;
    Vec4f b = iA - (5.0f*iB) / 2.0f + 2.0f*iC - iD / 2.0f;
    Vec4f c = -iA / 2.0f + iC / 2.0f;
    Vec4f d = iB;
    return a * iT * iT * iT + b * iT * iT + c * iT + d;
}
#endif

ULIS_NAMESPACE_END

