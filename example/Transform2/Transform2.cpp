// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Transform2.cpp
* @author       Clement Berthaud
* @brief        Transform application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_AVX );
    FHardwareMetrics hw;
    FSchedulePolicy policy_cache_efficient( ScheduleTime_Sync, ScheduleRun_Multi,ScheduleMode_Chunks,ScheduleParameter_Length,hw.L1CacheSize());
    FSchedulePolicy policy_mono_chunk(ScheduleTime_Sync, ScheduleRun_Mono,ScheduleMode_Chunks,ScheduleParameter_Count,1);
    FSchedulePolicy policy_multi_scanlines(ScheduleTime_Sync, ScheduleRun_Multi,ScheduleMode_Scanlines );

    FBlock source;
    FBlock canvas( 1024, 1024, fmt );
    {
        std::string path = "C:/Users/PRAXINOS/Documents/work/test.png";

        ctx.XLoadBlockFromDisk( source, path );
        ctx.Clear( canvas, canvas.Rect(), policy_cache_efficient );
    }
    ctx.Finish();

    int w = source.Width();
    int h = source.Height();
    FVec2F src[4] = { FVec2F( 0, 0 ), FVec2F( w, 0 ), FVec2F( w, h ), FVec2F( 0, h ) };
    FVec2F dst[4] = { FVec2F( 0, 0 ), FVec2F( w, 0 ), FVec2F( 0, h ), FVec2F( w, h ) };

    FVec2F delta( 256 );
    for( int i = 0; i < 4; ++i )
        dst[i] += delta;

    FMat3F homography = FMat3F::MakeHomography( src, dst );
    FVec2F test[4];
    for( int i = 0; i < 4; ++i )
        test[i] = homography.ApplyHomography( src[i] );

    for( int i = 0; i < 4; ++i )
        if( dst[i] != test[i] )
            auto dummy = 0;

    FVec2F testSrc = FVec2F( w, h ) / 2;
    FVec2F testDst = homography.ApplyHomography( testSrc );

    auto dummy = 0;

    {
        ctx.TransformPerspective(
              source
            , canvas
            , source.Rect()
            , homography
            , Resampling_Bilinear
            , Border_Transparent
            , FColor::Transparent
            , FSchedulePolicy::MonoChunk
        );
    }
    ctx.Finish();


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

    int exit_code = app.exec();

    delete  label;
    delete  image;
    delete  widget;

    return  exit_code;
}
