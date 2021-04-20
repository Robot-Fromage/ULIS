// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         VoronoiNoiseMT_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation of a Noise type.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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
InvokeVoronoiNoiseMT_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FVoronoiNoiseCommandArgs* cargs
)
{
    // Gather basic data for image traversal
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    T* ULIS_RESTRICT dst = reinterpret_cast< T* >( jargs->dst );

    // RNG
    const int32 seedy = cargs->dstRect.y + jargs->line + 1;
    uint32 localPRNGSeed = ( cargs->seed % seedy ) * 5323 + ( 2396403 % ( seedy + 64578 ) * seedy );

    // Gather x y
    const int y = jargs->line;
    const int x1 = cargs->dstRect.x;
    const int x2 = cargs->dstRect.w + x1;

    // Main scanline process loop
    for( int x = x1; x < x2; ++x ) {
        int closest_index = 0;
        float max_distance_squared = INT_MAX;
        for( int i = 0; i < cargs->points.size(); ++i )
        {
            float current_distance_squared = ( FVec2F( x, y ) - cargs->points[i] ).DistanceSquared();
            if( current_distance_squared < max_distance_squared )
            {
                max_distance_squared = current_distance_squared;
                closest_index = i;
            }
        }

        float floatvalue = FMath::Clamp( std::sqrt( max_distance_squared ) / cargs->normalisationFactor, 0.f, 1.f );
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

ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleVoronoiNoiseMT_MEM_Generic, FSimpleBufferJobArgs, FVoronoiNoiseCommandArgs, &InvokeVoronoiNoiseMT_MEM_Generic< T > )
ULIS_NAMESPACE_END

