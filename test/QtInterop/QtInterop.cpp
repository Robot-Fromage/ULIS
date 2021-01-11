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

void OnEventCompleteCallDirty( const FRectI& iGeometry, void* iUserData )
{
    reinterpret_cast< FBlock* >( iUserData )->Dirty( iGeometry );
}

int
main( int argc, char *argv[] ) {
    // Common
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_MEM );
    FHardwareMetrics hw;
    FSchedulePolicy policy( ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );

    // Data
    FBlock blockA( 1024, 1024, fmt );
    FBlock blockB( 1024, 1024, fmt );
    FBlock blockC( 1024, 1024, fmt );
    FBlock canvas( 1024, 1024, fmt );

    FString stringA;
    FString stringB( "Yes" );
    FString stringC( stringB );
    stringB.Append( stringC );
    stringB.Append( " No" );
    stringB.Append( 'A' );
    stringB.Prepend( "Yes" );
    stringB.Prepend( stringC );
    stringB.Prepend( 'A' );

    auto dummy = 0;

    // Operation
    /*
    {
        TArray< FEvent > events( 12 );
        // events[0]    : Clear A
        // events[1]    : Clear B
        // events[2]    : Clear C
        // events[3]    : Fill A
        // events[4]    : Fill B
        // events[5]    : Fill C
        // events[6]    : Blend C
        // events[7]    : Blend B
        // events[8]    : Blend A
        // events[9]    : Blend Color
        // events[10]   : Blend AA
        // events[11]   : Blend Tiled
        TArray< FColor > colors( 4 );
        colors[0] = FColor::RGB( 255, 0, 0 );
        colors[1] = FColor::RGB( 0, 255, 0 );
        colors[2] = FColor::GreyAF( 0.5f );
        colors[3] = FColor::GreyAF( 0.f );
        ctx.Clear( blockA, blockA.Rect(), policy, 0, nullptr, &events[0] );
        ctx.Clear( blockB, blockB.Rect(), policy, 0, nullptr, &events[1] );
        ctx.Clear( blockC, blockC.Rect(), policy, 0, nullptr, &events[2] );
        ctx.Fill( blockA, blockA.Rect(), colors[0], policy, 1, &events[0], &events[3] );
        ctx.Fill( blockB, blockB.Rect(), colors[1], policy, 1, &events[1], &events[4] );
        ctx.Fill( blockC, blockC.Rect(), colors[2], policy, 1, &events[2], &events[5] );
        ctx.Blend( blockC, canvas, blockC.Rect(), FVec2I( 0, 0 ), Blend_Normal, Alpha_Normal, 1.f, policy, 3, &events[3], &events[6] );
        ctx.Blend( blockA, canvas, blockA.Rect(), FVec2I( 0, 0 ), Blend_Normal, Alpha_Normal, 0.5f, policy, 1, &events[6], &events[7] );
        ctx.Blend( blockB, canvas, blockB.Rect(), FVec2I( 0, 0 ), Blend_Normal, Alpha_Normal, 0.1f, policy, 1, &events[7], &events[8] );
        ctx.BlendColor( colors[3], canvas, canvas.Rect(), Blend_BayerDither8x8, Alpha_Normal, 0.5f, policy, 1, &events[8], &events[9] );
        ctx.BlendAA( blockB, canvas, FRectI( 0, 0, 512, 512 ), FVec2F( 64.5f, 64.5f ), Blend_Normal, Alpha_Normal, 1.f, policy, 1, &events[9], &events[10] );
        ctx.BlendTiled( blockA, canvas, FRectI( 16, 16, 32, 32 ), FRectI( 512, 512, 128, 128 ), FVec2I(), Blend_Normal, Alpha_Normal, 1.f, policy, 1, &events[10 ], &events[11] );
        ctx.Finish();
        //ctx.Wait( events[11] );
    }
    */

    // Bake Qt App / Window
    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( canvas.Bits(), canvas.Width(), canvas.Height(), canvas.BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
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

