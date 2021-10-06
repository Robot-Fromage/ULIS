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
    , mChildrenStatus(ChildrenStatus_Unlocked)
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

    /*  Initialized to 1 instead of 0 to have fake parent.
        That fake parent will then be removed in SetOnEventReady().
        Avoids usage of mutexes in SetOnEventReady() and OnParentEventComplete().
    */
    mParentUnfinished.store(iNumWait + 1, ::std::memory_order_relaxed);
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
    {
        std::lock_guard<std::mutex> lock (mStatusFinishedMutex);
        if (mStatus != eEventStatus::EventStatus_Finished)
        {
            mChildren.push_back(iChild);
            return;
        }
    }
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
    uint64 numParentUnfinished = mParentUnfinished.fetch_sub(1, std::memory_order_relaxed);
    if (numParentUnfinished == 1)
    {
        mOnInternalEventReady.ExecuteIfBound(this);
    }
}

void
FInternalEvent::SetOnInternalEventReady(FOnInternalEventReady iOnEventReady)
{
    mOnInternalEventReady = iOnEventReady;
    OnParentEventComplete(); //remove fake parent
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
    BuildWaitList( iNumWait, iWaitList );
    mGeometry = iGeometry;
}

void
FInternalEvent::NotifyAllJobsFinished()
{
#ifdef USE_LOCK
    {//scope lock
        std::lock_guard<std::mutex> lock (mStatusFinishedMutex);
        mStatus = eEventStatus::EventStatus_Finished;
    }

#else
    eChildrenStatus expectedValue = ChildrenStatus_Unlocked;
    while (!mChildrenStatus.compare_exchange_weak(expectedValue, ChildrenStatus_Finished, ::std::memory_order_acquire, ::std::memory_order_relaxed))
    {
    }
    mStatus = eEventStatus::EventStatus_Finished;
#endif

    for (int i = 0; i < mChildren.size(); i++)
    {
        mChildren[i]->OnParentEventComplete();
        mChildren[i] = nullptr;
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

