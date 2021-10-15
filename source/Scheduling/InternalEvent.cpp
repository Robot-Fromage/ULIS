// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         InternalEvent.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FInternalEvent class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Scheduling/InternalEvent.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/Command.h"

#define USE_LOCK

ULIS_NAMESPACE_BEGIN
FInternalEvent::~FInternalEvent()
{
}

FInternalEvent::FInternalEvent(
    const FOnEventComplete& iOnEventComplete
)
    : mChildren()
#ifndef USE_LOCK
    , mChildrenStatus(ChildrenStatus_Unlocked)
#endif
    , mCommand( nullptr )
    , mStatus( eEventStatus::EventStatus_Idle )
    , mGeometry( FRectI() )
    , mOnEventComplete(iOnEventComplete)
    , mParentUnfinished(0)
    , mStatusFinishedMutex()
{
}

//static
FSharedInternalEvent
FInternalEvent::MakeShared(
    const FOnEventComplete& iOnEventComplete
)
{
    return  std::make_shared< FInternalEvent >( iOnEventComplete );
}

void
FInternalEvent::BuildWaitList( uint32 iNumWait, const FEvent* iWaitList )
{
#ifdef ULIS_ASSERT_ENABLED
    ULIS_ASSERT( Status() == eEventStatus::EventStatus_Idle, "Reuse of an already used event." );
#endif // ULIS_ASSERT_ENABLED

    // Initialized to 1 instead of 0 to have fake parent.
    // That fake parent will then be removed in SetOnEventReady().
    // Avoids usage of mutexes in SetOnEventReady() and OnParentEventComplete().
    // relaxed : because we just increase the counter, but we don't ensure any memory to be shared in other threads
    mParentUnfinished.fetch_add(iNumWait + 1, ::std::memory_order_relaxed);

    //Add this event as a child of each event in the WaitList
    for( uint32 i = 0; i < iNumWait; ++i )
    {
        iWaitList[i].d->m->AddChild(shared_from_this());
    }

#ifdef ULIS_ASSERT_ENABLED
    CheckCyclicSelfReference();
#endif // ULIS_ASSERT_ENABLED
}

void
FInternalEvent::AddChild(FSharedInternalEvent iChild)
{
#ifdef USE_LOCK
    //lock the write to mChildren and the read to mStatus because those are not thread-safe actions
    std::unique_lock<std::mutex> lock (mStatusFinishedMutex);
    
    //If this event is not marked as finished, we can safely add the child and let the parent handle it
    if (mStatus != eEventStatus::EventStatus_Finished)
    {
        mChildren.push_back(iChild);
        return;
    }

    //If this event is already marked as finished, we cannot add the child, we just need to inform the child that its parent has finished
    //Thus, we can unlock
    lock.unlock();

    iChild->OnParentEventComplete();
#else
    eChildrenStatus expectedValue = ChildrenStatus_Unlocked;
    if (mChildrenStatus.compare_exchange_strong(expectedValue, ChildrenStatus_Locked, ::std::memory_order_acquire, ::std::memory_order_relaxed ))
    {
        //std::lock_guard<std::mutex> lock (mStatusFinishedMutex);
        //if (mStatus != eEventStatus::EventStatus_Finished)
        //{
        mChildren.push_back(iChild);
        mChildrenStatus.store(ChildrenStatus_Unlocked, ::std::memory_order_release);
        return;
        //}
    }
    iChild->OnParentEventComplete();
#endif
}

void
FInternalEvent::OnParentEventComplete()
{   
    //Decrement the parent unfinished count
    //if the count was 1, then there is no more parent to wait on
    //we can then execute EventReady callback
    //We need to acquire here, because we need to ensure all parents correctly finished
    //We need to release here, because we need to ensure other threads what this thread did has been done
    //And in the case where OnParentEventComplete() has been called from SetOnInternalEventReady()
    //we also need to ensure mOnInternalEventReady to be set
    //Thus, we can use acq_rel to perform acquire and release
    if (mParentUnfinished.fetch_sub(1, std::memory_order_acq_rel) == 1)
    {
        mOnInternalEventReady.ExecuteIfBound(this);
    }
}

void
FInternalEvent::SetOnInternalEventReady(FOnInternalEventReady iOnEventReady)
{
    //Set the event ready callback and remove the fake parent by calling OnParentEventComplete()
    mOnInternalEventReady = iOnEventReady;
    OnParentEventComplete();
}

bool
FInternalEvent::IsBound() const
{
    return  mCommand != nullptr;
}

#ifdef ULIS_ASSERT_ENABLED
void
FInternalEvent::CheckCyclicSelfReference() const
{
    CheckCyclicSelfReference_imp( this );
}

void
FInternalEvent::CheckCyclicSelfReference_imp( const FInternalEvent* iPin ) const
{
    for( uint32 i = 0; i < mChildren.size(); ++i )
    {
        FInternalEvent* pin = mChildren[i].get();
        ULIS_ASSERT( pin != iPin, "Bad self reference in wait list." );
        pin->CheckCyclicSelfReference_imp( iPin );
    }
}
#endif

eEventStatus
FInternalEvent::Status() const
{
    return  mStatus;
}

void
FInternalEvent::Bind( FCommand* iCommand, uint32 iNumWait, const FEvent* iWaitList, const FRectI& iGeometry )
{
    ULIS_ASSERT( !IsBound(), "Event already bound !" )
    mCommand = iCommand;
    mGeometry = iGeometry;
    BuildWaitList( iNumWait, iWaitList );
}

void
FInternalEvent::NotifyAllJobsFinished()
{
#ifdef USE_LOCK
    //lock the write to mStatus as it can be read concurrently by AddChild()
    //it also ensures that when we are out of this lock, mChildren contains exactly what we need
    std::unique_lock<std::mutex> lock (mStatusFinishedMutex);
    mStatus = eEventStatus::EventStatus_Finished;
    lock.unlock(); //unlock because, after status is set to finished, no one is adding any child again, so we can access it safely
#else
    eChildrenStatus expectedValue = ChildrenStatus_Unlocked;
    while (!mChildrenStatus.compare_exchange_weak(expectedValue, ChildrenStatus_Finished, ::std::memory_order_acquire, ::std::memory_order_relaxed))
    {
    }
    mStatus = eEventStatus::EventStatus_Finished;
#endif

    //
    for (int i = 0; i < mChildren.size(); i++)
    {
        mChildren[i]->OnParentEventComplete(); //inform the child that this event is finished
        mChildren[i] = nullptr; //release the child, because we don't need it in this case anymore
    }
    mOnEventComplete.ExecuteIfBound( mGeometry );
}

void
FInternalEvent::NotifyQueued()
{
    mStatus = eEventStatus::EventStatus_Queued;
}

void
FInternalEvent::Wait() const
{
    while( mStatus != eEventStatus::EventStatus_Finished )
    {
    }
}

const FCommand*
FInternalEvent::Command() const
{
    return mCommand;
}

ULIS_NAMESPACE_END

