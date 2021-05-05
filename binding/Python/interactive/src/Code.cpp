// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Code.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
/*
#include "Canvas.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

SCanvas::~SCanvas() {
    delete  mTimer;
    delete  mImage;
    delete  mPixmap;
    delete  mLabel;
}

SCanvas::SCanvas( FULISLoader& iHandle )
    : QWidget( nullptr )
    , mHandle( iHandle )
    , mCanvas( FBlock( 800, 600, mHandle.Format() ) )
    , mImage( nullptr )
    , mPixmap( nullptr )
    , mLabel( nullptr )
    , mTimer( nullptr )
{
    setFixedSize( 800, 600 );

    FContext& ctx = mHandle.Context();
    ctx.Fill( mCanvas, FColor::White );
    ctx.Finish();

    mImage  = new QImage( mCanvas.Bits(), mCanvas.Width(), mCanvas.Height(), mCanvas.BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    mPixmap = new QPixmap( QPixmap::fromImage( *mImage ) );
    mLabel  = new QLabel( this );
    mLabel->setPixmap( *mPixmap );
    this->QWidget::setFixedSize( mPixmap->size() );

    mTimer = new QTimer();
    mTimer->setInterval( 1000.0 / 24.0 );
    QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( tickEvent() ) );
    mTimer->start();

    py::module_ pyULIS4 = py::module_::import("pyULIS4");
    py::exec( R"(
        from pyULIS4 import *
        canvas = FBlock( 800, 600, Format_RGBA8 )
    )" );

    py::module_ main = py::module_::import("__main__");
    main.add_object( "canvas2", py::cast( &mCanvas ), true );

    // Sample backward / forward
    // py::object obj = py::cast( &mCanvas );
    //FBlock* canvas = pyCanvas.cast< FBlock* >();
    //std::cout << canvas->Format();
    //std::cout << canvas->Rect().w;
    //auto dummy = 0;
}

void
SCanvas::mouseMoveEvent( QMouseEvent* event ) {
}

void
SCanvas::keyPressEvent( QKeyEvent* event ) {
}

void
SCanvas::tickEvent() {
    FContext& ctx = mHandle.Context();
    ctx.Fill( mCanvas, FColor::White );
    ctx.Finish();
    //py::object pyCanvas = pyULIS4.attr( "FBlock" )( 800, 600, Format_RGBA8 );

    auto message = "Hello world from python embed !"_s;

    try {
        py::exec(R"(
            print( "block from py", canvas2.Width() )
        )");
    } catch( const std::exception& ) {
        std::cout << "error" << std::endl;
    }

    py::module_ main = py::module_::import("__main__");
    py::object canvas_attr = main.attr( "canvas" );
    FBlock* canvas = canvas_attr.cast< FBlock* >();
    std::cout << "block from cpp" << canvas->Width() << std::endl;
    auto dummy = 0;
}

*/