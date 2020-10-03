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
///             and FRasterContext.
///
///             \sa FRasterContext
///             \sa FSchedulePolicy
///             \sa FOldThreadPool
///             \sa FHardwareMetrics
///             \sa FCommandQueue
class FEvent::FEvent_Private
{
public:
    FEvent_Private();
    bool Hooked() const;
    void Hook( std::shared_ptr< FInternalEvent > iHandle );
    eEventStatus Status() const;

private:
    std::shared_ptr< FInternalEvent > mHandle;
};

ULIS_NAMESPACE_END

