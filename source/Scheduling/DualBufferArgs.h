// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         DualBuffer.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the DualBuffer arguments and scheduling functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
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
/// @class      FDualBufferCommandArgs
/// @brief      The FDualBufferCommandArgs class provides a class to implement
///             the arguments objects for operations, used on simple buffers.
/// @details    FDualBufferCommandArgs is usually used for simple operations on a
///             single buffer, such as Clear or Fill, for instance.
class FDualBufferCommandArgs
    : public ICommandArgs
{
public:
    virtual ~FDualBufferCommandArgs() override {}
    FDualBufferCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
    )
        : ICommandArgs()
        , src( iSrc )
        , dst( iDst )
        , srcRect( iSrcRect )
        , dstRect( iDstRect )
    {}

    const FBlock& src;
    FBlock& dst;
    const FRectI srcRect;
    const FRectI dstRect;
};

/////////////////////////////////////////////////////
/// @class      FDualBufferJobArgs
/// @brief      The FDualBufferJobArgs class provides a class to implement
///             the arguments objects for operations, used on simple buffers.
/// @details    FDualBufferJobArgs is usually used for simple operations on a
///             single buffer, such as Clear or Fill, for instance.
class FDualBufferJobArgs final
    : public IJobArgs
{
public:
    ~FDualBufferJobArgs() override {}

    const uint8* ULIS_RESTRICT src;
    uint8* ULIS_RESTRICT dst;
    int64 size;
};

/////////////////////////////////////////////////////
// Job Building
void
BuildDualBufferJob_Scanlines(
      const FDualBufferCommandArgs* iCargs
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
    , const int64 iIndex
    , FDualBufferJobArgs& oJargs
)
{
    const FFormatMetrics& fmt               = iCargs->src.FormatMetrics();
    const uint8* const ULIS_RESTRICT src    = iCargs->src.Bits() + iCargs->srcRect.x * fmt.BPP;
    uint8* const ULIS_RESTRICT dst          = iCargs->dst.Bits() + iCargs->dstRect.x * fmt.BPP;
    const int64 src_bps                     = static_cast< int64 >( iCargs->src.BytesPerScanLine() );
    const int64 dst_bps                     = static_cast< int64 >( iCargs->dst.BytesPerScanLine() );
    const int64 size                        = iCargs->dstRect.w * fmt.BPP;
    oJargs.src                              = src + ( iCargs->srcRect.y + iIndex ) * src_bps;
    oJargs.dst                              = dst + ( iCargs->dstRect.y + iIndex ) * dst_bps;
    oJargs.size                             = size;
}

void
BuildDualBufferJob_Chunks(
      const FDualBufferCommandArgs* iCargs
    , const int64 iSize
    , const int64 iCount
    , const int64 iOffset
    , const int64 iIndex
    , FDualBufferJobArgs& oJargs
)
{
    const uint8* const ULIS_RESTRICT src    = iCargs->src.Bits();
    uint8* const ULIS_RESTRICT dst          = iCargs->dst.Bits();
    const int64 btt                         = static_cast< int64 >( iCargs->src.BytesTotal() );
    oJargs.src                              = src + iIndex;
    oJargs.dst                              = dst + iIndex;
    oJargs.size                             = FMath::Min( iOffset + iSize, btt ) - iOffset;
}

template<
      typename TJobArgs
    , typename TCommandArgs
    , void (*TDelegateInvoke)(
          const TJobArgs*
        , const TCommandArgs*
        )
>
void
ScheduleDualBufferJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    const FDualBufferCommandArgs* cargs = dynamic_cast< const FDualBufferCommandArgs* >( iCommand->Args() );
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
        , BuildDualBufferJob_Scanlines
        , BuildDualBufferJob_Chunks
    );
}

#define ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( iName, iJobArgs, iCommandArgs, iDelegateInvocation )        \
void                                                                                                            \
iName(                                                                                                          \
      FCommand* iCommand                                                                                        \
    , const FSchedulePolicy& iPolicy                                                                            \
    , bool iContiguous                                                                                          \
)                                                                                                               \
{                                                                                                               \
    ScheduleDualBufferJobs< iJobArgs, iCommandArgs, iDelegateInvocation >( iCommand, iPolicy, iContiguous );    \
}
ULIS_NAMESPACE_END

