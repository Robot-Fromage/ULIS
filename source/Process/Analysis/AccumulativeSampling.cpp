// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AccumulativeSampling.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the AccumulativeSampling API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/Analysis/AccumulativeSampling.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations YPass
void
InvokeAccumulativeSamplingYPassMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    float* dst = reinterpret_cast< float* >( jargs->dst ) + fmt.SPP;

    for( uint32 y = 1; y < jargs->size; ++y ) {
        for( uint8 j = 0; j < fmt.SPP; ++j )
            dst[j] = static_cast< float >( dst[j] + *( dst - fmt.SPP + j ) );
        dst += fmt.SPP;
    }
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleAccumulativeSamplingYPassMT_MEM, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeAccumulativeSamplingYPassMT_MEM )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedAccumulativeSamplingXPassInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedAccumulativeSamplingYPassInvocationSchedulerSelector )

ULIS_NAMESPACE_END

