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
#pragma once
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
    : mWaitList( TArray< FSharedInternalEvent >() )
    , mCommand( nullptr )
    , mStatus( eEventStatus::EventStatus_Idle )
    , mNumJobsRemaining( UINT64_MAX )
    , mGeometry( FRectI() )
    , mOnEventComplete( iOnEventComplete )
{
}

//static
FSharedInternalEvent
FInternalEvent::Make(
    const FOnEventComplete& iOnEventComplete
)
{
    return  std::make_shared< FInternalEvent >( iOnEventComplete );
}

const TArray< FSharedInternalEvent >&
FInternalEvent::WaitList() const
{
    return  mWaitList;
}

void
FInternalEvent::BuildWaitList( uint32 iNumWait, const FEvent* iWaitList )
{
    mWaitList.Reserve( iNumWait );
    for( uint32 i = 0; i < iNumWait; ++i )
        mWaitList.PushBack( iWaitList[i].d->m );

#ifdef ULIS_ASSERT_ENABLED
    CheckCyclicSelfReference();
#endif // ULIS_ASSERT_ENABLED
}

bool
FInternalEvent::IsBound() const
{
    return  mCommand != nullptr;
}

bool
FInternalEvent::IsReady() const
{
    for( uint32 i = 0; i < mWaitList.Size(); ++i )
        if( mWaitList[i]->Status() != eEventStatus::EventStatus_Finished )
            return  false;

    return  true;
}

void
FInternalEvent::CheckCyclicSelfReference() const
{
    CheckCyclicSelfReference_imp( this );
}

void
FInternalEvent::CheckCyclicSelfReference_imp( const FInternalEvent* iPin ) const
{
    for( uint32 i = 0; i < mWaitList.Size(); ++i )
    {
        FInternalEvent* pin = mWaitList[i].get();
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
    mNumJobsRemaining = mCommand->NumJobs();
    mGeometry = iGeometry;
}

void
FInternalEvent::NotifyOneJobFinished()
{
    --mNumJobsRemaining;
    if( mNumJobsRemaining == 0 ) {
        delete  mCommand;
        SetStatus( eEventStatus::EventStatus_Finished );
        mOnEventComplete.ExecuteIfBound( mGeometry );
    }
}

ULIS_NAMESPACE_END

