// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ClearFillCopy.cpp
* @author       Clement Berthaud
* @brief        ClearFillCopy application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBAF;
    FContext ctx( queue, fmt );

    ::ULIS::FBlock block1( 64, 64, fmt );
    ::ULIS::FBlock block2( 1024, 1024, fmt );
    ::ULIS::FBlock block3( 1024, 1024, Format_RGBA8 );

    ctx.Fill( block1, FColor::RGBA8( 15, 51, 112 ) );
    ctx.Fill( block2, FColor::Blue );
    ctx.Finish();
    ctx.Blend( block1, block2, FRectI::Auto, FVec2I( 64, 64 ) );
    ctx.Finish();
    ctx.ConvertFormat( block2, block3 );
    ctx.Finish();

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( block3.Bits()
                                        , block3.Width()
                                        , block3.Height()
                                        , block3.BytesPerScanLine()
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

