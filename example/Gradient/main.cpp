// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Blend.cpp
* @author       Clement Berthaud
* @brief        Blend application for ULIS.
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
    FBlock canvas( 400, 400, fmt );
    FGradient gradient( fmt );
    gradient.AddColorStep( 0.f, FColor::Blue );
    gradient.AddColorStep( 1.f, FColor::Red );
    FSanitizedGradient grad = gradient.Sanitized( fmt );
    FVec2I p0( 0 );
    FVec2I p1( 400, 400 );
    ctx.RasterGradient( canvas, p0, p1, grad, 4.f / 255.f, Gradient_Reflected );
    ctx.Finish();

    FBlock proxy( 800, 600, Format_RGBA8 );
    ctx.ConvertFormat( canvas, proxy );
    ctx.Finish();

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( proxy.Bits()
                                        , proxy.Width()
                                        , proxy.Height()
                                        , proxy.BytesPerScanLine()
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

    return  exit_code;
}

