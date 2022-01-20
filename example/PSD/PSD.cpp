// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PSD.cpp
* @author       Clement Berthaud
* @brief        PSD application for ULIS.
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


    std::string path = "C:/Users/conta/Documents/work/psdwork.psd";
    bool fileExist = false;

    eFormat fmt;
    FContext::LoadPSDFromDiskMetrics( path, &fileExist, &fmt );

    FContext ctx( queue, fmt, PerformanceIntent_AVX );
    FLayerStack layerStack( 1, 1, fmt );

    auto startTime = std::chrono::steady_clock::now();

    ctx.XLoadPSDFromDisk( layerStack, path );
    ctx.Finish(); 

    FBlock blockCanvas( layerStack.Width(), layerStack.Height(), layerStack.Format() );
    //ctx.Flatten( layerStack, blockCanvas );
    ctx.Finish();

    FBlock blockCanvasConverted( layerStack.Width(), layerStack.Height(), Format_RGBA8 );
    ctx.ConvertFormat( blockCanvas, blockCanvasConverted );
    ctx.Finish();
    
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();

    std::cout << "ULIS XLoadPSDFromDisk took: " << delta << "ms." << std::endl;

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockCanvasConverted.Bits()
                                        , blockCanvasConverted.Width()
                                        , blockCanvasConverted.Height()
                                        , blockCanvasConverted.BytesPerScanLine()
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

    // Return exit code.
    return  exit_code;
}

