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

#include "Process/Raster/RasterArc.h"
#include "Process/Raster/RasterCircle.h"
#include "Process/Raster/RasterEllipse.h"
#include "Process/Raster/RasterLine.h"
#include "Process/Raster/RasterPolygon.h"
#include "Process/Raster/RasterQuadraticBezier.h"
#include "Process/Raster/RasterRectangle.h"
#include "Process/Raster/RasterRotatedEllipse.h"

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

class FDrawLineSPCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawLineSPCommandArgs() override
    {
    }

    FDrawLineSPCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
        , const FVec2F& iP0
        , const FVec2F& iP1
        , const FColor& iColor
    )
        : FSimpleBufferCommandArgs(iBlock,iRect)
        , p0(iP0)
        , p1(iP1)
        , color (iColor)
    {}

    FVec2F p0;
    FVec2F p1;
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

class FDrawCircleSPCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawCircleSPCommandArgs() override
    {
    }

    FDrawCircleSPCommandArgs(
        FBlock& iBlock
        , const FRectI& iRect
        , const FVec2F& iCenter
        , const float   iRadius
        , const FColor& iColor
        , const bool    iFilled
    )
        : FSimpleBufferCommandArgs(iBlock, iRect)
        , center(iCenter)
        , radius(iRadius)
        , color(iColor)
        , filled(iFilled)
    {}

    FVec2F center;
    float  radius;
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

class FDrawArcSPCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawArcSPCommandArgs() override
    {
    }

    FDrawArcSPCommandArgs(
         FBlock&        iBlock
        , const FRectI& iRect
        , const FVec2F& iCenter
        , const float   iRadius
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

    FVec2F center;
    float  radius;
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

class FDrawEllipseSPCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawEllipseSPCommandArgs() override
    {
    }

    FDrawEllipseSPCommandArgs(
         FBlock&        iBlock
        , const FRectI& iRect
        , const FVec2F& iCenter
        , const float   iA
        , const float   iB
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

    FVec2F center;
    float  a;
    float  b;
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

class FDrawRotatedEllipseSPCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawRotatedEllipseSPCommandArgs() override
    {
    }

    FDrawRotatedEllipseSPCommandArgs(
        FBlock&        iBlock
        , const FRectI& iRect
        , const FVec2F& iCenter
        , const float   iA
        , const float   iB
        , const int     iRotationDegrees
        , const FColor& iColor
        , const bool    iFilled
    )
        : FSimpleBufferCommandArgs(iBlock, iRect)
        , center(iCenter)
        , a(iA)
        , b(iB)
        , rotationDegrees(iRotationDegrees)
        , color(iColor)
        , filled(iFilled)
    {}

    FVec2F center;
    float  a;
    float  b;
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

class FDrawPolygonSPCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawPolygonSPCommandArgs() override
    {
    }

    FDrawPolygonSPCommandArgs(
         FBlock&        iBlock
        , const FRectI& iRect
        , const std::vector< FVec2F >& iPoints
        , const FColor& iColor
        , const bool    iFilled
    )
        : FSimpleBufferCommandArgs(iBlock,iRect)
        , points( iPoints )
        , color( iColor )
        , filled( iFilled )
    {}

    std::vector< FVec2F > points;
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

class FDrawQuadraticBezierSPCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawQuadraticBezierSPCommandArgs() override
    {
    }

    FDrawQuadraticBezierSPCommandArgs(
          FBlock&       iBlock
        , const FRectI& iRect
        , const FVec2F& iCtrlPt0
        , const FVec2F& iCtrlPt1
        , const FVec2F& iCtrlPt2
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

    FVec2F ctrlPt0;
    FVec2F ctrlPt1;
    FVec2F ctrlPt2;
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
InvokeDrawLineSPMT_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FDrawLineSPCommandArgs* cargs
)
{
    DrawLineSP<T>(cargs->dst,cargs->p0,cargs->p1,cargs->color,cargs->dstRect);
}

template<typename T>
void
InvokeDrawCircleAAMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawCircleCommandArgs* cargs
)
{
    DrawCircleAA<T>(cargs->dst,cargs->center,cargs->radius,cargs->color,cargs->filled,cargs->dstRect);
}

template<typename T>
void
InvokeDrawCircleSPMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawCircleSPCommandArgs* cargs
)
{
    DrawCircleSP<T>(cargs->dst,cargs->center,cargs->radius,cargs->color,cargs->filled,cargs->dstRect);
}

template<typename T>
void
InvokeDrawArcAAMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawArcCommandArgs* cargs
)
{
    DrawArcAA<T>( cargs->dst, cargs->center, cargs->radius, cargs->startDegree, cargs->endDegree, cargs->color, cargs->dstRect );
}

template<typename T>
void
InvokeDrawArcSPMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawArcSPCommandArgs* cargs
)
{
    DrawArcSP<T>( cargs->dst, cargs->center, cargs->radius, cargs->startDegree, cargs->endDegree, cargs->color, cargs->dstRect );
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
InvokeDrawEllipseSPMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawEllipseSPCommandArgs* cargs
)
{
    DrawEllipseSP<T>( cargs->dst, cargs->center, cargs->a, cargs->b, cargs->color, cargs->filled, cargs->dstRect );
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
InvokeDrawRotatedEllipseSPMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawRotatedEllipseSPCommandArgs* cargs
)
{
    DrawRotatedEllipseSP<T>( cargs->dst, cargs->center, cargs->a, cargs->b, cargs->rotationDegrees, cargs->color, cargs->filled, cargs->dstRect );
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
InvokeDrawPolygonSPMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawPolygonSPCommandArgs* cargs
)
{
    DrawPolygonSP<T>( cargs->dst, cargs->points, cargs->color, cargs->filled, cargs->dstRect );
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

template<typename T>
void
InvokeDrawQuadraticBezierSPMT_MEM_Generic(
      const FSimpleBufferJobArgs*   jargs
    , const FDrawQuadraticBezierSPCommandArgs* cargs
)
{
    DrawQuadraticBezierSP<T>( cargs->dst, cargs->ctrlPt0, cargs->ctrlPt1, cargs->ctrlPt2, cargs->weight, cargs->color, cargs->dstRect );
}


/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawLineMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawLineAAMT_MEM_Generic, FSimpleBufferJobArgs, FDrawLineCommandArgs, &InvokeDrawLineAAMT_MEM_Generic<T> )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawLineSPMT_MEM_Generic, FSimpleBufferJobArgs, FDrawLineSPCommandArgs, &InvokeDrawLineSPMT_MEM_Generic<T> )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawCircleMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE(ScheduleDrawCircleAAMT_MEM_Generic, FSimpleBufferJobArgs, FDrawCircleCommandArgs, &InvokeDrawCircleAAMT_MEM_Generic<T> )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE(ScheduleDrawCircleSPMT_MEM_Generic, FSimpleBufferJobArgs, FDrawCircleSPCommandArgs, &InvokeDrawCircleSPMT_MEM_Generic<T> )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawArcMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawArcAAMT_MEM_Generic, FSimpleBufferJobArgs, FDrawArcCommandArgs, &InvokeDrawArcAAMT_MEM_Generic<T> )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawArcSPMT_MEM_Generic, FSimpleBufferJobArgs, FDrawArcSPCommandArgs, &InvokeDrawArcSPMT_MEM_Generic<T> )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawEllipseMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawEllipseAAMT_MEM_Generic, FSimpleBufferJobArgs, FDrawEllipseCommandArgs, &InvokeDrawEllipseAAMT_MEM_Generic<T> )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawEllipseSPMT_MEM_Generic, FSimpleBufferJobArgs, FDrawEllipseSPCommandArgs, &InvokeDrawEllipseSPMT_MEM_Generic<T> )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawRotatedEllipseMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawRotatedEllipseAAMT_MEM_Generic, FSimpleBufferJobArgs, FDrawRotatedEllipseCommandArgs, &InvokeDrawRotatedEllipseAAMT_MEM_Generic<T> )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawRotatedEllipseSPMT_MEM_Generic, FSimpleBufferJobArgs, FDrawRotatedEllipseSPCommandArgs, &InvokeDrawRotatedEllipseSPMT_MEM_Generic<T> )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawRectangleMT_MEM );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawPolygonMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawPolygonAAMT_MEM_Generic, FSimpleBufferJobArgs, FDrawPolygonCommandArgs, &InvokeDrawPolygonAAMT_MEM_Generic<T> )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawPolygonSPMT_MEM_Generic, FSimpleBufferJobArgs, FDrawPolygonSPCommandArgs, &InvokeDrawPolygonSPMT_MEM_Generic<T> )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawQuadraticBezierMT_MEM );
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawQuadraticBezierAAMT_MEM_Generic, FSimpleBufferJobArgs, FDrawQuadraticBezierCommandArgs, &InvokeDrawQuadraticBezierAAMT_MEM_Generic<T> )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawQuadraticBezierSPMT_MEM_Generic, FSimpleBufferJobArgs, FDrawQuadraticBezierSPCommandArgs, &InvokeDrawQuadraticBezierSPMT_MEM_Generic<T> )

ULIS_DECLARE_DISPATCHER( FDispatchedDrawLineInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawLineAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawLineSPInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawCircleInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawCircleAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawCircleSPInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawArcInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawArcAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawArcSPInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawEllipseInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawEllipseAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawEllipseSPInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawRotatedEllipseInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawRotatedEllipseAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawRotatedEllipseSPInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawRectangleInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawPolygonInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawPolygonAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawPolygonSPInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawQuadraticBezierInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawQuadraticBezierAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawQuadraticBezierSPInvocationSchedulerSelector )

ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawLineInvocationSchedulerSelector
    , &ScheduleDrawLineMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawLineAAInvocationSchedulerSelector
    ,&ScheduleDrawLineAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawLineSPInvocationSchedulerSelector
    ,&ScheduleDrawLineSPMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawCircleInvocationSchedulerSelector
    ,&ScheduleDrawCircleMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawCircleAAInvocationSchedulerSelector
    ,&ScheduleDrawCircleAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawCircleSPInvocationSchedulerSelector
    ,&ScheduleDrawCircleSPMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawArcInvocationSchedulerSelector
    ,&ScheduleDrawArcMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawArcAAInvocationSchedulerSelector
    ,&ScheduleDrawArcAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawArcSPInvocationSchedulerSelector
    ,&ScheduleDrawArcSPMT_MEM_Generic<T>
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
      FDispatchedDrawEllipseSPInvocationSchedulerSelector
    ,&ScheduleDrawEllipseSPMT_MEM_Generic<T>
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
      FDispatchedDrawRotatedEllipseSPInvocationSchedulerSelector
    ,&ScheduleDrawRotatedEllipseSPMT_MEM_Generic<T>
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
      FDispatchedDrawPolygonSPInvocationSchedulerSelector
    ,&ScheduleDrawPolygonSPMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawQuadraticBezierInvocationSchedulerSelector
    ,&ScheduleDrawQuadraticBezierMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawQuadraticBezierAAInvocationSchedulerSelector
    ,&ScheduleDrawQuadraticBezierAAMT_MEM_Generic<T>
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawQuadraticBezierSPInvocationSchedulerSelector
    ,&ScheduleDrawQuadraticBezierSPMT_MEM_Generic<T>
)

ULIS_NAMESPACE_END

