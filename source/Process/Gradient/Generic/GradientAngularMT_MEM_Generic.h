// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         GradientAngularMT_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation of a Gradient type.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Core/Constants.h"
#include "Image/Block.h"
#include "Image/Gradient.h"
#include "Process/Gradient/GradientArgs.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeGradientAngularMT_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FGradientCommandArgs* cargs
)
{
    // Gather basic data for image traversal
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    T* ULIS_RESTRICT dst = reinterpret_cast< T* >( jargs->dst );

    // Gather gradient data
    const FSanitizedGradient& grad = cargs->gradient;
    const uint8* ULIS_RESTRICT indexLutColor = grad.IndexLUTColor();
    const uint8* ULIS_RESTRICT indexLutAlpha = grad.IndexLUTAlpha();
    const TArray< FColorStep >& colorSteps = grad.ColorSteps();
    const TArray< FAlphaStep >& alphaSteps = grad.AlphaSteps();
    const FColorStep* prevColorStep;
    const FColorStep* nextColorStep;
    const FAlphaStep* prevAlphaStep;
    const FAlphaStep* nextAlphaStep;

    // Gather geometric data for gradient parameter computation
    const FVec2F p0 = cargs->start;
    const FVec2F p1 = cargs->end;
    const FVec2F delta = p1 - p0;
    const float angle = atan2f( p0.y - p1.y, p0.x - p1.x );
    const float pi2 = FMath::kPIf * 2;
    // Gather x y
    FVec2F src;
    float t;
    const int y = jargs->line;
    const int x1 = cargs->dstRect.x;
    const int x2 = cargs->dstRect.w + x1;

    // RNG
    const int32 seedy = cargs->dstRect.y + jargs->line + 1;
    uint32 localPRNGSeed = ( 8253729 % seedy ) * 5323 + ( 2396403 % ( seedy + 64578 ) * seedy );

    // Main scanline process loop
    for( int x = x1; x < x2; ++x ) {
        // Retrieve source point ( integer coordinates, top left corner of pixel ).
        src.x = static_cast< float >( x );
        src.y = static_cast< float >( y );
        t = fmodf( pi2 + atan2f( ( p0.y - src.y ), ( p0.x - src.x ) ) - angle, pi2 );

        localPRNGSeed = 8253729 * localPRNGSeed + 2396403;
        const ufloat toss1 = ( localPRNGSeed % 65537 ) / 65537.f;
        localPRNGSeed = 8253729 * localPRNGSeed + 2396403;
        const ufloat toss2 = ( localPRNGSeed % 65537 ) / 65537.f;

        const float dither = ( toss1 - toss2 );
        const ufloat param = FMath::Clamp( ( t + ( dither * cargs->dithering * MaxType< T >() ) / pi2 ) / pi2, 0.f, 1.f );
        uint8 index = static_cast< uint8 >( param * FSanitizedGradient::range );
        if( index >= FSanitizedGradient::range ) index = FSanitizedGradient::range - 1;
        prevColorStep = &( colorSteps[ indexLutColor[ index ] ] );
        nextColorStep = &( colorSteps[ indexLutColor[ index ] + 1 ] );
        prevAlphaStep = &( alphaSteps[ indexLutAlpha[ index ] ] );
        nextAlphaStep = &( alphaSteps[ indexLutAlpha[ index ] + 1 ] );
        const ufloat prevColorParam = prevColorStep->Param();
        const ufloat nextColorParam = nextColorStep->Param();
        const ufloat prevAlphaParam = prevAlphaStep->Param();
        const ufloat nextAlphaParam = nextAlphaStep->Param();
        const ufloat deltaColorParam = nextColorParam - prevColorParam;
        const ufloat deltaAlphaParam = nextAlphaParam - prevAlphaParam;
        const FColor& prevColor = prevColorStep->Value();
        const FColor& nextColor = nextColorStep->Value();
        const ufloat prevAlpha = prevAlphaStep->Value();
        const ufloat nextAlpha = nextAlphaStep->Value();
        const ufloat linearParamColor = deltaColorParam == 0.f ? 0.f : ( param - prevColorParam ) / deltaColorParam;
        const ufloat linearParamAlpha = deltaAlphaParam == 0.f ? 0.f : ( param - prevAlphaParam ) / deltaAlphaParam;

        // Interpolation Color
        for( uint8 i = 0; i < fmt.NCC; ++i ) {
            const uint8 r = fmt.IDT[i];
            dst[r] = static_cast< T >( prevColor.ChannelT< T >( i ) * ( 1.f - linearParamColor ) +  nextColor.ChannelT< T >( i ) * linearParamColor );
        }

        // Interpolation Alpha
        const ufloat interpolatedAlpha = prevAlpha * ( 1.f - linearParamAlpha ) + nextAlpha * linearParamAlpha;
        dst[ fmt.AID ] = static_cast< T >( ( prevColor.AlphaT< T >() * ( 1.f - linearParamColor ) +  nextColor.AlphaT< T >() * linearParamColor ) * interpolatedAlpha );

        // Advance in buffer
        dst += fmt.SPP;
    }
}

ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleGradientAngularMT_MEM_Generic, FSimpleBufferJobArgs, FGradientCommandArgs, &InvokeGradientAngularMT_MEM_Generic< T > )
ULIS_NAMESPACE_END

