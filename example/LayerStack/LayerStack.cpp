// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.cpp
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
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
    FContext ctx( queue, fmt, PerformanceIntent_Max );
    FHardwareMetrics hw;
    FSchedulePolicy policy_sync_cache_efficient( ScheduleTime_Sync, ScheduleRun_Multi,ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policy_sync_mono_chunk( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );
    FSchedulePolicy policy_sync_multi_scanlines( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Scanlines );
    FSchedulePolicy policy_sync_mono_scanlines( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Scanlines );

    uint16 w = 1024;
    uint16 h = 1024;
    FBlock canvas( w, h, fmt );
    FLayerStack layerStack( w, h, fmt );
    FLayerFolder* folder0 = new FLayerFolder( "folder0", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack );
    FLayerFolder* folder1 = new FLayerFolder( "folder1", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack );
    FLayerFolder* folder00 = new FLayerFolder( "folder00", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, folder0 );
    layerStack.AddLayer( new FLayerImage( "image0", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    layerStack.AddLayer( new FLayerImage( "image1", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    layerStack.AddLayer( folder0 );
    folder0->AddLayer( new FLayerImage( "image00", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    folder0->AddLayer( folder00 );
    folder00->AddLayer( new FLayerImage( "image000", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    folder00->AddLayer( new FLayerImage( "image001", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    folder00->AddLayer( new FLayerImage( "image002", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    folder0->AddLayer( new FLayerImage( "image01", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    layerStack.AddLayer( folder1 );
    folder1->AddLayer( new FLayerImage( "image10", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    folder1->AddLayer( new FLayerImage( "image11", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    folder1->AddLayer( new FLayerImage( "image12", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    folder1->AddLayer( new FLayerImage( "image13", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    layerStack.AddLayer( new FLayerImage( "image2", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    layerStack.AddLayer( new FLayerImage( "image3", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );

    auto startTime = std::chrono::steady_clock::now();

    {
        ctx.Flatten(
              layerStack
            , canvas
            , canvas.Rect()
            , FVec2I( 0 )
            , policy_sync_multi_scanlines
            , 0
            , nullptr
            , nullptr
        );
    }

    ctx.Finish();

    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();

    std::cout << "ULIS Layer Stack Flatten took: " << delta << "ms." << std::endl;

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( canvas.Bits()
                                        , canvas.Width()
                                        , canvas.Height()
                                        , canvas.BytesPerScanLine()
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

