// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         TaskEvent.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the FTaskEvent class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/SchedulePolicy.h"

ULIS_NAMESPACE_BEGIN
enum eTaskStatus : uint8 {
      TaskStatus_Idle
    , TaskStatus_Scheduled
    , TaskStatus_Processing
    , TaskStatus_Finished
};

/////////////////////////////////////////////////////
/// @class      FTaskEvent
/// @brief      The FTaskEvent class provides a way to get asynchronous status
///             information about how a task is being processed, once it has been
///             dispatched and scheduled on a multithreaded system, in
///             coordination with a FOldThreadPool and a FCommandQueue.
/// @details    The FTaskEvent allows to get a handle of the chosen policy for
///             a given task, and wether it has been completed or not. It is used
///             in conjunction with FOldThreadPool, FSchedulePolicy, FCommandQueue
///             and FRasterContext.
///
///             \sa FRasterContext
///             \sa FSchedulePolicy
///             \sa FOldThreadPool
///             \sa FDevice
///             \sa FCommandQueue
class ULIS_API FTaskEvent
{
public:
    /*! Destructor */
    ~FTaskEvent();

    /*! Constructor */
    FTaskEvent();

    /*! Getter for the task policy */
    const FSchedulePolicy& Policy() const;

private:
    FSchedulePolicy mPolicy;
    eTaskStatus     mStatus;
};

ULIS_NAMESPACE_END

