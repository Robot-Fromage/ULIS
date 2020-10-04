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
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
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

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleBlend( eBlendMode iBlendingMode ) const
    {
        switch( BlendingModeQualifier( iBlendingMode ) ) {
            case BlendQualifier_Misc            : return  mScheduleBlendMisc;
            case BlendQualifier_Separable       : return  mScheduleBlendSeparable;
            case BlendQualifier_NonSeparable    : return  mScheduleBlendNonSeparable;
        }
    }

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleBlendSubpixel( eBlendMode iBlendingMode ) const
    {
        switch( BlendingModeQualifier( iBlendingMode ) ) {
            case BlendQualifier_Misc            : return  mScheduleBlendMiscSubpixel;
            case BlendQualifier_Separable       : return  mScheduleBlendSeparableSubpixel;
            case BlendQualifier_NonSeparable    : return  mScheduleBlendNonSeparableSubpixel;
        }
    }

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
    // Sanitize geometry
    FRectI src_roi = iSourceRect & iSource.Rect();
    FRectI dst_target = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    FRectI dst_fit    = dst_target & iBackdrop.Rect();

    // Check no-op
    if( dst_fit.Area() <= 0 )
        return;

    // Bake and push command
    mCommandQueue.Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleBlend( iBlendingMode )
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
}

ULIS_NAMESPACE_END

