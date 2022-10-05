// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TextShowcase.cpp
* @author       Clement Berthaud
* @brief        TextShowcase application for ULIS.
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
#include <codecvt>

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_AVX );
    FHardwareMetrics hw;
    FSchedulePolicy policy_cache_efficient( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policy_mono_chunk( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );

    FFontEngine fontEngine;
    int fontSize = 12;
    int entryHeight = 16;
    int entryWidth = 256;
    int gridx = 5;
    int gridy = 40;
    int canvasWidth = entryWidth * gridx;
    int canvasHeight = entryHeight * gridy;

    FRectI globalRect( 0, 0, canvasWidth, canvasHeight );
    FBlock* blockCanvas = new  FBlock( globalRect.w, globalRect.h, Format_RGBA8 );
    FColor black = FColor::Black;
    FColor white = FColor::White;
    ctx.Fill( *blockCanvas, white, blockCanvas->Rect(), policy_cache_efficient );
    ctx.Finish();

    int i = 0;
    for( auto family : fontEngine.Records() ) {
        for( auto style : family.second.Styles() ) {
            int x = ( i / gridy ) * entryWidth;
            int y = ( i % gridy ) * entryHeight;
            const FFontStyleEntry& key = style.second;

            FFont font( fontEngine, key.Family(), key.Style() );

            std::string txt = key.Family() + " " + key.Style();
            typedef std::codecvt_utf8<wchar_t> convert_type;
            std::wstring_convert<convert_type, wchar_t> converter;
            std::wstring wtxt = converter.from_bytes(txt);

            ctx.RasterTextAA( *blockCanvas, wtxt, font, fontSize, FMat3F::MakeTranslationMatrix( x, y + fontSize ), black, policy_mono_chunk );
            ctx.Finish();
            ++i;
        }
    }

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockCanvas->Bits()
                                        , blockCanvas->Width()
                                        , blockCanvas->Height()
                                        , blockCanvas->BytesPerScanLine()
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

    delete  blockCanvas;

    return  exit_code;
}

