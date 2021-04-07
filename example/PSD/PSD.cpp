// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PSD.cpp
* @author       Clement Berthaud
* @brief        PSD application for ULIS.
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

#include "Layer/LayerStack.h"

using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_AVX );
    FHardwareMetrics hw;
    FSchedulePolicy policy_sync_cache_efficient( ScheduleTime_Sync, ScheduleRun_Multi,ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policy_sync_mono_chunk( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );
    FSchedulePolicy policy_sync_multi_scanlines( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Scanlines );
    FSchedulePolicy policy_sync_mono_scanlines( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Scanlines );

    FLayerStack layerStack( 1, 1, fmt );

    auto startTime = std::chrono::steady_clock::now();

    ctx.XLoadPSDFromDisk( layerStack, "C:/Users/PRAXINOS/Documents/work/psdTests/RGBA32bits.psd" );
    ctx.Finish();

    FLayerImage* layerImage = dynamic_cast< FLayerImage* >( layerStack.Layers()[0] );

    /*FBlock blockCanvas(layerStack.Width(), layerStack.Height(), layerStack.Format());
    ctx.Flatten( layerStack, blockCanvas );
    ctx.Finish();*/

    FBlock blockCanvasConverted(layerStack.Width(), layerStack.Height(), Format_RGBA8);
    ctx.ConvertFormat(layerImage->Block(), blockCanvasConverted );
    ctx.Finish();
    
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();

    std::cout << "ULIS XLoadPSDFromDisk took: " << delta << "ms." << std::endl;

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockCanvasConverted.Bits()
                                        , blockCanvasConverted.Width()
                                        , blockCanvasConverted.Height()
                                        , blockCanvasConverted.BytesPerScanLine()
                                        , QImage::Format_RGBA8888 );
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

    // Return exit code.
    return  exit_code;
}

