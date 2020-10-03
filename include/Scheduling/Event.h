// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Event.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the FEvent class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// eEventStatus
enum eEventStatus : uint8
{
      EventStatus_Error
    , EventStatus_Queued
    , EventStatus_Scheduled
    , EventStatus_Processing
    , EventStatus_Finished
};


/////////////////////////////////////////////////////
// Safety Checks for Events
void EventHookCheck_imp( const FEvent* iEvent );
void EventSelfCheck_imp( uint32 iNumWait, const FEvent* iWaitList, const FEvent* iEvent );

#if defined( ULIS_DEBUG )
#define EventHookCheck( EV ) EventHookCheck_imp( EV );
#define EventSelfCheck( NUM, LIST, EV ) EventSelfCheck_imp( NUM, LIST, EV );
#else
#define EventHookCheck( EV )
#define EventSelfCheck( NUM, LIST, EV )
#endif
#define EventChecks( NUM, LIST, EV ) EventHookCheck( EV ); EventSelfCheck( NUM, LIST, EV );

/////////////////////////////////////////////////////
/// @class      FEvent
/// @brief      The FEvent class provides a way to get asynchronous status
///             information about how a task is being processed, once it has been
///             dispatched and scheduled on a multithreaded system, in
///             coordination with a FOldThreadPool and a FCommandQueue.
/// @details    The FEvent allows to get a handle of the chosen policy for
///             a given task, and wether it has been completed or not. It is used
///             in conjunction with FOldThreadPool, FSchedulePolicy, FCommandQueue
///             and FRasterContext.
///
///             \sa FRasterContext
///             \sa FSchedulePolicy
///             \sa FOldThreadPool
///             \sa FHardwareMetrics
///             \sa FCommandQueue
class ULIS_API FEvent
{
    class FEvent_Private;
    friend class FRasterContext;
    friend class FInternalEvent;
    friend void EventHookCheck_imp( const FEvent* iEvent );

public:
    /*! Destructor */
    ~FEvent();

    /*! Constructor */
    FEvent();

    /*! Getter for status */
    eEventStatus Status() const;

private:
    FEvent_Private* d;
};

ULIS_NAMESPACE_END

