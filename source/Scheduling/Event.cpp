// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Event.cpp
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides definition for the FEvent class.
* @license      Please refer to LICENSE.md
*/
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FEvent
FEvent::~FEvent()
{
    delete  d;
}

FEvent::FEvent(
    const FOnEventComplete& iOnEventComplete
)
    : d( new FEvent_Private( iOnEventComplete ) )
{
}

FEvent::FEvent( const FEvent& iOther )
    : d( new FEvent_Private( *iOther.d ) )
{
}

FEvent&
FEvent::operator=( const FEvent& iOther )
{
    *d = *iOther.d;
    return  *this;
}

eEventStatus
FEvent::Status() const
{
    return  d->Status();
}

void
FEvent::Wait() const
{
    d->Wait();
}

//static
FEvent
FEvent::NoOP()
{
    FEvent ev;
    ev.d->m->NotifyAllJobsFinished();
    return  ev;
}

ULIS_NAMESPACE_END

