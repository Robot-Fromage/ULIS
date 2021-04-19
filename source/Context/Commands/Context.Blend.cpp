// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Blend.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the blend API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Image/Block.h"
#include "Process/Blend/Blend.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Process/Conv/Conv.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Helper for Redundant operations
namespace detail {
Vec4i
BuildRGBA8IndexTable( uint8 iRS )
{
    Vec4i result;
    switch( iRS ) {
        case 1:  for( int i = 0; i < 4; ++i ) result.insert( i, ( 3 - i )                             ); return  result;
        case 2:  for( int i = 0; i < 4; ++i ) result.insert( i, ( i + 1 ) > 3 ? 0 : i + 1             ); return  result;
        case 3:  for( int i = 0; i < 4; ++i ) result.insert( i, ( 3 - i ) - 1 < 0 ? 3 : ( 3 - i ) - 1 ); return  result;
        default: for( int i = 0; i < 4; ++i ) result.insert( i, i                                     ); return  result;
    }
}
} // namespace detail

/////////////////////////////////////////////////////
// Entry points
ulError
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
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iBackdrop
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iBackdrop.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iBackdrop.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_aim = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleBlend( iBlendingMode )
            , new FBlendCommandArgs(
                  iSource
                , iBackdrop
                , src_roi
                , dst_roi
                , FVec2F( 0.f )
                , FVec2F( 1.f )
                , iBlendingMode
                , iAlphaMode
                , FMath::Clamp( iOpacity, 0.f, 1.f )
                , dst_roi.Position() - dst_aim.Position()
                , dst_roi.Size()
                , mContextualDispatchTable->mArgConvForwardBlendNonSeparable
                , mContextualDispatchTable->mArgConvBackwardBlendNonSeparable
                , detail::BuildRGBA8IndexTable( iSource.FormatMetrics().RSC )
                , iSource.BytesPerScanLine()
            )
            , iPolicy
            , false // ( ( src_roi == src_rect ) && ( dst_roi == dst_rect ) && ( src_rect == dst_rect ) )
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
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
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iBackdrop
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iBackdrop.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iBackdrop.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const int xmin = static_cast< int >( FMath::RoundToNegativeInfinity( iPosition.x ) );
    const int ymin = static_cast< int >( FMath::RoundToNegativeInfinity( iPosition.y ) );
    const int xmax = static_cast< int >( FMath::RoundToPositiveInfinity( iPosition.x + src_roi.w ) );
    const int ymax = static_cast< int >( FMath::RoundToPositiveInfinity( iPosition.y + src_roi.h ) );
    const FRectI dst_aim = FRectI::FromMinMax( xmin, ymin, xmax, ymax );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Forward arguments baking
    const FVec2F subpixelComponent = iPosition.DecimalPart();
    const FVec2I shift = dst_roi.Position() - dst_aim.Position();
    const int coverageX = src_roi.w - ( src_roi.x + shift.x ) >= dst_roi.w ? dst_roi.w : static_cast< int >( dst_roi.w - ceil( subpixelComponent.x ) );
    const int coverageY = src_roi.h - ( src_roi.y + shift.y ) >= dst_roi.h ? dst_roi.h : static_cast< int >( dst_roi.h - ceil( subpixelComponent.y ) );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleBlendSubpixel( iBlendingMode )
            , new FBlendCommandArgs(
                  iSource
                , iBackdrop
                , src_roi
                , dst_roi
                , subpixelComponent
                , 1.f - subpixelComponent
                , iBlendingMode
                , iAlphaMode
                , FMath::Clamp( iOpacity, 0.f, 1.f )
                , shift
                , FVec2I( coverageX, coverageY )
                , mContextualDispatchTable->mArgConvForwardBlendNonSeparable
                , mContextualDispatchTable->mArgConvBackwardBlendNonSeparable
                , detail::BuildRGBA8IndexTable( iSource.FormatMetrics().RSC )
                , iSource.BytesPerScanLine()
            )
            , iPolicy
            , false
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
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
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iBackdrop
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iBackdrop.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iBackdrop.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_aim = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleAlphaBlend
            , new FBlendCommandArgs(
                  iSource
                , iBackdrop
                , src_roi
                , dst_roi
                , FVec2F( 0.f )
                , FVec2F( 1.f )
                , eBlendMode::Blend_Normal
                , eAlphaMode::Alpha_Normal
                , FMath::Clamp( iOpacity, 0.f, 1.f )
                , dst_roi.Position() - dst_aim.Position()
                , dst_roi.Size()
                , mContextualDispatchTable->mArgConvForwardBlendNonSeparable
                , mContextualDispatchTable->mArgConvBackwardBlendNonSeparable
                , detail::BuildRGBA8IndexTable( iSource.FormatMetrics().RSC )
                , iSource.BytesPerScanLine()
            )
            , iPolicy
            , false // ( ( src_roi == src_rect ) && ( dst_roi == dst_rect ) && ( src_rect == dst_rect ) )
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
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
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iBackdrop
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iBackdrop.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iBackdrop.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const int xmin = static_cast< int >( FMath::RoundToNegativeInfinity( iPosition.x ) );
    const int ymin = static_cast< int >( FMath::RoundToNegativeInfinity( iPosition.y ) );
    const int xmax = static_cast< int >( FMath::RoundToPositiveInfinity( iPosition.x + src_roi.w ) );
    const int ymax = static_cast< int >( FMath::RoundToPositiveInfinity( iPosition.y + src_roi.h ) );
    const FRectI dst_aim = FRectI::FromMinMax( xmin, ymin, xmax, ymax );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Forward arguments baking
    const FVec2F subpixelComponent = iPosition.DecimalPart();
    const FVec2I shift = dst_roi.Position() - dst_aim.Position();
    const int coverageX = src_roi.w - ( src_roi.x + shift.x ) >= dst_roi.w ? dst_roi.w : static_cast< int >( dst_roi.w - ceil( subpixelComponent.x ) );
    const int coverageY = src_roi.h - ( src_roi.y + shift.y ) >= dst_roi.h ? dst_roi.h : static_cast< int >( dst_roi.h - ceil( subpixelComponent.y ) );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleAlphaBlendSubpixel
            , new FBlendCommandArgs(
                  iSource
                , iBackdrop
                , src_roi
                , dst_roi
                , subpixelComponent
                , 1.f - subpixelComponent
                , eBlendMode::Blend_Normal
                , eAlphaMode::Alpha_Normal
                , FMath::Clamp( iOpacity, 0.f, 1.f )
                , shift
                , FVec2I( coverageX, coverageY )
                , mContextualDispatchTable->mArgConvForwardBlendNonSeparable
                , mContextualDispatchTable->mArgConvBackwardBlendNonSeparable
                , detail::BuildRGBA8IndexTable( iSource.FormatMetrics().RSC )
                , iSource.BytesPerScanLine()
            )
            , iPolicy
            , false
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
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
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iBackdrop
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iBackdrop.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iBackdrop.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_roi = iBackdropRect.Sanitized() & dst_rect;

     // Check no-op
    if( src_roi.Area() <= 0 || dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Forward arguments baking
    FVec2I mod_shift(
          FMath::PyModulo( - FMath::PyModulo( iPosition.x - ( dst_roi.x - iBackdropRect.x ), src_roi.w ), src_roi.w )
        , FMath::PyModulo( - FMath::PyModulo( iPosition.y - ( dst_roi.y - iBackdropRect.y ), src_roi.h ), src_roi.h )
    );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleTiledBlend( iBlendingMode )
            , new FBlendCommandArgs(
                  iSource
                , iBackdrop
                , src_roi
                , dst_roi
                , FVec2F( 0.f )
                , FVec2F( 1.f )
                , iBlendingMode
                , iAlphaMode
                , FMath::Clamp( iOpacity, 0.f, 1.f )
                , mod_shift
                , dst_roi.Size()
                , mContextualDispatchTable->mArgConvForwardBlendNonSeparable
                , mContextualDispatchTable->mArgConvBackwardBlendNonSeparable
                , detail::BuildRGBA8IndexTable( iSource.FormatMetrics().RSC )
                , iSource.BytesPerScanLine()
                , true
            )
            , iPolicy
            , false
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
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
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Forward arguments baking
    FBlock* color = new FBlock( 1, 1, iBackdrop.Format() );
    FPixel proxy = color->Pixel( 0, 0 );
    ISample::ConvertFormat( iColor, proxy );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleTiledBlend( iBlendingMode )
            , new FBlendCommandArgs(
                  *color
                , iBackdrop
                , src_roi
                , dst_roi
                , FVec2F( 0.f )
                , FVec2F( 1.f )
                , iBlendingMode
                , iAlphaMode
                , FMath::Clamp( iOpacity, 0.f, 1.f )
                , FVec2I( 0 )
                , dst_roi.Size()
                , mContextualDispatchTable->mArgConvForwardBlendNonSeparable
                , mContextualDispatchTable->mArgConvBackwardBlendNonSeparable
                , detail::BuildRGBA8IndexTable( color->FormatMetrics().RSC )
                , color->BytesPerScanLine()
                , true
                , color
            )
            , iPolicy
            , false
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

