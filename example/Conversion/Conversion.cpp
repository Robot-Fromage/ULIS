// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Conversion.cpp
* @author       Clement Berthaud
* @brief        Conversion application for ULIS3.
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
    // Gather start time to output the time it took to perform the conversion operation
    auto startTime = std::chrono::steady_clock::now();

    // Start by defining pool thread to host workers
    // Define the perfIntent flag to use Multithread, Type Specializations, SSE42 or AVX2 if available.
    // ( Note 1: if both SSE42 and AVX2 are available, AVX2 will be chosen. )
    // ( Note 2: often, SSE42 and AVX2 optimisations are available only if Type Specializations are enabled too. )
    // Finally, detect host device to get runtime information about support for SSE and AVX features.
    FThreadPool* threadPool = XCreateThreadPool();
    uint32 perfIntent = ULIS_PERF_MT | ULIS_PERF_SSE42 | ULIS_PERF_AVX2;
    FHardwareMetrics host = FHardwareMetrics::Detect();

    // Create two blocks: one in LAB, one in RGB for hosting the result conversion and display.
    // First block is LabD meaning layout in memory LabLabLabLab... with channel type double. No alpha, not needed here.
    // Second block is RGBA8 meaning layout in memory RGBARGBARGBA... with channel type uint8. With alpha, no problem for conversion compatibility.
    int w = 800;
    int h = 600;
    FBlock* blockLAB = new  FBlock( w, h, ULIS_FORMAT_LabD );
    FBlock* blockRGB = new  FBlock( w, h, ULIS_FORMAT_RGBA8 );

    // To illustrate the conversion operation, we will first perform a gradient in the Lab block, taking avantage of the nice color interpolation in LAB
    // We will then convert the Lab gradient to RGB to see the result, and observe the interpolation goes through a more appropriate chromatic path
    // We first bake our colors here, we specify pure red and green in RGB, then we convert this to Lab in order to be able to interpolate Lab values directly.
    FColor rgbGreen(   ULIS_FORMAT_RGB8, {   0, 255,   0 } );
    FColor rgbRed(     ULIS_FORMAT_RGB8, { 255,   0,   0 } );
    FColor labGreen = Conv( rgbGreen,  ULIS_FORMAT_LabD );
    FColor labRed   = Conv( rgbRed,    ULIS_FORMAT_LabD );

    // Now we perform a gradient manually in the lab block here to illustrate the interpolation occured in LAB
    // When performing later conversion to RGB, we'll see the nice result of the color blending.
    // This manual gradient computation is far from optimal but it lets us see what really happens in term of computation in the Lab block.
    // For more efficiency, we could use the dedicated Gradient functions, or at least multithread this sample.
    for( int x = 0; x < w; ++x ) {
        double lerp  = x / static_cast< double >( w );  // Compute gradient parameter for a linear horizontal gradient over the entire image.
        double L = lerp * labGreen.LD() + ( 1.0 - lerp ) * labRed.LD();
        double a = lerp * labGreen.aD() + ( 1.0 - lerp ) * labRed.aD();
        double b = lerp * labGreen.bD() + ( 1.0 - lerp ) * labRed.bD();
        for( int y = 0; y < h; ++y ) {
            // We won't explain proxies here, see the Pixel sample for more info.
            FPixel prox = blockLAB->Pixel( x, y );
            prox.SetLD( L );
            prox.SetaD( a );
            prox.SetbD( b );
        }
    }

    // Perform the conversion.
    // The conversion happens into blockRGB, using its format to read the appropriate conversion to use.
    // We could also use the XConv version which would have yield us a new block in a desired format.
    // The fact that the source block lacks alpha is not a concern for us, max alpha values are assumed by default if the destination has alpha like here.
    // For the basic conv operations, we always assume an RGB block is using the sRGB colorspace, and that Lab blocks use CIELab with illuminant D60.
    // conversion to XYZ and subsequent linear RGB are done assuming this predicate too.
    // The channel type is different for the two blocks here too, so conv can be used to convert a block depth too ( e.g: RGBA16 to RGBA8 ).
    // Notice we passed the ULIS_BLOCKING flag here, that means the Conv function will not return until the conversion is complete.
    // We don't care about stalling here since there are no multihtreaded operations following, yet we need to ensure the blockRGB is valid before we go on.
    Conv( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockLAB, blockRGB );

    // Get rid of block Lab, we don't need it anymore.
    delete  blockLAB;

    // Before displaying the window, gather the end time and delta to output the time it took to process all ULIS3 operations.
    // We are not interested in the time it took Qt to create the window.
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();

    // Let's recap:
    // We performed two allocations
    // We performed a manual gradient
    // We performed 1 block conversion from LabD to RGBA8
    // That makes, worst case:
    //      2 Alloc     ( 800*600 )
    //      1 Gradient  ( 800*600 )
    //      1 Conv      ( 1280*800 )
    // Average on my desktop setup: 53ms            | Conversion only ( without manual unoptimized gradient ) : 13ms
    // Average on my laptop setup:  <unavailable>   | Conversion only ( without manual unoptimized gradient ) : <unavailable>
    // Remember: everything is multithreaded, SSE and AVX are used whenever possible, everything is computed on CPU
    // Print out the result time.
    std::cout << "ULIS3 Conversion: Conversion took " << delta << "ms." << std::endl;

    // Create a Qt application and a simple window to display the result block we computed.
    // We create a QImage from the blockCanvas data, QImage does not own the data, so it still lives in blockCanvas, so we don't delete it right now.
    // For Qt Interoperability, several formats are compatible with ULIS3 formats. Here we chose RGBA8888 which has the same memory layout as ULIS_FORMAT_RGBA8
    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockRGB->Bits()
                                        , blockRGB->Width()
                                        , blockRGB->Height()
                                        , blockRGB->BytesPerScanLine()
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
    delete  blockRGB;
    XDeleteThreadPool( threadPool );

    // Return exit code.
    return  exit_code;
}

