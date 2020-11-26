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
    };

    FFillCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
        , const bool iWhole
    )
        : ICommandArgs()
        , block( iBlock )
        , rect( iRect )
        , contiguous( iWhole )
        {}

    FBlock& block;
    const FRectI rect;
    const bool contiguous;
};

/////////////////////////////////////////////////////
// FFillJobArgs
class FFillJobArgs final
    : public IJobArgs
{
public:
    ~FFillJobArgs() override {};
    FFillJobArgs(
          uint8* const iDst
        , const int64 iSize
    )
        : IJobArgs()
        , dst( iDst )
        , size( iSize )
    {}

    uint8* const ULIS_RESTRICT dst;
    const int64 size;
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

