// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Misc.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the misc API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Misc/Extract.h"
#include "Process/Misc/Filter.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include <vector>

ULIS_NAMESPACE_BEGIN
ulError
FContext::Extract(
      const FBlock& iSource
    , FBlock& iDestination
    , uint8 iSourceExtractMask
    , uint8 iDestinationExtractMask
    , bool iUseRawMask
    , const FRectI& iSourceRect
    , const FVec2I& iPosition
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iDestination
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_roi = FRectI::FromPositionAndSize( iPosition, src_roi.Size() ) & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Forward arguments baking
    const FFormatMetrics& srcFormatMetrics( iSource.FormatMetrics() );
    const FFormatMetrics& dstFormatMetrics( iDestination.FormatMetrics() );

    // Channels
    uint8 max_channels_both = FMath::Min( FMath::Max( srcFormatMetrics.SPP, dstFormatMetrics.SPP ), static_cast< uint8 >( ULIS_MAX_CHANNELS ) );
    std::vector< uint8 > sourceChannelsToExtract;
    std::vector< uint8 > destinationChannelsToExtract;
    sourceChannelsToExtract.reserve( max_channels_both );
    destinationChannelsToExtract.reserve( max_channels_both );
    for( int i = 0; i < max_channels_both; ++i ) {
        if( iSourceExtractMask & ( 1 << i ) )
            sourceChannelsToExtract.push_back( iUseRawMask ? i : srcFormatMetrics.IDT[i] );

        if( iDestinationExtractMask & ( 1 << i ) )
            destinationChannelsToExtract.push_back( iUseRawMask ? i : dstFormatMetrics.IDT[i] );
    }
    sourceChannelsToExtract.shrink_to_fit();
    destinationChannelsToExtract.shrink_to_fit();

    ULIS_ASSERT_RETURN_ERROR( sourceChannelsToExtract.size() == destinationChannelsToExtract.size(), "Extract masks don't map", FinishEventNo_OP( iEvent, ULIS_ERROR_BAD_INPUT_DATA ) );
    ULIS_ASSERT_RETURN_ERROR( sourceChannelsToExtract.size() && destinationChannelsToExtract.size(), "Bad Extraction parameters", FinishEventNo_OP( iEvent, ULIS_ERROR_BAD_INPUT_DATA ) );

    // Strides
    uint8* sourceStrides = new uint8[ sourceChannelsToExtract.size() ];
    uint8* destinationStrides = new uint8[ destinationChannelsToExtract.size() ];
    sourceStrides[0] = sourceChannelsToExtract[0];
    destinationStrides[0] = destinationChannelsToExtract[0];
    for( int i = 1; i < sourceChannelsToExtract.size(); ++i ) {
        sourceStrides[i] = sourceChannelsToExtract[i] - sourceChannelsToExtract[i-1];
        destinationStrides[i] = destinationChannelsToExtract[i] - destinationChannelsToExtract[i-1];
    }

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleExtract
            , new FExtractCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
                , sourceStrides
                , destinationStrides
                , QueryDispatchedExtractInvocation( iSource.Type(), iDestination.Type() )
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
FContext::Filter(
      std::function< void( const FBlock&, const uint8* ) > iInvocation
    , const FBlock& iSource
    , const FRectI& iSourceRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;

    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleFilter
            , new FFilterCommandArgs(
                  const_cast< FBlock& >( iSource )
                , src_roi
                , iInvocation
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
FContext::FilterInPlace(
      std::function< void( FBlock&, uint8* ) > iInvocation
    , FBlock& iDestination
    , const FRectI& iDestinationRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iDestination.Rect();
    const FRectI src_roi = iDestinationRect.Sanitized() & src_rect;

    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleFilterInPlace
            , new FFilterInPlaceCommandArgs(
                  iDestination
                , src_roi
                , iInvocation
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
FContext::FilterInto(
      std::function< void( const FBlock&, const uint8*, FBlock&, uint8* ) > iInvocation
    , const FBlock& iSource
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
          &iSource != &iDestination
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_aim = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleFilterInto
            , new FFilterIntoCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
                , iInvocation
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
FContext::sRGBToLinear(
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
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mSchedulesRGBToLinear
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
FContext::LinearTosRGB(
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
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleLinearTosRGB
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

ULIS_NAMESPACE_END

