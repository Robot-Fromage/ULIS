// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BrownianNoiseMT_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation of a Noise type.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Core/Constants.h"
#include "Image/Block.h"
#include "Process/Noise/NoiseArgs.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeBrownianNoiseMT_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FBrownianNoiseCommandArgs* cargs
)
{
    // Gather basic data for image traversal
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    T* ULIS_RESTRICT dst = reinterpret_cast< T* >( jargs->dst );

    // Gather x y
    const int y = jargs->line;
    const int x1 = cargs->dstRect.x;
    const int x2 = cargs->dstRect.w + x1;

    // Main scanline process loop
    for( int x = x1; x < x2; ++x ) {
        FVec2F pointNoise = FVec2F( x, y ) * cargs->frequency;
        float amplitude = 1.f;
        float floatvalue = 0.f; // ?

        for( uint8 i = 0; i < cargs->numLayers; ++i )
        {
            floatvalue  += cargs->noise.eval( pointNoise ) * amplitude;
            pointNoise  *= cargs->frequencyMult;
            amplitude   *= cargs->amplitudeMult;
        }

        floatvalue /= cargs->amplitudeMax;
        T value = ConvType< float, T >( floatvalue );

        for( uint8 i = 0; i < fmt.NCC; ++i ) {
            const uint8 r = fmt.IDT[i];
            dst[r] = value;
        }

        if( fmt.HEA )
            dst[fmt.AID] = MaxType< T >();

        dst += fmt.SPP;
    }
}

ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleBrownianNoiseMT_MEM_Generic, FSimpleBufferJobArgs, FBrownianNoiseCommandArgs, &InvokeBrownianNoiseMT_MEM_Generic< T > )
ULIS_NAMESPACE_END

