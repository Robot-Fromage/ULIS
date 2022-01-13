// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Text.cpp
* @author       Clement Berthaud
* @brief        Text application for ULIS.
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
#include <blend2d.h>
using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt );

    int w = 480;
    int h = 480;
    FBlock canvas( w, h, Format_RGBA8 );
    ctx.Clear( canvas );
    ctx.Finish();

    BLImage img( w, h, BL_FORMAT_PRGB32 );
    {
        BLContext blctx( img );
        blctx.setCompOp(BL_COMP_OP_SRC_COPY);
        blctx.fillAll();
        BLPath path;
        path.moveTo(26, 31);
        path.cubicTo(642, 132, 587, -136, 25, 464);
        path.cubicTo(882, 404, 144, 267, 27, 31);
        blctx.setCompOp(BL_COMP_OP_SRC_OVER);
        blctx.setFillStyle(BLRgba32(0xFFFFFFFF));
        blctx.fillPath(path);
        blctx.end();
    }

    BLImageData data;
    BLResult err = img.getData( &data );

    QApplication app( argc, argv );
    QWidget* widget = new QWidget();
    //QImage* image = new QImage( canvas.Bits(), canvas.Width(), canvas.Height(), canvas.BytesPerScanLine(), QImage::Format_RGBA8888 );
    QImage* image = new QImage( (uint8*)data.pixelData, data.size.w, data.size.h, data.stride, QImage::Format_RGBA8888 );
    QPixmap pixmap = QPixmap::fromImage( *image );
    QLabel* label = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();

    int exit_code = app.exec();
    delete  label;
    delete  image;
    delete  widget;
    return  exit_code;
}

