// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         ClearFillCopy.cpp
* @author       Clement Berthaud
* @brief        ClearFillCopy application for ULIS2.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS2>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include <chrono>

using namespace ::ul2;

int
main( int argc, char *argv[] ) {
    // Gather start time to output the time it took to perform the blend composition
    auto startTime = std::chrono::steady_clock::now();

    // Start by defining pool thread to host workers
    // Define the perfIntent flag to use Multithread, Type Specializations, SSE42 or AVX2 if available.
    // ( Note 1: if both SSE42 and AVX2 are available, AVX2 will be chosen. )
    // ( Note 2: often, SSE42 and AVX2 optimisations are available only if Type Specializations are enabled too. )
    // Finally, detect host device to get runtime information about support for SSE and AVX features.
    FThreadPool  threadPool;
    uint32 perfIntent = ULIS2_PERF_MT | ULIS2_PERF_TSPEC | ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2;
    FHostDeviceInfo host = FHostDeviceInfo::Detect();

    // Create one RGBA8 block to host the operations
    // Specify ULIS2_FORMAT_RGBA8 as desired format for display
    FBlock* blockCanvas = new FBlock( 900, 600, ULIS2_FORMAT_RGBA8 );

    // Start processing the blocks
    // We will first fill a part of the block
    // Next we will clear a part of the block
    // Finally we will copy a part of the block
    // Let's first define the fill color for our block:
    FPixelValue color = FPixelValue::FromRGBA8( 255, 0, 0 );

    // First perform the Copy, specifying threadPool, blocking flag, performance intent, host, callback, and parameters as usual.
    // The first 5 parameters are common to most ULIS2 functions and are used to know how to perform a task.
    // The user provides intent and control over the CPU optimization dispatch method ( MEM, SSE, AVX ) and over the CPU multithreading dispatch too.
    // Notice the NONBLOCKING here: we don't need to fence because we work on different areas of the block for the first two operations.
    Fill(   &threadPool, ULIS2_NONBLOCKING, perfIntent, host, ULIS2_NOCB, blockCanvas, color, FRect( 0, 0, 300, 600 ) );
    Clear(  &threadPool, ULIS2_NONBLOCKING, perfIntent, host, ULIS2_NOCB, blockCanvas, FRect( 300, 0, 300, 600 ) );
    Fence( threadPool );
    Copy(   &threadPool, ULIS2_BLOCKING, perfIntent, host, ULIS2_NOCB, blockCanvas, blockCanvas, FRect( 0, 0, 300, 600 ), FVec2I( 600, 0 ) );

    // Now let's see something interesting:
    // We can fill a block in a specific format like RGBA8 with a color source that uses any model, and any format, without worries
    // For example, we can fill a RGB block from a Lab or HSV source
    FPixelValue colorHSV = FPixelValue::FromHSVAF( 0.5f, 1.f, 1.f );
    Fill( &threadPool, ULIS2_NONBLOCKING, perfIntent, host, ULIS2_NOCB, blockCanvas, colorHSV, FRect( 0, 300, 900, 300 ) );

    // Before displaying the window, gather the end time and delta to output the time it took to process all ULIS2 operations.
    // We are not interested in the time it took Qt to create the window.
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();

    // Let's recap:
    // We performed a block allocation
    // We performed 1 fill
    // We performed 1 clears
    // We performed 1 copy
    // That makes:
    //      1 Alloc     ( 900*600 )
    //      1 Fill      ( 300*600 )
    //      1 Clear     ( 300*600 )
    //      1 Copy      ( 300*600 )
    // Average on my desktop setup: 14ms
    // Average on my laptop setup:  <unavailable>
    // Remember: everything is multithreaded, SSE and AVX are used whenever possible, everything is computed on CPU
    // Print out the result time.
    std::cout << "ULIS2 ClearFillCopy: Composition took " << delta << "ms." << std::endl;

    // Create a Qt application and a simple window to display the result block we computed.
    // We create a QImage from the blockCanvas data, QImage does not own the data, so it still lives in blockCanvas, so we don't delete it right now.
    // For Qt Interoperability, several formats are compatible with ULIS2 formats. Here we chose RGBA8888 which has the same memory layout as ULIS2_FORMAT_RGBA8
    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockCanvas->DataPtr()
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

