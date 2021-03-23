// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MainWindow.cpp
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "MainWindow.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <cstdlib>

SMainWindow::~SMainWindow() {
    mCtx.Finish();
    delete  mImage;
    delete  mPixmap;
    delete  mLabel;
    delete  mTimer;
}

SMainWindow::SMainWindow()
    : mPool()
    , mQueue( mPool )
    , mFmt( Format_RGBA8 )
    , mCtx( mQueue, mFmt, PerformanceIntent_Max )
    , mHw()
    , mPolicyCacheEfficient( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, mHw.L1CacheSize() )
    , mPolicyMultiScanlines( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Scanlines )
    , mTemp()
    , mCanvas( 1280, 800, mFmt )
    , mImage( nullptr )
    , mPixmap( nullptr )
    , mLabel( nullptr )
    , mTimer( nullptr )
{
    {
        mCtx.Clear( mTemp, mTemp.Rect(), mPolicyCacheEfficient );
        mCtx.Clear( mCanvas, mCanvas.Rect(), mPolicyCacheEfficient );
    }
    mCtx.Finish();

    mImage = new QImage( mCanvas.Bits(), mCanvas.Width(), mCanvas.Height(), mCanvas.BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    mPixmap = new QPixmap( QPixmap::fromImage( *mImage ) );
    mLabel = new QLabel( this );
    mLabel->setPixmap( *mPixmap );
    this->QWidget::setFixedSize( mPixmap->size() );

    mTimer = new QTimer();
    mTimer->setInterval( 1000.0 / 24.0 );
    QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( tickEvent() ) );
    mTimer->start();

    buttons.resize( kNumRasterOP * kNumRasterMode );
    for( int i = 0; i < kNumRasterOP; ++i ) {
        for( int j = 0; j < kNumRasterMode; ++j ) {
            BuildButton( buttons[i], eRasterOp(i), eRasterMode(j) );
        }
    }
}

void
SMainWindow::mousePressEvent( QMouseEvent* event ) {
}

void
SMainWindow::mouseMoveEvent( QMouseEvent* event ) {
}

void
SMainWindow::mouseReleaseEvent( QMouseEvent* event ) {
}

void
SMainWindow::keyPressEvent( QKeyEvent* event ) {
}

void
SMainWindow::tickEvent() {
    mPixmap->convertFromImage( *mImage );
    mLabel->setPixmap( *mPixmap );
}

void
SMainWindow::BuildButton( QPushButton* ioButton, eRasterOp iRasterOp, eRasterMode iRasterMode ) {

}

