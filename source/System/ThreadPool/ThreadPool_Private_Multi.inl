// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool_Private_Multi.inl
* @author       Clement Berthaud
* @brief        This file provides the definition for the FThreadPool_Private
*               class for generic systems.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/ThreadPool/ThreadPool_Private_Multi.h"

ULIS_NAMESPACE_BEGIN
FThreadPool_Private::~FThreadPool_Private()
{
    // Complete commands
    WaitForCompletion();

    // Notify stop conditioncd 
    {
        mStopCommands = true;
        mStopJobs = true;
    }

    //Add a dummy command to force wait_dequeue to wake up
    mScheduledCommands.enqueue(nullptr);

    // Join all threads
    mScheduler.join();

    for (auto& t : mWorkers)
    {
        //Add a dummy job to force wait_dequeue to wake up
        mJobs.enqueue(nullptr);
        t.join();
    }

}

FThreadPool_Private::FThreadPool_Private( uint32 iNumWorkers )
    : mStopCommands( false )
    , mStopJobs( false )
    , mNumWaitingCommands( 0 )
    , mNumScheduledCommands( 0 )
    , mNumScheduledJobs( 0 )
    , mDefaultScheduleCommandProducerToken(mScheduledCommands)
{
    uint32 max = FMath::Clamp( iNumWorkers, uint32( 1 ), MaxWorkers() );
    mWorkers.reserve( max );
    for (uint32 i = 0; i < max; ++i)
    {
        mWorkersProducerTokens.emplace_back(mScheduledCommands);
    }

    for (uint32 i = 0; i < max; ++i)
    {
        mWorkers.emplace_back( std::bind( &FThreadPool_Private::WorkProcess, this ) );
    }

    mScheduler = std::thread( std::bind( &FThreadPool_Private::ScheduleProcess, this ) );
}

void
FThreadPool_Private::ScheduleCommands( TQueue< const FCommand* >& ioCommands )
{
    static std::thread::id mainThreadId = std::this_thread::get_id();

    assert(mainThreadId == std::this_thread::get_id());
    //What we do here is just enqueue the commands when they are ready
    //If they are already ready, FOnEventReady will be immediately fired
    //If not, it will be fired later
    //Emptying the queue is ok, as the command still exist and is owned by the Event
    uint32 size = static_cast< uint32 >( ioCommands.Size() );
    mNumWaitingCommands.fetch_add( size, std::memory_order_release );
    while( !ioCommands.IsEmpty() )
    {
        const FCommand* cmd = ioCommands.Front();
        ioCommands.Pop();
        cmd->Event()->SetOnEventReady(
            FOnEventReady(
                [this, cmd]()
                {
                    mNumScheduledCommands.fetch_add( 1, std::memory_order_release );

                    std::thread::id threadId = std::this_thread::get_id();

                    for( int i = 0; i < mWorkers.size(); i++ )
                    {
                        if (threadId == mWorkers[i].get_id())
                        {
                            mScheduledCommands.enqueue(mWorkersProducerTokens[i], cmd);
                            mNumWaitingCommands.fetch_sub(1, std::memory_order_release);
                            return;
                        }
                    }

                    mScheduledCommands.enqueue(mDefaultScheduleCommandProducerToken, cmd);
                    mNumWaitingCommands.fetch_sub(1, std::memory_order_release);
                }
            )
        );
    }
}

void
FThreadPool_Private::WaitForCompletion()
{
    static std::thread::id mainThreadId = std::this_thread::get_id();

    assert(mainThreadId == std::this_thread::get_id());
    // When waiting we are not pushing anything more in the queue, so we can assume queue.size_approx() == 0 is okay to watch
    // To wait for all the frame's tasks to complete before rendering:
    while (mNumWaitingCommands.load(std::memory_order_acquire) != 0)
	    continue;

    while (mNumScheduledCommands.load(std::memory_order_acquire) != 0)
	    continue;

    while (mNumScheduledJobs.load(std::memory_order_acquire) != 0)
	    continue;
}

void
FThreadPool_Private::SetNumWorkers(uint32 iNumWorkers)
{
    WaitForCompletion();

    // Notify stop condition
    {
        //std::lock_guard< std::mutex > lock( mJobsQueueMutex );
        mStopJobs = true;
        //cvJob.notify_all();
    }

    // Join all threads
    for (auto& t : mWorkers)
    {
        //Add a dummy job to force wait_dequeue to wake up
        mJobs.enqueue(nullptr);
        t.join();
    }

    mStopJobs = false;

    uint32 max = FMath::Min( iNumWorkers, MaxWorkers() );
    mWorkers.clear();
    mWorkersProducerTokens.clear();
    mWorkers.reserve( max );
    for( uint32 i = 0; i < max; ++i )
    {
        mWorkersProducerTokens.emplace_back(mScheduledCommands);
    }

    for (uint32 i = 0; i < max; ++i)
    {
        mWorkers.emplace_back(std::bind(&FThreadPool_Private::WorkProcess, this));
    }
}

uint32
FThreadPool_Private::GetNumWorkers() const
{
    return  static_cast< uint32 >( mWorkers.size() );
}

//static
uint32
FThreadPool_Private::MaxWorkers()
{
    return  std::thread::hardware_concurrency();
}

void
FThreadPool_Private::WorkProcess()
{
    ::moodycamel::ConsumerToken consumerToken(mScheduledCommands);
    while( !mStopJobs )
    {
        const FJob* job = nullptr;
        mJobs.wait_dequeue(consumerToken, job);

        while (!mStopJobs)
        {   
            if (job)
            {
                //Execute Job
                job->Execute();
            }

            mNumScheduledJobs.fetch_sub( 1, std::memory_order_release );
            if (!mJobs.try_dequeue(consumerToken, job))
                break;
        }
    }
}

void
FThreadPool_Private::ScheduleProcess()
{
    ::moodycamel::ConsumerToken consumerToken(mScheduledCommands);
    ::moodycamel::ProducerToken producerToken(mJobs);
    while( !mStopCommands )
    {
        const FCommand* command = nullptr;
        mScheduledCommands.wait_dequeue(consumerToken, command);

        while (!mStopCommands)
        {
            if (command)
            {
                if( command->SchedulePolicy().TimePolicy() == eScheduleTimePolicy::ScheduleTime_Async )
                    const_cast< FCommand* >( command )->ProcessAsyncScheduling();

                const TArray< const FJob* >& jobs = command->Jobs();
                
                //ScheduleJobs( jobs );
                mNumScheduledJobs.fetch_add(jobs.Size(), std::memory_order_release);
                mJobs.enqueue_bulk(producerToken, jobs.Data(), jobs.Size());                
            }

            mNumScheduledCommands.fetch_sub( 1, std::memory_order_release );
            if (!mScheduledCommands.try_dequeue(consumerToken, command))
                break;
        }
    }
}

ULIS_NAMESPACE_END

