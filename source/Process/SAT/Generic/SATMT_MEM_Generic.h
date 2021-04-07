// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SATMT_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the SATMT_MEM_Generic API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations Regular
template< typename T >
void
InvokeBuildSATXPassMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
    const FFormatMetrics& src_fmt = cargs->src.FormatMetrics();
    const FFormatMetrics& dst_fmt = cargs->dst.FormatMetrics();
    const T* ULIS_RESTRICT src = reinterpret_cast< const T* >( jargs->src );
    float*   ULIS_RESTRICT dst = reinterpret_cast< float* >( jargs->dst );

    for( uint8 j = 0; j < src_fmt.SPP; ++j )
        dst[j] = static_cast< float >( src[j] );

    src += src_fmt.SPP;
    dst += dst_fmt.SPP;

    for( int x = 1; x < cargs->srcRect.w; ++x ) {
        for( uint8 j = 0; j < src_fmt.SPP; ++j )
            dst[j] = static_cast< float >( src[j] + *( dst - dst_fmt.SPP + j ) );
        src += src_fmt.SPP;
        dst += dst_fmt.SPP;
    }
}

template< typename T >
void
InvokeBuildSATYPassMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
    // Warning, explanation of dirty SAT trick:
    // we trick the scheduler into scheduling columns by swapping width and
    // height before scheduling the command so we have the right amount of
    // tasks, jobs, and workers assigned, but the jargs->dst assigned is
    // garbage, hence the PixelBits used to query the appropriate column at row
    // one. We then proceed to run along the column, until we reach
    // cargs->dstRect.h because the dimensions have been swapped back.
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    const uint32 stride = cargs->dst.Width() * fmt.SPP;
    float* dst = reinterpret_cast< float* >( cargs->dst.PixelBits( jargs->line, 1 ) );

    for( int y = 1; y < cargs->dstRect.h; ++y ) {
        for( uint8 j = 0; j < fmt.SPP; ++j ) {
            auto jojo = dst - stride + j;
            dst[j] = static_cast< float >( dst[j] + *( dst - stride + j ) );
        }
        dst += stride;
    }
}

template< typename T >
void
InvokeBuildPremultSATXPassMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
    const FFormatMetrics& src_fmt = cargs->src.FormatMetrics();
    const FFormatMetrics& dst_fmt = cargs->dst.FormatMetrics();
    const T* ULIS_RESTRICT src = reinterpret_cast< const T* >( jargs->src );
    float*   ULIS_RESTRICT dst = reinterpret_cast< float* >( jargs->dst );
    float max = static_cast< float >( MaxType< T >() );

    {
        float alpha = static_cast< float >( src[src_fmt.AID] );
        dst[src_fmt.AID] = alpha;
        for( uint8 j = 0; j < src_fmt.NCC; ++j ) {
            uint8 r = src_fmt.IDT[j];
            dst[r] = static_cast< float >( src[r] * alpha / max );
        }
    }

    src += src_fmt.SPP;
    dst += dst_fmt.SPP;

    for( int x = 1; x < cargs->srcRect.w; ++x ) {
        float alpha = static_cast< float >( src[src_fmt.AID] );
        dst[src_fmt.AID] = alpha + *( dst - src_fmt.SPP + src_fmt.AID );
        for( uint8 j = 0; j < src_fmt.NCC; ++j ) {
            uint8 r = src_fmt.IDT[j];
            dst[r] = static_cast< float >( src[r] * alpha / max + *( dst - src_fmt.SPP + r ) );
        }
        src += src_fmt.SPP;
        dst += dst_fmt.SPP;
    }
}

template< typename T >
void
InvokeBuildPremultSATYPassMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
    // Warning, explanation of dirty SAT trick:
    // we trick the scheduler into scheduling columns by swapping width and
    // height before scheduling the command so we have the right amount of
    // tasks, jobs, and workers assigned, but the jargs->dst assigned is
    // garbage, hence the PixelBits used to query the appropriate column at row
    // one. We then proceed to run along the column, until we reach
    // cargs->dstRect.h because the dimensions have been swapped back.
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    const uint32 stride = cargs->dst.Width() * fmt.SPP;
    float* dst = reinterpret_cast< float* >( cargs->dst.PixelBits( jargs->line, 1 ) );

    for( int y = 1; y < cargs->dstRect.h; ++y ) {
        for( uint8 j = 0; j < fmt.SPP; ++j ) {
            auto jojo = dst - stride + j;
            dst[j] = static_cast< float >( dst[j] + *( dst - stride + j ) );
        }
        dst += stride;
    }
}

/////////////////////////////////////////////////////
// Scheduling
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildSATXPassMT_MEM_Generic, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildSATXPassMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildSATYPassMT_MEM_Generic, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildSATYPassMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildPremultSATXPassMT_MEM_Generic, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildPremultSATXPassMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildPremultSATYPassMT_MEM_Generic, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildPremultSATYPassMT_MEM_Generic< T > )

ULIS_NAMESPACE_END

