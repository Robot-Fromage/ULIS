// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Event_Private.cpp
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides definition for the FEvent_Private class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FEvent::FEvent_Private
FEvent_Private::FEvent_Private(
    const FOnEventComplete& iOnEventComplete
)
    : m( FInternalEvent::MakeShared( iOnEventComplete ) )
{
}

eEventStatus
FEvent_Private::Status() const
{
    return  m->Status();
}

void
FEvent_Private::Wait() const
{
    m->Wait();
}

ULIS_NAMESPACE_END

