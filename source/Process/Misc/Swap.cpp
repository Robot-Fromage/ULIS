// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Swap.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Swap API.
* @license      Please refer to LICENSE.md
*/
#include "Process/Misc/Swap.h"
#include "Image/Block.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"
#include "Scheduling/SimpleBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeSwapMT_MEM( const FSimpleBufferJobArgs* jargs, const FSwapCommandArgs* cargs ) {
    uint8* dst = jargs->dst;
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    uint8* tmp = new uint8[ fmt.BPC ];
    const uint8 c1 = cargs->channel1 * fmt.BPC;
    const uint8 c2 = cargs->channel2 * fmt.BPC;
    for( uint32 i = 0; i < jargs->size; i+= fmt.BPP ) {
        memcpy( tmp, dst + c1, fmt.BPC );
        memcpy( dst + c1, dst + c2, fmt.BPC );
        memcpy( dst + c2, tmp, fmt.BPC );
        dst += fmt.BPP;
    }
    delete [] tmp;
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleSwapMT_MEM, FSimpleBufferJobArgs, FSwapCommandArgs, &InvokeSwapMT_MEM )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedSwapInvocationSchedulerSelector )

ULIS_NAMESPACE_END

