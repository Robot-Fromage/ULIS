// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStackRenderer.CPP
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "LayerStackRenderer.h"

//static
::ULIS::FEvent
FLayerStackRenderer::RenderImage(
      ::ULIS::FContext& iCtx
    , ::ULIS::FLayerImage& iLayer
    , ::ULIS::FBlock& ioBlock
    , const ::ULIS::FRectI& iRect
    , const ::ULIS::FVec2I& iPos
    , const ::ULIS::FSchedulePolicy& iPolicy
    , ::ULIS::uint32 iNumWait
    , const ::ULIS::FEvent* iWaitList
)
{
    ::ULIS::FEvent eventBlend;
    ::ULIS::ulError err = iCtx.Blend(
          *iLayer.Block()
        , ioBlock
        , iRect
        , iPos
        , iLayer.BlendMode()
        , iLayer.AlphaMode()
        , iLayer.Opacity()
        , iPolicy
        , iNumWait
        , iWaitList
        , &eventBlend
    );
    ULIS_ASSERT( !err, "Error during layer image blend" );
    return  eventBlend;
}

//static
::ULIS::FEvent
FLayerStackRenderer::RenderFolder(
      ::ULIS::FContext& iCtx
    , ::ULIS::FLayerFolder& iLayer
    , ::ULIS::FBlock& ioBlock
    , const ::ULIS::FRectI& iRect
    , const ::ULIS::FVec2I& iPos
    , const ::ULIS::FSchedulePolicy& iPolicy
    , ::ULIS::uint32 iNumWait
    , const ::ULIS::FEvent* iWaitList
)
{
    ::ULIS::FRectI src_rect = ::ULIS::FRectI::FromPositionAndSize( ::ULIS::FVec2I( 0 ), iRect.Size() );
    ::ULIS::FBlock* temp = new ::ULIS::FBlock( src_rect.w, src_rect.h, iCtx.Format() );
    ::ULIS::FEvent ev;
    iCtx.Clear( *temp, ::ULIS::FRectI::Auto, ::ULIS::FSchedulePolicy::CacheEfficient, 0, nullptr, &ev );
    const int max = static_cast< int >( iLayer.Children().Size() ) - 1;
    for( int i = max; i >= 0; --i ) {
        ::ULIS::ILayer* layer = dynamic_cast< ::ULIS::ILayer* >( &( iLayer.Children()[i]->Self() ) );
        ULIS_ASSERT( layer, "Bad children cannot be cast to layer" );
        switch( layer->TypeID() ) {
            case ::ULIS::FLayerStack::StaticTypeID(): {
                ::ULIS::FLayerStack* stack = dynamic_cast< ::ULIS::FLayerStack* >( layer );
                ULIS_ASSERT( false, "Should not be able to have a stack embedded in a stack" );
                break;
            }
            case ::ULIS::FLayerImage::StaticTypeID(): {
                ::ULIS::FLayerImage* image = dynamic_cast< ::ULIS::FLayerImage* >( layer );
                ULIS_ASSERT( image, "Layer cannot be cast to image, this is inconsistent with the StaticTypeID !" );
                ev = FLayerStackRenderer::RenderImage( iCtx, *image, *temp, iRect, ::ULIS::FVec2I( 0 ), iPolicy, 1, &ev );
                break;
            }
            case ::ULIS::FLayerFolder::StaticTypeID(): {
                ::ULIS::FLayerFolder* folder = dynamic_cast< ::ULIS::FLayerFolder* >( layer );
                ULIS_ASSERT( folder, "Layer cannot be cast to folder, this is inconsistent with the StaticTypeID !" );
                ev = FLayerStackRenderer::RenderFolder( iCtx, *folder, *temp, iRect, ::ULIS::FVec2I( 0 ), iPolicy, 1, &ev );
                break;
            }
            case ::ULIS::FLayerText::StaticTypeID(): {
                ::ULIS::FLayerText* text = dynamic_cast< ::ULIS::FLayerText* >( layer );
                ULIS_ASSERT( text, "Layer cannot be cast to text, this is inconsistent with the StaticTypeID !" );
                ev = FLayerStackRenderer::RenderText( iCtx, *text, *temp, iRect, ::ULIS::FVec2I( 0 ), iPolicy, 1, &ev );
                break;
            }
            case FLayerVector::StaticTypeID(): {
                FLayerVector* vector = dynamic_cast< FLayerVector* >( layer );
                ULIS_ASSERT( vector, "Layer cannot be cast to vector, this is inconsistent with the StaticTypeID !" );
                ev = FLayerStackRenderer::RenderVector( iCtx, *vector, *temp, iRect, ::ULIS::FVec2I( 0 ), iPolicy, 1, &ev );
                break;
            }
        }
    }

    ::ULIS::FEvent eventBlend( ::ULIS::FOnEventComplete( [temp]( const ::ULIS::FRectI& ) { delete  temp; } ) );
    ::ULIS::TArray< ::ULIS::FEvent > events( iNumWait + 1 );
    for( ::ULIS::uint32 i = 0; i < iNumWait; ++i )
        events[i] = iWaitList[i];
    events[ iNumWait ] = ev;

    ::ULIS::ulError err = iCtx.Blend(
          *temp
        , ioBlock
        , src_rect
        , iPos
        , iLayer.BlendMode()
        , iLayer.AlphaMode()
        , iLayer.Opacity()
        , iPolicy
        , iNumWait + 1
        , &events[0]
        , &eventBlend
    );
    ULIS_ASSERT( !err, "Error during layer folder blend" );
    return  eventBlend;
}

//static
::ULIS::FEvent
FLayerStackRenderer::RenderText(
      ::ULIS::FContext& iCtx
    , ::ULIS::FLayerText& iLayer
    , ::ULIS::FBlock& ioBlock
    , const ::ULIS::FRectI& iRect
    , const ::ULIS::FVec2I& iPos
    , const ::ULIS::FSchedulePolicy& iPolicy
    , ::ULIS::uint32 iNumWait
    , const ::ULIS::FEvent* iWaitList
)
{
    ::ULIS::FRectI text_rect = ::ULIS::FContext::TextMetrics( iLayer.Text(), iLayer.Font(), iLayer.FontSize(), iLayer.Matrix() );
    ::ULIS::FRectI dst_rect = ::ULIS::FRectI::FromPositionAndSize( iPos, iRect.Size() );
    ::ULIS::FRectI dst_roi = text_rect & dst_rect;
    ::ULIS::FRectI src_roi = ::ULIS::FRectI::FromPositionAndSize( ::ULIS::FVec2I( 0 ), dst_roi.Size() );

    ::ULIS::FMat3F fixedMatrix = iLayer.Matrix() * ::ULIS::FMat3F::MakeTranslationMatrix( float( -text_rect.x ), float( -text_rect.y ) );

    ::ULIS::FBlock* temp = new ::ULIS::FBlock( src_roi.w, src_roi.h, iCtx.Format() );

    ::ULIS::FEvent eventClear;
    iCtx.Clear( *temp, ::ULIS::FRectI::Auto, ::ULIS::FSchedulePolicy::CacheEfficient, 0, nullptr, &eventClear );
    ::ULIS::FEvent eventText;
    if( iLayer.IsAntiAliased() ) {
        iCtx.RasterTextAA( *temp, iLayer.Text(), iLayer.Font(), iLayer.FontSize(), fixedMatrix, iLayer.TextColor(), ::ULIS::FSchedulePolicy::MonoChunk, 1, &eventClear, &eventText );
    } else {
        iCtx.RasterText( *temp, iLayer.Text(), iLayer.Font(), iLayer.FontSize(), fixedMatrix, iLayer.TextColor(), ::ULIS::FSchedulePolicy::MonoChunk, 1, &eventClear, &eventText );
    }

    ::ULIS::FEvent eventBlend( ::ULIS::FOnEventComplete( [temp]( const ::ULIS::FRectI& ) { delete  temp; } ) );
    ::ULIS::TArray< ::ULIS::FEvent > events( iNumWait + 1 );
    for( ::ULIS::uint32 i = 0; i < iNumWait; ++i )
        events[i] = iWaitList[i];
    events[ iNumWait ] = eventText;

    ::ULIS::ulError err = iCtx.Blend(
          *temp
        , ioBlock
        , src_roi
        , text_rect.Position()
        , iLayer.BlendMode()
        , iLayer.AlphaMode()
        , iLayer.Opacity()
        , iPolicy
        , iNumWait + 1
        , &events[0]
        , &eventBlend
    );
    ULIS_ASSERT( !err, "Error during layer text blend" );
    return  eventBlend;
}

//static
::ULIS::FEvent
FLayerStackRenderer::RenderVector(
      ::ULIS::FContext& iCtx
    , FLayerVector& iLayer
    , ::ULIS::FBlock& ioBlock
    , const ::ULIS::FRectI& iRect
    , const ::ULIS::FVec2I& iPos
    , const ::ULIS::FSchedulePolicy& iPolicy
    , ::ULIS::uint32 iNumWait
    , const ::ULIS::FEvent* iWaitList
)
{
    ULIS_DEBUG_PRINTF( "FLayerStackRenderer::RenderVector not implemented yet !" )
    ::ULIS::FEvent ev;
    iCtx.Dummy_OP( iNumWait, iWaitList, &ev );
    return  ev;
}

//static
::ULIS::FEvent
FLayerStackRenderer::RenderStack(
      ::ULIS::FContext& iCtx
    , ::ULIS::FLayerStack& iStack
    , ::ULIS::FBlock& ioBlock
    , const ::ULIS::FRectI& iRect
    , const ::ULIS::FVec2I& iPos
    , const ::ULIS::FSchedulePolicy& iPolicy
    , ::ULIS::uint32 iNumWait
    , const ::ULIS::FEvent* iWaitList
)
{
    ::ULIS::FEvent ev;
    iCtx.Clear( ioBlock, ::ULIS::FRectI::Auto, ::ULIS::FSchedulePolicy::CacheEfficient, iNumWait, iWaitList, &ev );
    const int max = static_cast< int >( iStack.Children().Size() ) - 1;
    for( int i = max; i >= 0; --i ) {
        ::ULIS::ILayer* layer = dynamic_cast< ::ULIS::ILayer* >( &( iStack.Children()[i]->Self() ) );
        ULIS_ASSERT( layer, "Bad children cannot be cast to layer" );
        switch( layer->TypeID() ) {
            case ::ULIS::FLayerStack::StaticTypeID(): {
                ::ULIS::FLayerStack* stack = dynamic_cast< ::ULIS::FLayerStack* >( layer );
                ULIS_ASSERT( false, "Should not be able to have a stack embedded in a stack" );
                break;
            }
            case ::ULIS::FLayerImage::StaticTypeID(): {
                ::ULIS::FLayerImage* image = dynamic_cast< ::ULIS::FLayerImage* >( layer );
                ULIS_ASSERT( image, "Layer cannot be cast to image, this is inconsistent with the StaticTypeID !" );
                ev = FLayerStackRenderer::RenderImage( iCtx, *image, ioBlock, iRect, iPos, iPolicy, 1, &ev );
                break;
            }
            case ::ULIS::FLayerFolder::StaticTypeID(): {
                ::ULIS::FLayerFolder* folder = dynamic_cast< ::ULIS::FLayerFolder* >( layer );
                ULIS_ASSERT( folder, "Layer cannot be cast to folder, this is inconsistent with the StaticTypeID !" );
                ev = FLayerStackRenderer::RenderFolder( iCtx, *folder, ioBlock, iRect, iPos, iPolicy, 1, &ev );
                break;
            }
            case ::ULIS::FLayerText::StaticTypeID(): {
                ::ULIS::FLayerText* text = dynamic_cast< ::ULIS::FLayerText* >( layer );
                ULIS_ASSERT( text, "Layer cannot be cast to text, this is inconsistent with the StaticTypeID !" );
                ev = FLayerStackRenderer::RenderText( iCtx, *text, ioBlock, iRect, iPos, iPolicy, 1, &ev );
                break;
            }
            case FLayerVector::StaticTypeID(): {
                FLayerVector* vector = dynamic_cast< FLayerVector* >( layer );
                ULIS_ASSERT( vector, "Layer cannot be cast to vector, this is inconsistent with the StaticTypeID !" );
                ev = FLayerStackRenderer::RenderVector( iCtx, *vector, ioBlock, iRect, iPos, iPolicy, 1, &ev );
                break;
            }
        }
    }
    return  ev;
}

//static
::ULIS::ulError
FLayerStackRenderer::Render(
      ::ULIS::FContext& iCtx
    , ::ULIS::FLayerStack& iStack
    , ::ULIS::FBlock& oDestination
    , const ::ULIS::FRectI& iSourceRect
    , const ::ULIS::FVec2I& iPosition
    , const ::ULIS::FSchedulePolicy& iPolicy
    , ::ULIS::uint32 iNumWait
    , const ::ULIS::FEvent* iWaitList
    , ::ULIS::FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR(
            ( iCtx.Format() == iStack.Format() ) && ( iCtx.Format() == oDestination.Format() )
        , "Formats mismatch."
        , iCtx.FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const ::ULIS::FRectI src_rect = iStack.Rect();
    const ::ULIS::FRectI dst_rect = oDestination.Rect();
    const ::ULIS::FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const ::ULIS::FRectI dst_aim = ::ULIS::FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    const ::ULIS::FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Sanitized().Area() <= 0 )
        return  iCtx.FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    ::ULIS::FEvent ev = FLayerStackRenderer::RenderStack( iCtx, iStack, oDestination, src_roi, dst_roi.Position(), iPolicy, iNumWait, iWaitList );
    iCtx.Dummy_OP( 1, &ev, iEvent );

    return  ULIS_NO_ERROR;
}

