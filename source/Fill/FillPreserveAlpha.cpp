// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         FillPreserveAlpha.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the FillPreserveAlpha API.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Fill/FillPreserveAlpha.h"
#include "Image/Block.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Job Building
template< void (*TDelegateInvoke)( const FSimpleBufferJobArgs*, const FFillPreserveAlphaCommandArgs* ) >
void
ScheduleFillPreserveAlphaJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    const FFillPreserveAlphaCommandArgs* cargs  = dynamic_cast< const FFillPreserveAlphaCommandArgs* >( iCommand->Args() );
    RangeBasedSchedulingBuildJobs<
          FSimpleBufferJobArgs
        , FFillCommandArgs
        , TDelegateInvoke
        , BuildFillJob_Scanlines
        , BuildFillJob_Chunks
    >
    (
          iCommand
        , iPolicy
        , static_cast< int64 >( cargs->block.BytesTotal() )
        , cargs->rect.h
        , iContiguous
    );
}

/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
template< typename T >
void
InvokeFillPreserveAlphaMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FFillPreserveAlphaCommandArgs* cargs
)
{
    const uint8* src = cargs->color.Bits();
    const FFormatMetrics& fmt = cargs->block.Format();
    const uint8 stride = cargs->block.BytesPerPixel();
    uint8* ULIS_RESTRICT dst = jargs->dst;
    for( uint32 i = 0; i < jargs->size; ++i ) {
        const T alpha = dst[ iFmt.AID ];
        memcpy( dst, src, stride );
        dst[ fmt.AID ] = alpha;
        dst += stride;
    }
}

/////////////////////////////////////////////////////
// Schedulers
template< typename T >
void
ScheduleFillPreserveAlphaMT_MEM(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    const FFillPreserveAlphaCommandArgs* cargs = dynamic_cast< const FFillPreserveAlphaCommandArgs* >( iCommand->Args() );
    const uint8 bpp = cargs->block.BytesPerPixel();
    const uint32 bps = cargs->block.BytesPerScanLine();
    ScheduleFillPreserveAlphaJobs< &InvokeFillPreserveAlphaMT_MEM< T > >( iCommand, iPolicy, iContiguous );
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedFillPreserveAlphaInvocationSchedulerSelector )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedFillPreserveAlphaInvocationSchedulerSelector )

ULIS_NAMESPACE_END

