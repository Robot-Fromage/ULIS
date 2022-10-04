// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CommandQueue_Private.cpp
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides the definition for the FCommandQueue_Private class.
* @license      Please refer to LICENSE.md
*/
#include "Scheduling/CommandQueue_Private.h"
#include "System/ThreadPool/ThreadPool.h"
#include "System/ThreadPool/ThreadPool_Private.h"

ULIS_NAMESPACE_BEGIN
FCommandQueue_Private::~FCommandQueue_Private()
{
    // Cleanse unprocessed commands
    while( !mQueue.IsEmpty() )
    {
        const FCommand* cmd = mQueue.Front();
        delete  cmd;
        mQueue.Pop();
    }
}

FCommandQueue_Private::FCommandQueue_Private( FThreadPool& iPool )
    : mPool( iPool )
    , mQueue( tQueue() )
{
}

void
FCommandQueue_Private::Flush()
{
    mPool.d->ScheduleCommands( mQueue );
    mQueue.Clear();
}

void
FCommandQueue_Private::Finish()
{
    Flush();
    Fence();
}

void
FCommandQueue_Private::Fence()
{
    mPool.WaitForCompletion();
}

void
FCommandQueue_Private::Push( const FCommand* iCommand )
{
    ULIS_ASSERT( iCommand, "Error: no input command" );
    iCommand->Event()->NotifyQueued();
    mQueue.Push( iCommand );
}

ULIS_NAMESPACE_END

