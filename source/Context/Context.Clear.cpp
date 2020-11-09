// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Context.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the clear API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
void
FContext::Clear(
          FBlock& iBlock
        , const FRectI& iRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
)
{
    // Sanitize geometry
    const FRectI src_roi = iRect.Sanitized() & iBlock.Rect();

    // Check no-op
    if( src_roi.Area() <= 0 )
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

void
FContext::AlphaBlend(
      const FBlock& iSource
    , FBlock& iBackdrop
    , const FRectI& iSourceRect
    , const FVec2I& iPosition
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
              mContextualDispatchTable->mScheduleAlphaBlend
            , new FBlendCommandArgs(
                  iSource
                , iBackdrop
                , src_roi
                , FVec2F( 0.f )
                , FVec2F( 1.f )
                , eBlendMode::Blend_Normal
                , eAlphaMode::Alpha_Normal
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
FContext::AlphaBlendAA(
      const FBlock& iSource
    , FBlock& iBackdrop
    , const FRectI& iSourceRect
    , const FVec2F& iPosition
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
              mContextualDispatchTable->mScheduleAlphaBlendSubpixel
            , new FBlendCommandArgs(
                  iSource
                , iBackdrop
                , src_roi
                , subpixelComponent
                , 1.f - subpixelComponent
                , eBlendMode::Blend_Normal
                , eAlphaMode::Alpha_Normal
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

void
FContext::BlendTiled(
      const FBlock& iSource
    , FBlock& iBackdrop
    , const FRectI& iSourceRect
    , const FRectI& iBackdropRect
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
    const FRectI dst_roi = iBackdropRect.Sanitized() & iBackdrop.Rect();

     // Check no-op
    if( src_roi.Area() <= 0 || dst_roi.Area() <= 0 )
        return  FinishEventNoOP( iEvent );

    // Forward arguments baking
    FVec2I mod_shift(
          FMath::PyModulo( - FMath::PyModulo( iPosition.x - ( dst_roi.x - iBackdropRect.x ), src_roi.w ), src_roi.w )
        , FMath::PyModulo( - FMath::PyModulo( iPosition.y - ( dst_roi.y - iBackdropRect.y ), src_roi.h ), src_roi.h )
    );

    // Bake and push command
    mCommandQueue.Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleTiledBlendSubpixel( iBlendingMode )
            , new FBlendCommandArgs(
                  iSource
                , iBackdrop
                , src_roi
                , FVec2F( 0.f )
                , FVec2F( 1.f )
                , iBlendingMode
                , iAlphaMode
                , FMath::Clamp( iOpacity, 0.f, 1.f )
                , mod_shift
                , dst_roi.Size()
                , dst_roi
            )
            , iPolicy
            , iNumWait
            , iWaitList
            , iEvent
        )
    );
}

void
FContext::BlendColor(
      const ISample& iColor
    , FBlock& iBackdrop
    , const FRectI& iBackdropRect
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
    const FRectI src_roi = FRectI( 0, 0, 1, 1 );
    const FRectI dst_roi = iBackdropRect.Sanitized() & iBackdrop.Rect();

     // Check no-op
    if( src_roi.Area() <= 0 || dst_roi.Area() <= 0 )
        return  FinishEventNoOP( iEvent );

    // Forward arguments baking
    FBlock* color = new FBlock( 1, 1, iBackdrop.Format() );
    FPixel proxy = color->Pixel( 0, 0 );
    Conv( iColor, proxy );

    // Bake and push command
    mCommandQueue.Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleTiledBlendSubpixel( iBlendingMode )
            , new FBlendCommandArgs(
                  *color
                , iBackdrop
                , src_roi
                , FVec2F( 0.f )
                , FVec2F( 1.f )
                , iBlendingMode
                , iAlphaMode
                , FMath::Clamp( iOpacity, 0.f, 1.f )
                , FVec2I( 0, 0 )
                , dst_roi.Size()
                , dst_roi
                , color
            )
            , iPolicy
            , iNumWait
            , iWaitList
            , iEvent
        )
    );
}

ULIS_NAMESPACE_END

