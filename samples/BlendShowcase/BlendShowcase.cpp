// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         BlendShowcase.cpp
* @author       Clement Berthaud
* @brief        BlendShowcase application for ULIS2.
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

    std::string pathBase = "C:/Users/PRAXINOS/Documents/work/base_160.png";
    std::string pathOver = "C:/Users/PRAXINOS/Documents/work/over_160.png";

    uint32 perfIntentLoad   = ULIS2_PERF_MT | ULIS2_PERF_TSPEC | ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2;
    uint32 perfIntentFill   = ULIS2_PERF_TSPEC | ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2;
    uint32 perfIntentCopy   = ULIS2_PERF_TSPEC | ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2;
    uint32 perfIntentBlend  = ULIS2_PERF_TSPEC | ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2;
    uint32 perfIntentText   = 0;
    FBlock* blockBase = XLoadFromFile( &threadPool, ULIS2_NONBLOCKING, perfIntentLoad, host, ULIS2_NOCB, pathBase, ULIS2_FORMAT_RGBA8 );
    FBlock* blockOver = XLoadFromFile( &threadPool, ULIS2_NONBLOCKING, perfIntentLoad, host, ULIS2_NOCB, pathOver, ULIS2_FORMAT_RGBA8 );
    Fence( threadPool );

    FRect sourceRect = blockBase->Rect();
    int w = sourceRect.w * 8;
    int h = sourceRect.h * 5;

    int shadeW = sourceRect.w;
    int shadeH = 20;
    FBlock* blockCanvas = new  FBlock( w, h, ULIS2_FORMAT_RGBA8 );
    FBlock* blockShade = new  FBlock( shadeW, shadeH, ULIS2_FORMAT_RGBA8 );
    FRect shadeRect = blockBase->Rect();
    FPixelValue black( ULIS2_FORMAT_RGBA8, { 0, 0, 0, 255 } );
    FPixelValue white( ULIS2_FORMAT_RGBA8, { 255, 255, 255, 255 } );
    Fill( &threadPool, ULIS2_NONBLOCKING, ULIS2_PERF_TSPEC | ULIS2_PERF_AVX2, host, ULIS2_NOCB, blockShade, black, shadeRect );

    FFontEngine fontEngine;
    FFontRegistry fontRegistry( fontEngine );
    FFont font( fontRegistry, "Segoe UI", "Light" );

    for( int i = 0; i < NUM_BLENDING_MODES; ++i ) {
        int x = ( i % 8 ) * sourceRect.w;
        int y = ( i / 8 ) * sourceRect.h;
        Copy(   &threadPool, ULIS2_BLOCKING, perfIntentCopy, host, ULIS2_NOCB, blockBase, blockCanvas, sourceRect, FVec2I( x, y ) );
        Blend(  &threadPool, ULIS2_BLOCKING, perfIntentBlend, host, ULIS2_NOCB, blockOver, blockCanvas, sourceRect, FVec2F( x, y ), ULIS2_NOAA, static_cast< eBlendingMode >( i ), AM_NORMAL, 0.5f );
        Blend(  &threadPool, ULIS2_BLOCKING, perfIntentBlend, host, ULIS2_NOCB, blockShade, blockCanvas, shadeRect, FVec2F( x, y + sourceRect.h - shadeH ), ULIS2_NOAA, BM_NORMAL, AM_NORMAL, 0.5f );
        Blend(  &threadPool, ULIS2_BLOCKING, perfIntentBlend, host, ULIS2_NOCB, blockShade, blockCanvas, shadeRect, FVec2F( x, y + sourceRect.h - shadeH ), ULIS2_NOAA, BM_BAYERDITHER8x8, AM_NORMAL, 0.5f );
        std::string bm = kwBlendingMode[i];
        typedef std::codecvt_utf8<wchar_t> convert_type;
        std::wstring_convert<convert_type, wchar_t> converter;
        std::wstring wbm = converter.from_bytes(bm);
        RenderText( &threadPool, ULIS2_BLOCKING, perfIntentText, host, ULIS2_NOCB, blockCanvas, wbm, font, 16, white, FTransform2D( MakeTranslationMatrix( x + 4, 4 + y + sourceRect.h - shadeH ) ), ULIS2_NOAA );
    }

    Fence( threadPool );
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

    delete  label;
    delete  image;
    delete  widget;

    delete  blockCanvas;
    delete  blockShade;

    return  exit_code;
}

