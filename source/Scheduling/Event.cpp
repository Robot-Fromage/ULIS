// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Event.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FEvent class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Safety Checks for Events
void EventHookCheck_imp( const FEvent* iEvent )
{
    if( iEvent )
        ULIS_ASSERT( !( iEvent->d->Hooked() ), "Reusing an event multiple times is illegal and will lead to corrupted concurrency state." );
}

void EventSelfCheck_imp( uint32 iNumWait, const FEvent* iWaitList, const FEvent* iEvent )
{
    if( iNumWait )
    {
        for( uint32 i = 0; i < iNumWait; ++i )
        {
            ULIS_ASSERT( &iWaitList[i] != iEvent, "Event waiting for itself is illegal and will lead to an infinite loop." );
        }
    }
}

/////////////////////////////////////////////////////
// FEvent
FEvent::~FEvent()
{
    delete  d;
}

FEvent::FEvent()
    : d( new FEvent_Private() )
{
}

eEventStatus
FEvent::Status() const
{
    return  d->Status();
}

ULIS_NAMESPACE_END

