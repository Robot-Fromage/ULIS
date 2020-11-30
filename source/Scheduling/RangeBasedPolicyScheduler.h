// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         RangeBasedPolicyScheduler.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the RangeBasedPolicyScheduler tools.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Scanline Job Building
template<
      typename TJobArgs
    , typename TCommandArgs
    , void (*TDelegateInvoke)(
          const TJobArgs*
        , const TCommandArgs*
        )
    , void(*TDelegateBuildJobScanline)(
          const TCommandArgs*
        , const int64
        , const int64
        , const int64
        , TJobArgs&
        )
>
ULIS_FORCEINLINE
static
void
RangeBasedSchedulingDelegateBuildJobs_Scanlines(
      FCommand* iCommand
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
)
{
    const TCommandArgs* cargs  = dynamic_cast< const TCommandArgs* >( iCommand->Args() );
    for( int64 i = 0; i < iNumJobs; ++i )
    {
        uint8* buf = new uint8[ iNumTasksPerJob * sizeof( TJobArgs ) ];
        TJobArgs* jargs = reinterpret_cast< TJobArgs* >( buf );
        for( int i = 0; i < iNumTasksPerJob; ++i ) {
            new ( buf ) TJobArgs();
            TDelegateBuildJobScanline( cargs, iNumJobs, iNumTasksPerJob, i, jargs[i] );
        }
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< TJobArgs, TCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
    }
}

/////////////////////////////////////////////////////
// Chunk Job Building
template<
      typename TJobArgs
    , typename TCommandArgs
    , void (*TDelegateInvoke)(
          const TJobArgs*
        , const TCommandArgs*
        )
    , void(*TDelegateBuildJobScanline)(
          const TCommandArgs*
        , const int64
        , const int64
        , const int64
        , const int64
        , TJobArgs&
        )
>
ULIS_FORCEINLINE
static
void
RangeBasedSchedulingDelegateBuildJobs_Chunk(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iSize
    , const int64 iCount
)
{
    const TCommandArgs* cargs  = dynamic_cast< const TCommandArgs* >( iCommand->Args() );
    int64 index = 0;
    for( int i = 0; i < iCount; ++i )
    {
        uint8* buf = new uint8[ sizeof( TJobArgs ) ];
        TJobArgs* jargs = reinterpret_cast< TJobArgs* >( buf );
        new ( buf ) TJobArgs();
        TDelegateBuildJobScanline( cargs, iSize, iCount, index, i, jargs[i] );

        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< TJobArgs, TCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
        index += iSize;
    }
    return;
}

/////////////////////////////////////////////////////
// Master Job Building
template<
      void (*TDelegateScanlines)( FCommand*, const FSchedulePolicy&, const int64, const int64 )
    , void (*TDelegateChunks)( FCommand*, const FSchedulePolicy&, const int64, const int64 )
>
ULIS_FORCEINLINE
static
void
RangeBasedPolicyScheduleJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iBytesTotal
    , const int64 iNumScanlines
    , const bool iChunkAllowed
)
{
    if( iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Mono )
        if( iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines )
            goto mono_scanlines;
        else
            if( !( iChunkAllowed ) )
                goto mono_scanlines;
            else
                goto mono_chunks;
    else
        if( iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines )
            goto multi_scanlines;
        else
            if( !( iChunkAllowed ) )
                goto multi_scanlines;
            else
                if( iPolicy.ParameterPolicy() == eScheduleParameterPolicy::ScheduleParameter_Count )
                    goto multi_chunks_count;
                else
                    goto multi_chunks_length;

mono_scanlines:
    {
        TDelegateScanlines( iCommand, iPolicy, 1, iNumScanlines );
        return;
    }

multi_scanlines:
    {
        TDelegateScanlines( iCommand, iPolicy, iNumScanlines, 1 );
        return;
    }

mono_chunks:
    {
        TDelegateChunks( iCommand, iPolicy, iBytesTotal, 1 );
        return;
    }

multi_chunks_count:
    {
        const int64 count = FMath::Max( iPolicy.Value(), int64(1) );
        const int64 size = int64( FMath::Ceil( iBytesTotal / float( count ) ) );
        TDelegateChunks( iCommand, iPolicy, size, count );
        return;
    }

multi_chunks_length:
    {
        const int64 size = FMath::Max( iPolicy.Value(), int64(1) );
        const int64 count = int64( FMath::Ceil( iBytesTotal / float( size ) ) );
        TDelegateChunks( iCommand, iPolicy, size, count );
        return;
    }
}

ULIS_NAMESPACE_END

