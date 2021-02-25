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
InvokeDrawCircleAndresMT_MEM(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawCircleCommandArgs* cargs
)
{
    DrawCircleAndres( cargs->dst, cargs->center, cargs->radius, cargs->color, cargs->filled, cargs->dstRect );
}

void
InvokeDrawCircleBresenhamMT_MEM(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawCircleCommandArgs* cargs
)
{
    DrawCircleBresenham( cargs->dst, cargs->center, cargs->radius, cargs->color, cargs->filled, cargs->dstRect );
}


void
InvokeDrawArcAndresMT_MEM(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawArcCommandArgs* cargs
)
{
    DrawArcAndres( cargs->dst, cargs->center, cargs->radius, cargs->startDegree, cargs->endDegree, cargs->color, cargs->dstRect );
}



void
InvokeDrawArcBresenhamMT_MEM(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawArcCommandArgs* cargs
)
{
    //DrawArcBresenham( cargs->dst, cargs->center, cargs->radius, cargs->startDegree, cargs->endDegree, cargs->color );
}


void
InvokeDrawEllipseMT_MEM(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawEllipseCommandArgs* cargs
)
{
    //DrawEllipse( cargs->dst, cargs->center, cargs->a, cargs->b, cargs->color, cargs->filled, cargs->)
}

void
InvokeDrawRotatedEllipseMT_MEM(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawRotatedEllipseCommandArgs* cargs
)
{
}


void
InvokeDrawRectangleMT_MEM(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawRectangleCommandArgs* cargs
)
{
}

void
InvokeDrawPolygonMT_MEM(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawPolygonCommandArgs* cargs
)
{
}

void
InvokeDrawQuadraticBezierMT_MEM(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawQuadraticBezierCommandArgs* cargs
)
{
}


/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawLineMT_MEM, FSimpleBufferJobArgs, FDrawLineCommandArgs, &InvokeDrawLineMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawCircleAndresMT_MEM, FSimpleBufferJobArgs, FDrawCircleCommandArgs, &InvokeDrawCircleAndresMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawCircleBresenhamMT_MEM, FSimpleBufferJobArgs, FDrawCircleCommandArgs, &InvokeDrawCircleBresenhamMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawArcAndresMT_MEM, FSimpleBufferJobArgs, FDrawArcCommandArgs, &InvokeDrawArcAndresMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawArcBresenhamMT_MEM, FSimpleBufferJobArgs, FDrawArcCommandArgs, &InvokeDrawArcBresenhamMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawEllipseMT_MEM, FSimpleBufferJobArgs, FDrawEllipseCommandArgs, &InvokeDrawEllipseMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawRotatedEllipseMT_MEM, FSimpleBufferJobArgs, FDrawRotatedEllipseCommandArgs, &InvokeDrawRotatedEllipseMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawRectangleMT_MEM, FSimpleBufferJobArgs, FDrawRectangleCommandArgs, &InvokeDrawRectangleMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawPolygonMT_MEM, FSimpleBufferJobArgs, FDrawPolygonCommandArgs, &InvokeDrawPolygonMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawQuadraticBezierMT_MEM, FSimpleBufferJobArgs, FDrawQuadraticBezierCommandArgs, &InvokeDrawQuadraticBezierMT_MEM )

ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawLineInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawLineAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawCircleAndresInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawCircleAndresAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawCircleBresenhamInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawCircleBresenhamAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawArcAndresInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawArcAndresAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawArcBresenhamInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawArcBresenhamAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawEllipseInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawEllipseAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawRotatedEllipseInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawRotatedEllipseAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawRectangleInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawPolygonInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawPolygonAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawQuadraticBezierInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawQuadraticBezierAAInvocationSchedulerSelector )


ULIS_NAMESPACE_END

