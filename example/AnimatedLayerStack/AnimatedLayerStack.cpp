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

    FAnimatedLayerStack* stack;
    ULAAssignStack( stack, w, h, fmt )
    ULADef( SetName( "stack" ) )
    [
        ULACreateChild( FAnimatedLayerImage )
        ULADef( SetName( "layer0" ) )
    ];

    FAnimatedLayerImage& layer0 = stack->Find< FAnimatedLayerImage >( "layer0" );
    for( int i = 0; i < 5; ++i )
        layer0.PushNewCel();

    ctx.Clear( canvas, canvas.Rect() );
    ctx.Finish();

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

