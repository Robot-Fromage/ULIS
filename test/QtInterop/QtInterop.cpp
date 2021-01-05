// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         QtInterop.cpp
* @author       Clement Berthaud
* @brief        Gui application for ULIS3.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <Windows.h>

using namespace ::ULIS;

void OnEventCompleteDo( const FRectI& iGeometry, void* iUserData )
{
    reinterpret_cast< FBlock* >( iUserData )->Dirty( iGeometry );
}

int
main( int argc, char *argv[] ) {
    // Common
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt );
    FHardwareMetrics hw;
    FSchedulePolicy policy( ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    //FSchedulePolicy policy( ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );

    // Data
    FBlock block( 256, 256, fmt );

    // Operation
    FEvent evt_clear( FOnEventComplete( &OnEventCompleteDo, &block ) );
    ctx.Fill( block, block.Rect(), FColor::RGBA8( 255, 0, 0 ), policy, 0, nullptr, &evt_clear );
    ctx.Finish();

    // Bake Qt App / Window
    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( block.Bits(), block.Width(), block.Height(), block.BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    QPixmap         pixmap  = QPixmap::fromImage( *image );
    QLabel*         label   = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();

    // Main exec loop in GUI Thread
    int exit_code = app.exec();

    // Cleanup
    delete  label;
    delete  image;
    delete  widget;

    return  exit_code;
}

