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
    FContext ctx( queue, fmt, PerformanceIntent_AVX );
    FHardwareMetrics hw;
    FFontEngine fontEngine;
    FFont font( fontEngine, "Segoe UI", "Regular" );
    FFont font2( fontEngine, "Segoe UI", "Regular" );
    FSchedulePolicy policy_cache_efficient( ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policy_mono_chunk( ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );

    // Data
    FBlock blockA( 1024, 1024, fmt );
    FBlock blockB( 1024, 1024, fmt );
    FBlock blockC( 1024, 1024, fmt );
    FBlock canvas( 1024, 1024, fmt );

    TArray< FVec2F > src( 4 );
    src[0] = FVec2F( 0, 0 );
    src[1] = FVec2F( 100, 0 );
    src[2] = FVec2F( 100, 100 );
    src[3] = FVec2F( 0, 100 );
    TArray< FVec2F > dst( 4 );
    dst[0] = FVec2F( 0, 0 );
    dst[1] = FVec2F( 80, 20 );
    dst[2] = FVec2F( 80, 80 );
    dst[3] = FVec2F( 0, 100 );
    FMat3F persp( FMat3F::MakeHomography( src.Data(), dst.Data() ) );
    FMat3F inpersp = persp.Inverse();
    TArray< FVec2F > res( 4 );
    res[0] = inpersp.ApplyHomography( dst[0] );
    res[1] = inpersp.ApplyHomography( dst[1] );
    res[2] = inpersp.ApplyHomography( dst[2] );
    res[3] = inpersp.ApplyHomography( dst[3] );
    auto dummy = 0;

    std::cout << FullLibraryInformationString().Data();


    // Operations
    {
        TArray< FEvent > events( 12 );
        TArray< FColor > colors( 4 );
        colors[0] = FColor::RGB( 255, 0, 0 );
        colors[1] = FColor::RGB( 0, 255, 0 );
        colors[2] = FColor::GreyAF( 0.5f );
        colors[3] = FColor::GreyAF( 0.f );
        ctx.Clear( blockA, blockA.Rect(), policy_mono_chunk, 0, nullptr, &events[0] );
        ctx.Clear( blockB, blockB.Rect(), policy_mono_chunk, 0, nullptr, &events[1] );
        ctx.Clear( blockC, blockC.Rect(), policy_mono_chunk, 0, nullptr, &events[2] );
        ctx.Fill( blockA, blockA.Rect(), colors[0], policy_mono_chunk, 1, &events[0], &events[3] );
        ctx.Fill( blockB, blockB.Rect(), colors[1], policy_mono_chunk, 1, &events[1], &events[4] );
        ctx.Fill( blockC, blockC.Rect(), colors[2], policy_mono_chunk, 1, &events[2], &events[5] );
        ctx.Blend( blockC, canvas, blockC.Rect(), FVec2I( 0, 0 ), Blend_Normal, Alpha_Normal, 1.f, policy_cache_efficient, 3, &events[3], &events[6] );
        ctx.Blend( blockA, canvas, blockA.Rect(), FVec2I( 0, 0 ), Blend_Normal, Alpha_Normal, 0.5f, policy_cache_efficient, 1, &events[6], &events[7] );
        ctx.Blend( blockB, canvas, blockB.Rect(), FVec2I( 0, 0 ), Blend_Normal, Alpha_Normal, 0.1f, policy_cache_efficient, 1, &events[7], &events[8] );
        ctx.BlendColor( colors[3], canvas, canvas.Rect(), Blend_BayerDither8x8, Alpha_Normal, 0.5f, policy_cache_efficient, 1, &events[8], &events[9] );
        ctx.BlendAA( blockB, canvas, FRectI( 0, 0, 512, 512 ), FVec2F( 64.5f, 64.5f ), Blend_Normal, Alpha_Normal, 1.f, policy_cache_efficient, 1, &events[9], &events[10] );
        ctx.BlendTiled( blockA, canvas, FRectI( 16, 16, 32, 32 ), FRectI( 512, 512, 128, 128 ), FVec2I(), Blend_Normal, Alpha_Normal, 1.f, policy_cache_efficient, 1, &events[10 ], &events[11] );
        ctx.BlendColor( FColor::RGB( 255, 255, 255, 255 ), canvas, canvas.Rect(), Blend_Normal, Alpha_Normal, 0.75f, policy_cache_efficient, 1, &events[ 11 ], nullptr );
        ctx.Finish();

        auto zozo = FMat3F::MakeRotationMatrix( 0.01f );
        auto zizi = FMat3F::MakeRotationMatrix( 0.01f ) * FMat3F();
        ctx.TransformAffine(
              blockC
            , canvas
            , blockC.Rect()
            , FMat3F::MakeRotationMatrix( 0.01f ) * FMat3F::MakeTranslationMatrix( 50.f, 50.f )
            , Resampling_Bilinear
            , Border_Transparent
            , FColor()
            , policy_mono_chunk
            , 0
            , nullptr
            , nullptr
        );

        ctx.Finish();

        //std::wstring test;
        //for( int i = 32; i < 127; ++i )
        //    test += wchar_t( i );
        //FEvent testev;
        //ctx.RasterText( canvas, test, font, 32, FMat3F::MakeTranslationMatrix( 50, 240 ), FColor::RGB( 127, 127, 255 ), FSchedulePolicy(), 0, 0, &testev );
        //ctx.RasterText( canvas, test, font, 32, FMat3F::MakeTranslationMatrix( 50, 280 ), FColor::RGB( 127, 127, 255 ), FSchedulePolicy(), 1, &testev );
        //ctx.Finish();
        //ctx.Finish();
    }

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

