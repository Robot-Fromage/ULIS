// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Event.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the FEvent class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Core/Callback.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// eEventStatus
enum eEventStatus : uint8
{
      EventStatus_Idle
    , EventStatus_Queued
    , EventStatus_Finished
};

/////////////////////////////////////////////////////
/// @class      FEvent
/// @brief      The FEvent class provides a way to get asynchronous status
///             information about how a task is being processed, once it has been
///             dispatched and scheduled on a multithreaded system, in
///             coordination with a FThreadPool and a FCommandQueue.
/// @details    The FEvent allows to get a handle of the chosen policy for
///             a given task, and wether it has been completed or not. It is used
///             in conjunction with FThreadPool, FSchedulePolicy, FCommandQueue
///             and FContext.
///
///             \sa FContext
///             \sa FSchedulePolicy
///             \sa FThreadPool
///             \sa FHardwareMetrics
///             \sa FCommandQueue
class ULIS_API FEvent
{
    class FEvent_Private;
    friend class FInternalEvent;
    friend class FContext;
    friend class FCommand;

public:
    /*! Destructor */
    ~FEvent();

    /*! Constructor */
    FEvent( const FOnEventComplete& iOnEventComplete = FOnEventComplete() );

    FEvent( const FEvent& ) = delete;
    FEvent& operator=( const FEvent& ) = delete;

private:
    FEvent_Private* d;
    FOnEventComplete mOnEventComplete;
};

ULIS_NAMESPACE_END

