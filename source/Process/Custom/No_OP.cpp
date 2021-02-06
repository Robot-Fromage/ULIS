// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         No_OP.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementations for the No_OP API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/Custom/No_OP.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeNo_OP(
      const FNo_OPJobArgs* jargs
    , const FNo_OPCommandArgs* cargs
)
{
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
void
ScheduleNo_OP(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
    , bool iForceMonoChunk
)
{
    RangeBasedSchedulingBuildJobs<
          FNo_OPJobArgs
        , FNo_OPCommandArgs
        , &InvokeNo_OP
    >
    (
          iCommand
        , iPolicy
        , 1
        , 1
        , iContiguous
        , true
        , BuildNo_OPJob_Scanlines
        , BuildNo_OPJob_Chunks
    );
}

ULIS_NAMESPACE_END

