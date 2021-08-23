// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RangeBasedPolicyScheduler.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the RangeBasedPolicyScheduler tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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
    , typename TDelegateBuildJobScanlines
>
ULIS_FORCEINLINE
static
void
RangeBasedSchedulingDelegateBuildJobs_Scanlines(
      FCommand* iCommand
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
    , TDelegateBuildJobScanlines iDelegateBuildJobScanlines
)
{
    ULIS_ASSERT( iNumJobs == 1 || iNumTasksPerJob == 1, "Logic error, one of these values should equal 1" );
    iCommand->ReserveJobs( iNumJobs );
    const TCommandArgs* cargs  = dynamic_cast< const TCommandArgs* >( iCommand->Args() );
    for( int64 i = 0; i < iNumJobs; ++i )
    {
        IJobArgs** jargs = new IJobArgs*[ iNumTasksPerJob ];
        for( int j = 0; j < iNumTasksPerJob; ++j ) {
            TJobArgs* largs = new TJobArgs();
            jargs[ j ] = largs;
            iDelegateBuildJobScanlines( cargs, iNumJobs, iNumTasksPerJob, i + j, *largs );
        }
        FJob* job = new FJob(
              static_cast< uint32 >( iNumTasksPerJob )
            , &ResolveScheduledJobInvocation< TJobArgs, TCommandArgs, TDelegateInvoke >
            , jargs
            , iCommand
        );
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
    , typename TDelegateBuildJobChunks
>
ULIS_FORCEINLINE
static
void
RangeBasedSchedulingDelegateBuildJobs_Chunks(
      FCommand* iCommand
    , const int64 iSize
    , const int64 iNumChunks
    , TDelegateBuildJobChunks iDelegateBuildJobChunks
)
{
    iCommand->ReserveJobs( iNumChunks );
    const TCommandArgs* cargs  = dynamic_cast< const TCommandArgs* >( iCommand->Args() );
    int64 offset = 0;
    for( int i = 0; i < iNumChunks; ++i )
    {
        IJobArgs** jargs = new IJobArgs*[ 1 ];
        TJobArgs* largs = new TJobArgs();
        jargs[ 0 ] = largs;
        iDelegateBuildJobChunks( cargs, iSize, iNumChunks, offset, i, *largs );

        FJob* job = new FJob(
              1
            , &ResolveScheduledJobInvocation< TJobArgs, TCommandArgs, TDelegateInvoke >
            , jargs
            , iCommand
        );
        iCommand->AddJob( job );
        offset += iSize;
    }
    return;
}

/////////////////////////////////////////////////////
// Master Job Building
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
ULIS_FORCEINLINE
static
void
RangeBasedSchedulingBuildJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iBytesTotal
    , const int64 iNumScanlines
    , const bool iChunkAllowed
    , const bool iForceMonoChunk
    , TDelegateBuildJobScanlines iDelegateBuildJobScanlines
    , TDelegateBuildJobChunks iDelegateBuildJobChunks
)
{
    if( iForceMonoChunk )
        goto mono_chunks;

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
        RangeBasedSchedulingDelegateBuildJobs_Scanlines<
              TJobArgs
            , TCommandArgs
            , TDelegateInvoke
            , TDelegateBuildJobScanlines
        >
        (
              iCommand
            , 1
            , iNumScanlines
            , iDelegateBuildJobScanlines
        );
        return;
    }

multi_scanlines:
    {
        RangeBasedSchedulingDelegateBuildJobs_Scanlines<
              TJobArgs
            , TCommandArgs
            , TDelegateInvoke
            , TDelegateBuildJobScanlines
        >
        (
              iCommand
            , iNumScanlines
            , 1
            , iDelegateBuildJobScanlines
        );
        return;
    }

mono_chunks:
    {
        RangeBasedSchedulingDelegateBuildJobs_Chunks<
              TJobArgs
            , TCommandArgs
            , TDelegateInvoke
            , TDelegateBuildJobChunks
        >
        (
              iCommand
            , iBytesTotal
            , 1
            , iDelegateBuildJobChunks
        );
        return;
    }

multi_chunks_count:
    {
        const int64 count = FMath::Max( iPolicy.Value(), int64(1) );
        const int64 size = int64( FMath::Ceil( iBytesTotal / float( count ) ) );
        RangeBasedSchedulingDelegateBuildJobs_Chunks<
              TJobArgs
            , TCommandArgs
            , TDelegateInvoke
            , TDelegateBuildJobChunks
        >
        (
              iCommand
            , size
            , count
            , iDelegateBuildJobChunks
        );
        return;
    }

multi_chunks_length:
    {
        const int64 size = FMath::Max( iPolicy.Value(), int64(1) );
        const int64 count = int64( FMath::Ceil( iBytesTotal / float( size ) ) );
        RangeBasedSchedulingDelegateBuildJobs_Chunks<
              TJobArgs
            , TCommandArgs
            , TDelegateInvoke
            , TDelegateBuildJobChunks
        >
        (
              iCommand
            , size
            , count
            , iDelegateBuildJobChunks
        );
        return;
    }
}

ULIS_NAMESPACE_END

