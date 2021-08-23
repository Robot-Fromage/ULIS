// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Thread.Pool.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include "ULIS/Thread/ULIS.Thread.Pool.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FThreadPool
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FThreadPool::FThreadPool(unsigned int n)
    : busy( 0 )
    , stop( false )
    , processed( 0 )
{
    for( unsigned int i = 0; i < n; ++i )
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


} // namespace ULIS

