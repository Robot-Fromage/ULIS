// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Context.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the blend API entry
*               points in the FContext class.
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
    ULIS_ASSERT( &iSource != &iBackdrop,                    "Source and Backdrop are the same block." );
    ULIS_ASSERT( iSource.Format() == iBackdrop.Format(),    "Formats mismatch." );

    // Sanitize geometry
    const FRectI src_roi = iSourceRect.Sanitized() & iSource.Rect();
    const FRectI dst_aim = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    const FRectI dst_fit = dst_aim & iBackdrop.Rect();

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
                , FVec2F( 0.f )
                , FVec2F( 1.f )
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
    ULIS_ASSERT( &iSource != &iBackdrop,                    "Source and Backdrop are the same block." );
    ULIS_ASSERT( iSource.Format() == iBackdrop.Format(),    "Formats mismatch." );

    // Sanitize geometry
    const FRectI src_roi = iSourceRect.Sanitized() & iSource.Rect();
    const int xmin = static_cast< int >( FMath::RoundToNegativeInfinity( iPosition.x ) );
    const int ymin = static_cast< int >( FMath::RoundToNegativeInfinity( iPosition.y ) );
    const int xmax = static_cast< int >( FMath::RoundToPositiveInfinity( iPosition.x + src_roi.w ) );
    const int ymax = static_cast< int >( FMath::RoundToPositiveInfinity( iPosition.y + src_roi.h ) );
    const FRectI dst_aim = FRectI::FromMinMax( xmin, ymin, xmax, ymax );
    const FRectI dst_fit = dst_aim & iBackdrop.Rect();

    // Check no-op
    if( dst_fit.Area() <= 0 )
        return  FinishEventNoOP( iEvent );

    // Forward arguments baking
    const FVec2F subpixelComponent = iPosition.DecimalPart();
    const FVec2I shift = dst_fit.Position() - dst_aim.Position();
    const int coverageX = src_roi.w - ( src_roi.x + shift.x ) >= dst_fit.w ? dst_fit.w : static_cast< int >( dst_fit.w - ceil( subpixelComponent.x ) );
    const int coverageY = src_roi.h - ( src_roi.y + shift.y ) >= dst_fit.h ? dst_fit.h : static_cast< int >( dst_fit.h - ceil( subpixelComponent.y ) );

    // Bake and push command
    mCommandQueue.Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleBlendSubpixel( iBlendingMode )
            , new FBlendCommandArgs(
                  iSource
                , iBackdrop
                , src_roi
                , subpixelComponent
                , 1.f - subpixelComponent
                , iBlendingMode
                , iAlphaMode
                , FMath::Clamp( iOpacity, 0.f, 1.f )
                , shift
                , FVec2I( coverageX, coverageY )
                , dst_fit
            )
            , iPolicy
            , iNumWait
            , iWaitList
            , iEvent
        )
    );
}

ULIS_NAMESPACE_END

