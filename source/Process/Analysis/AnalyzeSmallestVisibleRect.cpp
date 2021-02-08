// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnalyzeSmallestVisibleRect.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the AnalyzeSmallestVisibleRect API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/Analysis/AnalyzeSmallestVisibleRect.h"
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
void
InvokeAnalyzeSmallestVisibleRectXPassMT_MEM(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->src.Format();
    const uint8* ULIS_RESTRICT src = jargs->src;
    uint16* ULIS_RESTRICT dst = reinterpret_cast< uint16* >( jargs->dst );

    // dst is a strip ( assumed CMYK16 )
    // left top right bot
    // { ULIS_UINT16_MAX, ULIS_UINT16_MAX, 0, 0 };
    dst[0] = ULIS_UINT16_MAX;
    dst[1] = ULIS_UINT16_MAX;
    dst[2] = 0;
    dst[3] = 0;
    bool found = false;
    uint32 cmp = 0;
    for( uint32 i = 0; i < jargs->size; ++i ) {
        if( memcmp( ( src + fmt.AID ), &cmp, fmt.BPC ) ) {
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
void
InvokeAnalyzeSmallestVisibleRectYPassMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.Format();
    const uint16* ULIS_RESTRICT src = reinterpret_cast< const uint16* >( jargs->dst );
    FColor dst_holder( fmt.FMT );
    uint16* ULIS_RESTRICT dst = reinterpret_cast< uint16* >( dst_holder.Bits() );

    // dst is a dot ( assumed CMYK16 )
    // left top right bot
    // { ULIS_UINT16_MAX, ULIS_UINT16_MAX, 0, 0 };
    dst[0] = ULIS_UINT16_MAX;
    dst[1] = ULIS_UINT16_MAX;
    dst[2] = 0;
    dst[3] = 0;

    for( uint32 i = 0; i < jargs->size; ++i ) {
        if( src[0] < dst[0] ) dst[0] = src[0];
        if( src[1] < dst[1] ) dst[1] = src[1];
        if( src[2] > dst[2] ) dst[2] = src[2];
        if( src[3] > dst[3] ) dst[3] = src[3];
        src += fmt.SPP;
    }
    memcpy( dst, src, fmt.BPP );
}
/////////////////////////////////////////////////////
// Dispatch
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleAnalyzeSmallestVisibleRectXPassMT_MEM, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeAnalyzeSmallestVisibleRectXPassMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleAnalyzeSmallestVisibleRectYPassMT_MEM, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeAnalyzeSmallestVisibleRectYPassMT_MEM )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedAnalyzeSmallestVisibleRectXPassInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedAnalyzeSmallestVisibleRectYPassInvocationSchedulerSelector )

ULIS_NAMESPACE_END

