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

    auto startTime = std::chrono::steady_clock::now();
    {
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

