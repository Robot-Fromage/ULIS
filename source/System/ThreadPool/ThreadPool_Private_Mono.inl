// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool_Private_Mono.inl
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides the definition for the FThreadPool_Private
*               class for systems without actual thread support.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/ThreadPool/ThreadPool_Private_Mono.h"

ULIS_NAMESPACE_BEGIN
FThreadPool_Private::~FThreadPool_Private()
{
}

FThreadPool_Private::FThreadPool_Private( uint32 iNumWorkers )
{
}

void
FThreadPool_Private::ScheduleCommands( TQueue< const FCommand* >& ioCommands )
{
    //What we do here is just enqueue the commands when they are ready
    //If they are already ready, FOnEventReady will be immediately fired
    //If not, it will be fired later
    //Emptying the queue is ok, as the command still exist and is owned by the Event
    uint32 size = static_cast< uint32 >( ioCommands.Size() );
    TQueue< const FCommand* > scheduledCommands;
    while( !ioCommands.IsEmpty() )
    {
        const FCommand* cmd = ioCommands.Front();
        ioCommands.Pop();
        cmd->Event()->SetOnEventReady(
            FOnEventReady(
                [this, &scheduledCommands, cmd]()
                {
                    scheduledCommands.Push(cmd);
                }
            )
        );
    }

    while( !scheduledCommands.IsEmpty() )
    {
        const FCommand* cmd = scheduledCommands.Front();
        scheduledCommands.Pop();
        const TArray< const FJob* >& jobs = cmd->Jobs();
        const uint64 size = jobs.Size();
        for( uint64 i = 0; i < size; ++i ) {
            jobs[i]->Execute();
        }
    }
}

void
FThreadPool_Private::WaitForCompletion()
{
}

void
FThreadPool_Private::SetNumWorkers( uint32 iNumWorkers )
{
}

uint32
FThreadPool_Private::GetNumWorkers() const
{
    return  1;
}

//static
uint32
FThreadPool_Private::MaxWorkers()
{
    return  1;
}

ULIS_NAMESPACE_END

