// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        LayerStack with vector layer application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <chrono>

#include "LayerStackRenderer.h"

using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    // Core
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt );
    FFontEngine fontEngine;

    // Document data
    uint16 w = 1024;
    uint16 h = 1024;
    FFont font( fontEngine, "Segoe UI", "Bold" );
    FBlock canvas( w, h, fmt );
    FLayerStack* stack;
    ULAssignStack( stack, w, h, fmt )
    ULDef( SetName( "stack" ) )
    [
        ULCreateChild( FLayerText )
        ULDef( SetName( "Title" ) )
        ULDef( SetTextColor( FColor::White ) )
        ULDef( SetFontSize( 38 ) )
        ULDef( SetTranslation( 80, 80 ) )
        ULDef( SetText( L"LayerStack with Vector Layer !" ) )
    ]
    [
        ULCreateChild( FLayerVector )
        ULDef( SetName( "Vector" ) )
    ]
    [
        ULCreateChild( FLayerImage )
        ULDef( SetName( "Background" ) )
    ];

    // Process Document
    FBlock& background = *( stack->Find< FLayerImage >( "Background" ) ).Block();
    ctx.Fill( background, FColor::RGBA8( 50, 50, 50, 255 ) );
    ctx.Clear( canvas, canvas.Rect() );
    ctx.Finish();

    // Render Document
    auto startTime = std::chrono::steady_clock::now();
    {
        FLayerStackRenderer::Render( ctx, *stack, canvas );
        ctx.Finish();
    }
    auto endTime = std::chrono::steady_clock::now();

    // Quick Benchmark
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();
    std::cout << "ULIS Layer Stack Flatten took: " << delta << "ms." << std::endl;

    // Render to Screen
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

    // Main Loop
    int exit_code = app.exec();

    // Cleanup
    delete  label;
    delete  image;
    delete  widget;
    delete  stack;
    return  exit_code;
}

