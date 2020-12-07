// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Blend.cpp
* @author       Clement Berthaud
* @brief        Blend application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
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
    // Gather start time to output the time it took to perform the blend composition
    auto startTime = std::chrono::steady_clock::now();

    // Start by defining pool thread to host workers
    // Define the perfIntent flag to use Multithread, Type Specializations, SSE42 or AVX2 if available.
    // ( Note 1: if both SSE42 and AVX2 are available, AVX2 will be chosen. )
    // ( Note 2: often, SSE42 and AVX2 optimisations are available only if Type Specializations are enabled too. )
    // Finally, detect host device to get runtime information about support for SSE and AVX features.
    FThreadPool* threadPool = XCreateThreadPool();
    uint32 perfIntent = 0;
    FHardwareMetrics host = FHardwareMetrics::Detect();

    // Collect hard-coded paths to images.
    std::string pathBase = "C:/Users/PRAXINOS/Documents/work/base_160.png";
    std::string pathOver = "C:/Users/PRAXINOS/Documents/work/over_160.png";

    // Load both blocks Base and Over.
    // Specify ULIS_FORMAT_RGBA8 as desired format,
    // meaning that if the loaded block format is not already RGBA8,
    // a conversion will be performed to obtain the expected format.
    // Specify ULIS_NONBLOCKING flag, so that pool will not wait for completion after each function
    // We can do that because both loading processes are independant and do not interfere with each other
    // Passing ULIS_NONBLOCKING avoids stalling beetween the two functions.
    // ( Note: the 'X' prefix before a function name always means the function allocates a block and returns the pointer,
    // the caller is now responsible for the FBlock* lifetime, and should delete it ).
    FBlock* blockBase = XLoadFromFile( threadPool, ULIS_NONBLOCKING, perfIntent, host, ULIS_NOCB, pathBase, Format_RGBA8 );
    FBlock* blockOver = XLoadFromFile( threadPool, ULIS_NONBLOCKING, perfIntent, host, ULIS_NOCB, pathOver, Format_RGBA8 );

    // Fence the pool here,
    // After the two calls to XLoadFromFile, the functions returned immediately even though the data isn't loaded yet
    // ( Note: the blocks buffers have been allocated and the pointers to blockBase and blockOver are valid though )
    // A fence allows the program to halt here until the data is actually loaded.
    Fence( *threadPool );

    // Gather a few information on loaded images
    // They will be tiled onto a background block to display all the blending modes.
    // Expect small-ish blocks, e.g: 160*160px to avoid having a window overflowing the desktop
    // As of now there are 40 blending modes so the tiles will be layout in a 8 * 5 grid
    // Get source rect here to avoid collect the same rect many times
    // We will reuse its value many times.
    // We don't need to seek a subrect in Base / Over as we want to blend the full tile.
    // We also assume Base and Over share the same size here for nicer results.
    FRectI sourceRect = blockBase->Rect();
    int w = sourceRect.w * 8;
    int h = sourceRect.h * 5;

    // Allocate a new block
    // The caller is responsible for destructing the blockCanvas object here too.
    // The block has the same format ULIS_FORMAT_RGBA8 as requested for the two blocks before.
    FBlock* blockCanvas = new  FBlock( w, h, Format_RGBA8 );

    // Start processing the blocks
    // We will first tile the base block layout on a regular grid in the blockCanvas block
    // For that we do not need to perform a blend, a simple copy should suffice.
    // Then we blend the over block over the canvas, where the base has been copied first.
    for( int i = 0; i < NumBlendingModes; ++i ) {
        // Compute x & y in regular grid, remember we tile it in a 8 * 5 grid and NUM_BLENDING_MODES = 40.
        int x = ( i % 8 ) * sourceRect.w;
        int y = ( i / 8 ) * sourceRect.h;

        // First perform the Copy, specifying threadPool, blocking flag, performance intent, host, callback, and parameters as usual.
        // The first 5 parameters are common to most ULIS3 functions and are used to know how to perform a task.
        // The user provides intent and control over the CPU optimization dispatch method ( MEM, SSE, AVX ) and over the CPU multithreading dispatch too.
        // Notice the BLOCKING here: we don't want Copy and Blend to be concurrent as they work on the same region in a given loop iteration.
        Copy(   threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockBase, blockCanvas, sourceRect, FVec2I( x, y ) );

        // Then we perform the blend by iterating over all blending modes ( see i cast to eBlendMode enum value ).
        // By default we'll use a normal alphaMode for nicer results in this context, and an opacity of 0.5, which is a normalized value that corresponds to 50%, half-fade.
        Blend(  threadPool, ULIS_NONBLOCKING, perfIntent, host, ULIS_NOCB, blockOver, blockCanvas, sourceRect, FVec2F( x, y ), ULIS_NOAA, static_cast< eBlendMode >( i ), Alpha_Normal, 0.5f );
    }
    // Fence the pool here to make sure the very last blend is completed.
    // You may have noticed that we did not fence after Blend inside the loop.
    // This may strike you as odd, as one could be worried about data race and concurrency issues while reading and writing to blocks.
    // This is safe here because copy and blend *might* be concurrent only within a given loop iteration.
    // On the next loop, even if the Blend is not complete, the copy can take over and be scheduled regardless of blend completion status,
    // because it will work on another region. That way we can avoid a few stalls and this is perfectly safe concurrency wise.
    Fence( *threadPool );

    // Get rid of Base and Over, we don't need them anymore.
    delete  blockBase;
    delete  blockOver;

    // Before displaying the window, gather the end time and delta to output the time it took to process all ULIS3 operations.
    // We are not interested in the time it took Qt to create the window.
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();

    // Let's recap:
    // We performed two loads with potential conversion
    // We performed a block allocation
    // We performed 40 block copies
    // We performed 40 block blends with all the most sophisticated blending modes
    // That makes, worst case:
    //      4 Alloc     ( 160� )
    //      2 Conv      ( 160� )
    //      1 Alloc     ( 1280*800 )
    //      40 Copy     ( 160� )
    //      40 Blend    ( 160� )
    // Average on my desktop setup: 55ms
    // Average on my laptop setup:  <unavailable>
    // Remember: everything is multithreaded, SSE and AVX are used whenever possible, everything is computed on CPU
    // Print out the result time.
    std::cout << "ULIS3 Blend: Composition took " << delta << "ms." << std::endl;

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

    // Delete the thread pool
    XDeleteThreadPool( threadPool );

    // Return exit code.
    return  exit_code;
}

