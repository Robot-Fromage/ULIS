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
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Image/Block.h"
#include "Process/Misc/Extract.h"
#include "Process/Misc/Filter.h"
#include "Process/Misc/GammaCompress.h"
#include "Process/Misc/Premult.h"
#include "Process/Misc/Sanitize.h"
#include "Process/Misc/Swap.h"
#include "Process/Misc/Alloc.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Font/Font.h"
#include <vector>
#include <atomic>

ULIS_NAMESPACE_BEGIN
ulError
FContext::XCreateTestBlock(
      FBlock& iDestination
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    const eFormat fmt = Format();
    const int size = 64;
    FBlock* paper = new FBlock( 4, 3, fmt );
    const FColor background = FColor::RGB( 28, 25, 31, 240 );
    const FColor foreground = FColor::RGB( 170, 146, 64 );
    FFont* font = new FFont( FontEngine(), "Segoe UI", "Regular" );

    const std::wstring text = L"Test";
    FRectI textBox = TextMetrics( text, *font, 23 );
    FVec2I pos = FVec2I( size ) / 2 - textBox.Size() / 2;
    FMat3F mat = FMat3F::MakeTranslationMatrix( pos.x, pos.y + textBox.h );
    FRectI bgBox = textBox;
    int pad = 3;
    bgBox.x = pos.x - pad;
    bgBox.y = pos.y - pad;
    bgBox.w += pad * 2;
    bgBox.h += pad * 2;

    enum eEventName {
          Event_Alloc = 0
        , Event_Clear
        , Event_FillPaper
        , Event_DrawDot
        , Event_DrawLine
        , Event_Tile
        , Event_Strip
        , Event_FillBgBox
        , Event_RasterText
        , NumEvents
    };
    FEvent event[ NumEvents ];

    iDestination.LoadFromData( nullptr, size, size, fmt, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ) );
    XAllocateBlockData( iDestination, size, size, fmt, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ), FSchedulePolicy::MonoChunk, iNumWait, iWaitList, &event[Event_Alloc] );
    Clear( iDestination, FRectI::Auto, FSchedulePolicy::AsyncCacheEfficient, 1, &event[Event_Alloc], &event[Event_Clear] );
    Fill( *paper, background, FRectI::Auto, FSchedulePolicy::MonoChunk, 0, nullptr, &event[Event_FillPaper] );

    DrawLine( *paper, FVec2I(), FVec2I(), foreground, FRectI( 0, 0, 1, 1 ), FSchedulePolicy::MonoChunk, 1, &event[Event_FillPaper], &event[Event_DrawDot] );
    DrawLine( *paper, FVec2I( 1, 2 ), FVec2I( 3, 0 ), foreground, FRectI( 1, 0, 3, 3 ), FSchedulePolicy::MonoChunk, 1, &event[Event_FillPaper], &event[Event_DrawLine] );

    BlendTiled( *paper, iDestination, FRectI( 1, 0, 3, 3 ), FRectI( 0, 1, size, size ), FVec2I(), Blend_Normal, Alpha_Normal, 1.f, FSchedulePolicy::AsyncMultiScanlines, Event_DrawLine + 1, &event[0], &event[Event_Tile] );
    BlendTiled( *paper, iDestination, FRectI( 0, 0, 3, 1 ), FRectI( 0, 0, size, 1 ), FVec2I(), Blend_Normal, Alpha_Normal, 1.f, FSchedulePolicy::AsyncMonoScanlines, Event_DrawLine + 1, &event[0], &event[Event_Strip] );

    Fill( iDestination, background, bgBox, FSchedulePolicy::AsyncMonoChunk, 2, &event[Event_Tile], &event[Event_FillBgBox] );
    RasterTextAA( iDestination, text, *font, 22, mat, foreground, FSchedulePolicy::AsyncMonoChunk, 1, &event[Event_FillBgBox], &event[Event_RasterText] );

    FEvent finalizeEvent(
        FOnEventComplete(
            [font, paper]( const FRectI& ) {
                delete  font;
                delete  paper;
            }
        )
    );
    Dummy_OP( 1, &event[Event_RasterText], &finalizeEvent );
    Dummy_OP( 1, &finalizeEvent, iEvent );

    return  ULIS_NO_ERROR;
}

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
    for( size_t i = 1; i < sourceChannelsToExtract.size(); ++i ) {
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
                , uint8( sourceChannelsToExtract.size() )
                , QueryDispatchedExtractInvocation( iSource.Type(), iDestination.Type() )
            )
            , iPolicy
            , false // No chunk for now, ( ( src_roi == src_rect ) && ( dst_roi == dst_rect ) && ( src_rect == dst_rect ) )
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

ulError
FContext::Premultiply(
      FBlock& iBlock
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    if( !iBlock.HasAlpha() )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP );

    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iRect.Sanitized() & src_rect;

    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mSchedulePremultiply
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
FContext::Unpremultiply(
      FBlock& iBlock
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    if( !iBlock.HasAlpha() )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP );

    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iRect.Sanitized() & src_rect;

    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleUnpremultiply
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
FContext::SanitizeZeroAlpha(
      FBlock& iBlock
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    if( !iBlock.HasAlpha() )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP );

    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iRect.Sanitized() & src_rect;

    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleSanitize
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
FContext::Swap(
      FBlock& iBlock
    , uint8 iChannel1
    , uint8 iChannel2
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    if(    iBlock.SamplesPerPixel() == 1
        || iChannel1 == iChannel2
        || iChannel1 >= iBlock.SamplesPerPixel()
        || iChannel2 >= iBlock.SamplesPerPixel()
    )
    {
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP );
    }

    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iRect.Sanitized() & src_rect;

    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleSwap
            , new FSwapCommandArgs(
                  iBlock
                , src_roi
                , iChannel1
                , iChannel2
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
FContext::XAllocateBlockData(
      FBlock& iBlock
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , const FOnInvalidBlock& iOnInvalid
    , const FOnCleanupData& iOnCleanup
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              &ScheduleAlloc
            , new FAllocCommandArgs(
                  iBlock
                , FRectI( 0, 0, iWidth, iHeight )
                , FVec2I( iWidth, iHeight )
                , iFormat
                , iColorSpace
                , iOnInvalid
                , iOnCleanup
            )
            , iPolicy
            , false
            , true
            , iNumWait
            , iWaitList
            , iEvent
            , FRectI( 0, 0, iWidth, iHeight )
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::XDeallocateBlockData(
      FBlock& iBlock
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              &ScheduleDealloc
            , new FSimpleBufferCommandArgs(
                  iBlock
                , iBlock.Rect()
            )
            , iPolicy
            , false
            , true
            , iNumWait
            , iWaitList
            , iEvent
            , iBlock.Rect()
        )
    );

    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

