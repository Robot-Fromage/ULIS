// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Copy.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Copy entry point functions.
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
// FCopyCommandArgs
class FCopyCommandArgs final
    : public ICommandArgs
{
public:
    ~FCopyCommandArgs() override
    {
    };

    FCopyCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , const bool iContiguous
    )
        : ICommandArgs()
        , src( iSrc )
        , dst( iDst )
        , srcRect( iSrcRect )
        , dstRect( iDstRect )
        , contiguous( iContiguous )
        {}

    const FBlock& src;
    FBlock& dst;
    const FRectI srcRect;
    const FRectI dstRect;
    const bool contiguous;
};

/////////////////////////////////////////////////////
// FCopyJobArgs
class FCopyJobArgs final
    : public IJobArgs
{
public:

    ~FCopyJobArgs() override {};
    FCopyJobArgs(
          const uint8* const iSrc
        , uint8* const iDst
        , const int64 iSize
    )
        : IJobArgs()
        , src( iSrc )
        , dst( iDst )
        , size( iSize )
    {}

    const uint8* const ULIS_RESTRICT src;
    uint8* const ULIS_RESTRICT dst;
    const int64 size;
};

/////////////////////////////////////////////////////
// Scheduler
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleCopyMT_AX2 );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleCopyMT_SSE4_2 );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleCopyMT_MEM );

/////////////////////////////////////////////////////
// Dispatch
ULIS_DECLARE_DISPATCHER( FDispatchedCopyInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP(
      FDispatchedCopyInvocationSchedulerSelector
    , &ScheduleCopyMT_AX2
    , &ScheduleCopyMT_SSE4_2
    , &ScheduleCopyMT_MEM
)

ULIS_NAMESPACE_END

