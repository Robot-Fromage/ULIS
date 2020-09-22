// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         CommandQueue.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FCommandQueue class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/CommandQueue.h"
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN
FCommandQueue::~FCommandQueue()
{
    CleanseIdle();
    CleanseScheduled();
}

FCommandQueue::FCommandQueue( FThreadPool& iPool )
    : mIdleQueue( tQueue() )
    , mScheduledQueue( tQueue() )
    , mPool( iPool )
{
}

void
FCommandQueue::Flush()
{
    while( !mIdleQueue.IsEmpty() )
    {
        FCommand* cmd = mIdleQueue.Front();
        mIdleQueue.Pop();

        if( cmd->IsReady() ) {
            FTaskEvent* evt = cmd->Event();
            if( evt )
                evt->SetScheduled();
            mScheduledQueue.Push( cmd );
            cmd->Execute( mPool );
        } else {
            mIdleQueue.Push( cmd );
        }
    }
}

void
FCommandQueue::Finish()
{
    Flush();
    Fence();
}

void
FCommandQueue::Fence()
{
    mPool.WaitForCompletion();
    CleanseScheduled();
}

void
FCommandQueue::Push( FCommand* iCommand )
{
    mIdleQueue.Push( iCommand );
    if( iCommand->Policy().FlowPolicy() == eScheduleFlowPolicy::ScheduleFlow_Blocking )
        Finish();
}

void
FCommandQueue::CleanseIdle()
{
    while( !mIdleQueue.IsEmpty() )
    {
        FCommand* cmd = mIdleQueue.Front();
        delete  cmd;
        mIdleQueue.Pop();
    }
}

void
FCommandQueue::CleanseScheduled()
{
    while( !mScheduledQueue.IsEmpty() )
    {
        FCommand* cmd = mScheduledQueue.Front();
        delete  cmd;
        mScheduledQueue.Pop();
    }
}

ULIS_NAMESPACE_END

