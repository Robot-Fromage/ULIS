// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Text.cpp
* @author       Clement Berthaud
* @brief        Convolution application for ULIS.
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
    FSchedulePolicy policy_cache_efficient( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policy_mono_chunk( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );

    int w = 600;
    int h = 600;
    FBlock blockSource( w, h, Format_RGBA8 );
    FBlock blockCanvas( w, h, Format_RGBA8 );
    ctx.Clear( blockSource, blockSource.Rect(), FSchedulePolicy::CacheEfficient );
    ctx.Flush();
    ctx.Fill( blockSource, FColor::Red, FRectI( 200, 200, 200, 200 ), FSchedulePolicy::CacheEfficient );
    ctx.Fence();
    ctx.Flush();
    ctx.Fill( blockSource, FColor::RGBA8( 0, 0, 255, 255 ), FRectI( 300, 300, 100, 100 ), FSchedulePolicy::CacheEfficient );
    ctx.Fence();
    ctx.Flush();
    FKernel blur = FKernel(
          FVec2I( 5, 5 )
        , {
              0.00f, 0.12f, 0.25f, 0.12f, 0.00f
            , 0.12f, 0.25f, 0.50f, 0.25f, 0.12f
            , 0.25f, 0.50f, 1.00f, 0.50f, 0.25f
            , 0.12f, 0.25f, 0.50f, 0.25f, 0.12f
            , 0.00f, 0.12f, 0.25f, 0.12f, 0.00f
          }
    ).Normalized();

    FKernel edgeKernel( FVec2I( 10 ), 1.f );
    edgeKernel.Normalize();
    ctx.ConvolvePremult( blockSource, blockCanvas, edgeKernel, blockSource.Rect(), FVec2I(), Resampling_Bilinear, Border_Transparent, FColor::Transparent, FSchedulePolicy::MultiScanlines );
    ctx.Finish();

    FRectI rect;
    ctx.AnalyzeSmallestVisibleRect( blockSource, &rect, blockSource.Rect(), FSchedulePolicy::MultiScanlines );
    ctx.Finish();

    FColor average;
    ctx.AccumulateSample( blockSource, &average, rect, FSchedulePolicy::MultiScanlines );
    ctx.Finish();

    FBlock test( 5, 5, Format_RGBA8 );
    ctx.Fill( test, FColor::Red, test.Rect() );
    ctx.Finish();

    FBlock SAT( test.Width(), test.Height(), ctx.SummedAreaTableMetrics( test ) );
    ctx.BuildPremultipliedSummedAreaTable( test, SAT, FSchedulePolicy::MultiScanlines );
    ctx.Finish();
    for( int y = 0; y < SAT.Height(); ++y ) {
        for( int x = 0; x < SAT.Width(); ++x ) {
            FPixel proxy = SAT.Pixel( x, y );
            std::cout << proxy.RF() << " ";
        }
        std::cout << std::endl;
    }

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

