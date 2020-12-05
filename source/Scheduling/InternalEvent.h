// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         InternalEvent.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the FInternalEvent class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/Array.h"
#include "Scheduling/Event.h"
#include <memory>
#include <chrono>

ULIS_NAMESPACE_BEGIN
class FInternalEvent;
typedef std::shared_ptr< FInternalEvent > FSharedInternalEvent;

/////////////////////////////////////////////////////
/// @class      FInternalEvent
/// @brief      The FInternalEvent class provides a way to get asynchronous status
///             information about how a task is being processed, once it has been
///             dispatched and scheduled on a multithreaded system, in
///             coordination with a FThreadPool and a FCommandQueue.
/// @details    The FInternalEvent allows to get a handle of the chosen policy for
///             a given task, and wether it has been completed or not. It is used
///             in conjunction with FThreadPool, FSchedulePolicy, FCommandQueue
///             and FContext.
///
///             \sa FContext
///             \sa FSchedulePolicy
///             \sa FThreadPool
///             \sa FHardwareMetrics
///             \sa FCommandQueue
class FInternalEvent
{
    friend class std::_Ref_count_obj< FInternalEvent >;

public:
    /*! Destructor */
    ~FInternalEvent();

private:
    /*! Constructor */
    FInternalEvent();

public:
    static FSharedInternalEvent Make();
    const TArray< FSharedInternalEvent >& WaitList() const;
    void BuildWaitList( uint32 iNumWait, const FEvent* iWaitList );
    bool IsCommandBound() const;
    void BindCommand( FCommand* iCommand );
    void CheckCyclicSelfReference() const;
    void SetStatus( eEventStatus iStatus );
    eEventStatus Status() const;
    bool IsReady() const;

private:
    void CheckCyclicSelfReference_imp( const FInternalEvent* iPin ) const;

private:
    TArray< FSharedInternalEvent > mWaitList;
    FCommand* mCommand;
    uint32 mNumJobsRemaining;
    uint32 mStartTime;
    uint32 mEndTime;
    eEventStatus mStatus;
};

ULIS_NAMESPACE_END

