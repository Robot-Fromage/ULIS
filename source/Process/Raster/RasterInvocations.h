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
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawLineMT_MEM );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawLineAAMT_MEM );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawCircleAndresMT_MEM );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawCircleAndresAAMT_MEM );

ULIS_DECLARE_DISPATCHER( FDispatchedDrawLineInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawLineAAInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawCircleAndresInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedDrawCircleAndresAAInvocationSchedulerSelector )

ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawLineInvocationSchedulerSelector
    , &ScheduleDrawLineMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawLineAAInvocationSchedulerSelector
    ,&ScheduleDrawLineAAMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawCircleAndresInvocationSchedulerSelector
    ,&ScheduleDrawCircleAndresMT_MEM
)
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawCircleAndresAAInvocationSchedulerSelector
    ,&ScheduleDrawCircleAndresAAMT_MEM
)

ULIS_NAMESPACE_END

