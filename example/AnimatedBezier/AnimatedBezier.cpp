// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedBezier.cpp
* @author       Clement Berthaud
* @brief        AnimatedBezier application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "AnimatedBezier.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <cstdlib>

SWindow::~SWindow() {
    mCtx.Finish();
    delete  mImage;
    delete  mPixmap;
    delete  mLabel;
    delete  mTimer;
}

SWindow::SWindow()
    : mPool()
    , mQueue( mPool )
    , mFmt( Format_RGBA8 )
    , mCtx( mQueue, mFmt, PerformanceIntent_Max )
    , mHw()
    , mPolicyCacheEfficient( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, mHw.L1CacheSize() )
    , mPolicyMultiScanlines( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Scanlines )
    , mSrc()
    , mDst( 1000, 1000, mFmt )
    , mCtrlPts( 4 )
    , mImage( nullptr )
    , mAngle( 0.f )
    , mPixmap( nullptr )
    , mLabel( nullptr )
    , mTimer( nullptr )
{
    {
        std::string path = "C:/Users/PRAXINOS/Documents/work/TEST.png";
        mCtx.XLoadBlockFromDisk( mSrc, path );
        mCtx.Clear( mDst, mDst.Rect(), mPolicyCacheEfficient );
    }
    mCtx.Finish();

    mImage = new QImage( mDst.Bits(), mDst.Width(), mDst.Height(), mDst.BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    mPixmap = new QPixmap( QPixmap::fromImage( *mImage ) );
    mLabel = new QLabel( this );
    mLabel->setPixmap( *mPixmap );
    this->QWidget::setFixedSize( mPixmap->size() );
    mTimer = new QTimer();
    mTimer->setInterval( 1000.0 / 24.0 );
    QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( tickEvent() ) );
    mTimer->start();
    mCtrlPts[0] = { FVec2F( 300, 300 ), FVec2F(), FVec2F() };
    mCtrlPts[1] = { FVec2F( 700, 300 ), FVec2F(), FVec2F() };
    mCtrlPts[2] = { FVec2F( 700, 700 ), FVec2F(), FVec2F() };
    mCtrlPts[3] = { FVec2F( 300, 700 ), FVec2F(), FVec2F() };
}

void
SWindow::tickEvent() {
    float len = 300;
    mAngle += 0.08f;
    float evoAngle0 = mAngle;
    float evoAngle1 = evoAngle0 + FMath::kPIf / 2;
    float evoAngle2 = evoAngle1 + FMath::kPIf / 2;
    float evoAngle3 = evoAngle2 + FMath::kPIf / 2;
    mCtrlPts[0].ctrlCW  = mCtrlPts[0].point + FVec2F( cos( evoAngle0 ), sin( evoAngle0 ) ) * len;
    mCtrlPts[0].ctrlCCW = mCtrlPts[0].point + FVec2F( cos( evoAngle1 ), sin( evoAngle1 ) ) * len;
    mCtrlPts[1].ctrlCW  = mCtrlPts[1].point + FVec2F( cos( evoAngle1 ), sin( evoAngle1 ) ) * len;
    mCtrlPts[1].ctrlCCW = mCtrlPts[1].point + FVec2F( cos( evoAngle2 ), sin( evoAngle2 ) ) * len;
    mCtrlPts[2].ctrlCW  = mCtrlPts[2].point + FVec2F( cos( evoAngle2 ), sin( evoAngle2 ) ) * len;
    mCtrlPts[2].ctrlCCW = mCtrlPts[2].point + FVec2F( cos( evoAngle3 ), sin( evoAngle3 ) ) * len;
    mCtrlPts[3].ctrlCW  = mCtrlPts[3].point + FVec2F( cos( evoAngle3 ), sin( evoAngle3 ) ) * len;
    mCtrlPts[3].ctrlCCW = mCtrlPts[3].point + FVec2F( cos( evoAngle0 ), sin( evoAngle0 ) ) * len;

    mCtx.Fence();
    mPixmap->convertFromImage( *mImage );
    mLabel->setPixmap( *mPixmap );

    // Gross estimate of bezier with large thresold and plot size for interactivity in debug.
    // Notice this new pattern: scheduling and flushing at the end of the tick, to let the workers
    // work when the GUI does its own stuff, and simply fencing before the render.
    // Small gain, but still noticeable.
    FEvent eventClear;
    mCtx.Fill( mDst, mDst.Rect(), FColor::RGB( 30, 255, 150 ), mPolicyCacheEfficient, 0, nullptr, &eventClear );
    mCtx.TransformBezier( mSrc, mDst, mCtrlPts, 4.f, 8, mSrc.Rect(), Resampling_NearestNeighbour, Border_Transparent, FColor::Transparent, mPolicyMultiScanlines, 1, &eventClear, nullptr );
    mCtx.Flush();
}

