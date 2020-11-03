// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         ThreadPool.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FThreadPool class.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Thread/ThreadPool.h"
#include "Maths/Maths.h"

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FThreadPool
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FThreadPool::FThreadPool(unsigned int n)
    : busy( 0 )
    , stop( false )
    , processed( 0 )
{
    unsigned int max = FMaths::Min( n, std::thread::hardware_concurrency() );
    for( unsigned int i = 0; i < max; ++i )
        workers.emplace_back( std::bind( &FThreadPool::ThreadProcess, this ) );
}

FThreadPool::~FThreadPool()
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

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void FThreadPool::WaitForCompletion()
{
    std::unique_lock< std::mutex > lock( queue_mutex );
    cv_finished.wait( lock, [ this ](){ return tasks.empty() && ( busy == 0 ); } );
}


void FThreadPool::SetNumWorkers( unsigned int iValue )
{
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


//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- Private API
void FThreadPool::ThreadProcess()
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

ULIS2_NAMESPACE_END

