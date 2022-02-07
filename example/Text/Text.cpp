// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Text.cpp
* @author       Clement Berthaud
* @brief        Text application for ULIS.
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
    FContext ctx( queue, fmt, PerformanceIntent_AVX );

    int w = 600;
    int h = 600;
    FBlock* blockCanvas = new FBlock( w, h, Format_RGBA8 );
    FColor color = FColor::RGBA8( 255, 0, 0 );
    FWString str1 = L"Text.";
    FFontEngine fontEngine;
    FFont font( fontEngine, "Segoe UI", "Bold" );
    int fontSize = 160;
    FMat3F transform1( FMat3F::MakeTranslationMatrix( 0, 0 ) );
    FRectI rect1 = ctx.TextMetrics( str1, font, fontSize, transform1 );
    rect1.x = ( w - rect1.w ) / 2;
    rect1.y = ( h - rect1.h ) / 2;
    FColor backgroundColor = FColor::RGBA8( 180, 0, 255 );
    FColor fontColor       = FColor::RGBA8( 255, 255, 255, 250 );

    auto startTime = std::chrono::steady_clock::now();
    // Let's process the block:
    ctx.Fill( *blockCanvas, backgroundColor, blockCanvas->Rect() );
    ctx.Finish();
    ctx.RasterTextAA( *blockCanvas, str1, font, fontSize, transform1, fontColor );
    ctx.Finish();

    // We are not interested in the time it took Qt to create the window.
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();
    // Average on my desktop setup: 14ms
    // Average on my laptop setup:  <unavailable>
    // Remember: everything is multithreaded, SSE and AVX are used whenever possible, everything is computed on CPU
    // Print out the result time.
    std::cout << "ULIS Text: Composition took " << delta << "ms." << std::endl;

    // Create a Qt application and a simple window to display the result block we computed.
    // We create a QImage from the blockCanvas data, QImage does not own the data, so it still lives in blockCanvas, so we don't delete it right now.
    // For Qt Interoperability, several formats are compatible with ULIS formats. Here we chose RGBA8888 which has the same memory layout as ULIS_FORMAT_RGBA8
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

    // Get error code. This function returns when all Qt Windows are closed.
    int exit_code = app.exec();

    // Delete Qt Components.
    delete  label;
    delete  image;
    delete  widget;

    // Delete our block Canvas.
    delete  blockCanvas;

    // Return exit code.
    return  exit_code;
}

