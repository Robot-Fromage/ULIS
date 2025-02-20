// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ClearFillCopy.cpp
* @author       Clement Berthaud
* @brief        ClearFillCopy application for ULIS.
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
    // Start by defining pool thread to host workers.
    FThreadPool pool;

    // Define the command queue, multiple queues can use the same pool.
    // They can also use different pools to fine tune performances and stalling.
    FCommandQueue queue( pool );

    // Document format is RGBA8, this will concern the context and the processed blocks.
    eFormat fmt = Format_RGBA8;

    // Define the context, multiple contexts can share a queue or get to each their own.
    // Here we specify our perf intent is to use AVX when available, for all image processing tasks.
    // If AVX isn't available it will fallback to SSE, and if it is not available either we'll fallback
    // to generic MEM version ( no SIMD )
    FContext ctx( queue, fmt, PerformanceIntent_AVX );

    // Create one RGBA8 block to host the operations
    // Using Format_RGBA8 as desired format for display
    FBlock* blockCanvas = new FBlock( 1024, 1024, fmt );
    FBlock* blockCanvas2 = new FBlock( 1024, 1024, fmt );

    // Start processing the blocks
    // We will first fill a part of the block
    // Next we will clear a part of the block
    // Finally we will copy a part of the block
    // Let's first define the fill color for our block:
    FColor color = FColor::RGBA8( 255, 0, 0 );
    ctx.Fill( *blockCanvas, color );
    ctx.Finish();

    // Gather start time to output the time it took to perform the blend composition
    auto startTime = std::chrono::steady_clock::now();

    for( int i = 0; i < 100; ++i ) {
        for( int x = 0; x < 1024; x += 16 ) {
            for( int y = 0; y < 1024; y += 16 ) {
                ctx.Copy( *blockCanvas, *blockCanvas2, FRectI( x, y, 16, 16 ), FVec2I( x, y ), FSchedulePolicy::MonoScanlines );
                ctx.Flush();
            }
        }
        ctx.Fence();
    }

    // Before displaying the window, gather the end time and delta to output the time it took to process all ULIS operations.
    // We are not interested in the time it took Qt to create the window.
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count() / 100.f;

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
    std::cout << "ULIS ClearFillCopy: Composition took " << delta << "ms." << std::endl;

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

