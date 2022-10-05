// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Generic_OP.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Generic_OP API.
* @license      Please refer to LICENSE.md
*/
#include "Process/Custom/Generic_OP.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeGeneric_OP(
      const FGeneric_OPJobArgs* jargs
    , const FGeneric_OPCommandArgs* cargs
)
{
    cargs->invocation();
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
void
ScheduleGeneric_OP(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
    , bool iForceMonoChunk
)
{
    RangeBasedSchedulingBuildJobs<
          FGeneric_OPJobArgs
        , FGeneric_OPCommandArgs
        , &InvokeGeneric_OP
    >
    (
          iCommand
        , iPolicy
        , 1
        , 1
        , 1
        , iContiguous
        , true
        , BuildGeneric_OPJob_Scanlines
        , BuildGeneric_OPJob_Chunks
    );
}

ULIS_NAMESPACE_END

