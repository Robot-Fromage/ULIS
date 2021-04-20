// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Layer.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the Layer API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Image/Block.h"
#include "Layer/LayerStack.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Process/Conv/Conv.h"
#include "Process/Layer/PSD.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Entry points
ulError
FContext::Flatten(
      FLayerStack& iStack
    , FBlock& oDestination
    , const FRectI& iSourceRect
    , const FVec2I& iPosition
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR(
          iStack.Format() == oDestination.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iStack.Rect();
    const FRectI dst_rect = oDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_aim = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    const uint64 size = iStack.Layers().Size();
    const int64 max = static_cast< int64 >( size ) - 1;
    TArray< FEvent > events( 1 );
    events.Reserve( size );
    ulError err = Clear( oDestination, dst_roi, iPolicy, iNumWait, iWaitList, size ? &(events.Front()) : iEvent );
    ULIS_ASSERT_RETURN_ERROR( !err, "Error during stack flatten preprocess clear of dst block", err );

    for( int64 i = max; i >= 0; --i ) {
        eLayerType type = iStack.Layers()[i]->Type();
        FLayerImage* layer = nullptr;
        int num_wait = 1;
        switch( type ) {
            case Layer_Image: {
                layer = dynamic_cast< FLayerImage* >( iStack.Layers()[i] );
                break;
            }
            case Layer_Folder: {
                FLayerFolder* folder = dynamic_cast< FLayerFolder* >( iStack.Layers()[i] );
                layer = dynamic_cast< FLayerImage* >( folder );
                events.PushBack();
                err = RenderLayerFolder( *folder, dst_roi, iPolicy, 0, nullptr, &events.Back() );
                ULIS_ASSERT_RETURN_ERROR( !err, "Error during stack flatten process render of folder block", err );
                num_wait = 2;
                break;
            }
            case Layer_Text: {
                FLayerText* text = dynamic_cast< FLayerText* >( iStack.Layers()[i] );
                layer = dynamic_cast< FLayerImage* >( text );
                events.PushBack();
                err = RenderLayerText( *text, iPolicy, 0, nullptr, &events.Back() );
                ULIS_ASSERT_RETURN_ERROR( !err, "Error during folder render subfolder", err );
                num_wait = 2;
                break;
            }
        }

        if( layer )
        {
            events.PushBack();
            err = Blend(
                    layer->Block()
                , oDestination
                , src_roi
                , iPosition
                , layer->BlendMode()
                , layer->AlphaMode()
                , layer->Opacity()
                , iPolicy
                , num_wait
                , &events[ events.Size() - 1 - num_wait ]
                , i ? &events.Back() : iEvent
            );
            ULIS_ASSERT_RETURN_ERROR( !err, "Error during stack flatten process blend in dst block", err );
        }
    }

    return  ULIS_NO_ERROR;
}

ulError
FContext::RenderLayerFolder(
      FLayerFolder& iFolder
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iRect.Sanitized();
    const FRectI dst_rect = iFolder.Block().Rect();
    const FRectI dst_roi = src_rect & dst_rect;

    const uint64 size = iFolder.Layers().Size();
    const int64 max = static_cast< int64 >( size ) - 1;
    TArray< FEvent > events( 1 );
    events.Reserve( size );
    ulError err = Clear( iFolder.Block(), dst_roi, iPolicy, iNumWait, iWaitList, size ? &( events.Front() ) : iEvent );
    ULIS_ASSERT_RETURN_ERROR( !err, "Error during folder render preprocess clear of folder block", err );

    for( int64 i = max; i >= 0; --i ) {
        eLayerType type = iFolder.Layers()[i]->Type();
        FLayerImage* layer = nullptr;
        int num_wait = 1;
        switch( type ) {
            case Layer_Image: {
                layer = dynamic_cast< FLayerImage* >( iFolder.Layers()[i] );
                break;
            }
            case Layer_Folder: {
                FLayerFolder* folder = dynamic_cast< FLayerFolder* >( iFolder.Layers()[i] );
                layer = dynamic_cast< FLayerImage* >( folder );
                events.PushBack();
                err = RenderLayerFolder( *folder, dst_roi, iPolicy, 0, nullptr, &events.Back() );
                ULIS_ASSERT_RETURN_ERROR( !err, "Error during folder render subfolder", err );
                num_wait = 2;
                break;
            }
            case Layer_Text: {
                FLayerText* text = dynamic_cast< FLayerText* >( iFolder.Layers()[i] );
                layer = dynamic_cast< FLayerImage* >( text );
                events.PushBack();
                err = RenderLayerText( *text, iPolicy, 0, nullptr, &events.Back() );
                ULIS_ASSERT_RETURN_ERROR( !err, "Error during folder render subfolder", err );
                num_wait = 2;
                break;
            }
        }

        if( layer )
        {
            events.PushBack();
            err = Blend(
                  layer->Block()
                , iFolder.Block()
                , dst_roi
                , dst_roi.Position()
                , layer->BlendMode()
                , layer->AlphaMode()
                , layer->Opacity()
                , iPolicy
                , num_wait
                , &events[ events.Size() - 1 - num_wait ]
                , i ? &events.Back() : iEvent
            );
            ULIS_ASSERT_RETURN_ERROR( !err, "Error during stack flatten process blend in dst block", err );
        }
    }

    return  ULIS_NO_ERROR;
}

ulError
FContext::RenderLayerText(
      FLayerText& iText
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    const bool bAA = iText.AA();
    if( bAA ) {
        RasterTextAA(
              iText.Block()
            , iText.Text()
            , iText.Font()
            , iText.FontSize()
            , iText.Transform()
            , iText.Color()
            , iPolicy
            , iNumWait
            , iWaitList
            , iEvent
        );
    } else {
        RasterText(
              iText.Block()
            , iText.Text()
            , iText.Font()
            , iText.FontSize()
            , iText.Transform()
            , iText.Color()
            , iPolicy
            , iNumWait
            , iWaitList
            , iEvent
        );
    }

    return  ULIS_NO_ERROR;
}


// static
void
FContext::LoadPSDFromDiskMetrics(
      const std::string& iPath
    , bool *oFileExists
    , eFormat *oFormat
)
{
    FPSDOperations::GetContextFormatFromFile( iPath, oFileExists, oFormat );
}

ulError
FContext::XLoadPSDFromDisk(
      FLayerStack& iStack
    , const std::string& iPath
    , const FRectI& iSourceRect
    , const FVec2I& iPosition
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    FPSDOperations op( iPath, iStack );
    ULIS_ASSERT_RETURN_ERROR( !op.Import(), "Bad input data", ULIS_ERROR_BAD_INPUT_DATA );

    // Genarating layer stack from imported info
    eFormat layerStackFormat = iStack.Format();
    FLayerRoot* currentRoot = &iStack;

    Dummy_OP( iNumWait, iWaitList, nullptr );

    TArray< FEvent > eventConvert;
    eventConvert.Reserve( op.GetLayersInfo().Size() );
    const int64 max = static_cast< int64 >( op.GetLayersInfo().Size() ) - 1;
    for( int64 i = max; i >= 0; i-- )
    {
        // Divider 0 means rasterizable layer ( image )
        // Divider 1 means opened folder ( gui uncollapse )
        // Divider 2 means closed folder ( gui collapse )
        // Divider 3 means break folder ( flag )
        if( op.GetLayersInfo()[i].mDividerType == 0 )
        {
            const FString layerName( op.GetLayersInfo()[i].mName );
            const eFormat fmt = op.GetLayersInfo()[i].mLayerFormat;
            const uint32 right  = op.GetLayersInfo()[i].mRight;
            const uint32 left   = op.GetLayersInfo()[i].mLeft;
            const uint32 top    = op.GetLayersInfo()[i].mTop;
            const uint32 bottom = op.GetLayersInfo()[i].mBottom;
            const uint32 w = right - left;
            const uint32 h = bottom - top;

            FBlock* srcblock = nullptr;
            FBlock* layerBlock = new FBlock( op.GetImageWidth(), op.GetImageHeight(), layerStackFormat );

            if( op.GetBitDepth() == 32 )
                srcblock = new FBlock( (uint8*)op.GetLayersInfo()[i].mLayerImageDst32, w, h, fmt, nullptr, FOnInvalidBlock(), FOnCleanupData(&OnCleanup_FreeMemory) );
            else if( op.GetBitDepth() == 16 )
                srcblock = new FBlock( (uint8*)op.GetLayersInfo()[i].mLayerImageDst16, w, h, fmt, nullptr, FOnInvalidBlock(), FOnCleanupData(&OnCleanup_FreeMemory) );
            else if( op.GetBitDepth() == 8 )
                srcblock = new FBlock( (uint8*)op.GetLayersInfo()[i].mLayerImageDst, w, h, fmt, nullptr, FOnInvalidBlock(), FOnCleanupData(&OnCleanup_FreeMemory) );
            else
                ULIS_ASSERT_RETURN_ERROR( true, "Bad input data", ULIS_ERROR_BAD_INPUT_DATA );

            FRectI dstRect;
            if( srcblock )
                dstRect = FRectI( left, top, srcblock->Width(), srcblock->Height() );

            FEvent eventClear;
            Clear(
                  *layerBlock
                , layerBlock->Rect()
                , FSchedulePolicy::CacheEfficient
                , 0
                , nullptr
                , &eventClear
            );

            eventConvert.PushBack(
                FEvent(
                    FOnEventComplete(
                        [srcblock]( const FRectI& ) {
                            delete  srcblock;
                        }
                    )
                )
            );
            if( srcblock ) {
                ConvertFormat(
                      *srcblock
                    , *layerBlock
                    , srcblock->Rect()
                    , FVec2I( left, top )
                    , FSchedulePolicy::MultiScanlines
                    , 1
                    , &eventClear
                    , &eventConvert.Back()
                );
            }

            float opacity = float( op.GetLayersInfo()[i].mOpacity / 255.0f );
            bool isAlphaLocked = op.GetLayersInfo()[i].mFlags & 0x01;
            //bool isVisible = !( op.GetLayersInfo()[i].mFlags & 0x02 );
            eBlendMode blendMode = op.GetBlendingModeFromPSD( op.GetLayersInfo()[i].mBlendModeKey );

            currentRoot->AddLayer(
                new FLayerImage(
                      layerBlock
                    , layerName
                    , op.GetImageWidth()
                    , op.GetImageHeight()
                    , layerStackFormat
                    , blendMode
                    , isAlphaLocked ? eAlphaMode::Alpha_Top : eAlphaMode::Alpha_Normal
                    , opacity
                    , currentRoot
                )
            );

            //Todo: Locked
        }
        // Open folder / Closed Folder
        else if(
               op.GetLayersInfo()[i].mDividerType == 1
            || op.GetLayersInfo()[i].mDividerType == 2
        )
        {
            const FString layerName( op.GetLayersInfo()[i].mName );
            float opacity = float( op.GetLayersInfo()[i].mOpacity / 255.0f );
            bool isAlphaLocked = op.GetLayersInfo()[i].mFlags & 0x01;
            //bool isVisible = !( op.GetLayersInfo()[i].mFlags & 0x02 );
            eBlendMode blendMode = op.GetBlendingModeFromPSD( op.GetLayersInfo()[i].mBlendModeKey );

            FLayerFolder* layerFolder = new FLayerFolder(
                layerName
                , op.GetImageWidth()
                , op.GetImageHeight()
                , layerStackFormat
                , blendMode
                , isAlphaLocked ? eAlphaMode::Alpha_Top : eAlphaMode::Alpha_Normal
                , opacity
                , currentRoot
            );

            currentRoot->AddLayer( layerFolder );

            currentRoot = layerFolder;
        }
        // Break -> Exit current folder
        else if( op.GetLayersInfo()[i].mDividerType == 3 )
        {
            currentRoot = currentRoot->Parent();
        }
    }
    Dummy_OP( uint32(eventConvert.Size()), eventConvert.Data(), iEvent );
    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

