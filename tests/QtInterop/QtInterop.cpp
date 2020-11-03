// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS3
*__________________
*
* @file         QtInterop.cpp
* @author       Clement Berthaud
* @brief        Gui application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

using namespace ::ul3;

int
main( int argc, char *argv[] ) {
    FThreadPool* threadPool = XCreateThreadPool();
    uint32 perfIntent = ULIS3_PERF_SSE42 | ULIS3_PERF_AVX2;
    FHostDeviceInfo host = FHostDeviceInfo::Detect();

    FBlock* blockA = new FBlock( 1024, 1024, ULIS3_FORMAT_RGBA8 );

    ::ul3::Clear( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, blockA, blockA->Rect() );
    FPixelValue color = FPixelValue::FromRGBA8( 255, 0, 0, 255 );
    FBlock col( color.Ptr(), 1, 1, ULIS3_FORMAT_RGBA8 );

    FVec2F P0( 474.984253, 551.79988 );
    FVec2F P1( 474.984253, 551.79988 );
    FVec2F P2( 561.006348, 549.796814 );
    FVec2F P3( 687.998413, 551.799988 );

    Blend( threadPool, ULIS3_BLOCKING, ULIS3_PERF_SSE42, host, ULIS3_NOCB, &col, blockA, FRect( 0, 0, 1, 1 ), P0, ULIS3_AA, BM_NORMAL, AM_NORMAL, 1.f );
    Blend( threadPool, ULIS3_BLOCKING, ULIS3_PERF_SSE42, host, ULIS3_NOCB, &col, blockA, FRect( 0, 0, 1, 1 ), P1, ULIS3_AA, BM_NORMAL, AM_NORMAL, 1.f );
    Blend( threadPool, ULIS3_BLOCKING, ULIS3_PERF_SSE42, host, ULIS3_NOCB, &col, blockA, FRect( 0, 0, 1, 1 ), P2, ULIS3_AA, BM_NORMAL, AM_NORMAL, 1.f );
    Blend( threadPool, ULIS3_BLOCKING, ULIS3_PERF_SSE42, host, ULIS3_NOCB, &col, blockA, FRect( 0, 0, 1, 1 ), P3, ULIS3_AA, BM_NORMAL, AM_NORMAL, 1.f );
    //std::vector< FVec2F > points;
    //uint32 count = 2;
    //CatmullRomPoints( P0, P1, P2, P3, count, &points, 0.5f );
    FCatmullRomSpline spline( P0, P1, P2, P3 );
    std::vector< FCatmullRomLUTElement > points;
    spline.GenerateLinearLUT( &points, 10.f );
    for( int i = 0; i < points.size(); ++i ) {
        Blend( threadPool, ULIS3_BLOCKING, ULIS3_PERF_SSE42, host, ULIS3_NOCB, &col, blockA, FRect( 0, 0, 1, 1 ), points[i].position, ULIS3_AA, BM_NORMAL, AM_NORMAL, 1.f );
        /*
        int x = points[i].position.x;
        int y = points[i].position.y;
        Fill( threadPool, 1, 2, host, 0, blockA, color, FRect( x, y, 5, 5 ) );
        */
    }

    // Qt Window
    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockA->DataPtr(), blockA->Width(), blockA->Height(), blockA->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    QPixmap         pixmap  = QPixmap::fromImage( *image );
    QLabel*         label   = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();

    int exit_code = app.exec();

    delete  label;
    delete  image;
    delete  widget;

    delete  blockA;
    XDeleteThreadPool( threadPool );
    return  exit_code;
}

