// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/**
*
*   ULIS
*__________________
*
* @file         BlendShowcase.cpp
* @author       Clement Berthaud
* @brief        BlendShowcase application for ULIS3.
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
    FHostDeviceInfo host = FHostDeviceInfo::Detect();

    std::string pathBase = "C:/Users/PRAXINOS/Documents/work/base_160.png";
    std::string pathOver = "C:/Users/PRAXINOS/Documents/work/over_160.png";

    uint32 perfIntentLoad   = ULIS_PERF_MT | ULIS_PERF_SSE42 | ULIS_PERF_AVX2;
    uint32 perfIntentFill   = ULIS_PERF_SSE42 | ULIS_PERF_AVX2;
    uint32 perfIntentCopy   = ULIS_PERF_SSE42 | ULIS_PERF_AVX2;
    uint32 perfIntentBlend  = ULIS_PERF_SSE42 | ULIS_PERF_AVX2;
    uint32 perfIntentText   = 0;
    FBlock* blockBase = XLoadFromFile( threadPool, ULIS_NONBLOCKING, perfIntentLoad, host, ULIS_NOCB, pathBase, ULIS_FORMAT_RGBA8 );
    FBlock* blockOver = XLoadFromFile( threadPool, ULIS_NONBLOCKING, perfIntentLoad, host, ULIS_NOCB, pathOver, ULIS_FORMAT_RGBA8 );
    Fence( *threadPool );

    FRectI sourceRect = blockBase->Rect();
    int w = sourceRect.w * 8;
    int h = sourceRect.h * 5;

    int shadeW = sourceRect.w;
    int shadeH = 20;
    FBlock* blockCanvas = new  FBlock( w, h, ULIS_FORMAT_RGBA8 );
    FBlock* blockShade = new  FBlock( shadeW, shadeH, ULIS_FORMAT_RGBA8 );
    FRectI shadeRect = blockBase->Rect();
    FColor black( ULIS_FORMAT_RGBA8, { 0, 0, 0, 255 } );
    FColor white( ULIS_FORMAT_RGBA8, { 255, 255, 255, 255 } );
    Fill( threadPool, ULIS_NONBLOCKING, ULIS_PERF_AVX2, host, ULIS_NOCB, blockShade, black, shadeRect );

    FTextEngine TextEngine;
    FFontRegistry fontRegistry( TextEngine );
    FFont font( fontRegistry, "Segoe UI", "Light" );

    for( int i = 0; i < NUM_BLENDING_MODES; ++i ) {
        int x = ( i % 8 ) * sourceRect.w;
        int y = ( i / 8 ) * sourceRect.h;
        Copy(   threadPool, ULIS_BLOCKING, perfIntentCopy, host, ULIS_NOCB, blockBase, blockCanvas, sourceRect, FVec2I( x, y ) );
        Blend(  threadPool, ULIS_BLOCKING, perfIntentBlend, host, ULIS_NOCB, blockOver, blockCanvas, sourceRect, FVec2F( x, y ), ULIS_NOAA, static_cast< eBlendMode >( i ), AM_NORMAL, 0.5f );
        Blend(  threadPool, ULIS_BLOCKING, perfIntentBlend, host, ULIS_NOCB, blockShade, blockCanvas, shadeRect, FVec2F( x, y + sourceRect.h - shadeH ), ULIS_NOAA, BM_NORMAL, AM_NORMAL, 0.5f );
        Blend(  threadPool, ULIS_BLOCKING, perfIntentBlend, host, ULIS_NOCB, blockShade, blockCanvas, shadeRect, FVec2F( x, y + sourceRect.h - shadeH ), ULIS_NOAA, BM_BAYERDITHER8x8, AM_NORMAL, 0.5f );
        std::string bm = kwBlendingMode[i];
        typedef std::codecvt_utf8<wchar_t> convert_type;
        std::wstring_convert<convert_type, wchar_t> converter;
        std::wstring wbm = converter.from_bytes(bm);
        RenderText( threadPool, ULIS_BLOCKING, perfIntentText, host, ULIS_NOCB, blockCanvas, wbm, font, 16, white, FTransformation2D::MakeTranslationTransform( x + 4, 4 + y + sourceRect.h - shadeH ), ULIS_NOAA );
    }

    Fence( *threadPool );
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

    delete  label;
    delete  image;
    delete  widget;

    delete  blockCanvas;
    delete  blockShade;
    XDeleteThreadPool( threadPool );

    return  exit_code;
}

