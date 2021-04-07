// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AccumulativeSampling.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the AccumulativeSampling entry point functions.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Image/Color.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations XPass
template< typename T >
void
InvokeAccumulativeSamplingXPassMT_MEM(
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

    const uint32 len = cargs->srcRect.w;
    for( uint32 x = 1; x < len; ++x ) {
        float alpha = static_cast< float >( src[fmt.AID] );
        dst[fmt.AID] += alpha;
        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            dst[r] += static_cast< float >( src[r] * alpha / max );
        }
        src += fmt.SPP;
    }
}

/////////////////////////////////////////////////////
// Invocations YPass
template< typename T >
void
InvokeAccumulativeSamplingYPassMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    float* dst = reinterpret_cast< float* >( jargs->dst ) + fmt.SPP;

    const uint32 len = static_cast< uint32 >( jargs->size / fmt.BPP );
    for( uint32 y = 1; y < len; ++y ) {
        for( uint8 j = 0; j < fmt.SPP; ++j )
            dst[j] = static_cast< float >( dst[j] + *( dst - fmt.SPP + j ) );
        dst += fmt.SPP;
    }

    dst -= fmt.SPP;
    for( uint8 j = 0; j < fmt.SPP; ++j )
        dst[j] /= static_cast< float >( MaxType< T >() );
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleAccumulativeSamplingXPassMT_MEM, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeAccumulativeSamplingXPassMT_MEM< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleAccumulativeSamplingYPassMT_MEM, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeAccumulativeSamplingYPassMT_MEM< T > )
ULIS_DECLARE_DISPATCHER( FDispatchedAccumulativeSamplingXPassInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedAccumulativeSamplingYPassInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedAccumulativeSamplingXPassInvocationSchedulerSelector
    , &ScheduleAccumulativeSamplingXPassMT_MEM< T >
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedAccumulativeSamplingYPassInvocationSchedulerSelector
    , &ScheduleAccumulativeSamplingYPassMT_MEM< T >
)

ULIS_NAMESPACE_END

