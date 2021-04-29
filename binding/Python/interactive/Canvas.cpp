// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Canvas.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
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
    ctx.Clear( mCanvas );
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
    ctx.Clear( mCanvas );
    ctx.Finish();
}

