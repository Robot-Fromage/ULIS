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
    eFormat fmt = Format_LabA8;
    FContext ctx( queue, fmt );

    // Gather start time to output the time it took to perform the blend composition
    auto startTime = std::chrono::steady_clock::now();

    FBlock canvas;
    FEvent eventCreate;
    FBlock proxy( 64, 64, Format_RGBA8 );
    ctx.XCreateTestBlock( canvas, FSchedulePolicy::CacheEfficient, 0, nullptr, &eventCreate );
    ctx.ConvertFormat( canvas, proxy, FRectI::Auto, FVec2I(), FSchedulePolicy::AsyncMultiScanlines, 1, &eventCreate );
    ctx.Finish();

    // Before displaying the window, gather the end time and delta to output the time it took to process all ULIS operations.
    // We are not interested in the time it took Qt to create the window.
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();

    // Print out the result time.
    std::cout << "ULIS Test: Composition took " << delta << "ms." << std::endl;
    #define O
    #define X
    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( proxy.Bits()
             O X O                      , proxy.Width()
             X X O                      , proxy.Height()
             O O X                      , proxy.BytesPerScanLine()
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

