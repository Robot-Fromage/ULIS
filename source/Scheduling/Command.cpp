// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Command.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FCommand class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Scheduling/Command.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Scheduling/Job.h"
#include "System/ThreadPool.h"

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
    , bool iContiguous
    , bool iForceMonoChunk
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
    , const FRectI& iEventGeometry
)
    : mArgs( iArgs )
    , mEvent( nullptr )
    , mJobs( TArray< const FJob* >() )
{
    // Bind Event
    if( iEvent ) {
        mEvent = iEvent->d->m;
        ULIS_ASSERT( !( mEvent->IsBound() ), "Cannot reuse an event that is already bound to a command" );
    } else {
        mEvent = FInternalEvent::MakeShared();
    }

    // Start Enqueuing Jobs
    iSched( this, iPolicy, iContiguous, iForceMonoChunk );

    mEvent->Bind( this, iNumWait, iWaitList, iEventGeometry );
}

const ICommandArgs*
FCommand::Args() const
{
    return  mArgs;
}

void
FCommand::ReserveJobs( uint64 iNum )
{
    mJobs.Reserve( iNum );
}

void
FCommand::AddJob( const FJob* iJob )
{
    mJobs.EmplaceBack( iJob );
}

uint64
FCommand::NumJobs() const
{
    return  mJobs.Size();
}

bool
FCommand::ReadyForProcessing() const
{
    return  mEvent->ReadyForProcessing();
}

bool
FCommand::ReadyForScheduling() const
{
    return  mEvent->ReadyForScheduling();
}

FSharedInternalEvent
FCommand::Event() const
{
    return  mEvent;
}

const TArray< const FJob* >&
FCommand::Jobs() const
{
    return  mJobs;
}

ULIS_NAMESPACE_END

