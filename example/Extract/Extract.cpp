// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Extract.cpp
* @author       Clement Berthaud
* @brief        Extract application for ULIS.
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
    eFormat format = Format_RGBA8;
    FContext ctx( queue, format );
    FHardwareMetrics hw;
    FSchedulePolicy policy_sync_cache_efficient( ScheduleTime_Sync, ScheduleRun_Multi,ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policy_sync_mono_chunk( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );
    FSchedulePolicy policy_sync_multi_scanlines( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Scanlines );
    FSchedulePolicy policy_sync_mono_scanlines( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Scanlines );

    FVec2I size( 800, 600 );
    FBlock* blockSRC = new FBlock( size.x, size.y, format );
    ctx.Clear( *blockSRC );
    ctx.Finish();

    FVec2I mid = size / 2;
    int rad = 250;
    for( int x = 0; x < size.x; ++x ) {
        for( int y = 0; y < size.y; ++y ) {
            FVec2I pos( x, y );
            FVec2I delta = pos - mid;
            int dist = delta.Distance();
            if( abs( dist - rad ) < 2 ) {
                FPixel prox = blockSRC->Pixel( x, y );
                prox.SetAlpha8( 128 );
            }
        }
    }
    FBlock* blockDST = new FBlock( size.x, size.y, Format_G8 );
    ctx.Extract( *blockSRC, *blockDST, 1 << blockSRC->AlphaIndex(), 1, false );
    ctx.Finish();

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockDST->Bits()
                                        , blockDST->Width()
                                        , blockDST->Height()
                                        , blockDST->BytesPerScanLine()
                                        , QImage::Format_Alpha8 );
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

    // Delete our block Canvas.
    delete  blockSRC;
    delete  blockDST;

    // Return exit code.
    return  exit_code;
}

