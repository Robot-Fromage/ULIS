// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Clear.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the clear API entry
*               points in the FContext class.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Clear/Clear.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Scheduling/SimpleBufferArgs.h"
#include "Sparse/TiledBlock.h"

ULIS_NAMESPACE_BEGIN
ulError
FContext::Clear(
          FBlock& iBlock
        , const FRectI& iRect
        , const FSchedulePolicy& iPolicy
        , uint32 iNumWait
        , const FEvent* iWaitList
        , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iRect.Sanitized() & src_rect;

    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleClear
            , new FSimpleBufferCommandArgs(
                  iBlock
                , src_roi
            )
            , iPolicy
            , src_roi == src_rect
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , src_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::Clear(
          FTiledBlock& iBlock
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
            Clear( *tmp, poi, iPolicy, iNumWait, iWaitList, &events.Back() );
        }
    }
    Dummy_OP( events.Size(), &events[0], iEvent );
    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

