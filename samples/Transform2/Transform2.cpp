// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS3
*__________________
*
* @file         Transform.cpp
* @author       Clement Berthaud
* @brief        Transform application for ULIS3.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
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
    FThreadPool  threadPool;
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = 0 | ULIS3_PERF_TSPEC | ULIS3_PERF_SSE42 | ULIS3_PERF_AVX2;

    std::string path = "C:/Users/PRAXINOS/Documents/work/VENICE.png";
    FBlock* source = XLoadFromFile( &threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, path, ULIS3_FORMAT_RGBA8 );
    FBlock* canvas = new  FBlock( 1200, 1000, ULIS3_FORMAT_RGBA8 );
    Fill( &threadPool, ULIS3_BLOCKING, 0, host, ULIS3_NOCB, canvas, FPixelValue( ULIS3_FORMAT_G8, { 255 } ), canvas->Rect() );
    FTransform2D transform = ComposeMatrix( MakeTranslationMatrix( 20, 20 ), MakeScaleMatrix( 4, 4 ) );
    //TransformAffine( &threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, source, canvas, FRect( 7, 22, 50, 20 ), transform, INTERP_BILINEAR );
    Resize( &threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, source, canvas, source->Rect(), FVec2F( 256, 256 ), FVec2F( 20, 20 ), INTERP_AREA );

    delete  source;

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( canvas->DataPtr()
                                        , canvas->Width()
                                        , canvas->Height()
                                        , canvas->BytesPerScanLine()
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

    delete  canvas;

    return  exit_code;
}
