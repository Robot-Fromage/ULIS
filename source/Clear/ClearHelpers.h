// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         BlendHelpers.h
* @author       Clement Berthaud
* @brief        This file provides some helper macros and functions for blending.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Clear/ClearArgs.h"
#include "Scheduling/SchedulePolicy.h"
#include "Scheduling/Command.h"
#include "Scheduling/Job.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"

ULIS_NAMESPACE_BEGIN
template< void (*IMP)( const FClearJobArgs*, const FClearCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildClearJobs_Scanlines(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
)
{
    const FClearCommandArgs* cargs  = dynamic_cast< const FClearCommandArgs* >( iCommand->Args() );
    const FFormatMetrics& fmt       = cargs->block.FormatMetrics();
    uint8* const ULIS_RESTRICT dst  = cargs->block.Bits() + cargs->rect.x * fmt.BPP;
    const int64 bps                 = static_cast< int64 >( cargs->block.BytesPerScanLine() );
    const int64 size                = cargs->rect.w * fmt.BPP;

    for( int i = 0; i < iNumJobs; ++i )
    {
        uint8* buf = new uint8[ iNumTasksPerJob * sizeof( FClearJobArgs ) ];
        FClearJobArgs* jargs = reinterpret_cast< FClearJobArgs* >( buf );
        for( int i = 0; i < iNumTasksPerJob; ++i )
            new ( buf ) FClearJobArgs(
              dst + ( cargs->rect.y + i ) * bps
            , size
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FClearJobArgs, FClearCommandArgs, IMP >
            , jargs );
        iCommand->AddJob( job );
    }
}

template< void (*IMP)( const FClearJobArgs*, const FClearCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildClearJobs_Chunks(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iSize
    , const int64 iCount
)
{
    const FClearCommandArgs* cargs  = dynamic_cast< const FClearCommandArgs* >( iCommand->Args() );
    uint8* const ULIS_RESTRICT dst  = cargs->block.Bits();
    const int64 btt                 = static_cast< int64 >( cargs->block.BytesTotal() );

    int64 index = 0;
    for( int i = 0; i < iCount; ++i )
    {
        uint8* buf = new uint8[ sizeof( FClearJobArgs ) ];
        FClearJobArgs* jargs = reinterpret_cast< FClearJobArgs* >( buf );
        new ( buf ) FClearJobArgs(
              dst + index
            , FMath::Min( index + iSize, btt ) - index
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FClearJobArgs, FClearCommandArgs, IMP >
            , jargs );
        iCommand->AddJob( job );
        index += iSize;
    }
    return;
}

template< void (*IMP)( const FClearJobArgs*, const FClearCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildClearJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    const FClearCommandArgs* cargs  = dynamic_cast< const FClearCommandArgs* >( iCommand->Args() );
    const int64 btt                 = static_cast< int64 >( cargs->block.BytesTotal() );

    if( iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Mono )
        if( iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines )
            goto mono_scanlines;
        else
            if( !( cargs->whole ) )
                goto mono_scanlines;
            else
                goto mono_chunks;
    else
        if( iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines )
            goto multi_scanlines;
        else
            if( !( cargs->whole ) )
                goto multi_scanlines;
            else
                if( iPolicy.ParameterPolicy() == eScheduleParameterPolicy::ScheduleParameter_Count )
                    goto multi_chunks_count;
                else
                    goto multi_chunks_length;

mono_scanlines:
    {
        BuildClearJobs_Scanlines< IMP >( iCommand, iPolicy, 1, cargs->rect.h );
        return;
    }

multi_scanlines:
    {
        BuildClearJobs_Scanlines< IMP >( iCommand, iPolicy, cargs->rect.h, 1 );
        return;
    }

mono_chunks:
    {
        BuildClearJobs_Chunks< IMP >( iCommand, iPolicy, btt, 1 );
        return;
    }

multi_chunks_count:
    {
        const int64 count = FMath::Max( iPolicy.Value(), int64(1) );
        const int64 size = int64( FMath::Ceil( btt / float( count ) ) );
        BuildClearJobs_Chunks< IMP >( iCommand, iPolicy, size, count );
        return;
    }

multi_chunks_length:
    {
        const int64 size = FMath::Max( iPolicy.Value(), int64(1) );
        const int64 count = int64( FMath::Ceil( btt / float( size ) ) );
        BuildClearJobs_Chunks< IMP >( iCommand, iPolicy, size, count );
        return;
    }
}

ULIS_NAMESPACE_END

