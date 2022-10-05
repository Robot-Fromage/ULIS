// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Blend.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the blend API entry
*               points in the FContext class.
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
#include "Sparse/TiledBlock.h"

ULIS_NAMESPACE_BEGIN
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
          iSource.Format() == iBackdrop.Format() && iSource.Format() == Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iBackdrop.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_aim = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Sanitized().Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

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
FContext::Blend(
      const FTiledBlock& iSource
    , FTiledBlock& iBackdrop
    , const FRectI& iSourceRect
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
    const FRectI src_rect = iSourceRect.Sanitized();
    const FRectI dst_rect = src_rect;
    const int x1 = static_cast< int >( FMath::RoundToNegativeInfinity( dst_rect.x / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int y1 = static_cast< int >( FMath::RoundToNegativeInfinity( dst_rect.y / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int x2 = static_cast< int >( FMath::RoundToPositiveInfinity( ( dst_rect.x + dst_rect.w ) / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int y2 = static_cast< int >( FMath::RoundToPositiveInfinity( ( dst_rect.y + dst_rect.h ) / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );

    TArray< FEvent > events;
    const int numOps = ( x2 - x1 ) * ( y2 - y1 );
    events.Reserve( numOps );
    for( int y = y1; y < y2; ++y ) {
        for( int x = x1; x < x2; ++x ) {
            const int u = x * FLQTree::sm_leaf_size_as_pixels;
            const int v = y * FLQTree::sm_leaf_size_as_pixels;
            const uint8* srcdat = iSource.QueryConstTile( FVec2I( u, v ) );
            FTile** tile = iBackdrop.QueryMutableTile( FVec2I( u, v ) );
            (*tile)->mLock = true;
            (*tile)->mDirty = true;
            FRectI tileRect( u, v, FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels );
            FRectI roi = ( iSourceRect & tileRect );
            roi.x = FMath::PyModulo( roi.x, static_cast< int >( FLQTree::sm_leaf_size_as_pixels ) );
            roi.y = FMath::PyModulo( roi.y, static_cast< int >( FLQTree::sm_leaf_size_as_pixels ) );
            ULIS_ASSERT( roi.Area() > 0, "Bad area" );
            FBlock* src = new FBlock( (uint8*)srcdat, FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels );
            FBlock* dst = new FBlock( *(*tile)->mClient, FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels );
            events.PushBack(
                FEvent(
                    FOnEventComplete(
                        [ src, dst, tile ]( const ::ULIS::FRectI& ) {
                            delete  src;
                            delete  dst;
                            (*tile)->mLock = false;
                        }
                    )
                )
            );
            Blend( *src, *dst, roi, roi.Position(), iBlendingMode, iAlphaMode, iOpacity, iPolicy, iNumWait, iWaitList, &events.Back() );
        }
    }
    Dummy_OP( events.Size(), &events[0], iEvent );
    iBackdrop.ExtendOperativeGeometryAfterMutableChange( iSourceRect );
    return  ULIS_NO_ERROR;
}

ulError
FContext::Blend(
      const FTiledBlock& iSource
    , FTiledBlock& iBackdrop
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
    const FRectI src_rect = iSourceRect.Sanitized();
    const FRectI dst_rect = FRectI::FromPositionAndSize( iPosition, src_rect.Size() );
    const int x1 = static_cast< int >( FMath::RoundToNegativeInfinity( dst_rect.x / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int y1 = static_cast< int >( FMath::RoundToNegativeInfinity( dst_rect.y / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int x2 = static_cast< int >( FMath::RoundToPositiveInfinity( ( dst_rect.x + dst_rect.w ) / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int y2 = static_cast< int >( FMath::RoundToPositiveInfinity( ( dst_rect.y + dst_rect.h ) / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );

    const int sx = FMath::PyModulo( iPosition.x + src_rect.x, static_cast< int >( FLQTree::sm_leaf_size_as_pixels ) );
    const int sy = FMath::PyModulo( iPosition.y + src_rect.y, static_cast< int >( FLQTree::sm_leaf_size_as_pixels ) );
    const int sx2 = FLQTree::sm_leaf_size_as_pixels - sx;
    const int sy2 = FLQTree::sm_leaf_size_as_pixels - sy;
    FRectI srcpois[4] = { { 0, 0, sx, sy }, { sx, 0, sx2, sy }, { 0, sy, sx, sy2 }, { sx, sy, sx2, sy2 } };
    TArray< FEvent > events;
    const int numOps = ( x2 - x1 ) * ( y2 - y1 );
    events.Reserve( numOps * 4 );
    for( int y = y1; y < y2; ++y ) {
        for( int x = x1; x < x2; ++x ) {
            const int u = x * FLQTree::sm_leaf_size_as_pixels;
            const int v = y * FLQTree::sm_leaf_size_as_pixels;

            FTile** tile = iBackdrop.QueryMutableTile( FVec2I( u, v ) );
            (*tile)->mLock = true;
            (*tile)->mDirty = true;
            for( int i = 0; i < 4; ++i ) {
                const int p = u - iPosition.x + src_rect.x + srcpois[i].x;
                const int q = v - iPosition.y + src_rect.y + srcpois[i].y;
                const uint8* srcdat = iSource.QueryConstTile( FVec2I( p, q ) );
                FRectI roi = FRectI::FromPositionAndSize( FVec2I( p, q ), srcpois[i].Size() ) & src_rect;
                roi.x = FMath::PyModulo( roi.x, static_cast< int >( FLQTree::sm_leaf_size_as_pixels ) );
                roi.y = FMath::PyModulo( roi.y, static_cast< int >( FLQTree::sm_leaf_size_as_pixels ) );
                if( roi.Area() <= 0 )
                    continue;
                FBlock* src = new FBlock( (uint8*)srcdat, FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels );
                FBlock* dst = new FBlock( *(*tile)->mClient, FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels );
                events.PushBack(
                    FEvent(
                        FOnEventComplete(
                            [ src, dst, tile ]( const ::ULIS::FRectI& ) {
                                delete  src;
                                delete  dst;
                                (*tile)->mLock = false;
                            }
                        )
                    )
                );
                Blend( *src, *dst, roi, srcpois[i].Position(), iBlendingMode, iAlphaMode, iOpacity, iPolicy, iNumWait, iWaitList, &events.Back() );
            }
        }
    }
    Dummy_OP( events.Size(), &events[0], iEvent );
    iBackdrop.ExtendOperativeGeometryAfterMutableChange( dst_rect );
    return  ULIS_NO_ERROR;
}

ulError
FContext::BlendBucket(
      const FBlock& iSource
    , FBlock& iBackdrop
    , const FRectI& iSourceRect
    , const TArray< FVec2I >& iPosition
    , eBlendMode iBlendingMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
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
          iSource.Format() == iBackdrop.Format() && iSource.Format() == Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_ERROR_FORMATS_MISMATCH )
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
    if( dst_roi.Sanitized().Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Forward arguments baking
    FVec2F subpixelComponent = iPosition.DecimalPart();
    if( xmin < 0 ) subpixelComponent.x = 1.f - subpixelComponent.x;
    if( ymin < 0 ) subpixelComponent.y = 1.f - subpixelComponent.y;
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
          iSource.Format() == iBackdrop.Format() && iSource.Format() == Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iBackdrop.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_aim = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Sanitized().Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

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
          iSource.Format() == iBackdrop.Format() && iSource.Format() == Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_ERROR_FORMATS_MISMATCH )
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
    if( dst_roi.Sanitized().Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

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
          iSource.Format() == iBackdrop.Format() && iSource.Format() == Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iBackdrop.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_roi = iBackdropRect.Sanitized() & dst_rect;

     // Check no-op
    if( src_roi.Area() <= 0 || dst_roi.Sanitized().Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

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
    if( dst_roi.Sanitized().Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

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

