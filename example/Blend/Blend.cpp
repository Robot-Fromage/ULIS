// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Blend.cpp
* @author       Clement Berthaud
* @brief        Blend application for ULIS3.
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

int
main( int argc, char *argv[] ) {
    // Start by defining thread pool, to host workers.
    // Then command queue to enqueue commands deferred commands.
    // Then working format and context for this format with specified performance intent.
    // Finally, some policies for how the tasks should be carried out.
    FThreadPool pool( 4 );
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_AVX );
    FHardwareMetrics hw;
    FSchedulePolicy policy_sync_cache_efficient( ScheduleTime_Sync, ScheduleRun_Multi,ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policy_sync_mono_chunk( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );
    FSchedulePolicy policy_sync_multi_scanlines( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Scanlines );
    FSchedulePolicy policy_sync_mono_scanlines( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Scanlines );

    // Gather start time to output the time it took to perform the blend composition
    auto startTime = std::chrono::steady_clock::now();

    // Create both "hollow" blocks Base and Over.
    FBlock blockBase;
    FBlock blockOver;
    {
        // Collect hard-coded paths to images.
        std::string pathBase = "C:/Users/PRAXINOS/Documents/work/base_160.png";
        std::string pathOver = "C:/Users/PRAXINOS/Documents/work/over_160.png";
        // Load from file into blocks
        ulError err;
        err = ctx.XLoadBlockFromDisk( blockBase, pathBase );
        ULIS_ASSERT( !err, "Load failed" );
        err = ctx.XLoadBlockFromDisk( blockOver, pathOver );
        ULIS_ASSERT( !err, "Load failed" );
        ULIS_ASSERT( blockBase.Format() == fmt, "Bad format assumption." );
        ULIS_ASSERT( blockOver.Format() == fmt, "Bad format assumption." );

        // Flush all commands
        ctx.Flush();

        // Wait for completion
        ctx.Fence();

        // Flush() + Fence() is equivalent to Finish()
    }

    // Gather a few information on loaded images
    // They will be tiled onto a background block to display all the blending modes.
    // Expect small-ish blocks, e.g: 160*160px to avoid having a window overflowing the desktop
    // As of now there are 40 blending modes so the tiles will be layout in a 8 * 5 grid
    // Get source rect here to avoid collect the same rect many times
    // We will reuse its value many times.
    // We don't need to seek a subrect in Base / Over as we want to blend the full tile.
    // We also assume Base and Over share the same size here for nicer results.
    FRectI srcRect = blockBase.Rect();
    int w = srcRect.w * 8;
    int h = srcRect.h * 5;

    // Allocate a new block
    // The caller is responsible for destructing the blockCanvas object here too.
    // The block has the same format ULIS_FORMAT_RGBA8 as requested for the two blocks before.
    FBlock* blockCanvas = new  FBlock( w, h, fmt );

    // Start processing the blocks
    // We will first tile the base block layout on a regular grid in the blockCanvas block
    // For that we do not need to perform a blend, a simple copy should suffice.
    // Then we blend the over block over the canvas, where the base has been copied first.
    for( int i = 0; i < NumBlendingModes; ++i ) {
        // Compute x & y in regular grid, remember we tile it in a 8 * 5 grid and NUM_BLENDING_MODES = 40.
        int x = ( i % 8 ) * srcRect.w;
        int y = ( i / 8 ) * srcRect.h;

        // Notice the async part:
        // All copies are non concurrent and can be scheduled without wait dependencies
        // But the blends are concurrent with their respective copy, so each blend waits on one copy.
        // The policies will default to mono scanlines despite our input because of the nature of the task ( unaligned non contiguous images )
        FEvent cp_ev;
        ctx.Copy( blockBase, *blockCanvas, srcRect, FVec2I( x, y ), policy_sync_mono_chunk, 0, nullptr, &cp_ev );
        ctx.Flush();
        ctx.Blend( blockOver, *blockCanvas, srcRect, FVec2I( x, y ), static_cast< eBlendMode >( i ), Alpha_Normal, 0.5f, policy_sync_mono_chunk, 1, &cp_ev, nullptr );
    }

    // Flush and Fence.
    ctx.Finish();

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
    //      4 Alloc     ( 160² )
    //      2 Conv      ( 160² )
    //      1 Alloc     ( 1280*800 )
    //      40 Copy     ( 160² )
    //      40 Blend    ( 160² )
    // Average on my desktop setup BEFORE ULIS3: 55ms
    // Average on my desktop setup NOW WITH ULIS4: 7ms
    // Average on my laptop setup:  <unavailable>
    // Remember: everything is multithreaded, SSE and AVX are used whenever possible, everything is computed on CPU
    // Print out the result time.
    std::cout << "ULIS Blend: Composition took " << delta << "ms." << std::endl;

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

    // Return exit code.
    return  exit_code;
}

