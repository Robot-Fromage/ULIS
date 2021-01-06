// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         InternalEvent.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the FInternalEvent class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Core/Callback.h"
#include "Memory/Array.h"
#include "Scheduling/Event.h"
#include "Math/Geometry/Rectangle.h"

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
    FInternalEvent( const FOnEventComplete& iOnEventComplete = FOnEventComplete() );

public:
    static FSharedInternalEvent Make( const FOnEventComplete& iOnEventComplete = FOnEventComplete() );
    const TArray< FSharedInternalEvent >& WaitList() const;
    bool IsBound() const;
    bool ReadyForProcessing() const;
    bool ReadyForScheduling() const;
    void CheckCyclicSelfReference() const;
    void SetStatus( eEventStatus iStatus );
    eEventStatus Status() const;
    void Bind( FCommand* iCommand, uint32 iNumWait, const FEvent* iWaitList, const FRectI& iGeometry );
    bool NotifyOneJobFinished();

private:
    void BuildWaitList( uint32 iNumWait, const FEvent* iWaitList );
    void CheckCyclicSelfReference_imp( const FInternalEvent* iPin ) const;

private:
    TArray< FSharedInternalEvent > mWaitList;
    FCommand* mCommand;
    eEventStatus mStatus;
    uint64 mNumJobsRemaining;
    FRectI mGeometry;
    FOnEventComplete mOnEventComplete;
};

ULIS_NAMESPACE_END

