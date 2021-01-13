// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Text.cpp
* @author       Clement Berthaud
* @brief        Text application for ULIS3.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include <chrono>

using namespace ::ul3;

int
main( int argc, char *argv[] ) {
    // Gather start time to output the time it took to perform the blend composition
    auto startTime = std::chrono::steady_clock::now();

    // Start by defining pool thread to host workers
    // Define the perfIntent flag to use Multithread, Type Specializations, SSE42 or AVX2 if available.
    // ( Note 1: if both SSE42 and AVX2 are available, AVX2 will be chosen. )
    // ( Note 2: often, SSE42 and AVX2 optimisations are available only if Type Specializations are enabled too. )
    // Finally, detect host device to get runtime information about support for SSE and AVX features.
    FThreadPool* threadPool = XCreateThreadPool();
    uint32 perfIntent = ULIS_PERF_MT | ULIS_PERF_SSE42;
    FHardwareMetrics host = FHardwareMetrics::Detect();

    // Start processing the block
    // Create one RGBA8 block to host the operations
    // Specify ULIS_FORMAT_RGBA8 as desired format for display
    // We will first fill the block background
    // Next we will then draw some text onto it.
    // Let's define everything:
    int w = 900;
    int h = 600;
    FBlock* blockCanvas = new FBlock( w, h, ULIS_FORMAT_RGBA8 );
    FColor color = FColor::FromRGBA8( 255, 0, 0 );
    std::wstring str1 = L"ULIS";
    std::wstring str2 = L"ユリッス";
    std::wstring str3 = L"ゆりっす";
    FFontEngine FontEngine;
    FFontRegistry fontRegistry( FontEngine );
    FFont fontEU( fontRegistry, "Arial", "Black" );
    FFont fontJA( fontRegistry, "Yu Gothic", "Bold" );
    FTransformation2D identityTransform = FTransformation2D();

    int fontSize = 64;
    FRectI rect1 = TextMetrics( str1, fontEU, fontSize, identityTransform );
    FRectI rect2 = TextMetrics( str2, fontJA, fontSize, identityTransform );
    FRectI rect3 = TextMetrics( str3, fontJA, fontSize, identityTransform );
    rect1.x = ( w - rect1.w ) / 2;
    rect2.x = ( w - rect2.w ) / 2;
    rect3.x = ( w - rect3.w ) / 2;
    rect2.y = rect1.h;
    rect3.y = rect1.h + rect2.h;
    int maxy = rect3.y + rect3.h;
    int miny = ( h - maxy ) / 2;
    rect1.y = miny;
    rect2.y = miny + rect1.h;
    rect3.y = miny + rect1.h + rect1.h;
    FTransformation2D transform1( FTransformation2D::MakeTranslationTransform( rect1.x, rect1.y ) );
    FTransformation2D transform2( FTransformation2D::MakeTranslationTransform( rect2.x, rect2.y ) );
    FTransformation2D transform3( FTransformation2D::MakeTranslationTransform( rect3.x, rect3.y ) );
    FColor backgroundColor = FColor::FromHSVAF( 0.5f, 0.9f, 0.8f, 1.f );
    FColor fontColor       = FColor::FromRGBA8( 255, 255, 255, 255 );

    // Let's process the block:
    Fill( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockCanvas, backgroundColor, blockCanvas->Rect() );
    RenderText( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockCanvas, str1, fontEU, fontSize, fontColor, transform1, ULIS_AA );
    RenderText( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockCanvas, str2, fontJA, fontSize, fontColor, transform2, ULIS_AA );
    RenderText( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockCanvas, str2, fontJA, fontSize, fontColor, transform3, ULIS_AA );

    // Before displaying the window, gather the end time and delta to output the time it took to process all ULIS3 operations.
    // We are not interested in the time it took Qt to create the window.
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();
    // Average on my desktop setup: 14ms
    // Average on my laptop setup:  <unavailable>
    // Remember: everything is multithreaded, SSE and AVX are used whenever possible, everything is computed on CPU
    // Print out the result time.
    std::cout << "ULIS3 Text: Composition took " << delta << "ms." << std::endl;

    // Create a Qt application and a simple window to display the result block we computed.
    // We create a QImage from the blockCanvas data, QImage does not own the data, so it still lives in blockCanvas, so we don't delete it right now.
    // For Qt Interoperability, several formats are compatible with ULIS3 formats. Here we chose RGBA8888 which has the same memory layout as ULIS_FORMAT_RGBA8
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
    XDeleteThreadPool( threadPool );

    // Return exit code.
    return  exit_code;
}

