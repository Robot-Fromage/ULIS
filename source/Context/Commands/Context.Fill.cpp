// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Fill.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the fill API entry
*               points in the FContext class.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Fill/Fill.h"
#include "Process/Conv/Conv.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Sparse/TiledBlock.h"

ULIS_NAMESPACE_BEGIN
ulError
FContext::Fill(
          FBlock& iBlock
        , const ISample& iColor
        , const FRectI& iRect
        , const FSchedulePolicy& iPolicy
        , uint32 iNumWait
        , const FEvent* iWaitList
        , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI rect = iBlock.Rect();
    const FRectI roi = iRect.Sanitized() & rect;

    // Check no-op
    if( roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Forward arguments baking
    // This one is a bit tricky so here is a breakdown of the steps:
    FColor color    = iColor.ToFormat( iBlock.Format() );       // iColor can be in any format, so first we convert it to the block format.
    uint8* srcb     = color.Bits();
    uint8 bpp       = iBlock.BytesPerPixel();                   // We gather the Bytes Per Pixel for the format
    uint32 size     = FMath::Max( uint32(32), uint32( bpp ) );  // We define a size that is max of 32 ( avx2 ) and BPP ( bytes )
    uint32 stride   = size - ( size % bpp );
    uint8* buf      = new uint8[ size ];                        // We allocate a buffer that is length size, it will be deleted in ~FFillCommandArgs()
    for( uint32 i = 0; i < stride; i+= bpp )                    // We repeat the color N times in the buffer ( as many can fit )
        memcpy( (void*)( ( buf ) + i ), srcb, bpp );


    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleFill
            , new FFillCommandArgs(
                  iBlock
                , buf
                , roi
            )
            , iPolicy
            , roi == rect
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::Fill(
          FTiledBlock& iBlock
        , const ISample& iColor
        , const FRectI& iRect
        , const FSchedulePolicy& iPolicy
        , uint32 iNumWait
        , const FEvent* iWaitList
        , FEvent* iEvent
)
{
    const int x1 = static_cast< int >( FMath::RoundToNegativeInfinity( iRect.x / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int y1 = static_cast< int >( FMath::RoundToNegativeInfinity( iRect.y / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int x2 = static_cast< int >( FMath::RoundToPositiveInfinity( ( iRect.x + iRect.w ) / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int y2 = static_cast< int >( FMath::RoundToPositiveInfinity( ( iRect.y + iRect.h ) / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    TArray< FEvent > events;
    const int numOps = ( x2 - x1 ) * ( y2 - y1 );
    events.Reserve( numOps );
    for( int y = y1; y < y2; ++y ) {
        for( int x = x1; x < x2; ++x ) {
            const int u = x * FLQTree::sm_leaf_size_as_pixels;
            const int v = y * FLQTree::sm_leaf_size_as_pixels;
            FTile** tile = iBlock.QueryMutableTile( FVec2I( u, v ) );
            (*tile)->mLock = true;
            (*tile)->mDirty = true;
            FRectI tileRect( u, v, FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels );
            FRectI poi = ( iRect & tileRect );
            poi.x = FMath::PyModulo( poi.x, static_cast< int >( FLQTree::sm_leaf_size_as_pixels ) );
            poi.y = FMath::PyModulo( poi.y, static_cast< int >( FLQTree::sm_leaf_size_as_pixels ) );
            ULIS_ASSERT( poi.Area() > 0, "Bad area" );
            FBlock* tmp = new FBlock( *(*tile)->mClient, FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels );
            events.PushBack(
                FEvent(
                    FOnEventComplete(
                        [tmp,tile]( const ::ULIS::FRectI& ) {
                            delete  tmp;
                            (*tile)->mLock = false;
                        }
                    )
                )
            );
            Fill( *tmp, iColor, poi, iPolicy, iNumWait, iWaitList, &events.Back() );
        }
    }
    Dummy_OP( events.Size(), &events[0], iEvent );
    iBlock.ExtendOperativeGeometryAfterMutableChange( iRect );
    return  ULIS_NO_ERROR;
}

ulError
FContext::FillPreserveAlpha(
          FBlock& iBlock
        , const ISample& iColor
        , const FRectI& iRect
        , const FSchedulePolicy& iPolicy
        , uint32 iNumWait
        , const FEvent* iWaitList
        , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI rect = iBlock.Rect();
    const FRectI roi = iRect.Sanitized() & rect;

    // Check no-op
    if( roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleFillPreserveAlpha
            , new FFillPreserveAlphaCommandArgs(
                  iBlock
                , iColor.ToFormat( iBlock.Format() )
                , roi
            )
            , iPolicy
            , roi == rect
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , roi
        )
    );

    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

