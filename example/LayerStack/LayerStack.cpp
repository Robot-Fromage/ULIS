// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.cpp
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <chrono>
using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt );

    uint16 w = 1024;
    uint16 h = 1024;
    FBlock canvas( w, h, fmt );

    auto onNodeAdded_print = []( const TRoot< ILayer >* iRoot, const TNode< ILayer >* iNode ) {
        ULIS_DEBUG_PRINTF( "Node added to container " << iRoot->Self().Name().Data() << ": " << iNode->Self().Name().Data() );
    };

    auto onNodeRemoved_print = []( const TRoot< ILayer >* iRoot, const TNode< ILayer >* iNode, bool iInDestructor ) {
        // Warning: can't use iRoot->Self().Name().Data() if called during destructor.
        ULIS_DEBUG_PRINTF( "Node removed from container " << ": " << iNode->Self().Name().Data() );
    };

    FFontEngine fontEngine;
    FFont font( fontEngine, "Segoe UI", "Bold" );

    FLayerStack* stack;
    ULAssignStack( stack, w, h, fmt )
    ULDef( SetName( "stack" ) )
    ULDef( AddOrSetUserData( new FTextUserData( "UserData" ) ) )
    ULDef( FOnNodeAdded::SetDelegate( onNodeAdded_print ) )
    ULDef( FOnNodeRemoved::SetDelegate( onNodeRemoved_print ) )
    [
        ULCreateChild( FLayerImage )
        ULDef( SetName( "0i" ) )
    ]
    [
        ULCreateChild( FLayerImage )
        ULDef( SetName( "1i" ) )
    ]
    [
        ULCreateChild( FLayerFolder )
        ULDef( SetName( "2f" ) )
        [
            ULCreateChild( FLayerText )
            ULDef( SetName( "2_0t" ) )
            ULDef( SetTextColor( FColor::White ) )
            ULDef( SetFontSize( 38 ) )
            ULDef( SetTranslation( 530, 530 ) )
        ]
        [
            ULCreateChild( FLayerText )
            ULDef( SetName( "2_0t" ) )
            ULDef( SetTextColor( FColor::White ) )
            ULDef( SetFontSize( 38 ) )
            ULDef( SetTranslation( 530, 580 ) )
        ]
        [
            ULCreateChild( FLayerImage )
            ULDef( SetName( "2_1i" ) )
        ]
        [
            ULCreateChild( FLayerImage )
            ULDef( SetName( "2_2i" ) )
        ]
    ]
    [
        ULCreateChild( FLayerImage )
        ULDef( SetName( "3i" ) )
    ];

    FBlock& img0 = *( stack->Find< FLayerImage >( "0i" ) ).Block();
    FBlock& img1 = *( stack->Find< FLayerImage >( "1i" ) ).Block();
    FBlock& img2_0 = *( stack->Find< FLayerFolder >( "2f" ).Find< FLayerImage >( "2_1i" ) ).Block();
    FBlock& img2_1 = *( stack->Find< FLayerFolder >( "2f" ).Find< FLayerImage >( "2_2i" ) ).Block();
    FBlock& img3 = *( stack->Find< FLayerImage >( "3i" ) ).Block();

    FLayerImage& layer = stack->Find< FLayerFolder >( "2f" ).Find< FLayerImage >( "2_1i" );
    layer.NotifyChange();

    ctx.Clear( img0 );
    ctx.Clear( img1 );
    ctx.Clear( img2_0 );
    ctx.Clear( img2_1 );
    ctx.Clear( img3 );
    ctx.Finish();

    ctx.Fill( img0, FColor::Red, FRectI( 64, 64, 64, 64 ) );
    ctx.Fill( img1, FColor::Blue, FRectI( 32, 32, 512, 512 ) );
    ctx.Fill( img2_0, FColor::Green, FRectI( 512, 0, 512, 1024 ) );
    ctx.Fill( img2_1, FColor::Yellow, FRectI( 256, 256, 512, 512 ) );
    ctx.Fill( img3, FColor::Black );
    ctx.Clear( canvas, canvas.Rect() );
    ctx.Finish();

    // Here is an example reference implementation of flatten
    // Flatten is not provided by default anymore because its implementation depends
    // on too many variables that are the responsibility of the host application
    // for instance, handling various flatten strategy: wether we need to blend many tiles
    // or the whole picture, and the handling of extra custom layer types such as vector layers
    // which require their own extra set of parameters. Same thing goes for the policy strategies that
    // can be fine tuned this way.
    // Also, this structure allows to implement more efficient caching systems, although it delegates
    // the responsibility to the application, it was found that deciding upon a caching framework would
    // prove to be inefficient for certain cases.
    auto startTime = std::chrono::steady_clock::now();
    {
        struct FLayerStackFlattener {

            static FEvent RenderImage(
                  FContext& iCtx
                , FLayerImage& iLayer
                , FBlock& ioBlock
                , const FRectI& iRect = FRectI::Auto
                , const FVec2I& iPos = FVec2I( 0 )
                , const FSchedulePolicy& iPolicy = FSchedulePolicy()
                , uint32 iNumWait = 0
                , const FEvent* iWaitList = nullptr
            )
            {
                FEvent ev;
                ulError err = iCtx.Blend(
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
                    , &ev
                );
                ULIS_ASSERT( !err, "Error during layer image blend" );
                return  ev;
            }

            static FEvent RenderFolder(
                  FContext& iCtx
                , FLayerFolder& iLayer
                , FBlock& ioBlock
                , const FRectI& iRect = FRectI::Auto
                , const FVec2I& iPos = FVec2I( 0 )
                , const FSchedulePolicy& iPolicy = FSchedulePolicy()
                , uint32 iNumWait = 0
                , const FEvent* iWaitList = nullptr
            )
            {
            }

            static FEvent RenderText(
                  FContext& iCtx
                , FLayerText& iLayer
                , FBlock& ioBlock
                , const FRectI& iRect = FRectI::Auto
                , const FVec2I& iPos = FVec2I( 0 )
                , const FSchedulePolicy& iPolicy = FSchedulePolicy()
                , uint32 iNumWait = 0
                , const FEvent* iWaitList = nullptr
            )
            {
                // This is the kind of part that can be cached according to various strategies.
                // By identifying when the layer text data changed ( string, color, pos, etc... )
                // We can invalidate a parallel caching structure and preallocate the rendered text
                // block, avoiding a reallocation of the temp render block for each flatten pass.
                // But it's not straightforward to decide within the ULIS implementation because we
                // can't know the typical use case beforehand, wether we need to blend the whole stack rect
                // a few times, or sub-tiles many many times, or both. The text render cannot take
                // tiles into account, but blending the temp block can.
                // Also, forcing a cached render of the text within the stack would have a greater
                // impact on memory whereas it's not always needed.
                FRectI text_rect = FContext::TextMetrics( iLayer.Text(), iLayer.Font(), iLayer.FontSize(), iLayer.Matrix() );
                FRectI dst_rect = FRectI::FromPositionAndSize( iPos, iRect.Size() );
                FRectI dst_roi = text_rect & dst_rect;
                FRectI src_roi = FRectI::FromPositionAndSize( FVec2I( 0 ), dst_roi.Size() );
                FMat3F fixedMatrix = iLayer.Matrix() * FMat3F::MakeTranslationMatrix( -text_rect.x, -text_rect.y );

                FBlock* temp = new FBlock( text_rect.w, text_rect.h, iCtx.Format() );

                FEvent eventClear;
                iCtx.Clear( *temp, FRectI::Auto, FSchedulePolicy::CacheEfficient, 0, nullptr, &eventClear );
                FEvent eventText;
                if( iLayer.IsAntiAliased() ) {
                    iCtx.RasterTextAA( *temp, iLayer.Text(), iLayer.Font(), iLayer.FontSize(), fixedMatrix, iLayer.TextColor(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventText );
                } else {
                    iCtx.RasterText( *temp, iLayer.Text(), iLayer.Font(), iLayer.FontSize(), fixedMatrix, iLayer.TextColor(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventText );
                }

                FEvent eventBlend(
                    FOnEventComplete(
                        [temp]( const FRectI& ) {
                            delete  temp;
                        }
                    )
                );
                TArray< FEvent > events( iNumWait + 1 );
                for( uint32 i = 0; i < iNumWait; ++i )
                    events[i] = iWaitList[i];
                events[ iNumWait ] = eventText;

                ulError err = iCtx.Blend(
                      *temp
                    , ioBlock
                    , src_roi
                    , iPos
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

            static FEvent RenderStack(
                  FContext& iCtx
                , FLayerStack& iStack
                , FBlock& ioBlock
                , const FRectI& iRect = FRectI::Auto
                , const FVec2I& iPos = FVec2I( 0 )
                , const FSchedulePolicy& iPolicy = FSchedulePolicy()
                , uint32 iNumWait = 0
                , const FEvent* iWaitList = nullptr
            )
            {
                FEvent ev;
                iCtx.Clear( ioBlock, FRectI::Auto, FSchedulePolicy::CacheEfficient, iNumWait, iWaitList, &ev );
                const int max = static_cast< int >( iStack.Children().Size() ) - 1;
                bool bFirst = true;
                for( int i = max; i >= 0; --i ) {
                    ILayer* layer = dynamic_cast< ILayer* >( &( iStack.Children()[i]->Self() ) );
                    ULIS_ASSERT( layer, "Bad children cannot be cast to layer" );
                    switch( layer->TypeID() ) {
                        case FLayerStack::StaticTypeID(): {
                            FLayerStack* stack = dynamic_cast< FLayerStack* >( layer );
                            ULIS_ASSERT( false, "Should not be able to have a stack embedded in a stack" );
                            break;
                        }
                        case FLayerImage::StaticTypeID(): {
                            FLayerImage* image = dynamic_cast< FLayerImage* >( layer );
                            ULIS_ASSERT( image, "Layer cannot be cast to image, this is inconsistent with the StaticTypeID !" );
                            ev = FLayerStackFlattener::RenderImage( iCtx, *image, ioBlock, iRect, iPos, iPolicy, 1, &ev );
                            break;
                        }
                        case FLayerFolder::StaticTypeID(): {
                            FLayerFolder* folder = dynamic_cast< FLayerFolder* >( layer );
                            ULIS_ASSERT( folder, "Layer cannot be cast to folder, this is inconsistent with the StaticTypeID !" );
                            ev = FLayerStackFlattener::RenderFolder( iCtx, *folder, ioBlock, iRect, iPos, iPolicy, 1, &ev );
                            break;
                        }
                        case FLayerText::StaticTypeID(): {
                            FLayerText* text = dynamic_cast< FLayerText* >( layer );
                            ULIS_ASSERT( text, "Layer cannot be cast to text, this is inconsistent with the StaticTypeID !" );
                            ev = FLayerStackFlattener::RenderText( iCtx, *text, ioBlock, iRect, iPos, iPolicy, 1, &ev );
                            break;
                        }
                    }
                }
                return  ev;
            }

            static ulError Flatten(
                  FContext& iCtx
                , FLayerStack& iStack
                , FBlock& oDestination
                , const FRectI& iSourceRect = FRectI::Auto
                , const FVec2I& iPosition = FVec2I( 0 )
                , const FSchedulePolicy& iPolicy = FSchedulePolicy::MultiScanlines
                , uint32 iNumWait = 0
                , const FEvent* iWaitList = nullptr
                , FEvent* iEvent = nullptr
            )
            {
                ULIS_ASSERT_RETURN_ERROR(
                      iCtx.Format() == iStack.Format() == oDestination.Format()
                    , "Formats mismatch."
                    , iCtx.FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_ERROR_FORMATS_MISMATCH )
                );

                // Sanitize geometry
                const FRectI src_rect = iStack.Rect();
                const FRectI dst_rect = oDestination.Rect();
                const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
                const FRectI dst_aim = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
                const FRectI dst_roi = dst_aim & dst_rect;

                // Check no-op
                if( dst_roi.Sanitized().Area() <= 0 )
                    return  iCtx.FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

                FEvent ev = FLayerStackFlattener::RenderStack( iCtx, iStack, oDestination, src_roi, dst_roi.Position(), iPolicy, iNumWait, iWaitList );
                iCtx.Dummy_OP( 1, &ev, iEvent );

                return  ULIS_NO_ERROR;
            }
        };
        //ctx.Flatten( *stack, canvas );
        ctx.Finish();
    }
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();
    std::cout << "ULIS Layer Stack Flatten took: " << delta << "ms." << std::endl;
    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( canvas.Bits()
                                        , canvas.Width()
                                        , canvas.Height()
                                        , canvas.BytesPerScanLine()
                                        , QImage::Format_RGBA8888 );
    QPixmap         pixmap  = QPixmap::fromImage( *image );
    QLabel*         label   = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();
    int exit_code = app.exec();
    delete  label;
    delete  image;
    delete  widget;
    delete  stack;
    return  exit_code;
}

