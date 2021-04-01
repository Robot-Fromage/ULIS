// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Analysis.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the Analysis API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Analysis/AccumulativeSampling.h"
#include "Process/Analysis/AnalyzeSmallestVisibleRect.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
ulError
FContext::AnalyzeSmallestVisibleRect(
      const FBlock& iBlock
    , FRectI* oRect
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR(
          oRect
        , "No input."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_BAD_INPUT_DATA )
    );

    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iRect.Sanitized() & src_rect;

    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    if( !iBlock.HasAlpha() ) {
        *oRect = src_roi;
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    }

    FEvent event_alloc;
    FBlock* strip = new FBlock( 1, src_roi.h, Format_CMYK16, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ) );
    XAllocateBlockData( *strip, 1, src_roi.h, Format_CMYK16, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ), FSchedulePolicy::MonoChunk, iNumWait, iWaitList, &event_alloc );

    FEvent xpass_event;
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleAnalyzeSmallestVisibleRectXPass
            , new FDualBufferCommandArgs(
                  iBlock
                , *strip
                , src_roi
                , strip->Rect()
            )
            , FSchedulePolicy::AsyncMultiScanlines
            , false // force scanline
            , false
            , 1
            , &event_alloc
            , &xpass_event
            , strip->Rect()
        )
    );

    FEvent ypass_event(
        FOnEventComplete(
            [strip, oRect]( const FRectI& ) {
                FPixel proxy = strip->Pixel( 0, strip->Height() - 1 );
                *oRect = FRectI::FromMinMax(
                      proxy.Channel16( 0 )
                    , proxy.Channel16( 1 )
                    , proxy.Channel16( 2 ) + 1
                    , proxy.Channel16( 3 ) + 1
                );
                delete  strip;
            }
        )
    );
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleAnalyzeSmallestVisibleRectYPass
            , new FSimpleBufferCommandArgs(
                  *strip
                , strip->Rect()
            )
            , FSchedulePolicy::AsyncMonoChunk
            , true
            , true
            , 1
            , &xpass_event
            , &ypass_event
            , strip->Rect()
        )
    );

    Dummy_OP( 1, &ypass_event, iEvent );
    return  ULIS_NO_ERROR;
}

ulError
FContext::AccumulateSample(
      const FBlock& iBlock
    , FColor* oColor
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR(
          oColor
        , "No input."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_BAD_INPUT_DATA )
    );

    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iRect.Sanitized() & src_rect;

    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    FEvent event_alloc;
    FBlock* strip = new FBlock(); // Hollow
    XAllocateBlockData( *strip, src_roi.h, 1, SummedAreaTableMetrics( iBlock ), nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ), iPolicy, iNumWait, iWaitList, &event_alloc );

    FEvent xpass_event;
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleAccumulativeSamplingXPass
            , new FDualBufferCommandArgs(
                  iBlock
                , *strip
                , src_roi
                , strip->Rect()
            )
            , iPolicy
            , false // force scanline
            , false
            , 1
            , &event_alloc
            , &xpass_event
            , strip->Rect()
        )
    );

    FEvent ypass_event(
        FOnEventComplete(
            [strip, oColor, &src_roi]( const FRectI& ) {
                FPixel proxy = strip->Pixel( strip->Width() - 1, 0 );
                proxy.Unpremultiply();
                float area = static_cast< float >( src_roi.Area() );
                for( uint8 i = 0; i < proxy.SamplesPerPixel(); ++i )
                    proxy.SetChannelF( i, proxy.ChannelF( i ) / area );
                delete  strip;
            }
        )
    );
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleAccumulativeSamplingYPass
            , new FSimpleBufferCommandArgs(
                  *strip
                , strip->Rect()
            )
            , iPolicy
            , false
            , false
            , 1
            , &xpass_event
            , &ypass_event
            , strip->Rect()
        )
    );
    Dummy_OP( 1, &ypass_event, iEvent );

    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

