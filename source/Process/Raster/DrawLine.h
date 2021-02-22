// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         DrawLine.h
* @author       Thomas Schmitt
* @brief        This file provides the declarations for the Raster DrawLine API.
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
        , const FVec2F& iP0
        , const FVec2F& iP1
        , const FColor& iColor
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , p0( iP0 )
        , p1( iP1 )
        , color ( iColor )
    {}

    FVec2F p0;
    FVec2F p1;
    FColor color;
};

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawLineMT_MEM );
ULIS_DECLARE_DISPATCHER( FDispatchedDrawLineInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedDrawLineInvocationSchedulerSelector
    , &ScheduleDrawLineMT_MEM
)

ULIS_NAMESPACE_END

