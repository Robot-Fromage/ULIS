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
    ScheduleSimpleBufferJobs< &InvokeFillPreserveAlphaMT_MEM< T > >( iCommand, iPolicy, iContiguous );
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedFillPreserveAlphaInvocationSchedulerSelector )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedFillPreserveAlphaInvocationSchedulerSelector )

ULIS_NAMESPACE_END

