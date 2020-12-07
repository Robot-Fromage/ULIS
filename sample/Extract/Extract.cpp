// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Extract.cpp
* @author       Clement Berthaud
* @brief        Extract application for ULIS3.
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
    FThreadPool* threadPool = XCreateThreadPool();
    uint32 perfIntent = /* ULIS_PERF_MT | */ ULIS_PERF_SSE42 | ULIS_PERF_AVX2;
    FHardwareMetrics host = FHardwareMetrics::Detect();

    FVec2I size( 800, 600 );
    FBlock* blockSRC = new FBlock( size.x, size.y, ULIS_FORMAT_RGBA8 );
    Clear( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockSRC, blockSRC->Rect() );
    FVec2I mid = size / 2;
    int rad = 250;
    for( int x = 0; x < size.x; ++x ) {
        for( int y = 0; y < size.y; ++y ) {
            FVec2I pos( x, y );
            FVec2I delta = pos - mid;
            int dist = delta.Distance();
            if( abs( dist - rad ) < 2 ) {
                FPixel prox = blockSRC->Pixel( x, y );
                prox.SetAlpha8( 128 );
            }
        }
    }

    FBlock* blockDST = XExtract( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockSRC, false, 1 << blockSRC->AlphaIndex(), ULIS_FORMAT_G8, false, 1 );

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockDST->Bits()
                                        , blockDST->Width()
                                        , blockDST->Height()
                                        , blockDST->BytesPerScanLine()
                                        , QImage::Format_Alpha8 );
    QPixmap         pixmap  = QPixmap::fromImage( *image );
    QLabel*         label   = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();

    // Get error code. This function returns when all Qt Windows are closed.
    int exit_code = app.exec();

    // Delete Qt Components.
    delete  label;
    delete  image;
    delete  widget;

    // Delete our block Canvas.
    delete  blockSRC;
    delete  blockDST;

    XDeleteThreadPool( threadPool );

    // Return exit code.
    return  exit_code;
}

