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
    eFormat fmt = Format_G8;
    FContext ctx( queue, fmt, PerformanceIntent_AVX );
    FBlock block( 1024, 1024, fmt );

    ctx.Clear( block );
    ctx.Finish();

    ctx.DrawCircleAA( block, FVec2I( 512 ), 510, FColor::Red, true );
    ctx.Finish();

    for( int i = 0; i < 2000; ++i ) {
        ctx.FillPreserveAlpha( block, FColor::Red );
        ctx.Finish();
    }

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( block.Bits()
                                        , block.Width()
                                        , block.Height()
                                        , block.BytesPerScanLine()
                                        , QImage::Format_Grayscale8 );
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

