// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool_Private_Mono.inl
* @author       Clement Berthaud
* @brief        This file provides the definition for the FThreadPool_Private
*               class for systems without actual thread support.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/ThreadPool_Private_Mono.h"

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
    while( !ioCommands.IsEmpty() )
    {
        const FCommand* cmd = ioCommands.Front();
        ioCommands.Pop();

        ULIS_ASSERT( cmd->ReadyForScheduling(), "Bad queue state, waiting on events that are not scheduled will hang forever." );

        if( cmd->ReadyForProcessing() )
        {
            FSharedInternalEvent evt = cmd->Event();
            const TArray< const FJob* >& jobs = cmd->Jobs();
            for( uint64 i = 0; i < jobs.Size(); ++i ) {
                jobs[i]->Execute();
                evt->NotifyOneJobFinished();
            }
        }
        else
        {
            ioCommands.Push( cmd );
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

