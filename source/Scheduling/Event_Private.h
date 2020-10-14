// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Event_Private.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the FEvent_Private class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Event.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FEvent_Private
/// @brief      The FEvent_Private class provides a way to get asynchronous status
///             information about how a task is being processed, once it has been
///             dispatched and scheduled on a multithreaded system, in
///             coordination with a FOldThreadPool and a FCommandQueue.
/// @details    The FEvent_Private allows to get a handle of the chosen policy for
///             a given task, and wether it has been completed or not. It is used
///             in conjunction with FOldThreadPool, FSchedulePolicy, FCommandQueue
///             and FContext.
///
///             \sa FContext
///             \sa FSchedulePolicy
///             \sa FOldThreadPool
///             \sa FHardwareMetrics
///             \sa FCommandQueue
class FEvent::FEvent_Private
{
    friend class FInternalEvent;
    friend class FContext;
    friend class FCommand;

public:
    FEvent_Private();

private:
    FSharedInternalEvent m;
};

ULIS_NAMESPACE_END
