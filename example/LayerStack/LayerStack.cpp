// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.cpp
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
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
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_Max );

    uint16 w = 1024;
    uint16 h = 1024;
    FBlock canvas( w, h, fmt );


    FLayerStack* stack = new FLayerStack( w, h, fmt, nullptr );
    stack->AddChild( new FLayerImage( "image0" ) );

    {
        ctx.Clear( canvas, canvas.Rect() );
        ctx.Finish();
    }

    auto startTime = std::chrono::steady_clock::now();
    {
        ctx.Finish();
    }
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();
    std::cout << "ULIS Layer Stack Flatten took: " << delta << "ms." << std::endl;
    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( canvas.Bits()
                                        , canvas.Width()
                                        , canvas.Height()
                                        , canvas.BytesPerScanLine()
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
    delete  stack;
    return  exit_code;
}

