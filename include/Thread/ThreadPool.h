// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         ThreadPool.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FThreadPool class.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
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

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FThreadPool
class ULIS2_API FThreadPool
{
public:
    // Construction / Destruction
    FThreadPool( unsigned int iCount = std::thread::hardware_concurrency() );
    ~FThreadPool();
    static uint32 MaxWorkers() { return  std::thread::hardware_concurrency(); }

public:
    // Public API
    template<class F>
    void ScheduleJob( F&& f )
    {
        std::unique_lock< std::mutex > lock( queue_mutex );
        tasks.push_back( std::forward< F >( f ) );
        cv_task.notify_one();
    }

    template<class F, typename ... Args >
    void ScheduleJob( F&& f, Args&& ... args )
    {
        std::unique_lock< std::mutex > lock( queue_mutex );
        tasks.push_back( std::bind( std::forward< F >( f ), args ... ) );
        cv_task.notify_one();
    }


    void            WaitForCompletion();
    unsigned int    GetProcessed() const { return  processed; }
    unsigned int    GetNumWorkers() const { return  (unsigned int)( workers.size() ); }
    void            SetNumWorkers( unsigned int );
    unsigned int    GetMaxWorkers() const { return  std::thread::hardware_concurrency(); }

private:
    // Private API
    void ThreadProcess();

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

ULIS2_API ULIS2_FORCEINLINE void Fence( FThreadPool& iPool ) {
    iPool.WaitForCompletion();
}

#define ULIS2_MACRO_INLINE_PARALLEL_FOR( _PERF, _POOL, _BLOCKING, _MAX, _FUNC, ... )    \
    if( ( _PERF & ULIS2_PERF_MT ) && _POOL->GetNumWorkers() > 1 )                        \
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


ULIS2_NAMESPACE_END

