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
    , const FTaskEvent* iWaitList
    , FTaskEvent* iEvent
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
    return  true;
}

void
FCommand::Execute()
{
    mSched( mArgs );
}

ULIS_NAMESPACE_END

