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
InvokeDrawCircleMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDrawCircleCommandArgs* cargs
)
{
    DrawCircle( cargs->dst, cargs->center, cargs->radius, cargs->color, cargs->filled, cargs->dstRect );
}

void
InvokeDrawArcMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDrawArcCommandArgs* cargs
)
{
    DrawArc( cargs->dst, cargs->center, cargs->radius, cargs->startDegree, cargs->endDegree, cargs->color, cargs->dstRect );
}

void
InvokeDrawEllipseMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDrawEllipseCommandArgs* cargs
)
{
    DrawEllipse( cargs->dst, cargs->center, cargs->a, cargs->b, cargs->color, cargs->filled, cargs->dstRect );
}

void
InvokeDrawRotatedEllipseMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDrawRotatedEllipseCommandArgs* cargs
)
{
    DrawRotatedEllipse( cargs->dst, cargs->center, cargs->a, cargs->b, cargs->rotationDegrees, cargs->color, cargs->filled, cargs->dstRect );
}

void
InvokeDrawRectangleMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDrawRectangleCommandArgs* cargs
)
{
    DrawRectangle( cargs->dst, cargs->topLeft, cargs->bottomRight, cargs->color, cargs->dstRect );
}

void
InvokeDrawPolygonMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDrawPolygonCommandArgs* cargs
)
{
    DrawPolygon( cargs->dst, cargs->points, cargs->color, cargs->filled, cargs->dstRect );
}

void
InvokeDrawQuadraticBezierMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDrawQuadraticBezierCommandArgs* cargs
)
{
    DrawQuadraticBezier( cargs->dst, cargs->ctrlPt0, cargs->ctrlPt1, cargs->ctrlPt2, cargs->weight, cargs->color, cargs->dstRect );
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawLineMT_MEM, FSimpleBufferJobArgs, FDrawLineCommandArgs, &InvokeDrawLineMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawCircleMT_MEM, FSimpleBufferJobArgs, FDrawCircleCommandArgs, &InvokeDrawCircleMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawArcMT_MEM, FSimpleBufferJobArgs, FDrawArcCommandArgs, &InvokeDrawArcMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawEllipseMT_MEM, FSimpleBufferJobArgs, FDrawEllipseCommandArgs, &InvokeDrawEllipseMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawRotatedEllipseMT_MEM, FSimpleBufferJobArgs, FDrawRotatedEllipseCommandArgs, &InvokeDrawRotatedEllipseMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawRectangleMT_MEM, FSimpleBufferJobArgs, FDrawRectangleCommandArgs, &InvokeDrawRectangleMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawPolygonMT_MEM, FSimpleBufferJobArgs, FDrawPolygonCommandArgs, &InvokeDrawPolygonMT_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawQuadraticBezierMT_MEM, FSimpleBufferJobArgs, FDrawQuadraticBezierCommandArgs, &InvokeDrawQuadraticBezierMT_MEM )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawLineInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawLineAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawLineSPInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawCircleInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawCircleAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawCircleSPInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawArcInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawArcAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawArcSPInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawEllipseInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawEllipseAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawEllipseSPInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawRotatedEllipseInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawRotatedEllipseAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawRotatedEllipseSPInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawRectangleInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawPolygonInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawPolygonAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawPolygonSPInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawQuadraticBezierInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawQuadraticBezierAAInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawQuadraticBezierSPInvocationSchedulerSelector )

ULIS_NAMESPACE_END

