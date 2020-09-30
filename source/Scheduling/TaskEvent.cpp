// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         TaskEvent.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FTaskEvent class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/TaskEvent.h"

ULIS_NAMESPACE_BEGIN
FTaskEvent::~FTaskEvent()
{
    // Nothing ATM
}

FTaskEvent::FTaskEvent()
    : mStatus( TaskStatus_Idle )
{
    // Nothing ATM
}

eTaskStatus
FTaskEvent::Status() const
{
    return  mStatus;
}

void
FTaskEvent::SetScheduled()
{
    mStatus = eTaskStatus::TaskStatus_Scheduled;
}

void
FTaskEvent::SetFinished()
{
    mStatus = eTaskStatus::TaskStatus_Finished;
}

ULIS_NAMESPACE_END

