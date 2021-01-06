// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool_Private_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the FThreadPool_Private
*               class for generic systems.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/ThreadPool.h"
#include "Memory/Array.h"
#include "Memory/Queue.h"
#include "Scheduling/Job.h"
#include "Scheduling/Command.h"
#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <random>
#include <thread>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FThreadPool_Private
/// @brief      The FThreadPool_Private class provides the private implementation
///             for FThreadPool using the pimpl idiom.
/// @details    This version of the private implementation is for generic
///             systems with multithreading support.
///
///             \sa FThreadPool
class FThreadPool::FThreadPool_Private
{
public:
    ~FThreadPool_Private();
    FThreadPool_Private( uint32 iNumWorkers = MaxWorkers() );
    FThreadPool_Private( const FThreadPool_Private& ) = delete;
    FThreadPool_Private& operator=( const FThreadPool_Private& ) = delete;
    void ScheduleCommand( const FCommand* iJob );
    void WaitForCompletion();
    void SetNumWorkers( uint32 iNumWorkers );
    uint32 GetNumWorkers() const;
    static uint32 MaxWorkers();

private:
    void ScheduleJob( const FJob* iJob );
    void WorkProcess();
    void ScheduleProcess();

private:
    // Private Data
    uint32                              mNumBusy;
    uint32                              mNumQueued;
    bool                                bStop;
    std::vector< std::thread >          mWorkers;
    std::deque< const FJob* >           mJobs;
    std::deque< const FCommand* >       mCommands;
    std::mutex                          mJobsQueueMutex;
    std::mutex                          mCommandsQueueMutex;
    std::condition_variable             cvJob;
    std::condition_variable             cvCommand;
    std::condition_variable             cvFinished;
    std::thread                         mScheduler;
};

FThreadPool::FThreadPool_Private::~FThreadPool_Private()
{
    // Notify stop condition
    std::unique_lock< std::mutex > latch( mJobsQueueMutex );
    bStop = true;
    cvJob.notify_all();
    latch.unlock();

    // Join all threads
    for( auto& t : mWorkers )
        t.join();

    mScheduler.join();
}

FThreadPool::FThreadPool_Private::FThreadPool_Private( uint32 iNumWorkers )
    : mNumBusy( 0 )
    , mNumQueued( 0 )
    , bStop( false )
    , mScheduler( std::bind( &FThreadPool::FThreadPool_Private::ScheduleProcess, this ) )
{
    uint32 max = FMath::Clamp( iNumWorkers, uint32( 1 ), MaxWorkers() );
    mWorkers.reserve( max );
    for( uint32 i = 0; i < max; ++i )
        mWorkers.emplace_back( std::bind( &FThreadPool::FThreadPool_Private::WorkProcess, this ) );
}

void
FThreadPool::FThreadPool_Private::ScheduleCommand( const FCommand* iCommand )
{
    ULIS_ASSERT( iCommand->ReadyForScheduling(), "Bad Events dependency, this command relies on unscheduled commands and will block the pool forever." );
    std::unique_lock< std::mutex > lock( mCommandsQueueMutex );
    mCommands.push_back( iCommand );
    cvCommand.notify_one();
}

void
FThreadPool::FThreadPool_Private::ScheduleJob( const FJob* iJob )
{
    std::unique_lock< std::mutex > lock( mJobsQueueMutex );
    mJobs.push_back( iJob );
    cvJob.notify_one();
}

void
FThreadPool::FThreadPool_Private::WaitForCompletion()
{
    std::unique_lock< std::mutex > lock( mJobsQueueMutex );
    cvFinished.wait( lock, [ this ](){ return mJobs.empty() && ( mNumBusy == 0 ); } );
}

void
FThreadPool::FThreadPool_Private::SetNumWorkers( uint32 iNumWorkers )
{
    WaitForCompletion();

    // Notify stop condition
    std::unique_lock< std::mutex > latch( mJobsQueueMutex );
    bStop = true;
    cvJob.notify_all();
    latch.unlock();

    // Join all threads
    for( auto& t : mWorkers )
        t.join();

    bStop = false;

    uint32 max = FMath::Min( iNumWorkers, MaxWorkers() );
    mWorkers.clear();
    for( uint32 i = 0; i < max; ++i )
        mWorkers.emplace_back( std::bind( &FThreadPool::FThreadPool_Private::WorkProcess, this ) );
}

uint32
FThreadPool::FThreadPool_Private::GetNumWorkers() const
{
    return  static_cast< uint32 >( mWorkers.size() );
}

//static
uint32
FThreadPool::FThreadPool_Private::MaxWorkers()
{
    return  std::thread::hardware_concurrency();
}

void
FThreadPool::FThreadPool_Private::WorkProcess()
{
    while( true )
    {
        std::unique_lock< std::mutex > latch( mJobsQueueMutex );
        cvJob.wait( latch, [ this ](){ return bStop || !mJobs.empty(); } );
        if( !mJobs.empty() )
        {
            // got work. set busy.
            ++mNumBusy;

            // pull from queue
            const FJob* job = mJobs.front();
            mJobs.pop_front();

            // release lock. run async
            latch.unlock();

            // Gather event
            FSharedInternalEvent evt = job->Parent()->Event();

            // run function outside context
            job->Execute();

            // lock again, run sync.
            latch.lock();

            // Notify event
            evt->NotifyOneJobFinished();

            // Managing internals
            --mNumBusy;
            cvFinished.notify_one();
        }
        else if( bStop )
        {
            break;
        }
    }
}

void
FThreadPool::FThreadPool_Private::ScheduleProcess()
{
    while( true )
    {
        std::unique_lock< std::mutex > latch( mCommandsQueueMutex );
        cvCommand.wait( latch, [ this ](){ return bStop || !mCommands.empty(); } );
        if( !mCommands.empty() )
        {
            // got work. set busy.
            ++mNumQueued;

            // pull from queue
            const FCommand* cmd = mCommands.front();
            mCommands.pop_front();

            // release lock. run async
            latch.unlock();

            // Push jobs
            bool ready = cmd->ReadyForProcessing();
            if( ready )
            {
                const TArray< const FJob* >& jobs = cmd->Jobs();
                for( uint64 i = 0; i < jobs.Size(); ++i )
                    ScheduleJob( mJobs[i] );
            }

            // lock again, run sync.
            latch.lock();

            if( !ready )
            {
                mCommands.push_back( cmd );
            }

            // Managing internals
            --mNumQueued;
            //cvFinished.notify_one();
        }
        else if( bStop )
        {
            break;
        }
    }
}

ULIS_NAMESPACE_END

