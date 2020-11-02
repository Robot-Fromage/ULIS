// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/**
*
*   ULIS
*__________________
*
* @file         TextShowcase.cpp
* @author       Clement Berthaud
* @brief        TextShowcase application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include <codecvt>

using namespace ::ul3;

int
main( int argc, char *argv[] ) {
    FThreadPool* threadPool = XCreateThreadPool();
    FHardwareMetrics host = FHardwareMetrics::Detect();

    FTextEngine TextEngine;
    FFontRegistry fontRegistry( TextEngine );
    int fontSize = 12;
    int entryHeight = 16;
    int entryWidth = 256;
    int gridx = 5;
    int gridy = 40;
    int canvasWidth = entryWidth * gridx;
    int canvasHeight = entryHeight * gridy;

    FRectI globalRect( 0, 0, canvasWidth, canvasHeight );
    FBlock* blockCanvas = new  FBlock( globalRect.w, globalRect.h, ULIS_FORMAT_RGBA8 );
    FColor black( ULIS_FORMAT_RGBA8, { 0, 0, 0, 255 } );
    FColor white( ULIS_FORMAT_RGBA8, { 255, 255, 255, 255 } );
    Fill( threadPool, ULIS_NONBLOCKING, ULIS_PERF_MT | ULIS_PERF_SSE42 | ULIS_PERF_AVX2, host, ULIS_NOCB, blockCanvas, white, globalRect );

    int i = 0;
    for( auto family : fontRegistry.Records() ) {
        for( auto style : family.second.Styles() ) {
            int x = ( i / gridy ) * entryWidth;
            int y = ( i % gridy ) * entryHeight;
            const FFontStyleEntry& key = style.second;

            FFont font( fontRegistry, key.Family(), key.Style() );

            std::string txt = key.Family() + " " + key.Style();
            typedef std::codecvt_utf8<wchar_t> convert_type;
            std::wstring_convert<convert_type, wchar_t> converter;
            std::wstring wtxt = converter.from_bytes(txt);

            RenderText( threadPool, ULIS_BLOCKING, 0, host, ULIS_NOCB, blockCanvas, wtxt, font, fontSize, black, FTransformation2D::MakeTranslationTransform( x, y ), ULIS_NOAA );
            ++i;
        }
    }

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

    delete  label;
    delete  image;
    delete  widget;

    delete  blockCanvas;
    XDeleteThreadPool( threadPool );

    return  exit_code;
}

