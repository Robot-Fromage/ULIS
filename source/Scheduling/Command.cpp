// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Command.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FCommand class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/Command.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN

FCommand::~FCommand()
{
    if( mArgs )
        delete  mArgs;

    for( uint64 i = 0; i < mJobs.Size(); ++i )
        delete  mJobs[i];
}

FCommand::FCommand(
      fpCommandScheduler iSched
    , const ICommandArgs* iArgs
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
    : mArgs( iArgs )
    , mEvent( nullptr )
    , mJobs( TArray< FJob* >() )
{
    // Bind Event
    if( iEvent )
    {
        mEvent = iEvent->d->m;
        ULIS_ASSERT( !( mEvent->IsCommandBound() ), "Cannot reuse an event that is already bound to a command" );
        mEvent->BuildWaitList( iNumWait, iWaitList );
        mEvent->BindCommand( this );
    }
    else
    {
        mEvent = FInternalEvent::Make();
        mEvent->BindCommand( this );
    }

    // Start Enqueuing Jobs
    iSched( this, iPolicy );
}

const ICommandArgs*
FCommand::Args() const
{
    return  mArgs;
}

void
FCommand::AddJob( FJob* iJob )
{
    mJobs.EmplaceBack( iJob );
}

bool
FCommand::IsReady() const
{
    return  mEvent->IsReady();
}

FSharedInternalEvent
FCommand::Event() const
{
    return  mEvent;
}

void
FCommand::Execute( FThreadPool& iPool ) const
{
    for( uint64 i = 0; i < mJobs.Size(); ++i )
        iPool.ScheduleJob( mJobs[i] );
}

ULIS_NAMESPACE_END

