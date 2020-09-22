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
// FRasterContext::FContextualDispatchTable
struct FRasterContext::FContextualDispatchTable
{
    friend class FRasterContext;

public:
    /*! Constructor */
    FContextualDispatchTable( const FDevice& iDevice, eFormat iFormat )
        : mScheduleBlendSeparable(              TDispatcher< FDispatchedBlendSeparableInvocationSchedulerSelector >             ::Query( iDevice, iFormat ) )
        , mScheduleBlendNonSeparable(           TDispatcher< FDispatchedBlendNonSeparableInvocationSchedulerSelector >          ::Query( iDevice, iFormat ) )
        , mScheduleBlendMisc(                   TDispatcher< FDispatchedBlendMiscInvocationSchedulerSelector >                  ::Query( iDevice, iFormat ) )
        , mScheduleBlendSeparableSubpixel(      TDispatcher< FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector >     ::Query( iDevice, iFormat ) )
        , mScheduleBlendNonSeparableSubpixel(   TDispatcher< FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector >  ::Query( iDevice, iFormat ) )
        , mScheduleBlendMiscSubpixel(           TDispatcher< FDispatchedBlendMiscSubpixelInvocationSchedulerSelector >          ::Query( iDevice, iFormat ) )
    {
        ULIS_ASSERT( mScheduleBlendSeparable,               "Error: No dispatch found." );
        ULIS_ASSERT( mScheduleBlendNonSeparable,            "Error: No dispatch found." );
        ULIS_ASSERT( mScheduleBlendMisc,                    "Error: No dispatch found." );
        ULIS_ASSERT( mScheduleBlendSeparableSubpixel,       "Error: No dispatch found." );
        ULIS_ASSERT( mScheduleBlendNonSeparableSubpixel,    "Error: No dispatch found." );
        ULIS_ASSERT( mScheduleBlendMiscSubpixel,            "Error: No dispatch found." );
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
    , const FDevice& iDevice
    , eFormat iFormat
)
    : mContextualDispatchTable( new  FContextualDispatchTable( iDevice, iFormat ) )
    , mCommandQueue( iQueue )
    , mDevice( iDevice )
    , mFormat( iFormat )
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
    , const FTaskEvent* iWaitList
    , FTaskEvent* iEvent
)
{
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
    ULIS_ASSERT( sched, "Error: No dispatch found." );

    // Bake and push command
    mCommandQueue.Push(
        new FCommand(
              sched
            , new FBlendArgs( {
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
            } )
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
    , const FTaskEvent* iWaitList
    , FTaskEvent* iEvent
)
{
    // Select implementation
    fpCommandScheduler sched = nullptr;
    switch( BlendingModeQualifier( iBlendingMode ) ) {
        case BlendQualifier_Misc            : sched = mContextualDispatchTable->mScheduleBlendMiscSubpixel;
        case BlendQualifier_Separable       : sched = mContextualDispatchTable->mScheduleBlendSeparableSubpixel;
        case BlendQualifier_NonSeparable    : sched = mContextualDispatchTable->mScheduleBlendNonSeparableSubpixel;
    }
    ULIS_ASSERT( sched, "Error: No dispatch found." );
}

ULIS_NAMESPACE_END

