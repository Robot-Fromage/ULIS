// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.cpp
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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
    FContext ctx( queue, fmt, PerformanceIntent_Max );

    uint16 w = 1024;
    uint16 h = 1024;
    FBlock canvas( w, h, fmt );

    /*
    FLayerBuilder< FLayerStack >::Assign( &stack, w, h, fmt )
    .AddChild(
        FLayerBuilder< FLayerImage >::Create()
        .Def( [&]( auto i ){ i->SetName( "Image0" ) ; } )
    );
    */

    FLayerStack* stack;
    ULAssignStack( stack, w, h, fmt )
    [
        ULCreateChild( FLayerImage, "Image0" )
        ULDef( SetColor( FColor::Black ) )
        ULDef( SetBlendMode( Blend_Multiply ) )
    ][
        ULCreateChild( FLayerImage, "Image1" )
        ULDef( SetColor( FColor::Red ) )
        ULDef( SetBlendMode( Blend_Normal ) )
    ][
        ULCreateChild( FLayerFolder, "Folder0" )
        ULDef( SetColor( FColor::Blue ) )
        ULDef( SetOpacity( 0.5f ) )
        [
            ULCreateChild( FLayerImage, "Image0_0" )
            ULDef( SetColor( FColor::White ) )
            ULDef( SetBlendMode( Blend_Overlay ) )
        ][
            ULCreateChild( FLayerImage, "Image0_1" )
            ULDef( SetColor( FColor::White ) )
            ULDef( SetBlendMode( Blend_Normal ) )
        ]
    ];

    FLayerImage& temp = stack->Find< FLayerFolder >( "Folder0" )
        .Find< FLayerImage >( "Image0_0" );

    {
        ctx.Clear( canvas, canvas.Rect() );
        ctx.Finish();
    }

    auto startTime = std::chrono::steady_clock::now();
    {
        /*
        ctx.Flatten(
              layerStack
            , canvas
        );
        ctx.Finish();
        */
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

    // Get error code. This function returns when all Qt Windows are closed.
    int exit_code = app.exec();

    // Delete Qt Components.
    delete  label;
    delete  image;
    delete  widget;

    // Delete ULIS Components
    delete  stack;

    // Return exit code.
    return  exit_code;
}

