// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnalyzeSmallestVisibleRect.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the AnalyzeSmallestVisibleRect entry point functions.
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
#include "Scheduling/SimpleBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations AnalyzeSmallestVisibleRect
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
template< typename T >
void
InvokeAnalyzeSmallestVisibleRectXPassMT_MEM(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->src.Format();
    const T* ULIS_RESTRICT src = reinterpret_cast< const T* >( jargs->src );
    uint16* ULIS_RESTRICT dst = reinterpret_cast< uint16* >( jargs->dst );

    // dst is a strip ( assumed CMYK16 )
    // left top right bot
    // { ULIS_UINT16_MAX, ULIS_UINT16_MAX, 0, 0 };
    dst[0] = ULIS_UINT16_MAX;
    dst[1] = ULIS_UINT16_MAX;
    dst[2] = 0;
    dst[3] = 0;
    bool found = false;
    for( int i = 0; i < cargs->srcRect.w; ++i ) {
        if( src[ fmt.AID ] != MinType< T >() ) {
            found = true;
            if( i < dst[0] ) dst[0] = i;
            if( i > dst[2] ) dst[2] = i;
        }
        src += fmt.SPP;
    }

    if( found ) {
        dst[1] = jargs->line;
        dst[3] = jargs->line;
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
template< typename T >
void
InvokeAnalyzeSmallestVisibleRectYPassMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.Format();
    uint16* src = reinterpret_cast< uint16* >( jargs->dst );
    FColor dst_holder( fmt.FMT );
    uint16* dst = reinterpret_cast< uint16* >( dst_holder.Bits() );

    // dst is a dot ( assumed CMYK16 )
    // left top right bot
    // { ULIS_UINT16_MAX, ULIS_UINT16_MAX, 0, 0 };
    dst[0] = ULIS_UINT16_MAX;
    dst[1] = ULIS_UINT16_MAX;
    dst[2] = 0;
    dst[3] = 0;

    for( int i = 0; i < cargs->dstRect.h; ++i ) {
        if( src[0] < dst[0] ) dst[0] = src[0];
        if( src[1] < dst[1] ) dst[1] = src[1];
        if( src[2] > dst[2] ) dst[2] = src[2];
        if( src[3] > dst[3] ) dst[3] = src[3];
        src += fmt.SPP;
    }
    memcpy( src - fmt.SPP, dst, fmt.BPP );
}
/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleAnalyzeSmallestVisibleRectXPassMT_MEM, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeAnalyzeSmallestVisibleRectXPassMT_MEM< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleAnalyzeSmallestVisibleRectYPassMT_MEM, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeAnalyzeSmallestVisibleRectYPassMT_MEM< T > )
ULIS_DECLARE_DISPATCHER( FDispatchedAnalyzeSmallestVisibleRectXPassInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedAnalyzeSmallestVisibleRectYPassInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedAnalyzeSmallestVisibleRectXPassInvocationSchedulerSelector
    , &ScheduleAnalyzeSmallestVisibleRectXPassMT_MEM< T >
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedAnalyzeSmallestVisibleRectYPassInvocationSchedulerSelector
    , &ScheduleAnalyzeSmallestVisibleRectYPassMT_MEM< T >
)

ULIS_NAMESPACE_END

