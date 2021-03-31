// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Text.cpp
* @author       Clement Berthaud
* @brief        Convolution application for ULIS.
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
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_AVX );
    FHardwareMetrics hw;
    FSchedulePolicy policy_cache_efficient( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policy_mono_chunk( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );

    int w = 600;
    int h = 600;
    FBlock blockSource( w, h, Format_RGBA8 );
    FBlock blockCanvas( w, h, Format_RGBA8 );
    ctx.Clear( blockSource, blockSource.Rect(), FSchedulePolicy::CacheEfficient );
    ctx.Flush();
    ctx.Fill( blockSource, FRectI( 200, 200, 200, 200 ), FColor::Red, FSchedulePolicy::CacheEfficient );
    ctx.Fence();
    ctx.Flush();
    FKernel kernel = FKernel(
          FVec2I( 5, 5 )
        , {
              0.00f, 0.12f, 0.25f, 0.12f, 0.00f
            , 0.12f, 0.25f, 0.50f, 0.25f, 0.12f
            , 0.25f, 0.50f, 1.00f, 0.50f, 0.25f
            , 0.12f, 0.25f, 0.50f, 0.25f, 0.12f
            , 0.00f, 0.12f, 0.25f, 0.12f, 0.00f
          }
    ).Normalized();

    ctx.ConvolvePremult( blockSource, blockCanvas, kernel, blockSource.Rect(), FVec2I(), Resampling_Bilinear, Border_Transparent, FColor::Transparent, FSchedulePolicy::MonoChunk );
    ctx.Finish();

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockCanvas.Bits()
                                        , blockCanvas.Width()
                                        , blockCanvas.Height()
                                        , blockCanvas.BytesPerScanLine()
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

