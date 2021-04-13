// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SimpleBufferArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the SimpleBuffer arguments and scheduling functions.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"
#include "Scheduling/ScheduleArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FSimpleBufferCommandArgs
/// @brief      The FSimpleBufferCommandArgs class provides a class to implement
///             the arguments objects for operations, used on simple buffers.
/// @details    FSimpleBufferCommandArgs is usually used for simple operations on a
///             single buffer, such as Clear or Fill, for instance.
class FSimpleBufferCommandArgs
    : public ICommandArgs
{
public:
    virtual ~FSimpleBufferCommandArgs() override {}
    FSimpleBufferCommandArgs(
          FBlock& iDst
        , const FRectI& iDstRect
    )
        : ICommandArgs( iDstRect )
        , dst( iDst )
    {}

    FBlock& dst;
};

/////////////////////////////////////////////////////
/// @class      FSimpleBufferJobArgs
/// @brief      The FSimpleBufferJobArgs class provides a class to implement
///             the arguments objects for operations, used on simple buffers.
/// @details    FSimpleBufferJobArgs is usually used for simple operations on a
///             single buffer, such as Clear or Fill, for instance.
class FSimpleBufferJobArgs final
    : public IJobArgs
{
public:
    ~FSimpleBufferJobArgs() override {}

    uint8* ULIS_RESTRICT dst;
    int64 size;
    uint32 line;
};

/////////////////////////////////////////////////////
// Job Building
static
void
BuildSimpleBufferJob_Scanlines(
      const FSimpleBufferCommandArgs* iCargs
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
    , const int64 iIndex
    , FSimpleBufferJobArgs& oJargs
)
{
    const FFormatMetrics& fmt       = iCargs->dst.FormatMetrics();
    uint8* const ULIS_RESTRICT dst  = iCargs->dst.Bits() + iCargs->dstRect.x * fmt.BPP;
    const int64 bps                 = static_cast< int64 >( iCargs->dst.BytesPerScanLine() );
    const int64 size                = iCargs->dstRect.w * fmt.BPP;
    oJargs.dst                      = dst + ( iCargs->dstRect.y + iIndex ) * bps;
    oJargs.size                     = size;
    oJargs.line                     = static_cast< uint32 >( iIndex );
}

static
void
BuildSimpleBufferJob_Chunks(
      const FSimpleBufferCommandArgs* iCargs
    , const int64 iSize
    , const int64 iCount
    , const int64 iOffset
    , const int64 iIndex
    , FSimpleBufferJobArgs& oJargs
)
{
    uint8* const ULIS_RESTRICT dst  = iCargs->dst.Bits();
    const int64 btt                 = static_cast< int64 >( iCargs->dst.BytesTotal() );
    oJargs.dst                      = dst + iOffset;
    oJargs.size                     = FMath::Min( iOffset + iSize, btt ) - iOffset;
    oJargs.line                     = ULIS_UINT16_MAX; // N/A for chunks
}

template<
      typename TJobArgs
    , typename TCommandArgs
    , void (*TDelegateInvoke)(
          const TJobArgs*
        , const TCommandArgs*
        )
    , typename TDelegateBuildJobScanlines
    , typename TDelegateBuildJobChunks
>
void
ScheduleSimpleBufferJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
    , bool iForceMonoChunk
    , TDelegateBuildJobScanlines iDelegateBuildJobScanlines
    , TDelegateBuildJobChunks iDelegateBuildJobChunks
)
{
    const FSimpleBufferCommandArgs* cargs = dynamic_cast< const FSimpleBufferCommandArgs* >( iCommand->Args() );
    RangeBasedSchedulingBuildJobs<
          TJobArgs
        , TCommandArgs
        , TDelegateInvoke
    >
    (
          iCommand
        , iPolicy
        , static_cast< int64 >( cargs->dst.BytesTotal() )
        , cargs->dstRect.h
        , iContiguous
        , iForceMonoChunk
        , iDelegateBuildJobScanlines
        , iDelegateBuildJobChunks
    );
}

#define ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE_CUSTOM( iName, iJobArgs, iCommandArgs, iDelegateInvocation, iDelegateBuildJobScanlines, iDelegateBuildJobChunks )  \
void                                                                                                                                                                    \
iName(                                                                                                                                                                  \
      FCommand* iCommand                                                                                                                                                \
    , const FSchedulePolicy& iPolicy                                                                                                                                    \
    , bool iContiguous                                                                                                                                                  \
    , bool iForceMonoChunk                                                                                                                                              \
)                                                                                                                                                                       \
{                                                                                                                                                                       \
    ScheduleSimpleBufferJobs<                                                                                                                                           \
          iJobArgs                                                                                                                                                      \
        , iCommandArgs                                                                                                                                                  \
        , iDelegateInvocation                                                                                                                                           \
    >                                                                                                                                                                   \
    (                                                                                                                                                                   \
          iCommand                                                                                                                                                      \
        , iPolicy                                                                                                                                                       \
        , iContiguous                                                                                                                                                   \
        , iForceMonoChunk                                                                                                                                               \
        , iDelegateBuildJobScanlines                                                                                                                                    \
        , iDelegateBuildJobChunks                                                                                                                                       \
    );                                                                                                                                                                  \
}

#define ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( iName, iJobArgs, iCommandArgs, iDelegateInvocation )  \
    ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE_CUSTOM(                                                    \
          iName                                                                                             \
        , iJobArgs                                                                                          \
        , iCommandArgs                                                                                      \
        , iDelegateInvocation                                                                               \
        , BuildSimpleBufferJob_Scanlines                                                                    \
        , BuildSimpleBufferJob_Chunks                                                                       \
    )

#define ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE_CUSTOM( iName, iJobArgs, iCommandArgs, iDelegateInvocation, iDelegateBuildJobScanlines, iDelegateBuildJobChunks )              \
    template< typename T > ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE_CUSTOM( iName, iJobArgs, iCommandArgs, iDelegateInvocation, iDelegateBuildJobScanlines, iDelegateBuildJobChunks )

#define ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( iName, iJobArgs, iCommandArgs, iDelegateInvocation )              \
    template< typename T > ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( iName, iJobArgs, iCommandArgs, iDelegateInvocation )

ULIS_NAMESPACE_END

