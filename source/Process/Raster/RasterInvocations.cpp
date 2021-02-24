// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterInvocations.cpp
* @author       Thomas Schmitt
* @brief        This file provides the invocations for the Raster API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/

#include "Process/Raster/RasterInvocations.h"
#include "Process/Raster/RasterAlgorithms.h"

#include "Image/Block.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"

ULIS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeDrawLineMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDrawLineCommandArgs* cargs
)
{
    DrawLine( cargs->dst, cargs->p0, cargs->p1, cargs->color, cargs->dstRect );
}

void
InvokeDrawLineAAMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDrawLineCommandArgs* cargs
)
{
    DrawLineAA( cargs->dst, cargs->p0, cargs->p1, cargs->color, cargs->dstRect );
}

void
InvokeDrawCircleAndresMT_MEM(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawCircleCommandArgs* cargs
)
{
    DrawCircleAndres( cargs->dst, cargs->center, cargs->radius, cargs->color, cargs->filled, cargs->dstRect );
}

void
InvokeDrawCircleAndresAAMT_MEM(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawCircleCommandArgs* cargs
)
{
    DrawCircleAndresAA( cargs->dst, cargs->center, cargs->radius, cargs->color, cargs->filled, cargs->dstRect );
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawLineMT_MEM, FSimpleBufferJobArgs, FDrawLineCommandArgs, &InvokeDrawLineMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawLineAAMT_MEM, FSimpleBufferJobArgs, FDrawLineCommandArgs, &InvokeDrawLineAAMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawCircleAndresMT_MEM, FSimpleBufferJobArgs, FDrawCircleCommandArgs, &InvokeDrawCircleAndresMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawCircleAndresAAMT_MEM, FSimpleBufferJobArgs, FDrawCircleCommandArgs, &InvokeDrawCircleAndresAAMT_MEM )

ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawLineInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawLineAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawCircleAndresInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawCircleAndresAAInvocationSchedulerSelector )

ULIS_NAMESPACE_END

