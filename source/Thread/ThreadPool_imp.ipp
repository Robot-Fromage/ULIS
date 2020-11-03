// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         ThreadPool_imp.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FThreadPool class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <iostream>
#include <deque>
#include <functional>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <random>
#include <atomic>

#include "Maths/Maths.h"

ULIS3_NAMESPACE_BEGIN

/////////////////////////////////////////////////////
// FThreadPool
class FThreadPool
{
public:
    // Construction / Destruction

    FThreadPool( unsigned int iCount = std::thread::hardware_concurrency() )
        : busy( 0 )
        , stop( false )
        , processed( 0 )
    {
        unsigned int max = FMaths::Min( iCount, std::thread::hardware_concurrency() );
        for( unsigned int i = 0; i < max; ++i )
            workers.emplace_back( std::bind( &FThreadPool::ThreadProcess, this ) );
    }

    ~FThreadPool()
    {
        // set stop-condition
        std::unique_lock< std::mutex > latch( queue_mutex );
        stop = true;
        cv_task.notify_all();
        latch.unlock();

        // all threads terminate, then we're done.
        for( auto& t : workers )
            t.join();
    }

public:
    // Public API
    template<class F>
    void ScheduleJob( F&& f ) {
        std::unique_lock< std::mutex > lock( queue_mutex );
        tasks.push_back( std::forward< F >( f ) );
        cv_task.notify_one();
    }

    template<class F, typename ... Args >
    void ScheduleJob( F&& f, Args&& ... args ) {
        std::unique_lock< std::mutex > lock( queue_mutex );
        tasks.push_back( std::bind( std::forward< F >( f ), args ... ) );
        cv_task.notify_one();
    }

    void WaitForCompletion() {
        std::unique_lock< std::mutex > lock( queue_mutex );
        cv_finished.wait( lock, [ this ](){ return tasks.empty() && ( busy == 0 ); } );
    }

    void SetNumWorkers( unsigned int iValue ) {
        WaitForCompletion();

        // set stop-condition
        std::unique_lock< std::mutex > latch( queue_mutex );
        stop = true;
        cv_task.notify_all();
        latch.unlock();

        // all threads terminate, then we're done.
        for( auto& t : workers )
            t.join();

        stop = false;

        workers.clear();
        for( unsigned int i = 0; i < iValue; ++i )
            workers.emplace_back( std::bind( &FThreadPool::ThreadProcess, this ) );
    }

    unsigned int    GetProcessed() const    { return  processed; }
    unsigned int    GetNumWorkers() const   { return  (unsigned int)( workers.size() ); }
    static uint32   MaxWorkers()            { return  std::thread::hardware_concurrency(); }

private:
    // Private API
    void ThreadProcess()
    {
        while( true )
        {
            std::unique_lock< std::mutex > latch( queue_mutex );
            cv_task.wait( latch, [ this ](){ return stop || !tasks.empty(); } );
            if( !tasks.empty() )
            {
                // got work. set busy.
                ++busy;

                // pull from queue
                auto fn = tasks.front();
                tasks.pop_front();

                // release lock. run async
                latch.unlock();

                // run function outside context
                fn();
                ++processed;

                latch.lock();
                --busy;
                cv_finished.notify_one();
            }
            else if( stop )
            {
                break;
            }
        }
    }

private:
    // Private Data
    unsigned int                        busy;
    bool                                stop;
    std::atomic_uint                    processed;
    std::vector< std::thread >          workers;
    std::deque< std::function<void()> > tasks;
    std::mutex                          queue_mutex;
    std::condition_variable             cv_task;
    std::condition_variable             cv_finished;
};

#define ULIS3_MACRO_INLINE_PARALLEL_FOR( _PERF, _POOL, _BLOCKING, _MAX, _FUNC, ... )    \
    if( ( _PERF & ULIS3_PERF_MT ) && _POOL->GetNumWorkers() > 1 )                       \
    {                                                                                   \
        for( int pLINE = 0; pLINE < _MAX; ++pLINE )                                     \
            _POOL->ScheduleJob( _FUNC, __VA_ARGS__ );                                   \
        if( _BLOCKING )                                                                 \
            _POOL->WaitForCompletion();                                                 \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        for( int pLINE = 0; pLINE < _MAX; ++pLINE )                                     \
            _FUNC( __VA_ARGS__ );                                                       \
    }


ULIS3_NAMESPACE_END

