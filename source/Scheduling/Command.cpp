// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
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

ULIS_NAMESPACE_BEGIN

FCommand::~FCommand()
{
    if( mArgs )
        delete  mArgs;
}

FCommand::FCommand(
      fpCommandScheduler iSched
    , const ICommandArgs* iArgs
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
    : mSched( iSched )
    , mArgs( iArgs )
    , mEvent( nullptr )
{
    // Bind Event
    if( iEvent )
    {
        mEvent = iEvent->d->m;
        ULIS_ASSERT( mEvent->IsCommandBound(), "Cannot reuse an event that is already bound to a command" );
        mEvent->BuildWaitList( iNumWait, iWaitList );
        mEvent->BindCommand( this );
    }
    else
    {
        mEvent = FInternalEvent::Make();
        mEvent->BindCommand( this );
    }

    // Start Scheduling
    //mSched( mArgs, iPolicy, iPool );
}

const ICommandArgs*
FCommand::Args() const
{
    return  mArgs;
}

ULIS_NAMESPACE_END

