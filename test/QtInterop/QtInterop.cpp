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

    // Data
    FBlock blockA( 256, 256, fmt );
    FBlock blockB( 256, 256, fmt );
    FBlock blockC( 256, 256, fmt );
    FBlock canvas( 256, 256, fmt );

    // Operation
    {
        TArray< FEvent > events( 6 );
        // events[0] : Clear A
        // events[1] : Clear B
        // events[2] : Clear C
        // events[4] : Fill A
        // events[5] : Fill B
        // events[6] : Fill C
        TArray< FColor > colors( 3 );
        colors[0] = FColor::RGB( 255, 0, 0 );
        colors[1] = FColor::RGB( 0, 255, 0 );
        colors[2] = FColor::GreyAF( 0.f );

        ctx.Clear( blockA, blockA.Rect(), policy, 0, nullptr, &events[0] );
        ctx.Clear( blockB, blockB.Rect(), policy, 0, nullptr, &events[1] );
        ctx.Clear( blockC, blockC.Rect(), policy, 0, nullptr, &events[2] );
        ctx.Fill( blockA, blockA.Rect(), colors[0], policy, 1, &events[0], &events[3] );
        ctx.Fill( blockB, blockB.Rect(), colors[1], policy, 1, &events[1], &events[4] );
        ctx.Fill( blockC, blockC.Rect(), colors[2], policy, 1, &events[2], &events[5] );
        ctx.Finish();
    }

    // Bake Qt App / Window
    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockC.Bits(), blockC.Width(), blockC.Height(), blockC.BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
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

