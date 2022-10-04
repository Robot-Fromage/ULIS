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

using namespace ::ul3;

int
main( int argc, char *argv[] ) {
    FThreadPool*    threadPool  = XCreateThreadPool();
    uint32          perfIntent  = ULIS_PERF_MT | ULIS_PERF_SSE42 | ULIS_PERF_AVX2;
    FHardwareMetrics host        = FHardwareMetrics::Detect();

    std::string pathBase = "C:/Users/PRAXINOS/Documents/work/base_160.png";
    std::string pathOver = "C:/Users/PRAXINOS/Documents/work/over_160.png";

    FBlock* blockBase = XLoadFromFile( threadPool, ULIS_NONBLOCKING, perfIntent, host, ULIS_NOCB, pathBase, ULIS_FORMAT_RGBA8 );
    FBlock* blockOver = XLoadFromFile( threadPool, ULIS_NONBLOCKING, perfIntent, host, ULIS_NOCB, pathOver, ULIS_FORMAT_RGBA8 );

    Fence( *threadPool );

    FBlock* blockCanvas = new  FBlock( 800, 600, ULIS_FORMAT_RGBA8 );
    Fill( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockCanvas, FColor::FromRGBAF( 0.5f, 0.5f, 1.f ), blockCanvas->Rect() );

    BlendTiled(  threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockBase, blockCanvas, FRectI( 40, 40, 80, 80 ), FRectI( -40, 0, 1000, 400 ), FVec2I( -20, -40 ), BM_DISSOLVE, AM_NORMAL, 0.3f );
    BlendColor(  threadPool, perfIntent, host, ULIS_NOCB, FColor( ULIS_FORMAT_BGR8, { 255, 0, 0 } ), blockCanvas, FRectI( 5, 80, 500, 150 ), BM_BAYERDITHER8x8, AM_NORMAL, 0.5f );
    BlendColor(  threadPool, perfIntent, host, ULIS_NOCB, FColor( ULIS_FORMAT_BGR8, { 0, 255, 0 } ), blockCanvas, FRectI( 5, 80, 500, 150 ), BM_HUE, AM_NORMAL, 0.5f );
    Blend(  threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockOver, blockCanvas, blockBase->Rect(), FVec2F(), ULIS_NOAA, BM_NORMAL, AM_NORMAL, 0.5f );


    #define _P0 threadPool, perfIntent, host, ULIS_NOCB
    #define _P1 blockCanvas
    #define _P2 BM_BAYERDITHER8x8, AM_NORMAL
    FColor color = FColor::FromRGBA8( 0, 0, 0, 255 );
    BlendColor( _P0, color, _P1, FRectI( 60,     100,    20, 20 ), _P2, 0.9f );
    BlendColor( _P0, color, _P1, FRectI( 80,     100,    20, 20 ), _P2, 0.8f );
    BlendColor( _P0, color, _P1, FRectI( 100,    100,    20, 20 ), _P2, 0.6f );
    BlendColor( _P0, color, _P1, FRectI( 120,    100,    20, 20 ), _P2, 0.4f );
    BlendColor( _P0, color, _P1, FRectI( 140,    100,    20, 20 ), _P2, 0.2f );
    BlendColor( _P0, color, _P1, FRectI( 160,    100,    20, 20 ), _P2, 0.1f );

    // Get rid of Base and Over, we don't need them anymore.
    delete  blockBase;
    delete  blockOver;

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockCanvas->Bits()
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

