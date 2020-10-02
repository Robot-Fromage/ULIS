// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         FRasterContext.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FRasterContext class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/RasterContext.h"
#include "Blend/BlendDispatch.h"
#include "Scheduling/CommandQueue.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Safety Checks for Events
void EventHookCheck_imp( const FEvent* iEvent )
{
    if( iEvent )
        ULIS_ASSERT( !( iEvent->Hooked() ), "Reusing an event multiple times is illegal and will lead to corrupted concurrency state." );
}

void EventSelfCheck_imp( uint32 iNumWait, const FEvent* iWaitList, const FEvent* iEvent )
{
    if( iNumWait )
    {
        for( uint32 i = 0; i < iNumWait; ++i )
        {
            ULIS_ASSERT( &iWaitList[i] != iEvent, "Event waiting for itself is illegal and will lead to an infinite loop." );
        }
    }
}

#if defined( ULIS_DEBUG )
#define EventHookCheck( EV ) EventHookCheck_imp( EV );
#define EventSelfCheck( NUM, LIST, EV ) EventSelfCheck_imp( NUM, LIST, EV );
#else
#define EventHookCheck( EV )
#define EventSelfCheck( NUM, LIST, EV )
#endif

/////////////////////////////////////////////////////
// FRasterContext::FContextualDispatchTable
struct FRasterContext::FContextualDispatchTable
{
    friend class FRasterContext;

public:
    /*! Constructor */
    FContextualDispatchTable( const FHardwareMetrics& iHardwareMetrics, eFormat iFormat )
        : mScheduleBlendSeparable(              TDispatcher< FDispatchedBlendSeparableInvocationSchedulerSelector >             ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleBlendNonSeparable(           TDispatcher< FDispatchedBlendNonSeparableInvocationSchedulerSelector >          ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleBlendMisc(                   TDispatcher< FDispatchedBlendMiscInvocationSchedulerSelector >                  ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleBlendSeparableSubpixel(      TDispatcher< FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector >     ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleBlendNonSeparableSubpixel(   TDispatcher< FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector >  ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleBlendMiscSubpixel(           TDispatcher< FDispatchedBlendMiscSubpixelInvocationSchedulerSelector >          ::Query( iHardwareMetrics, iFormat ) )
    {
    }

    /*! Destructor */
    ~FContextualDispatchTable()
    {}

private:
    const fpCommandScheduler mScheduleBlendSeparable;
    const fpCommandScheduler mScheduleBlendNonSeparable;
    const fpCommandScheduler mScheduleBlendMisc;
    const fpCommandScheduler mScheduleBlendSeparableSubpixel;
    const fpCommandScheduler mScheduleBlendNonSeparableSubpixel;
    const fpCommandScheduler mScheduleBlendMiscSubpixel;
};

/////////////////////////////////////////////////////
// FRasterContext: Construction / Destruction
FRasterContext::~FRasterContext()
{
    delete  mContextualDispatchTable;
}

FRasterContext::FRasterContext(
      FCommandQueue& iQueue
    , eFormat iFormat
)
    : mCommandQueue( iQueue )
    , mFormat( iFormat )
    , mHardwareMetrics( FHardwareMetrics() )
    , mContextualDispatchTable( new  FContextualDispatchTable( mHardwareMetrics, mFormat ) )
{
}

/////////////////////////////////////////////////////
// FRasterContext: Control Flow
void
FRasterContext::Flush()
{
    mCommandQueue.Flush();
}

void
FRasterContext::Finish()
{
    mCommandQueue.Finish();
}

void
FRasterContext::Fence()
{
    mCommandQueue.Fence();
}

eFormat
FRasterContext::Format() const
{
    return  mFormat;
}

/////////////////////////////////////////////////////
// FRasterContext: Blend
void
FRasterContext::Blend(
      const FBlock& iSource
    , FBlock& iBackdrop
    , const FRectI& iSourceRect
    , const FVec2I& iPosition
    , eBlendMode iBlendingMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Debug Safety Checks
    EventHookCheck( iEvent );
    EventSelfCheck( iNumWait, iWaitList, iEvent );

    // Sanitize geometry
    FRectI src_roi = iSourceRect & iSource.Rect();
    FRectI dst_target = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    FRectI dst_fit    = dst_target & iBackdrop.Rect();

    // Check no-op
    if( dst_fit.Area() <= 0 )
        return;

    // Select implementation
    fpCommandScheduler sched = nullptr;
    switch( BlendingModeQualifier( iBlendingMode ) ) {
        case BlendQualifier_Misc            : sched = mContextualDispatchTable->mScheduleBlendMisc;
        case BlendQualifier_Separable       : sched = mContextualDispatchTable->mScheduleBlendSeparable;
        case BlendQualifier_NonSeparable    : sched = mContextualDispatchTable->mScheduleBlendNonSeparable;
    }

    // Bake and push command
    mCommandQueue.Push(
        new FCommand(
              sched
            , new FBlendCommandArgs(
                  iSource
                , iBackdrop
                , src_roi
                , FVec2F()
                , FVec2F()
                , iBlendingMode
                , iAlphaMode
                , FMath::Clamp( iOpacity, 0.f, 1.f )
                , dst_fit.Position() - dst_target.Position()
                , dst_fit.Size()
                , dst_fit
            )
            , iPolicy
            , iNumWait
            , iWaitList
            , iEvent
        )
    );
}

void
FRasterContext::BlendAA(
      const FBlock& iSource
    , FBlock& iBackdrop
    , const FRectI& iSourceRect
    , const FVec2F& iPosition
    , eBlendMode iBlendingMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Debug Safety Checks
    EventHookCheck( iEvent );
    EventSelfCheck( iNumWait, iWaitList, iEvent );

    // Select implementation
    fpCommandScheduler sched = nullptr;
    switch( BlendingModeQualifier( iBlendingMode ) ) {
        case BlendQualifier_Misc            : sched = mContextualDispatchTable->mScheduleBlendMiscSubpixel;
        case BlendQualifier_Separable       : sched = mContextualDispatchTable->mScheduleBlendSeparableSubpixel;
        case BlendQualifier_NonSeparable    : sched = mContextualDispatchTable->mScheduleBlendNonSeparableSubpixel;
    }
}

ULIS_NAMESPACE_END

