// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Copy.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the copy API entry
*               points in the FContext class.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Copy/Copy.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Scheduling/DualBufferArgs.h"
#include "Sparse/TiledBlock.h"

ULIS_NAMESPACE_BEGIN
ulError
FContext::Copy(
          const FBlock& iSource
        , FBlock& iDestination
        , const FRectI& iSourceRect
        , const FVec2I& iPosition
        , const FSchedulePolicy& iPolicy
        , uint32 iNumWait
        , const FEvent* iWaitList
        , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iDestination.Format() && iSource.Format() == Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_roi = FRectI::FromPositionAndSize( iPosition, src_roi.Size() ) & dst_rect;

    // Check no-op
    if( dst_roi.Sanitized().Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleCopy
            , new FDualBufferCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
            )
            , iPolicy
            , ( ( src_roi == src_rect ) && ( dst_roi == dst_rect ) && ( src_rect == dst_rect ) )
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
FContext::Dump(
      const FTiledBlock& iSource
    , FBlock& iDestination
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
) {
    FRectI leafRect = iSource.LeafGeometry();
    const int x1 = static_cast< int >( FMath::RoundToNegativeInfinity( leafRect.x / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int y1 = static_cast< int >( FMath::RoundToNegativeInfinity( leafRect.y / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int x2 = static_cast< int >( FMath::RoundToPositiveInfinity( ( leafRect.x + leafRect.w ) / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    const int y2 = static_cast< int >( FMath::RoundToPositiveInfinity( ( leafRect.y + leafRect.h ) / static_cast< float >( FLQTree::sm_leaf_size_as_pixels ) ) );
    TArray< FEvent > events;
    const int numOps = ( x2 - x1 ) * ( y2 - y1 );
    events.Reserve( numOps );
    for( int y = y1; y < y2; ++y ) {
        for( int x = x1; x < x2; ++x ) {
            const int u = x * FLQTree::sm_leaf_size_as_pixels;
            const int v = y * FLQTree::sm_leaf_size_as_pixels;
            const uint8* tile = iSource.QueryConstTile( FVec2I( u, v ) );
            FRectI tileRect( u, v, FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels );
            FRectI src_poi = ( leafRect & tileRect );
            FRectI dst_poi = src_poi;
            src_poi.x = FMath::PyModulo( src_poi.x, static_cast< int >( FLQTree::sm_leaf_size_as_pixels ) );
            src_poi.y = FMath::PyModulo( src_poi.y, static_cast< int >( FLQTree::sm_leaf_size_as_pixels ) );
            dst_poi.Shift( -leafRect.Position() );
            ULIS_ASSERT( src_poi.Area() > 0, "Bad area" );
            FBlock* tmp = new FBlock( (uint8*)tile, FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels );
            events.PushBack(
                FEvent(
                    FOnEventComplete(
                        [tmp,tile]( const ::ULIS::FRectI& ) {
                            delete  tmp;
                        }
                    )
                )
            );
            Copy( *tmp, iDestination, src_poi, dst_poi.Position(), iPolicy, iNumWait, iWaitList, &events.Back() );
        }
    }
    Dummy_OP( events.Size(), &events[0], iEvent );
    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

