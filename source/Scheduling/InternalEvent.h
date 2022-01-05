// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         InternalEvent.h
* @author       Clement Berthaud && Eric Scholl
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
#include "ThirdParty/ConcurrentQueue/concurrentqueue.h"

#include <atomic>
#include <mutex>

ULIS_NAMESPACE_BEGIN
class FInternalEvent;
class FCommand;
typedef std::shared_ptr< FInternalEvent > FSharedInternalEvent;
typedef TLambdaCallback< void, const FInternalEvent* > FOnInternalEventComplete;
typedef TLambdaCallback< void, const FInternalEvent* > FOnInternalEventReady;

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
///             \sa FCPUInfo
///             \sa FCommandQueue
class FInternalEvent
    : public std::enable_shared_from_this<FInternalEvent>
{
    friend class FEvent;

public:
    /*! Destructor */
    ~FInternalEvent();

    /*! Constructor */
    FInternalEvent( const FOnEventComplete& iOnEventComplete = FOnEventComplete() );

public:
    static FSharedInternalEvent MakeShared( const FOnEventComplete& iOnEventComplete = FOnEventComplete() );
    bool IsBound() const;
#ifdef ULIS_ASSERT_ENABLED
    void CheckCyclicSelfReference() const;
#endif
    eEventStatus Status() const;
    void Bind( FCommand* iCommand, uint32 iNumWait, const FEvent* iWaitList, const FRectI& iGeometry );
    // void PostBindAsync();
    // bool NotifyOneJobFinished();
    void NotifyAllJobsFinished();
    void NotifyQueued();
    void Wait() const;

    void SetOnInternalEventReady(FOnInternalEventReady iOnEventReady);
    // void SetOnInternalEventComplete(FOnInternalEventComplete iOnEventComplete);

    const FCommand* Command() const;

private:
    void BuildWaitList( uint32 iNumWait, const FEvent* iWaitList );
#ifdef ULIS_ASSERT_ENABLED
    void CheckCyclicSelfReference_imp( const FInternalEvent* iPin ) const;
#endif
    void OnParentEventComplete();
    void AddChild(FSharedInternalEvent iChild);

private:
    enum eChildrenStatus : uint8
    {
        ChildrenStatus_Unlocked,
        ChildrenStatus_Locked,
        ChildrenStatus_Finished
    };

private:
    std::vector< FSharedInternalEvent > mChildren;
    //std::atomic<eChildrenStatus> mChildrenStatus;
    FCommand* mCommand;
    eEventStatus mStatus;
    FRectI mGeometry;
    FOnEventComplete mOnEventComplete;
    FOnInternalEventReady mOnInternalEventReady;
    std::atomic_uint64_t mParentUnfinished;
    std::mutex mStatusFinishedMutex;
    std::mutex mEventReadyMutex;
};

ULIS_NAMESPACE_END

