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
    FContext ctx( queue, fmt, PerformanceIntent_AVX );

    FBlock canvas( 800, 600, fmt );
    FGradient gradient( fmt );
    gradient.ColorSteps().PushBack( FColorStep::MakeShared( 0.f, FColor::Red ) );
    gradient.ColorSteps().PushBack( FColorStep::MakeShared( 0.5f, FColor::Yellow ) );
    gradient.ColorSteps().PushBack( FColorStep::MakeShared( 0.6f, FColor::Green ) );
    gradient.ColorSteps().PushBack( FColorStep::MakeShared( 1.f, FColor::Blue ) );
    gradient.AlphaSteps().PushBack( FAlphaStep::MakeShared( 0.f, 1.f ) );
    gradient.AlphaSteps().PushBack( FAlphaStep::MakeShared( 1.f, 1.f ) );
    FSanitizedGradient grad = gradient.Sanitized( fmt );
    ctx.RasterGradient(
          canvas
        , FVec2I( canvas.Rect().Size().x / 2, 0 )
        , FVec2I( canvas.Rect().Size().x, 0 )
        , grad
        , 2.f / 255.f
        , Gradient_Reflected
        , canvas.Rect()
        , FSchedulePolicy::MultiScanlines
        , 0
        , nullptr
        , nullptr
    );
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

    return  exit_code;
}

