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
    , mPolicy( iPolicy )
    , mNumWait( iNumWait )
    , mWaitList( iWaitList )
    , mEvent( iEvent )
{
}

bool
FCommand::IsReady() const
{
    if( mNumWait == 0 )
        return  true;

    for( uint32 i = 0; i < mNumWait; ++i )
        if( mWaitList[i].Status() != eEventStatus::EventStatus_Finished )
            return  false;

    return  true;
}

void
FCommand::Execute( FThreadPool& iPool )
{
    mSched( mArgs, mPolicy, iPool );
}

const FSchedulePolicy&
FCommand::Policy() const
{
    return  mPolicy;
}

FEvent*
FCommand::Event() const
{
    return  mEvent;
}

const ICommandArgs*
FCommand::Args() const
{
    return  mArgs;
}

ULIS_NAMESPACE_END

