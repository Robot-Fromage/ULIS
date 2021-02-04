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
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    const T* ULIS_RESTRICT src = reinterpret_cast< const T* >( jargs->src );
    float*   ULIS_RESTRICT dst = reinterpret_cast< float* >( jargs->dst );

    for( uint8 j = 0; j < fmt.SPP; ++j )
        dst[j] = static_cast< float >( src[j] );

    src += fmt.SPP;
    dst += fmt.SPP;

    for( uint32 x = 1; x < jargs->size; ++x ) {
        for( uint8 j = 0; j < fmt.SPP; ++j )
            dst[j] = static_cast< float >( src[j] + *( dst - fmt.SPP + j ) );
        src += fmt.SPP;
        dst += fmt.SPP;
    }
}

template< typename T >
void
InvokeBuildSATYPassMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    const uint32 stride = cargs->dst.Width() * fmt.SPP;
    float* dst = reinterpret_cast< float* >( jargs->dst ) + stride;

    for( uint32 y = 1; y < jargs->size; ++y ) {
        for( uint8 j = 0; j < fmt.SPP; ++j )
            dst[j] = static_cast< float >( dst[j] + *( dst - stride + j ) );
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
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    const T* ULIS_RESTRICT src = reinterpret_cast< const T* >( jargs->src );
    float*   ULIS_RESTRICT dst = reinterpret_cast< float* >( jargs->dst );
    float max = static_cast< float >( MaxType< T >() );

    {
        float alpha = static_cast< float >( src[fmt.AID] );
        dst[fmt.AID] = alpha;
        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            dst[r] = static_cast< float >( src[r] * alpha / max );
        }
    }

    src += fmt.SPP;
    dst += fmt.SPP;

    for( uint32 x = 1; x < jargs->size; ++x ) {
        float alpha = static_cast< float >( src[fmt.AID] );
        dst[fmt.AID] = alpha + *( dst - fmt.SPP + fmt.AID );
        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            dst[r] = static_cast< float >( src[r] * alpha / max + *( dst - fmt.SPP + r ) );
        }
        src += fmt.SPP;
        dst += fmt.SPP;
    }
}

template< typename T >
void
InvokeBuildPremultSATYPassMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    const uint32 stride = cargs->dst.Width() * fmt.SPP;
    float* dst = reinterpret_cast< float* >( jargs->dst ) + stride;

    for( uint32 y = 1; y < jargs->size; ++y ) {
        for( uint8 j = 0; j < fmt.SPP; ++j )
            dst[j] = static_cast< float >( dst[j] + *( dst - stride + j ) );
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

