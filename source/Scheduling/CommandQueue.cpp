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
    while( !mQueue.IsEmpty() )
    {
        FCommand* cmd = mQueue.Front();
        delete  cmd;
        mQueue.Pop();
    }
}

FCommandQueue::FCommandQueue( FThreadPool& iPool )
    : mQueue( tQueue() )
    , mPool( iPool )
{
}

void
FCommandQueue::Flush()
{
    while( !mQueue.IsEmpty() )
    {
        FCommand* cmd = mQueue.Front();

        FTaskEvent* evt = cmd->Event();
        if( evt )
            evt->SetScheduled();

        mQueue.Pop();
        mPool.ScheduleJob( cmd );
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
}

void
FCommandQueue::Push( FCommand* iCommand )
{
    mQueue.Push( iCommand );
    if( iCommand->Policy().FlowPolicy() == eScheduleFlowPolicy::ScheduleFlow_Blocking )
        Finish();
}

ULIS_NAMESPACE_END

