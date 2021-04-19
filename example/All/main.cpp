// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         All.cpp
* @author       Clement Berthaud
* @brief        Example of all ctx features for ULIS.
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

enum eDocumentFormat {
      kRGBA8
    , kRGBA16
    , kRGBA32
    , kRGBAF
    , kABGR8
    , kBGRA16
    , kHSVAF
    , kCMYKA16
    , kLabA32
    , kLabAF
    , kALab8
    , kNumDocumentFormat
};

static const char* kwDocumentFormat[] = {
      "RGBA8"
    , "RGBA16"
    , "RGBA32"
    , "RGBAF"
    , "ABGR8"
    , "BGRA16"
    , "HSVAF"
    , "CMYKA16"
    , "LabA32"
    , "LabAF"
    , "ALab8"
    , "Invalid"
};

static const eFormat eDocumentFormaMatchingTable[] = {
      Format_RGBA8
    , Format_RGBA16
    , Format_RGBA32
    , Format_RGBAF
    , Format_ABGR8
    , Format_BGRA16
    , Format_HSVAF
    , Format_CMYKA16
    , Format_LabA32
    , Format_LabAF
    , Format_ALab8
};

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_SSE );

    FBlock canvas( 1024, 1024, fmt );
    FRectI src( 16, 0, 16, 16 );
    {
        ctx.Clear( canvas );
        ctx.Finish();
        ctx.Clear( canvas, src, FSchedulePolicy::MonoChunk );
        ctx.Fill( canvas, FRectI( 16, 0, 8, 8 ), FColor::RGB( 45, 20, 30 ), FSchedulePolicy::MonoChunk );
        ctx.Fill( canvas, FRectI( 24, 0, 8, 8 ), FColor::RGB( 255, 100, 40 ), FSchedulePolicy::MonoChunk );
        ctx.Fill( canvas, FRectI( 16, 8, 8, 8 ), FColor::RGB( 75, 120, 35 ), FSchedulePolicy::MonoChunk );
        ctx.Fill( canvas, FRectI( 24, 8, 8, 8 ), FColor::RGB( 60, 130, 240 ), FSchedulePolicy::MonoChunk );
        ctx.Fill( canvas, FRectI( 32, 0, canvas.Width(), 16 ), FColor::RGB( 127, 127, 255 ), FSchedulePolicy::MultiScanlines );
        ctx.Fill( canvas, FRectI( 0, 16, canvas.Width(), 64 ), FColor::RGB( 127, 127, 255 ), FSchedulePolicy::MultiScanlines );
        ctx.Finish();
    }

    {
        for( int i = 0; i < NumBlendModes; ++ i ) {
            ctx.Blend( canvas, canvas, src, FVec2I( ( i + 2 ) * 16, 0 ), static_cast< eBlendMode >( i ), Alpha_Normal, 0.5f, FSchedulePolicy::MonoChunk );
            ctx.Flush();
        }
        ctx.Fence();

        for( int i = 0; i < NumBlendModes; ++ i ) {
            ctx.BlendAA( canvas, canvas, src, FVec2F( ( i + 2 ) * 16 + 0.5f, 16 ), static_cast< eBlendMode >( i ), Alpha_Normal, 0.5f, FSchedulePolicy::MonoChunk );
            ctx.Finish();
        }

        for( int i = 0; i < NumBlendModes; ++ i ) {
            ctx.BlendColor( FColor::HSVAF( float( i ) / NumBlendModes, 1.f, 1.f ), canvas, FRectI( ( i + 2 ) * 16, 32, 16, 16 ), static_cast< eBlendMode >( i ), Alpha_Normal, 0.5f, FSchedulePolicy::MonoChunk );
            ctx.Flush();
        }
        ctx.Fence();

        for( int i = 0; i < NumBlendModes; ++ i ) {
            ctx.AlphaBlend( canvas, canvas, src, FVec2F( ( i + 2 ) * 16, 48 ), 0.5f, FSchedulePolicy::MonoChunk );
            ctx.Flush();
        }
        ctx.Finish();

        for( int i = 0; i < NumBlendModes; ++ i ) {
            ctx.AlphaBlendAA( canvas, canvas, src, FVec2F( ( i + 2 ) * 16 + 0.5f, 64 ), 0.5f, FSchedulePolicy::MonoChunk );
            ctx.Finish();
        }

        ctx.BlendTiled( canvas, canvas, src, FRectI( 0, 80, canvas.Width(), 64 ), FVec2I(0), Blend_Normal, Alpha_Normal, 1.f, FSchedulePolicy::MultiScanlines );
        ctx.Finish();

        ctx.BlendColor( FColor::RGB( 255, 127, 255 ), canvas, FRectI( 0, 96, canvas.Width(), 16 ), Blend_BayerDither8x8, Alpha_Normal, 0.5f, FSchedulePolicy::MonoChunk );
        ctx.BlendColor( FColor::RGB( 255, 127, 255 ), canvas, FRectI( 0, 112, canvas.Width(), 16 ), Blend_BayerDither8x8, Alpha_Normal, 0.75f, FSchedulePolicy::MonoChunk );
        ctx.BlendColor( FColor::RGB( 255, 127, 255 ), canvas, FRectI( 0, 128, canvas.Width(), 16 ), Blend_BayerDither8x8, Alpha_Normal, 0.9f, FSchedulePolicy::MonoChunk );
        ctx.Finish();
    }

    {
        FGradient gradient0( fmt );
        gradient0.AddColorStep( 0.f, FColor::Blue );
        gradient0.AddColorStep( 1.f, FColor::Red );
        FSanitizedGradient grad0 = gradient0.Sanitized( fmt );
        FVec2I p0( 0, 16 );
        FVec2I p1( 0, 80 );
        ctx.RasterGradient( canvas, p0, p1, grad0, 4.f / 255.f, Gradient_Linear, FRectI( 0, 16, 32, 64 ) );

        FGradient gradient1( fmt );
        gradient1.AddColorStep( 0.f, FColor::RGB( 0, 0, 21 ) );
        gradient1.AddColorStep( 1.f, FColor::White );
        FSanitizedGradient grad1 = gradient1.Sanitized( fmt );
        FVec2I p2( 848, 40 );
        FVec2I p3( 1024, 0 );
        ctx.RasterGradient( canvas, p2, p3, grad1, 1.f / 255.f, Gradient_Angular, FRectI( 672, 0, 352, 80 ) );
        ctx.Finish();
    }

    {
        ctx.Copy( canvas, canvas, FRectI( 0, 0, 1024, 144 ), FVec2I( 0, 144 ), FSchedulePolicy::MultiScanlines );
        ctx.Finish();
        ctx.BlendColor( FColor::Black, canvas, FRectI( 0, 144, 1024, 144 ), Blend_Multiply, Alpha_Normal, 0.5f );
        ctx.Finish();
        ctx.Convolve( canvas, canvas, FKernel::GaussianBlur, FRectI( 0, 0, 20, 144 ), FVec2I( 0, 280 ) );
        ctx.Finish();
    }

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

