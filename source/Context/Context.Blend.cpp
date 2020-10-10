// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Context.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the blend API in 
*               the FContext class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Image/Block.h"
#include "Blend/BlendArgs.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
void
FContext::Blend(
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
    ULIS_ASSERT( &iSource != &iBackdrop, "Source and Backdrop are the same block." );

    // Sanitize geometry
    FRectI src_roi = iSourceRect.Sanitized() & iSource.Rect();
    FRectI dst_aim = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    FRectI dst_fit = dst_aim & iBackdrop.Rect();

    // Check no-op
    if( dst_fit.Area() <= 0 )
        return  FinishEventNoOP( iEvent );

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
                , dst_fit.Position() - dst_aim.Position()
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
FContext::BlendAA(
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

