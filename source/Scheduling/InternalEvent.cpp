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

ULIS_NAMESPACE_BEGIN
FInternalEvent::~FInternalEvent()
{
}

FInternalEvent::FInternalEvent(
    const FOnEventComplete& iOnEventComplete
)
    : mChildren( TArray< FSharedInternalEvent >() )
    , mCommand( nullptr )
    , mStatus( eEventStatus::EventStatus_Idle )
    , mNumJobsRemaining( UINT64_MAX )
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
    mParentUnfinished = iNumWait;
    //mWaitList.Reserve( iNumWait );
    for( uint32 i = 0; i < iNumWait; ++i )
    {
        //mWaitList.PushBack( iWaitList[i].d->m );
        iWaitList[i].d->m->AddChild(shared_from_this());
    }

#ifdef ULIS_ASSERT_ENABLED
    CheckCyclicSelfReference();
#endif // ULIS_ASSERT_ENABLED
}

void
FInternalEvent::AddChild(FSharedInternalEvent iChild)
{
    std::unique_lock<std::mutex> lock (mStatusFinishedMutex);
    if (mStatus == eEventStatus::EventStatus_Finished)
    {
        iChild->OnParentEventComplete();
        mChildren.PushBack(nullptr); //just for consistency of data, not really needed
    }
    else
    {
        mChildren.PushBack(iChild);
    }
}

void
FInternalEvent::OnParentEventComplete()
{   
    std::unique_lock<std::mutex> lock (mEventReadyMutex);
    --mParentUnfinished;
    if (mParentUnfinished == 0)
    {
        mOnEventReady.ExecuteIfBound();
    }
}

void
FInternalEvent::SetOnEventReady(FOnEventReady iOnEventReady)
{
    std::unique_lock<std::mutex> lock (mEventReadyMutex);
    mOnEventReady = iOnEventReady;
    if (mParentUnfinished == 0)
    {
        mOnEventReady.ExecuteIfBound();
    }
}

bool
FInternalEvent::IsBound() const
{
    return  mCommand != nullptr;
}

void
FInternalEvent::CheckCyclicSelfReference() const
{
    CheckCyclicSelfReference_imp( this );
}

void
FInternalEvent::CheckCyclicSelfReference_imp( const FInternalEvent* iPin ) const
{
    for( uint32 i = 0; i < mChildren.Size(); ++i )
    {
        FInternalEvent* pin = mChildren[i].get();
        ULIS_ASSERT( pin != iPin, "Bad self reference in wait list." );
        pin->CheckCyclicSelfReference_imp( iPin );
    }
}

void
FInternalEvent::SetStatus( eEventStatus iStatus )
{
    mStatus = iStatus;
}

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
    mNumJobsRemaining.store(mCommand->NumJobs(), std::memory_order_release);
    mGeometry = iGeometry;
}

void
FInternalEvent::PostBindAsync()
{
    mNumJobsRemaining.store(mCommand->NumJobs(), std::memory_order_release);
}

bool
FInternalEvent::NotifyOneJobFinished()
{
    uint64 count = mNumJobsRemaining.fetch_sub(1, std::memory_order_release);
    if( count == 1 ) {
        NotifyAllJobsFinished();
        return  true;
    }
    return  false;
}

void
FInternalEvent::NotifyAllJobsFinished()
{
    {//scope lock
        std::unique_lock<std::mutex> lock (mStatusFinishedMutex);
        SetStatus( eEventStatus::EventStatus_Finished );
        if (mStatus == eEventStatus::EventStatus_Finished)
        {
            for (int i = 0; i < mChildren.Size(); i++)
            {
                mChildren[i]->OnParentEventComplete();
                mChildren[i] = nullptr;
            }
        }
    }
    mOnEventComplete.ExecuteIfBound( mGeometry );

    if (mCommand)
    {
        delete  mCommand;
        mCommand = nullptr;
    }
}

void
FInternalEvent::NotifyQueued()
{
    SetStatus( eEventStatus::EventStatus_Queued );
}

void
FInternalEvent::Wait() const
{
    while( mStatus != eEventStatus::EventStatus_Finished )
    {
    }
}

ULIS_NAMESPACE_END

