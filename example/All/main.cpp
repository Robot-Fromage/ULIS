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
    , Format_RGBAF
    , Format_ABGR8
    , Format_BGRA16
    , Format_HSVAF
    , Format_CMYKA16
    , Format_LabAF
    , Format_ALab8
    , Format_GA16
};

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt );
    FFontEngine fontEngine;
    FFont arial( fontEngine, "Arial", "Black" );

    FBlock canvas( 1024, 1024, fmt );
    FRectI src( 16, 0, 16, 16 );

    // Gather start time to output the time it took to perform the blend composition
    auto startTime = std::chrono::steady_clock::now();

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
            ctx.Finish();
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
        FVec2I p1( 0, 64 );
        ctx.RasterGradient( canvas, p0, p1, grad0, 0.f, Gradient_Linear, FRectI( 0, 16, 32, 64 ), FSchedulePolicy::MonoChunk );

        FGradient gradient1( fmt );
        gradient1.AddColorStep( 0.f, FColor::RGB( 0, 0, 21 ) );
        gradient1.AddColorStep( 1.f, FColor::White );
        FSanitizedGradient grad1 = gradient1.Sanitized( fmt );
        FVec2I p2( 848, 40 );
        FVec2I p3( 1024, 0 );
        ctx.RasterGradient( canvas, p2, p3, grad1, 0.f, Gradient_Angular, FRectI( 672, 0, 352, 80 ) );
        ctx.Finish();
    }

    {
        ctx.Copy( canvas, canvas, FRectI( 0, 0, 1024, 144 ), FVec2I( 0, 144 ), FSchedulePolicy::MultiScanlines );
        ctx.Finish();
        ctx.BlendColor( FColor::Black, canvas, FRectI( 0, 144, 1024, 144 ), Blend_Multiply, Alpha_Normal, 0.5f );
        ctx.Finish();
    }

    {
        ctx.Clouds( canvas, -1, FRectI( 0, 288, 205, 144 ), FSchedulePolicy::MultiScanlines, 0, 0, 0 );
        ctx.ValueNoise( canvas, 1.f, -1, FRectI( 205, 288, 205, 144 ), FSchedulePolicy::MultiScanlines, 0, 0, 0 );
        ctx.BrownianNoise( canvas, 0.22f, 1.8f, 0.35f, 10, -1, FRectI( 410, 288, 205, 144 ), FSchedulePolicy::MultiScanlines, 0, 0, 0 );
        ctx.Finish();
    }

    {
        FBlock overlay( 500, 288, fmt );
        FGradient gradient0( fmt );
        gradient0.AddColorStep( 0.f, FColor::RGB( 0, 127, 255 ) );
        gradient0.AddColorStep( 0.5f, FColor::RGB( 255, 127, 0 ) );
        gradient0.AddColorStep( 1.f, FColor::RGB( 0, 127, 255 ) );
        gradient0.AddAlphaStep( 0.f, 0.f );
        gradient0.AddAlphaStep( 0.33f, 0.33f );
        gradient0.AddAlphaStep( 0.5f, 0.8f );
        gradient0.AddAlphaStep( 1.f, 0.f );
        FSanitizedGradient grad0 = gradient0.Sanitized( fmt );
        FVec2I p0( 500, 144 );
        FVec2I p1( 0, 0 );
        ctx.RasterGradient( overlay, p0, p1, grad0, 0.f, Gradient_Radial );
        ctx.Finish();
        ctx.Blend( overlay, canvas, overlay.Rect(), FVec2I( 1024 - 500, 0 ), Blend_BayerDither8x8, Alpha_Normal, 0.9f );
        ctx.Finish();
    }

    {
        FMat3F mat0 = FMat3F::MakeRotationMatrix( FMath::kPIf / 2.f ) * FMat3F::MakeScaleMatrix( 0.5f, 0.5f );
        FMat3F mat1 = FMat3F::MakeRotationMatrix( FMath::kPIf / 4.f ) * FMat3F::MakeScaleMatrix( 0.25f, 0.25f );
        ctx.TransformAffineTiled( canvas, canvas, FRectI( 128, 128, 256, 256 ), FRectI( 615, 288, 1024, 144 ), mat0, Resampling_NearestNeighbour );
        ctx.TransformAffineTiled( canvas, canvas, FRectI( 0, 0, 50, 50 ),  FRectI( 0, 432, 1024, 144 ), mat1, Resampling_Bilinear );
        ctx.Finish();
    }

    {
        ctx.Swap( canvas, 0, 2, FRectI( 16, 16, 16, 64 ), FSchedulePolicy::MonoChunk );
        ctx.Finish();
    }

    {
        ctx.RasterTextAA( canvas, L"Hello ULIS", arial, 15, FMat3F::MakeTranslationMatrix( 20, 20 ), FColor::White );
        ctx.Finish();
    }

    {
        ctx.Blend( canvas, canvas, FRectI::Auto, FVec2I(0), Blend_AngleCorrected, Alpha_Normal, 0.5f );
        ctx.Finish();
        ctx.Blend( canvas, canvas, FRectI::Auto, FVec2I(0), Blend_AngleCorrected, Alpha_Normal, 0.5f );
        ctx.Finish();
        ctx.Blend( canvas, canvas, FRectI::Auto, FVec2I(0), Blend_AngleCorrected, Alpha_Normal, 0.5f );
        ctx.Finish();
        ctx.Blend( canvas, canvas, FRectI::Auto, FVec2I(0), Blend_AngleCorrected, Alpha_Normal, 0.5f );
        ctx.Finish();
    }

    {
        FBlock mip;
        ctx.XBuildMipMap( canvas, mip, -1, FRectI( 0, 0, 1024, 576 ), Resampling_Bilinear );
        ctx.Finish();
        float scale = float( mip.Width() ) / 1024;
        ctx.Resize( mip, canvas, FRectI::Auto, FRectF( 0, 576, 1024, 576 * scale ), Resampling_Bilinear );
        ctx.Finish();
    }

    {
        FBlock noise( 1024, 1024, fmt );
        ctx.WhiteNoise( noise );
        ctx.Finish();
        ctx.Blend( noise, canvas, FRectI::Auto, FVec2I(0), Blend_Normal, Alpha_Normal, 0.25f );
        ctx.Finish();
    }

    {
        FMat3F mat0 = FMat3F::MakeRotationMatrix( FMath::kPIf / 5.f ) * FMat3F::MakeScaleMatrix( 0.2f, 0.2f );
        ctx.TransformAffineTiled( canvas, canvas, FRectI( 0, 0, 1024, 576 ), FRectI( 0, 620, 512, 1024 ), mat0, Resampling_Bilinear );
        ctx.Finish();
        ctx.Blend( canvas, canvas, FRectI( 0, 0, 1024, 576 ), FVec2I( 0, 1024 - 576 + 200), Blend_Behind, Alpha_Normal, 1.f );
        ctx.Finish();
    }

    {
        FBlock clouds( 1024, 1024, fmt );
        ctx.Clouds( clouds );
        ctx.Finish();
        ctx.Blend( clouds, canvas, FRectI::Auto, FVec2I( 0 ), Blend_BayerDither8x8, Alpha_Normal, 0.2f );
        ctx.Finish();
    }

    {
        ctx.VoronoiNoise( canvas, 80, -1, FRectI( 128, 128, 256, 256 ) );
        ctx.Finish();
    }

    {
        FKernel blur = FKernel( FVec2I( 16 ), 1.f ).Normalized();
        ctx.Convolve( canvas, canvas, blur, FRectI( 64, 64, 256, 256 ), FVec2I( 64, 512 ) );
        ctx.Finish();
    }

    FBlock proxy( 1024, 1024, Format_RGBA8 );
    ctx.ConvertFormat( canvas, proxy );
    ctx.Finish();

    // Before displaying the window, gather the end time and delta to output the time it took to process all ULIS operations.
    // We are not interested in the time it took Qt to create the window.
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();

    // Print out the result time.
    std::cout << "ULIS All: Composition took " << delta << "ms." << std::endl;

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( proxy.Bits()
                                        , proxy.Width()
                                        , proxy.Height()
                                        , proxy.BytesPerScanLine()
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

