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
#include <QPushButton>
#include <QIcon>
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
    , mMiniBlockAA( 12, 10, mFmt )
    , mMiniBlockSP( 12, 10, mFmt )
    , mCurrentRasterOp( kLine )
    , mCurrentRasterMode( kNone )
    , mImage( nullptr )
    , mPixmap( nullptr )
    , mLabel( nullptr )
    , mTimer( nullptr )
{
    {
        mCtx.Clear( mTemp, mTemp.Rect(), mPolicyCacheEfficient );
        mCtx.Clear( mCanvas, mCanvas.Rect(), mPolicyCacheEfficient );
        mCtx.Clear( mMiniBlockAA, mMiniBlockAA.Rect(), mPolicyCacheEfficient );
        mCtx.Clear( mMiniBlockSP, mMiniBlockSP.Rect(), mPolicyCacheEfficient );
    }
    mCtx.Flush();
    FFontEngine fontEngine;
    FFont font( fontEngine, "Segoe UI", "Black" );
    FEvent eventText;
    {
        mCtx.Fence();
        FColor blue = FColor::RGB( 0, 127, 255 );
        int size = mMiniBlockAA.Height() - 2;
        mCtx.RasterTextAA( mMiniBlockAA, L"AA", font, size, FMat3F::MakeTranslationMatrix( 0, size ), blue, mPolicyCacheEfficient, 0, nullptr, &eventText );
        mCtx.RasterTextAA( mMiniBlockSP, L"SP", font, size, FMat3F::MakeTranslationMatrix( 0, size ), blue, mPolicyCacheEfficient, 1, &eventText, nullptr );
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

    buttons.reserve( kNumRasterOP * kNumRasterMode );
    for( int i = 0; i < kNumRasterOP; ++i ) {
        for( int j = 0; j < kNumRasterMode; ++j ) {
            buttons.push_back( new QPushButton( this ) );
            BuildButton( buttons.back(), eRasterOp(i), eRasterMode(j) );
        }
    }
    mCtx.Finish();
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
    QObject::connect( ioButton, &QPushButton::clicked, [=](){ this->SetRaster( iRasterOp, iRasterMode ); } );
    FVec2I padding = 5;
    FVec2I size = 32;
    FVec2I center = size / 2;
    int radius = size.x / 2 - padding.x;
    FBlock* block = new FBlock( size.x, size.y, Format_RGBA8 );
    FColor black = FColor::RGB( 0, 0, 0 );
    FSchedulePolicy policyMonoChunk(
          ScheduleTime_Sync
        , ScheduleRun_Mono
        , ScheduleMode_Chunks
        , ScheduleParameter_Count
        , 1
    );
    FEvent eventClear;
    FEvent eventRaster;
    FEvent eventText(
        {
            [ block, size, padding, ioButton, iRasterMode, iRasterOp ]( const FRectI& ) {
                QImage image( block->Bits(), size.x, size.y, block->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
                QPixmap pixmap = QPixmap::fromImage( image );
                QIcon icon( pixmap );
                ioButton->setIcon( icon );
                ioButton->setFixedSize( size.x, size.y );
                ioButton->setIconSize( QSize( size.x, size.y ) );
                ioButton->move(
                      padding.x + int( iRasterMode ) * ( size.x + padding.x )
                    , padding.y + int( iRasterOp ) * ( size.y + padding.y )
                );
                delete  block;
            }
        }
    );
    mCtx.Clear( *block, block->Rect(), policyMonoChunk, 0, nullptr, &eventClear );

    if( iRasterMode == kNone ) {
        mCtx.Dummy_OP( 1, &eventRaster, &eventText );
    } else if( iRasterMode == kAA ) {
        mCtx.Blend( mMiniBlockAA, *block, mMiniBlockAA.Rect(), FVec2I( size.x - mMiniBlockAA.Width(), 1 ), Blend_Normal, Alpha_Normal, 1.f, policyMonoChunk, 1, &eventRaster, &eventText );
    } else if( iRasterMode == kSP ) {
        mCtx.Blend( mMiniBlockSP, *block, mMiniBlockSP.Rect(), FVec2I( size.x - mMiniBlockSP.Width(), 1 ), Blend_Normal, Alpha_Normal, 1.f, policyMonoChunk, 1, &eventRaster, &eventText );
    }

    switch( iRasterOp ) {
        case kLine: {
            if( iRasterMode == kNone ) {
                mCtx.DrawLine( *block, padding, size - padding, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                mCtx.DrawLineAA( *block, padding, size - padding, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                mCtx.DrawLineSP( *block, padding, size - padding, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kCircleAndres: {
            if( iRasterMode == kNone ) {
                mCtx.DrawCircleAndres( *block, center, radius, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                mCtx.DrawCircleAndresAA( *block, center, radius, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                mCtx.DrawCircleAndresSP( *block, center, radius, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kCircleBresenham: {
            if( iRasterMode == kNone ) {
                mCtx.DrawCircleBresenham( *block, center, radius, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                mCtx.DrawCircleBresenhamAA( *block, center, radius, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                mCtx.DrawCircleBresenhamSP( *block, center, radius, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kArcAndres: {
            if( iRasterMode == kNone ) {
                mCtx.DrawArcAndres( *block, center, radius, 315, 90, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                mCtx.DrawArcAndresAA( *block, center, radius, 315, 90, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                mCtx.DrawArcAndresSP( *block, center, radius, 315, 90, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kArcBresenham: {
            if( iRasterMode == kNone ) {
                mCtx.DrawArcBresenham( *block, center, radius, 315, 90, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                mCtx.DrawArcBresenhamAA( *block, center, radius, 315, 90, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                mCtx.DrawArcBresenhamSP( *block, center, radius, 315, 90, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kEllipse: {
            if( iRasterMode == kNone ) {
                mCtx.DrawEllipse( *block, center, radius, radius / 2, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                mCtx.DrawEllipseAA( *block, center, radius, radius / 2, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                mCtx.DrawEllipseSP( *block, center, radius, radius / 2, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kRotatedEllipse: {
            if( iRasterMode == kNone ) {
                mCtx.DrawRotatedEllipse( *block, center, radius, radius / 2, 45, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                mCtx.DrawRotatedEllipseAA( *block, center, radius, radius / 2, 45, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                mCtx.DrawRotatedEllipseSP( *block, center, radius, radius / 2, 45, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kRectangle: {
            if( iRasterMode == kNone ) {
                mCtx.DrawRectangle( *block, padding, size - padding, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                // Rectangle AA ?
                mCtx.DrawRectangle( *block, padding, size - padding, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                // Rectangle SP ?
                mCtx.DrawRectangle( *block, padding, size - padding, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kPolygon: {
            if( iRasterMode == kNone ) {
                std::vector< FVec2I > vec {
                      FVec2I( padding.x, padding.y )
                    , FVec2I( size.x - padding.x, padding.y * 2 )
                    , FVec2I( size.x - padding.x, size.y - padding.y )
                    , FVec2I( size.x - padding.x * 6, size.y - padding.y )
                    , FVec2I( center )
                };
                mCtx.DrawPolygon( *block, vec, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                std::vector< FVec2I > vec {
                      FVec2I( padding.x, padding.y )
                    , FVec2I( size.x - padding.x, padding.y * 2 )
                    , FVec2I( size.x - padding.x, size.y - padding.y )
                    , FVec2I( size.x - padding.x * 6, size.y - padding.y )
                    , FVec2I( center )
                };
                mCtx.DrawPolygonAA( *block, vec, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                std::vector< FVec2F > vec {
                      FVec2F( padding.x, padding.y )
                    , FVec2F( size.x - padding.x, padding.y * 2 )
                    , FVec2F( size.x - padding.x, size.y - padding.y )
                    , FVec2F( size.x - padding.x * 6, size.y - padding.y )
                    , FVec2F( center )
                };
                mCtx.DrawPolygonSP( *block, vec, black, false, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kQuadraticBezier: {
            FVec2I p0( padding.x, size.y - padding.y );
            FVec2I p1( center.x, padding.y );
            FVec2I p2( size.x - padding.x, size.y - padding.y );
            if( iRasterMode == kNone ) {
                mCtx.DrawQuadraticBezier( *block, p0, p1, p2, 1.f, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                mCtx.DrawQuadraticBezierAA( *block, p0, p1, p2, 1.f, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                mCtx.DrawQuadraticBezierSP( *block, p0, p1, p2, 1.f, black, block->Rect(), policyMonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
    }
}

void
SMainWindow::SetRaster( eRasterOp iRasterOp, eRasterMode iRasterMode ) {
    mCurrentRasterOp = iRasterOp;
    mCurrentRasterMode = iRasterMode;
}

