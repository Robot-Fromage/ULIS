// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         VectorInvocations.cpp
* @author       Gary Gabriel
* @brief        This file provides the invocations for the Raster API.
* @license      Please refer to LICENSE.md
*/

#include "Process/Vector/VectorInvocations.h"
#include "Image/Block.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"

ULIS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeDrawVectorObjectMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDrawVectorObjectCommandArgs* cargs
)
{
    cargs->vecObj.Draw( cargs->dst, cargs->blCtx );
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawVectorObjectMT_MEM, FSimpleBufferJobArgs, FDrawVectorObjectCommandArgs, &InvokeDrawVectorObjectMT_MEM )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawVectorObjectInvocationSchedulerSelector )

ULIS_NAMESPACE_END

