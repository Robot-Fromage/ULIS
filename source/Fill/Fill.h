// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Fill.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Fill entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Dispatch/Dispatcher.h"
#include "Image/Color.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FFillCommandArgs
class FFillCommandArgs final
    : public ICommandArgs
{
public:
    ~FFillCommandArgs() override
    {
        if( buffer )
            delete [] buffer;
    };

    FFillCommandArgs(
          FBlock& iBlock
        , FColor& iColor
        , const FRectI& iRect
        , const bool iWhole
    )
        : ICommandArgs()
        , block( iBlock )
        , color( iColor )
        , buffer( nullptr )
        , rect( iRect )
        , contiguous( iWhole )
        {}

    FBlock& block;
    FColor color;
    uint8* buffer;
    const FRectI rect;
    const bool contiguous;
};

/////////////////////////////////////////////////////
// Scheduler
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleFillMT_AX2 );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleFillMT_SSE4_2 );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleFillMT_MEM );

/////////////////////////////////////////////////////
// Dispatch
ULIS_DECLARE_DISPATCHER( FDispatchedFillInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP(
      FDispatchedFillInvocationSchedulerSelector
    , &ScheduleFillMT_AX2
    , &ScheduleFillMT_SSE4_2
    , &ScheduleFillMT_MEM
)

ULIS_NAMESPACE_END

