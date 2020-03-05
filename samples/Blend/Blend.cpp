// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Blend.cpp
* @author       Clement Berthaud
* @brief        Blend application for ULIS2.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS2>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

using namespace ::ul2;

int
main( int argc, char *argv[] ) {
    // Start by defining pool thread to host workers
    // Define the perfIntent flag to use Multithread, Type Specializations, SSE42 or AVX2 if available.
    // ( Note 1: if both SSE42 and AVX2 are available, AVX2 will be chosen. )
    // ( Note 2: often, SSE42 and AVX2 optimisations are available only if Type Specializations are enabled too. )
    // Finally, detect host device to get runtime information about support for SSE and AVX features.
    FThreadPool  threadPool;
    uint32 perfIntent = ULIS2_PERF_MT | ULIS2_PERF_TSPEC | ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2;
    FHostDeviceInfo host = FHostDeviceInfo::Detect();

    // Collect hard-coded paths to images.
    std::string pathBase = "C:/Users/PRAXINOS/Documents/work/base_160.png";
    std::string pathOver = "C:/Users/PRAXINOS/Documents/work/over_160.png";

    // Load both blocks Base and Over.
    // Specify ULIS2_FORMAT_RGBA8 as desired format,
    // meaning that if the loaded block format is not already RGBA8,
    // a conversion will be performed to obtain the expected format.
    // Specify ULIS2_NONBLOCKING flag, so that pool will not wait for completion after each function
    // We can do that because both loading processes are independant and do not interfere with each other
    // Passing ULIS2_NONBLOCKING avoids stalling beetween the two functions.
    // ( Note: the 'X' prefix before a function name always means the function allocates a block and returns the pointer,
    // the caller is now responsible for the FBlock* lifetime, and should delete it ).
    FBlock* blockBase = XLoadFromFile( &threadPool, ULIS2_NONBLOCKING, perfIntent, host, ULIS2_NOCB, pathBase, ULIS2_FORMAT_RGBA8 );
    FBlock* blockOver = XLoadFromFile( &threadPool, ULIS2_NONBLOCKING, perfIntent, host, ULIS2_NOCB, pathOver, ULIS2_FORMAT_RGBA8 );

    // Fence the pool here,
    // After the two calls to XLoadFromFile, the functions returned immediately even though the data isn't loaded yet
    // ( Note: the blocks buffers have been allocated and the pointers to blockBase and blockOver are valid though )
    // A fence allows the program to halt here until the data is actually loaded.
    Fence( threadPool );

    // Gather a few information on loaded images
    // They will be tiled onto a background block to display all the blending modes.
    // Expect small-ish blocks, e.g: 160*160px to avoid having a window overflowing the desktop
    // As of now there are 40 blending modes so the tiles will be layout in a 8 * 5 grid
    // Get source rect here to avoid collect the same rect many times
    // We will reuse its value many times.
    // We don't need to seek a subrect in Base / Over as we want to blend the full tile.
    // We also assume Base and Over share the same size here for nicer results.
    FRect sourceRect = blockBase->Rect();
    int w = sourceRect.w * 8;
    int h = sourceRect.h * 5;

    // Allocate a new block
    // The caller is responsible for destructing the blockCanvas object here too.
    // The block has the same format ULIS2_FORMAT_RGBA8 as requested for the two blocks before.
    FBlock* blockCanvas = new  FBlock( w, h, ULIS2_FORMAT_RGBA8 );

    // Start processing the blocks
    // We will first tile the base block layout on a regular grid in the blockCanvas block
    // For that we do not need to perform a blend, a simple copy should suffice.
    // Then we blend the over block over the canvas, where the base has been copied first.
    for( int i = 0; i < NUM_BLENDING_MODES; ++i ) {
        // Compute x & y in regular grid, remember we tile it in a 8 * 5 grid and NUM_BLENDING_MODES = 40.
        int x = ( i % 8 ) * sourceRect.w;
        int y = ( i / 8 ) * sourceRect.h;

        // First perform the Copy, specifying threadPool, blocking flag, performance intent, host, callback, and parameters as usual.
        // The first 5 parameters are commone to most ULIS2 functions and are used to know how to perform a task.
        // The user provides intent and control over the CPU optimization dispatch method ( MEM, SSE, AVX ) and over the CPU multithreading dispatch too.
        // Then we perform the blend by iterating over all blending modes ( see i cast to eBlendingMode enum value ).
        // By default we'll use a normal alphaMode for nicer results in this context, and an opacity of 0.5, which is a normalized value that corresponds to 50%, half-fade.
        Copy(   &threadPool, ULIS2_NONBLOCKING, perfIntent, host, ULIS2_NOCB, blockBase, blockCanvas, sourceRect, FVec2I( x, y ) );
        Blend(  &threadPool, ULIS2_NONBLOCKING, perfIntent, host, ULIS2_NOCB, blockOver, blockCanvas, sourceRect, FVec2F( x, y ), ULIS2_NOAA, static_cast< eBlendingMode >( i ), AM_NORMAL, 0.5f );
    }
    // Fence the pool here,
    // You may have noticed that both calls to Copy and Blend specify ULIS2_NONBLOCKING flag.
    // This may strike you as odd, as one could be worried about data race and concurrency issues while reading and writing to blocks.
    // Actually, there are actually 40 * 2 = 80 non blocking operations scheduled together, each of which will in turn schedule a task
    // for each scanline in the specified rects. Assuming the rect is valid and not cropped, and that height = 160, that will lead to 80 * 160 = 12800
    // scheduled scanline processes, with potential concurrency issues.
    // This is right. This setup is unsafe in and by itself. However, since it's assumed there are 160 scanlines and since two iterations are non concurrent,
    // and since no one has a machine with 160 cores and since blend operation is slower than copy operation, it is *most of the time* safe to do so.
    // Here it avoids many stalls during the blend, however for safety in a professional application you would most likely not mess with that and fence immediately after Copy.
    Fence( threadPool );

    // Get rid of Base and Over, we don't need them anymore.
    delete  blockBase;
    delete  blockOver;

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

