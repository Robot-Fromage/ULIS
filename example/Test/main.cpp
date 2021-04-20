// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         All.cpp
* @author       Clement Berthaud
* @brief        Example of all ctx features for ULIS.
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
    FFontEngine fontEngine;
    FFont font( fontEngine, "Segoe UI", "Regular" );

    // Gather start time to output the time it took to perform the blend composition
    auto startTime = std::chrono::steady_clock::now();

    int size = 64;
    /*
    FBlock canvas( size, size, fmt );
    FBlock paper( 4, 3, fmt );
    FColor background = FColor::RGB( 28, 25, 31, 240 );
    FColor foreground = FColor::RGB( 170, 146, 64 );
    ctx.Fill( paper, background );
    ctx.Finish();
    ctx.DrawLine( paper, FVec2I(), FVec2I(), foreground );
    ctx.DrawLine( paper, FVec2I( 1, 2 ), FVec2I( 3, 0 ), foreground );
    ctx.Finish();
    ctx.BlendTiled( paper, canvas, FRectI( 1, 0, 3, 3 ), FRectI( 1, 0, size, size ), FVec2I(), Blend_Normal, Alpha_Normal, 1.f, FSchedulePolicy::MonoChunk );
    ctx.BlendTiled( paper, canvas, FRectI( 0, 0, 1, 3 ), FRectI( 0, 0, 1, size ) );
    ctx.Finish();
    std::wstring text = L"Test";
    FRectI textBox = ctx.TextMetrics( text, font, 23 );
    FVec2I pos = canvas.Rect().Size() / 2 - textBox.Size() / 2;
    FMat3F mat = FMat3F::MakeTranslationMatrix( pos.x, pos.y + textBox.h );
    FRectI bgBox = textBox;
    int pad = 3;
    bgBox.x = pos.x - pad;
    bgBox.y = pos.y - pad;
    bgBox.w += pad * 2;
    bgBox.h += pad * 2;
    ctx.Fill( canvas, background, bgBox );
    ctx.Finish();
    ctx.RasterTextAA( canvas, text, font, 22, mat, foreground );
    ctx.Finish();
    */
    FBlock canvas;
    ctx.XCreateTestBlock( canvas );
    ctx.Finish();

    FBlock proxy( size, size, Format_RGBA8 );
    ctx.ConvertFormat( canvas, proxy );
    ctx.Finish();

    // Before displaying the window, gather the end time and delta to output the time it took to process all ULIS operations.
    // We are not interested in the time it took Qt to create the window.
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();

    // Print out the result time.
    std::cout << "ULIS All: Composition took " << delta << "ms." << std::endl;

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

