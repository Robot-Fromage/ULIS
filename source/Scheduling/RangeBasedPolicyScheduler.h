// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RangeBasedPolicyScheduler.h
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides declaration for the RangeBasedPolicyScheduler tools.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/JobChunks.h"
#include "Scheduling/JobScanlines.h"

ULIS_NAMESPACE_BEGIN

#ifdef NEW_JOBSYSTEM

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
    , const uint64 iNumJobs
    , const uint64 iNumTasksPerJob
    , const uint64 iNumTasks
    , TDelegateBuildJobScanlines iDelegateBuildJobScanlines
)
{
    //ULIS_ASSERT( iNumJobs == 1 || iNumTasksPerJob == 1, "Logic error, one of these values should equal 1" );
    const TCommandArgs* cargs  = dynamic_cast< const TCommandArgs* >( iCommand->Args() );
    FJobScanlines<TCommandArgs, TJobArgs, TDelegateBuildJobScanlines>* job = new FJobScanlines<TCommandArgs, TJobArgs, TDelegateBuildJobScanlines>(iNumJobs, iNumTasksPerJob, iNumTasks, &ResolveScheduledJobInvocation< TJobArgs, TCommandArgs, TDelegateInvoke >, cargs, iDelegateBuildJobScanlines);
    iCommand->SetJob(job);
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
    , const uint64 iSize
    , const uint64 iNumChunks
    , TDelegateBuildJobChunks iDelegateBuildJobChunks
)
{
    const TCommandArgs* cargs  = dynamic_cast< const TCommandArgs* >( iCommand->Args() );
    FJobChunks<TCommandArgs, TJobArgs, TDelegateBuildJobChunks>* job = new FJobChunks<TCommandArgs, TJobArgs, TDelegateBuildJobChunks>(iNumChunks, 1, iNumChunks, &ResolveScheduledJobInvocation< TJobArgs, TCommandArgs, TDelegateInvoke >, cargs, iSize, iDelegateBuildJobChunks);
    iCommand->SetJob(job);
}

#else
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
    , const uint64 iNumJobs
    , const uint64 iNumTasksPerJob
    , TDelegateBuildJobScanlines iDelegateBuildJobScanlines
)
{
    ULIS_ASSERT( iNumJobs == 1 || iNumTasksPerJob == 1, "Logic error, one of these values should equal 1" );
    iCommand->ReserveJobs( iNumJobs );
    const TCommandArgs* cargs  = dynamic_cast< const TCommandArgs* >( iCommand->Args() );
    for( uint64 i = 0; i < iNumJobs; ++i )
    {
        IJobArgs** jargs = new IJobArgs*[ iNumTasksPerJob ];
        for( int j = 0; j < iNumTasksPerJob; ++j ) {
            TJobArgs* largs = new TJobArgs();
            jargs[ j ] = largs;
            iDelegateBuildJobScanlines( cargs, i + j, *largs );
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
    , const uint64 iSize
    , const uint64 iNumChunks
    , TDelegateBuildJobChunks iDelegateBuildJobChunks
)
{
    iCommand->ReserveJobs( iNumChunks );
    const TCommandArgs* cargs  = dynamic_cast< const TCommandArgs* >( iCommand->Args() );
    uint64 offset = 0;
    for( int i = 0; i < iNumChunks; ++i )
    {
        IJobArgs** jargs = new IJobArgs*[ 1 ];
        TJobArgs* largs = new TJobArgs();
        jargs[ 0 ] = largs;
        iDelegateBuildJobChunks( cargs, iSize, offset, i, *largs );

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
#endif

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
    , const uint64 iBytesTotal
    , const uint64 iNumScanlines
    , const uint64 iBytesPerScanline
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
            , iNumScanlines
            , iDelegateBuildJobScanlines
        );
        return;
    }

multi_scanlines:
    {
        const uint64 size = FMath::Max( iPolicy.Value(), uint64(1) );
        const uint64 NumTaskPerJob = uint64( FMath::Ceil(size / float(iBytesPerScanline) ) );
        const uint64 NumJobs = uint64( FMath::Ceil(iNumScanlines / float(NumTaskPerJob) ) );
        // const uint64 size = FMath::Max( iPolicy.Value(), uint64(1) );
        // const uint64 count = uint64( FMath::Ceil( iBytesTotal / float( size ) ) );


        RangeBasedSchedulingDelegateBuildJobs_Scanlines<
              TJobArgs
            , TCommandArgs
            , TDelegateInvoke
            , TDelegateBuildJobScanlines
        >
        (
              iCommand
            , NumJobs
            , NumTaskPerJob // num scanline per job
            , iNumScanlines
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
        const uint64 count = FMath::Max( iPolicy.Value(), uint64(1) );
        const uint64 size = uint64( FMath::Ceil( iBytesTotal / float( count ) ) );
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
        const uint64 size = FMath::Max( iPolicy.Value(), uint64(1) );
        const uint64 count = uint64( FMath::Ceil( iBytesTotal / float( size ) ) );
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

