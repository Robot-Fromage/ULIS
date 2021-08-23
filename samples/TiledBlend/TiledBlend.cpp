// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS3
*__________________
*
* @file         Blend.cpp
* @author       Clement Berthaud
* @brief        Blend application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include <chrono>

using namespace ::ul3;

int
main( int argc, char *argv[] ) {
    FThreadPool*    threadPool  = XCreateThreadPool();
    uint32          perfIntent  = ULIS3_PERF_MT | ULIS3_PERF_SSE42 | ULIS3_PERF_AVX2;
    FHostDeviceInfo host        = FHostDeviceInfo::Detect();

    std::string pathBase = "C:/Users/PRAXINOS/Documents/work/base_160.png";
    std::string pathOver = "C:/Users/PRAXINOS/Documents/work/over_160.png";

    FBlock* blockBase = XLoadFromFile( threadPool, ULIS3_NONBLOCKING, perfIntent, host, ULIS3_NOCB, pathBase, ULIS3_FORMAT_RGBA8 );
    FBlock* blockOver = XLoadFromFile( threadPool, ULIS3_NONBLOCKING, perfIntent, host, ULIS3_NOCB, pathOver, ULIS3_FORMAT_RGBA8 );

    Fence( *threadPool );

    FBlock* blockCanvas = new  FBlock( 800, 600, ULIS3_FORMAT_RGBA8 );
    Fill( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, blockCanvas, FPixelValue::FromRGBAF( 0.5f, 0.5f, 1.f ), blockCanvas->Rect() );

    BlendTiled(  threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, blockBase, blockCanvas, FRect( 40, 40, 80, 80 ), FRect( -40, 0, 1000, 400 ), FVec2I( -20, -40 ), BM_DISSOLVE, AM_NORMAL, 0.3f );
    BlendColor(  threadPool, perfIntent, host, ULIS3_NOCB, FPixelValue( ULIS3_FORMAT_BGR8, { 255, 0, 0 } ), blockCanvas, FRect( 5, 80, 500, 150 ), BM_BAYERDITHER8x8, AM_NORMAL, 0.5f );
    BlendColor(  threadPool, perfIntent, host, ULIS3_NOCB, FPixelValue( ULIS3_FORMAT_BGR8, { 0, 255, 0 } ), blockCanvas, FRect( 5, 80, 500, 150 ), BM_HUE, AM_NORMAL, 0.5f );
    Blend(  threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, blockOver, blockCanvas, blockBase->Rect(), FVec2F(), ULIS3_NOAA, BM_NORMAL, AM_NORMAL, 0.5f );


    #define _P0 threadPool, perfIntent, host, ULIS3_NOCB
    #define _P1 blockCanvas
    #define _P2 BM_BAYERDITHER8x8, AM_NORMAL
    FPixelValue color = FPixelValue::FromRGBA8( 0, 0, 0, 255 );
    BlendColor( _P0, color, _P1, FRect( 60,     100,    20, 20 ), _P2, 0.9f );
    BlendColor( _P0, color, _P1, FRect( 80,     100,    20, 20 ), _P2, 0.8f );
    BlendColor( _P0, color, _P1, FRect( 100,    100,    20, 20 ), _P2, 0.6f );
    BlendColor( _P0, color, _P1, FRect( 120,    100,    20, 20 ), _P2, 0.4f );
    BlendColor( _P0, color, _P1, FRect( 140,    100,    20, 20 ), _P2, 0.2f );
    BlendColor( _P0, color, _P1, FRect( 160,    100,    20, 20 ), _P2, 0.1f );

    // Get rid of Base and Over, we don't need them anymore.
    delete  blockBase;
    delete  blockOver;

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockCanvas->DataPtr()
                                        , blockCanvas->Width()
                                        , blockCanvas->Height()
                                        , blockCanvas->BytesPerScanLine()
                                        , QImage::Format_RGBA8888 );
    QPixmap         pixmap  = QPixmap::fromImage( *image );
    QLabel*         label   = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();

    int exit_code = app.exec();

    // Delete Qt Components.
    delete  label;
    delete  image;
    delete  widget;

    // Delete our block Canvas.
    delete  blockCanvas;

    // Delete the thread pool
    XDeleteThreadPool( threadPool );

    // Return exit code.
    return  exit_code;
}

