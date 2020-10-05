// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         CommandQueue_Private.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FCommandQueue_Private class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/CommandQueue_Private.h"
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN
FCommandQueue::FCommandQueue_Private::~FCommandQueue_Private()
{
    while( !mQueue.IsEmpty() )
    {
        FCommand* cmd = mQueue.Front();
        delete  cmd;
        mQueue.Pop();
    }

    Finish();
}

FCommandQueue::FCommandQueue_Private::FCommandQueue_Private( FThreadPool& iPool )
    : mQueue( tQueue() )
    , mPool( iPool )
{
}

void
FCommandQueue::FCommandQueue_Private::Flush()
{
    while( !mQueue.IsEmpty() )
    {
        FCommand* cmd = mQueue.Front();
        mQueue.Pop();

        if( cmd->IsReady() ) {
            FEvent* evt = cmd->Event();
            if( evt )
                evt->SetScheduled();
            mScheduledQueue.Push( cmd );
            cmd->Execute( mPool );
        } else {
            mQueue.Push( cmd );
        }
    }
}

void
FCommandQueue::FCommandQueue_Private::Finish()
{
    Flush();
    Fence();
}

void
FCommandQueue::FCommandQueue_Private::Fence()
{
    mPool.WaitForCompletion();
}

void
FCommandQueue::FCommandQueue_Private::Push( FCommand* iCommand )
{
    ULIS_ASSERT( iCommand, "Error: no input command" );
    mQueue.Push( iCommand );
    if( iCommand->Policy().FlowPolicy() == eScheduleFlowPolicy::ScheduleFlow_Blocking )
        Finish();
}

ULIS_NAMESPACE_END

