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
#include "Layer/Layer/LayerStack.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Process/Conv/Conv.h"
#include "Process/Layer/PSD.h"

ULIS_NAMESPACE_BEGIN
/*
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
        , FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iStack.Rect();
    const FRectI dst_rect = oDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_aim = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Sanitized().Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    FEvent ev = iStack.RenderImage( *this, oDestination, src_roi, dst_roi.Position(), iPolicy, iNumWait, iWaitList );
    Dummy_OP( 1, &ev, iEvent );

    return  ULIS_NO_ERROR;
}

ulError
FContext::RenderLayerFolder(
      FLayerFolder& iFolder
    , FEvent* iEvent
)
{
    FEvent ev = iFolder.RenderImageCache( *this );
    Dummy_OP( 1, &ev, iEvent );

    return  ULIS_NO_ERROR;
}

ulError
FContext::RenderLayerText(
      FLayerText& iText
    , FEvent* iEvent
)
{
    FEvent ev = iText.RenderImageCache( *this );
    Dummy_OP( 1, &ev, iEvent );
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
    TRoot< ILayer >* currentRoot = &iStack;

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

            currentRoot->AddChild(
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
                , false
                , true
                , FColor::Transparent
                , op.GetImageWidth()
                , op.GetImageHeight()
                , layerStackFormat
                , blendMode
                , isAlphaLocked ? eAlphaMode::Alpha_Top : eAlphaMode::Alpha_Normal
                , opacity
                , currentRoot
            );

            currentRoot->AddChild( layerFolder );

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
*/
ULIS_NAMESPACE_END

