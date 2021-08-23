// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         TextShowcase.cpp
* @author       Clement Berthaud
* @brief        TextShowcase application for ULIS2.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS2>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include <codecvt>

using namespace ::ul2;

int
main( int argc, char *argv[] ) {
    FThreadPool  threadPool;
    FHostDeviceInfo host = FHostDeviceInfo::Detect();

    FFontEngine fontEngine;
    FFontRegistry fontRegistry( fontEngine );
    int fontSize = 12;
    int entryHeight = 16;
    int entryWidth = 256;
    int numEntries = fontRegistry.NumStyles();
    int gridx = 5;
    int gridy = 40;
    int canvasWidth = entryWidth * gridx;
    int canvasHeight = entryHeight * gridy;

    FRect globalRect( 0, 0, canvasWidth, canvasHeight );
    FBlock* blockCanvas = new  FBlock( globalRect.w, globalRect.h, ULIS2_FORMAT_RGBA8 );
    FPixelValue black( ULIS2_FORMAT_RGBA8, { 0, 0, 0, 255 } );
    FPixelValue white( ULIS2_FORMAT_RGBA8, { 255, 255, 255, 255 } );
    Fill( &threadPool, ULIS2_NONBLOCKING, ULIS2_PERF_MT | ULIS2_PERF_TSPEC | ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2, host, ULIS2_NOCB, blockCanvas, white, globalRect );
    std::cout << fontRegistry.NumStyles();
    int i = 0;
    for( auto family : fontRegistry.GetFamilies() ) {
        for( auto style : family.second.GetStyles() ) {
            int x = ( i / gridy ) * entryWidth;
            int y = ( i % gridy ) * entryHeight;
            const FFontStyleKey& key = style.second;

            FFont font( fontRegistry, key.GetFamilyName(), key.GetStyleName() );

            std::string txt = key.GetFamilyName() + " " + key.GetStyleName();
            typedef std::codecvt_utf8<wchar_t> convert_type;
            std::wstring_convert<convert_type, wchar_t> converter;
            std::wstring wtxt = converter.from_bytes(txt);

            RenderText( &threadPool, ULIS2_BLOCKING, 0, host, ULIS2_NOCB, blockCanvas, wtxt, font, fontSize, black, FTransform2D( MakeTranslationMatrix( x, y ) ), ULIS2_NOAA );
            ++i;
        }
    }

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

    delete  label;
    delete  image;
    delete  widget;

    delete  blockCanvas;

    return  exit_code;
}

