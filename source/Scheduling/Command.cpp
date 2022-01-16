// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Command.cpp
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides the definition for the FCommand class.
* @license      Please refer to LICENSE.md
*/
#include "Scheduling/Command.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Scheduling/Job.h"
#include "System/ThreadPool/ThreadPool.h"

ULIS_NAMESPACE_BEGIN

FCommand::~FCommand()
{
    if( mArgs )
        delete  mArgs;

#ifdef NEW_JOBSYSTEM

    if (mJob)
        delete mJob;
#else
    for( uint64 i = 0; i < mJobs.Size(); ++i )
        delete  mJobs[i];
#endif
}

FCommand::FCommand(
      fpCommandScheduler iSched
    , const ICommandArgs* iArgs
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
    , bool iForceMonoChunk
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
    , const FRectI& iEventGeometry
)
    : mArgs( iArgs )
    , mEvent( nullptr )
    
#ifdef NEW_JOBSYSTEM
    , mJob( nullptr )
#else
    , mJobs()
    , mJobIndex(0)
#endif
    , mWorkingThreads(0)
    , mSched( iSched )
    , mPolicy( iPolicy )
    , mContiguous( iContiguous )
    , mForceMonoChunk( iForceMonoChunk )
{
    // Bind Event
    if( iEvent ) {
        mEvent = iEvent->d->m;
        ULIS_ASSERT( !( mEvent->IsBound() ), "Cannot reuse an event that is already bound to a command" );
    } else {
        mEvent = FInternalEvent::MakeShared();
    }

    // Start Enqueuing Jobs
    // iSched( this, iPolicy, iContiguous, iForceMonoChunk );
    mEvent->Bind( this, iNumWait, iWaitList, iEventGeometry );

    if( mPolicy.TimePolicy() == eScheduleTimePolicy::ScheduleTime_Sync )
        ProcessAsyncScheduling();
}

const ICommandArgs*
FCommand::Args() const
{
    return  mArgs;
}

#ifdef NEW_JOBSYSTEM

void
FCommand::ExecuteConcurrently()
{
    mJob->ExecuteConcurrently();
}

void
FCommand::SetJob(IJob* iJob)
{
    mJob = iJob;
}

IJob*
FCommand::GetJob()
{
    return mJob;
}

uint64
FCommand::GetMaxConcurrency() const
{
    return mJob->GetMaxConcurrency();
}

#else
void
FCommand::ReserveJobs( uint64 iNum )
{
    mJobs.Reserve( iNum );
}

void
FCommand::AddJob( const FJob* iJob )
{
    mJobs.EmplaceBack( iJob );
}


FJob*
FCommand::GetJobForExecution()
{
    if (mJobIndex.load(::std::memory_order_relaxed) >= mJobs.Size())
        return nullptr;

    uint64 index = mJobIndex.fetch_add(1, ::std::memory_order_relaxed);
    if (index >= mJobs.Size())
        return nullptr;

    return const_cast<FJob*>(mJobs[index]);
}

uint64
FCommand::NumJobs() const
{
    return  mJobs.Size();
}

const TArray< const FJob* >&
FCommand::Jobs() const
{
    return  mJobs;
}

#endif

::std::atomic_uint32_t&
FCommand::WorkingThreads()
{
    return mWorkingThreads;
}

void
FCommand::ProcessAsyncScheduling()
{
    if (mSched)
    {
        mSched(this, mPolicy, mContiguous, mForceMonoChunk);
    }
    // mEvent->PostBindAsync();
}

FSharedInternalEvent
FCommand::Event() const
{
    return  mEvent;
}

const FSchedulePolicy&
FCommand::SchedulePolicy() const
{
    return mPolicy;
}

ULIS_NAMESPACE_END

