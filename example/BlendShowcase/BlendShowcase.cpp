// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BlendShowcase.cpp
* @author       Clement Berthaud
* @brief        BlendShowcase application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include <codecvt>

using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_GA8;
    FContext ctx( queue, fmt, PerformanceIntent_Max );

    // Create both "hollow" blocks Base and Over.
    FBlock blockBase( 160, 160, fmt );
    FBlock blockOver( 160, 160, fmt );
    {
        ctx.Fill( blockBase, FColor::Red );
        ctx.Fill( blockOver, FColor::Blue );
        ctx.Finish();
    }

    FRectI srcRect = blockBase.Rect();
    int w = srcRect.w * 8;
    int h = srcRect.h * 5;

    int shadeW = srcRect.w;
    int shadeH = 20;
    FBlock blockShade( shadeW, shadeH, fmt );
    FColor black = FColor::RGBA8( 0, 0, 0 );
    ctx.Fill( blockShade, black, blockShade.Rect(), FSchedulePolicy::CacheEfficient );
    ctx.Flush();

    FBlock blockCanvas( w, h, fmt );
    FRectI shadeRect = blockBase.Rect();
    FColor white = FColor::RGBA8( 255, 255, 255 );

    FFontEngine fontEngine;
    FFont font( fontEngine, "Segoe UI", "Light" );

    for( int i = 0; i < NumBlendModes; ++i ) {
        int x = ( i % 8 ) * srcRect.w;
        int y = ( i / 8 ) * srcRect.h;
        FEvent event_copy;
        ctx.Copy( blockBase, blockCanvas, srcRect, FVec2I( x, y ), FSchedulePolicy::MonoChunk, 0, nullptr, &event_copy );
        ctx.Flush();
        FEvent event_blend0;
        FEvent event_blend1;
        ctx.Blend( blockOver, blockCanvas, srcRect, FVec2I( x, y ), static_cast< eBlendMode >( i ), Alpha_Normal, 0.75f, FSchedulePolicy::MonoChunk, 1, &event_copy, &event_blend0 );
        ctx.Blend( blockShade, blockCanvas, srcRect, FVec2I( x, y + srcRect.h - shadeH ), Blend_Normal, Alpha_Normal, 0.5f, FSchedulePolicy::MonoChunk, 1, &event_blend0, &event_blend1 );
    }
    ctx.Finish();

    // We process the text in a separate loop because we have a concurrency constraint on the FFont object.
    for( int i = 0; i < NumBlendModes; ++i ) {
        int x = ( i % 8 ) * srcRect.w;
        int y = ( i / 8 ) * srcRect.h;
        std::string bm = kwBlendMode[i];
        typedef std::codecvt_utf8<wchar_t> convert_type;
        std::wstring_convert<convert_type, wchar_t> converter;
        std::wstring wbm = converter.from_bytes(bm);
        FWString wstr( wbm.c_str() );
        ctx.RasterText( blockCanvas, wstr, font, 16, FMat3F::MakeTranslationMatrix( x + 4, y + srcRect.h - 4 ), white, FSchedulePolicy::MonoChunk );
        ctx.Finish();
    }

    FBlock block8( w, h, Format_G8 );
    ctx.ConvertFormat( blockCanvas, block8 );
    ctx.Finish();

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( block8.Bits()
                                        , block8.Width()
                                        , block8.Height()
                                        , block8.BytesPerScanLine()
                                        , QImage::Format_Grayscale8 );
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

