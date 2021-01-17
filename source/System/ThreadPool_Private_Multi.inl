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
#include "System/ThreadPool_Private_Multi.h"

ULIS_NAMESPACE_BEGIN
FThreadPool_Private::~FThreadPool_Private()
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

FThreadPool_Private::FThreadPool_Private( uint32 iNumWorkers )
    : mNumBusy( 0 )
    , bStop( false )
    , mNumQueued( 0 )
{
    uint32 max = FMath::Clamp( iNumWorkers, uint32( 1 ), MaxWorkers() );
    mWorkers.reserve( max );
    for( uint32 i = 0; i < max; ++i )
        mWorkers.emplace_back( std::bind( &FThreadPool_Private::WorkProcess, this ) );

    mScheduler = std::thread( std::bind( &FThreadPool_Private::ScheduleProcess, this ) );
}

void
FThreadPool_Private::ScheduleCommands( TQueue< const FCommand* >& ioCommands )
{
    std::lock_guard< std::mutex > lock( mCommandsQueueMutex );
    uint32 size = ioCommands.Size();
    while( !ioCommands.IsEmpty() )
    {
        const FCommand* cmd = ioCommands.Front();
        ioCommands.Pop();
        ULIS_ASSERT( cmd->ReadyForScheduling(), "Bad Events dependency, this command relies on unscheduled commands and will block the pool forever." );
        mCommands.push_back( cmd );
    }
    mNumQueued.fetch_add( size );
}

void
FThreadPool_Private::ScheduleJob( const FJob* iJob )
{
    std::unique_lock< std::mutex > lock( mJobsQueueMutex );
    mJobs.push_back( iJob );
    cvJob.notify_one();
}

void
FThreadPool_Private::WaitForCompletion()
{
    /*
    while( true )
    {
        std::lock_guard< std::mutex > latch( mCommandsQueueMutex );

        std::unique_lock< std::mutex > lock( mJobsQueueMutex );
        cvJobsFinished.wait( lock, [ this ](){ return mJobs.empty() && ( mNumBusy == 0 ); } );

        if( mCommands.empty() )
            break;
    }
    */
    std::unique_lock< std::mutex > lock( mJobsQueueMutex );
    cvJobsFinished.wait( lock, [ this ](){ return mJobs.empty() && ( mNumBusy == 0 ) && ( mNumQueued == 0 ); } );
}

void
FThreadPool_Private::SetNumWorkers( uint32 iNumWorkers )
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
        mWorkers.emplace_back( std::bind( &FThreadPool_Private::WorkProcess, this ) );
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
            cvJobsFinished.notify_one();
        }
        else if( bStop )
        {
            break;
        }
    }
}

void
FThreadPool_Private::ScheduleProcess()
{
    while( true )
    {
        std::unique_lock< std::mutex > latch( mCommandsQueueMutex );

        if( !mCommands.empty() )
        {
            // pull from queue
            const FCommand* cmd = mCommands.front();
            mCommands.pop_front();

            latch.unlock();

            // Push jobs
            bool ready = cmd->ReadyForProcessing();
            if( ready )
            {
                const TArray< const FJob* >& jobs = cmd->Jobs();
                for( uint64 i = 0; i < jobs.Size(); ++i )
                    ScheduleJob( jobs[i] );
                mNumQueued.fetch_sub( 1 );
            }

            // lock again, run sync.
            latch.lock();

            if( !ready )
            {
                mCommands.push_back( cmd );
            }
        }
        else if( bStop )
        {
            break;
        }
    }
}

ULIS_NAMESPACE_END

