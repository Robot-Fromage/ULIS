// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Clear.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for the Clear API.
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
// FClearCommandArgs
class FClearCommandArgs final
    : public ICommandArgs
{
public:
    ~FClearCommandArgs() override
    {
    };

    FClearCommandArgs(
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
// FClearJobArgs
class FClearJobArgs final
    : public IJobArgs
{
public:
    ~FClearJobArgs() override {};
    FClearJobArgs(
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
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleClearMT_AX2 );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleClearMT_SSE4_2 );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleClearMT_MEM );

/////////////////////////////////////////////////////
// Dispatch
ULIS_DECLARE_DISPATCHER( FDispatchedClearInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP(
      FDispatchedClearInvocationSchedulerSelector
    , &ScheduleClearMT_AX2
    , &ScheduleClearMT_SSE4_2
    , &ScheduleClearMT_MEM
)

ULIS_NAMESPACE_END

