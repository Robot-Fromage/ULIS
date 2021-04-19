// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Blend.cpp
* @author       Clement Berthaud
* @brief        Blend application for ULIS.
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
    FContext ctx( queue, fmt );

    FBlock canvas( 1024, 1024, fmt );
    FBlock square( 16, 16, fmt );
    FVec2I p0( canvas.Rect().Size() / 2 );
    FVec2I p1( canvas.Rect().Size() );
    ctx.Clear( canvas );
    ctx.Flush();
    ctx.Fill( square, FRectI( 0, 0, 8, 8 ), FColor::RGB( 150, 220, 255 ) );
    ctx.Fill( square, FRectI( 8, 8, 8, 8 ), FColor::RGB( 150, 220, 255 ) );
    ctx.Fill( square, FRectI( 0, 8, 8, 8 ), FColor::White );
    ctx.Fill( square, FRectI( 8, 0, 8, 8 ), FColor::White );
    ctx.Flush();
    ctx.Fence();
    ctx.DrawCircleAndresAA( canvas, p0, 500, FColor::Black, true );
    ctx.Finish();
    ctx.BlendTiled( square, canvas, square.Rect(), canvas.Rect(), FVec2I( 0 ), Blend_Behind, Alpha_Normal, 1.f, FSchedulePolicy::MultiScanlines );
    ctx.Finish();

    FBlock mipMap;
    ctx.XBuildMipMap( canvas, mipMap, -1, canvas.Rect(), Resampling_Bicubic );
    ctx.Finish();

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( mipMap.Bits()
                                        , mipMap.Width()
                                        , mipMap.Height()
                                        , mipMap.BytesPerScanLine()
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

