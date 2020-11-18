// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         CopyHelpers.h
* @author       Clement Berthaud
* @brief        This file provides some helper macros and functions for copying.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Copy/CopyArgs.h"
#include "Scheduling/SchedulePolicy.h"
#include "Scheduling/Command.h"
#include "Scheduling/Job.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"

ULIS_NAMESPACE_BEGIN
template< void (*TDelegateInvoke)( const FCopyJobArgs*, const FCopyCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildCopyJobs_Scanlines(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
)
{
    const FCopyCommandArgs* cargs           = dynamic_cast< const FCopyCommandArgs* >( iCommand->Args() );
    const FFormatMetrics& fmt               = cargs->src.FormatMetrics();
    const uint8* const ULIS_RESTRICT src    = cargs->src.Bits() + cargs->srcRect.x * fmt.BPP;
    uint8* const ULIS_RESTRICT dst          = cargs->dst.Bits() + cargs->dstRect.x * fmt.BPP;
    const int64 src_bps                     = static_cast< int64 >( cargs->src.BytesPerScanLine() );
    const int64 dst_bps                     = static_cast< int64 >( cargs->dst.BytesPerScanLine() );
    const int64 size                        = cargs->dstRect.w * fmt.BPP;

    for( int i = 0; i < iNumJobs; ++i )
    {
        uint8* buf = new uint8[ iNumTasksPerJob * sizeof( FCopyJobArgs ) ];
        FCopyJobArgs* jargs = reinterpret_cast< FCopyJobArgs* >( buf );
        for( int i = 0; i < iNumTasksPerJob; ++i )
            new ( buf ) FCopyJobArgs(
              src + ( cargs->srcRect.y + i ) * src_bps
            , dst + ( cargs->dstRect.y + i ) * dst_bps
            , size
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FCopyJobArgs, FCopyCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
    }
}

template< void (*TDelegateInvoke)( const FCopyJobArgs*, const FCopyCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildCopyJobs_Chunks(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iSize
    , const int64 iCount
)
{
    const FCopyCommandArgs* cargs = dynamic_cast< const FCopyCommandArgs* >( iCommand->Args() );
    ULIS_ASSERT( cargs->contiguous, "Bad scheduling, illegal policy if contiguous flag is not set." );
    ULIS_ASSERT( cargs->src.BytesTotal() == cargs->dst.BytesTotal(), "Bad scheduling, chunk policy isn't possible if both blocks have different bytes total" );

    const uint8* const ULIS_RESTRICT src    = cargs->src.Bits();
    uint8* const ULIS_RESTRICT dst          = cargs->dst.Bits();
    const int64 btt                         = static_cast< int64 >( cargs->src.BytesTotal() );

    int64 index = 0;
    for( int i = 0; i < iCount; ++i )
    {
        uint8* buf = new uint8[ sizeof( FCopyJobArgs ) ];
        FCopyJobArgs* jargs = reinterpret_cast< FCopyJobArgs* >( buf );
        new ( buf ) FCopyJobArgs(
              src + index
            , dst + index
            , FMath::Min( index + iSize, btt ) - index
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FCopyJobArgs, FCopyCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
        index += iSize;
    }
    return;
}

template< void (*TDelegateInvoke)( const FCopyJobArgs*, const FCopyCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildCopyJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    const FCopyCommandArgs* cargs   = dynamic_cast< const FCopyCommandArgs* >( iCommand->Args() );
    const int64 btt                 = static_cast< int64 >( cargs->src.BytesTotal() );
    RangeBasedPolicyScheduleJobs< &BuildCopyJobs_Scanlines< TDelegateInvoke >, &BuildCopyJobs_Chunks< TDelegateInvoke > >( iCommand, iPolicy, btt, cargs->dstRect.h, cargs->contiguous );
}

ULIS_NAMESPACE_END

