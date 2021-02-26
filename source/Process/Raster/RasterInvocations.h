// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterInvocations.h
* @author       Thomas Schmitt
* @brief        This file provides the declarations of the invocations for the Raster API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/

#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"
#include "Process/Raster/RasterAlgorithms.h"

ULIS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////
// FDrawLineCommandArgs
class FDrawLineCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawLineCommandArgs() override
    {
    }

    FDrawLineCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
        , const FVec2I& iP0
        , const FVec2I& iP1
        , const FColor& iColor
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , p0( iP0 )
        , p1( iP1 )
        , color ( iColor )
    {}

    FVec2I p0;
    FVec2I p1;
    FColor color;
};

/////////////////////////////////////////////////////
// FDrawCircleCommandArgs
class FDrawCircleCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawCircleCommandArgs() override
    {
    }

    FDrawCircleCommandArgs(
         FBlock& iBlock
        , const FRectI& iRect
        , const FVec2I& iCenter
        , const int     iRadius
        , const FColor& iColor
        , const bool    iFilled
    )
        : FSimpleBufferCommandArgs(iBlock,iRect)
        , center(iCenter)
        , radius(iRadius)
        , color (iColor)
        , filled(iFilled)
    {}

    FVec2I center;
    int    radius;
    FColor color;
    bool   filled;
};

/////////////////////////////////////////////////////
// FDrawArcCommandArgs
class FDrawArcCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawArcCommandArgs() override
    {
    }

    FDrawArcCommandArgs(
         FBlock&        iBlock
        , const FRectI& iRect
        , const FVec2I& iCenter
        , const int     iRadius
        , const int     iStartDegree
        , const int     iEndDegree
        , const FColor& iColor
    )
        : FSimpleBufferCommandArgs(iBlock,iRect)
        , center(iCenter)
        , radius(iRadius)
        , startDegree( iStartDegree )
        , endDegree( iEndDegree )
        , color (iColor)
    {}

    FVec2I center;
    int    radius;
    int    startDegree;
    int    endDegree;
    FColor color;
};


/////////////////////////////////////////////////////
// FDrawEllipseCommandArgs
class FDrawEllipseCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawEllipseCommandArgs() override
    {
    }

    FDrawEllipseCommandArgs(
         FBlock&        iBlock
        , const FRectI& iRect
        , const FVec2I& iCenter
        , const int     iA
        , const int     iB
        , const FColor& iColor
        , const bool    iFilled
    )
        : FSimpleBufferCommandArgs(iBlock,iRect)
        , center(iCenter)
        , a( iA )
        , b( iB )
        , color( iColor )
        , filled( iFilled )
    {}

    FVec2I center;
    int    a;
    int    b;
    FColor color;
    bool   filled;
};

/////////////////////////////////////////////////////
// FDrawRotatedEllipseCommandArgs
class FDrawRotatedEllipseCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawRotatedEllipseCommandArgs() override
    {
    }

    FDrawRotatedEllipseCommandArgs(
         FBlock&        iBlock
        , const FRectI& iRect
        , const FVec2I& iCenter
        , const int     iA
        , const int     iB
        , const int     iRotationDegrees
        , const FColor& iColor
        , const bool    iFilled
    )
        : FSimpleBufferCommandArgs(iBlock,iRect)
        , center(iCenter)
        , a( iA )
        , b( iB )
        , rotationDegrees( iRotationDegrees )
        , color( iColor )
        , filled( iFilled )
    {}

    FVec2I center;
    int    a;
    int    b;
    int    rotationDegrees;
    FColor color;
    bool   filled;
};

/////////////////////////////////////////////////////
// FDrawRectangleCommandArgs
class FDrawRectangleCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawRectangleCommandArgs() override
    {
    }

    FDrawRectangleCommandArgs(
         FBlock&        iBlock
        , const FRectI& iRect
        , const FVec2I& iTopLeft
        , const FVec2I& iBottomRight
        , const FColor& iColor
    )
        : FSimpleBufferCommandArgs(iBlock,iRect)
        , topLeft( iTopLeft )
        , bottomRight( iBottomRight )
        , color( iColor )
    {}

    FVec2I topLeft;
    FVec2I bottomRight;
    FColor color;
};

/////////////////////////////////////////////////////
// FDrawPolygonCommandArgs
class FDrawPolygonCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawPolygonCommandArgs() override
    {
    }

    FDrawPolygonCommandArgs(
         FBlock&        iBlock
        , const FRectI& iRect
        , const std::vector< FVec2I >& iPoints
        , const FColor& iColor
        , const bool    iFilled
    )
        : FSimpleBufferCommandArgs(iBlock,iRect)
        , points( iPoints )
        , color( iColor )
        , filled( iFilled )
    {}

    std::vector< FVec2I > points;
    FColor color;
    bool   filled;
};

/////////////////////////////////////////////////////
// FDrawQuadraticBezierCommandArgs
class FDrawQuadraticBezierCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawQuadraticBezierCommandArgs() override
    {
    }

    FDrawQuadraticBezierCommandArgs(
          FBlock&       iBlock
        , const FRectI& iRect
        , const FVec2I& iCtrlPt0
        , const FVec2I& iCtrlPt1
        , const FVec2I& iCtrlPt2
        , const float   iWeight
        , const FColor& iColor
    )
        : FSimpleBufferCommandArgs(iBlock,iRect)
        , ctrlPt0( iCtrlPt0 )
        , ctrlPt1( iCtrlPt1 )
        , ctrlPt2( iCtrlPt2 )
        , weight(  iWeight )
        , color (  iColor )
    {}

    FVec2I ctrlPt0;
    FVec2I ctrlPt1;
    FVec2I ctrlPt2;
    float weight;
    FColor color;
};

/////////////////////////////////////////////////////
// Template Invocations

template<typename T>
void
InvokeDrawLineAAMT_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FDrawLineCommandArgs* cargs
)
{
    DrawLineAA<T>(cargs->dst,cargs->p0,cargs->p1,cargs->color,cargs->dstRect);
}

template<typename T>
void
InvokeDrawCircleAndresAAMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawCircleCommandArgs* cargs
)
{
    DrawCircleAndresAA<T>(cargs->dst,cargs->center,cargs->radius,cargs->color,cargs->filled,cargs->dstRect);
}

template<typename T>
void
InvokeDrawCircleBresenhamAAMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawCircleCommandArgs* cargs
)
{
    DrawCircleBresenhamAA<T>( cargs->dst, cargs->center, cargs->radius, cargs->color, cargs->filled, cargs->dstRect );
}

template<typename T>
void
InvokeDrawArcAndresAAMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawArcCommandArgs* cargs
)
{
    DrawArcAndresAA<T>( cargs->dst, cargs->center, cargs->radius, cargs->startDegree, cargs->endDegree, cargs->color, cargs->dstRect );
}

template<typename T>
void
InvokeDrawArcBresenhamAAMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawArcCommandArgs* cargs
)
{
    DrawArcBresenhamAA<T>( cargs->dst, cargs->center, cargs->radius, cargs->startDegree, cargs->endDegree, cargs->color, cargs->dstRect );
}

template<typename T>
void
InvokeDrawEllipseAAMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawEllipseCommandArgs* cargs
)
{
    DrawEllipseAA<T>( cargs->dst, cargs->center, cargs->a, cargs->b, cargs->color, cargs->filled, cargs->dstRect );
}

template<typename T>
void
InvokeDrawRotatedEllipseAAMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawRotatedEllipseCommandArgs* cargs
)
{
    DrawRotatedEllipseAA<T>( cargs->dst, cargs->center, cargs->a, cargs->b, cargs->rotationDegrees, cargs->color, cargs->filled, cargs->dstRect );
}

template<typename T>
void
InvokeDrawPolygonAAMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawPolygonCommandArgs* cargs
)
{
    DrawPolygonAA<T>( cargs->dst, cargs->points, cargs->color, cargs->filled, cargs->dstRect );
}

template<typename T>
void
InvokeDrawQuadraticBezierAAMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawQuadraticBezierCommandArgs* cargs
)
{
    DrawQuadraticBezierAA<T>( cargs->dst, cargs->ctrlPt0, cargs->ctrlPt1, cargs->ctrlPt2, cargs->weight, cargs->color, cargs->dstRect );
}


/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawLineMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawLineAAMT_MEM_Generic, FSimpleBufferJobArgs, FDrawLineCommandArgs, &InvokeDrawLineAAMT_MEM_Generic<T> )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawCircleAndresMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE(ScheduleDrawCircleAndresAAMT_MEM_Generic,FSimpleBufferJobArgs, FDrawCircleCommandArgs, &InvokeDrawCircleAndresAAMT_MEM_Generic<T>)
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawCircleBresenhamMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawCircleBresenhamAAMT_MEM_Generic, FSimpleBufferJobArgs, FDrawCircleCommandArgs, &InvokeDrawCircleBresenhamAAMT_MEM_Generic<T> )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawArcAndresMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawArcAndresAAMT_MEM_Generic, FSimpleBufferJobArgs, FDrawArcCommandArgs, &InvokeDrawArcAndresAAMT_MEM_Generic<T> )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawArcBresenhamMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawArcBresenhamAAMT_MEM_Generic, FSimpleBufferJobArgs, FDrawArcCommandArgs, &InvokeDrawArcBresenhamAAMT_MEM_Generic<T> )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawEllipseMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE(ScheduleDrawEllipseAAMT_MEM_Generic,FSimpleBufferJobArgs,FDrawEllipseCommandArgs,&InvokeDrawEllipseAAMT_MEM_Generic<T>)
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawRotatedEllipseMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE(ScheduleDrawRotatedEllipseAAMT_MEM_Generic,FSimpleBufferJobArgs,FDrawRotatedEllipseCommandArgs,&InvokeDrawRotatedEllipseAAMT_MEM_Generic<T>)
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawRectangleMT_MEM );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawPolygonMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE(ScheduleDrawPolygonAAMT_MEM_Generic,FSimpleBufferJobArgs,FDrawPolygonCommandArgs,&InvokeDrawPolygonAAMT_MEM_Generic<T>)
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawQuadraticBezierMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE(ScheduleDrawQuadraticBezierAAMT_MEM_Generic,FSimpleBufferJobArgs,FDrawQuadraticBezierCommandArgs,&InvokeDrawQuadraticBezierAAMT_MEM_Generic<T>)


ULIS_DECLARE_DISPATCHER( FDispatchedDrawLineInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawLineAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawCircleAndresInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawCircleAndresAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawCircleBresenhamInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawCircleBresenhamAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawArcAndresInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawArcAndresAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawArcBresenhamInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawArcBresenhamAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawEllipseInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawEllipseAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawRotatedEllipseInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawRotatedEllipseAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawRectangleInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawPolygonInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawPolygonAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawQuadraticBezierInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawQuadraticBezierAAInvocationSchedulerSelector )

ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawLineInvocationSchedulerSelector
    , &ScheduleDrawLineMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawLineAAInvocationSchedulerSelector
    ,&ScheduleDrawLineAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawCircleAndresInvocationSchedulerSelector
    ,&ScheduleDrawCircleAndresMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawCircleAndresAAInvocationSchedulerSelector
    ,&ScheduleDrawCircleAndresAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawCircleBresenhamInvocationSchedulerSelector
    ,&ScheduleDrawCircleBresenhamMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawCircleBresenhamAAInvocationSchedulerSelector
    ,&ScheduleDrawCircleBresenhamAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawArcAndresInvocationSchedulerSelector
    ,&ScheduleDrawArcAndresMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawArcAndresAAInvocationSchedulerSelector
    ,&ScheduleDrawArcAndresAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawArcBresenhamInvocationSchedulerSelector
    ,&ScheduleDrawArcBresenhamMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawArcBresenhamAAInvocationSchedulerSelector
    ,&ScheduleDrawArcBresenhamAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawEllipseInvocationSchedulerSelector
    ,&ScheduleDrawEllipseMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawEllipseAAInvocationSchedulerSelector
    ,&ScheduleDrawEllipseAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawRotatedEllipseInvocationSchedulerSelector
    ,&ScheduleDrawRotatedEllipseMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawRotatedEllipseAAInvocationSchedulerSelector
    ,&ScheduleDrawRotatedEllipseAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawRectangleInvocationSchedulerSelector
    ,&ScheduleDrawRectangleMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawPolygonInvocationSchedulerSelector
    ,&ScheduleDrawPolygonMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawPolygonAAInvocationSchedulerSelector
    ,&ScheduleDrawPolygonAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawQuadraticBezierInvocationSchedulerSelector
    ,&ScheduleDrawQuadraticBezierMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawQuadraticBezierAAInvocationSchedulerSelector
    ,&ScheduleDrawQuadraticBezierAAMT_MEM_Generic<T>
)


ULIS_NAMESPACE_END

