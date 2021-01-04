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
    void ScheduleJob( FJob* iJob );
    void WaitForCompletion();
    void SetNumWorkers( uint32 iNumWorkers );
    uint32 GetNumWorkers() const;
    static uint32 MaxWorkers();

private:
    void ThreadProcess();

private:
    // Private Data
    uint32                              mNumBusy;
    bool                                bStop;
    std::vector< std::thread >          mWorkers;
    std::deque< FJob* >                 mJobs;
    std::mutex                          mQueueMutex;
    std::condition_variable             cvJob;
    std::condition_variable             cvFinished;
};

FThreadPool::FThreadPool_Private::~FThreadPool_Private()
{
    // Notify stop condition
    std::unique_lock< std::mutex > latch( mQueueMutex );
    bStop = true;
    cvJob.notify_all();
    latch.unlock();

    // Join all threads
    for( auto& t : mWorkers )
        t.join();
}

FThreadPool::FThreadPool_Private::FThreadPool_Private( uint32 iNumWorkers )
    : mNumBusy( 0 )
    , bStop( false )
{
    uint32 max = FMath::Min( iNumWorkers, MaxWorkers() );
    for( uint32 i = 0; i < max; ++i )
        mWorkers.emplace_back( std::bind( &FThreadPool::FThreadPool_Private::ThreadProcess, this ) );
}

void
FThreadPool::FThreadPool_Private::ScheduleJob( FJob* iJob )
{
    std::unique_lock< std::mutex > lock( mQueueMutex );
    mJobs.push_back( iJob );
    cvJob.notify_one();
}

void
FThreadPool::FThreadPool_Private::WaitForCompletion()
{
    std::unique_lock< std::mutex > lock( mQueueMutex );
    cvFinished.wait( lock, [ this ](){ return mJobs.empty() && ( mNumBusy == 0 ); } );
}

void
FThreadPool::FThreadPool_Private::SetNumWorkers( uint32 iNumWorkers )
{
    WaitForCompletion();

    // Notify stop condition
    std::unique_lock< std::mutex > latch( mQueueMutex );
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
        mWorkers.emplace_back( std::bind( &FThreadPool::FThreadPool_Private::ThreadProcess, this ) );
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
FThreadPool::FThreadPool_Private::ThreadProcess()
{
    while( true )
    {
        std::unique_lock< std::mutex > latch( mQueueMutex );
        cvJob.wait( latch, [ this ](){ return bStop || !mJobs.empty(); } );
        if( !mJobs.empty() )
        {
            // got work. set busy.
            ++mNumBusy;

            // pull from queue
            FJob* job = mJobs.front();
            mJobs.pop_front();

            // release lock. run async
            latch.unlock();

            // run function outside context
            job->Execute();

            // lock again, run sync.
            latch.lock();

            // Gather event and set status if needed
            FSharedInternalEvent evt = job->Parent()->Event();
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

ULIS_NAMESPACE_END

