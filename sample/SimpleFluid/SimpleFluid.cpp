// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/**
*
*   ULIS
*__________________
*
* @file         SimpleFluid.cpp
* @author       Clement Berthaud
* @brief        SimpleFluid application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "SimpleFluid.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

#include <cstdlib>

SWindow::~SWindow() {
    delete  mWetCanvas;
    delete  mDryCanvas;
    delete  mParticle;
    delete  mImage;
    delete  mPixmap;
    delete  mLabel;
    delete  mTimer;
    delete  mBGParticleColor;
    delete  mDarkParticleColor;
    delete  mLightParticleColor;
    XDeleteThreadPool( mPool );
}


SWindow::SWindow()
    : mHost( FHostDeviceInfo::Detect() )
    , mPool( XCreateThreadPool() )
    , mWetCanvas( nullptr )
    , mDryCanvas( nullptr )
    , mParticle( nullptr )
    , mPos( 0, 0 )
    , mImage( nullptr )
    , mPixmap( nullptr )
    , mLabel( nullptr )
    , mTimer( nullptr )
    , mLeftButtonDown( false )
    , mParticleSize( 5 )
    , mCurrentParticleColor( nullptr )
    , mBGParticleColor( nullptr )
    , mDarkParticleColor( nullptr )
    , mLightParticleColor( nullptr )
{
    mWetCanvas = new FBlock( 1280, 900, ULIS_FORMAT_RGBA8 );
    mDryCanvas = new FBlock( 1280, 900, ULIS_FORMAT_RGBA8 );
    mParticles.reserve( 10000 );
    mImage = new QImage( mWetCanvas->Bits(), mWetCanvas->Width(), mWetCanvas->Height(), mWetCanvas->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    mPixmap = new QPixmap( QPixmap::fromImage( *mImage ) );
    mLabel = new QLabel( this );
    mLabel->setPixmap( *mPixmap );
    this->QWidget::setFixedSize( mPixmap->size() );
    mTimer = new QTimer();
    mTimer->setInterval( 1000.0 / 60.0 );
    QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( tickEvent() ) );
    mTimer->start();

    mBGParticleColor        = new FColor( ULIS_FORMAT_RGBA8, { 240, 240, 240, 255 } );
    mDarkParticleColor      = new FColor( ULIS_FORMAT_RGBA8, { 170, 40, 0, 255 } );
    mLightParticleColor     = new FColor( ULIS_FORMAT_RGBA8, { 255, 130, 80, 255 } );
    mCurrentParticleColor   = mBGParticleColor;
    mCurrentBlendingMode = BM_MULTIPLY;
    mCurrentOpacity = 0.05f;
    mMul = 1.f;
    RedrawParticle();
}


void
SWindow::mousePressEvent( QMouseEvent* event ) {
    if( event->button() == Qt::LeftButton ) {
        mLeftButtonDown = true;
        mPos = event->pos();
    }
}

void
SWindow::mouseMoveEvent( QMouseEvent* event ) {
    mPos = event->pos();
}

void
SWindow::mouseReleaseEvent( QMouseEvent* event ) {
    if( event->button() == Qt::LeftButton )
        mLeftButtonDown = false;
}

void
SWindow::keyPressEvent( QKeyEvent* event ) {
    if( event->key() == Qt::Key_Backspace ) {
        Clear( mPool, ULIS_BLOCKING, ULIS_PERF_MT | ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, mWetCanvas, mWetCanvas->Rect() );
        Clear( mPool, ULIS_BLOCKING, ULIS_PERF_MT | ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, mDryCanvas, mDryCanvas->Rect() );
        mParticles.clear();
    }

    if( event->key() == Qt::Key_1 ) {
        mCurrentParticleColor = mDarkParticleColor;
        mCurrentBlendingMode = BM_MULTIPLY;
        RedrawParticle();
    }

    if( event->key() == Qt::Key_2 ) {
        mCurrentParticleColor = mLightParticleColor;
        mCurrentBlendingMode = BM_NORMAL;
        RedrawParticle();
    }

    if( event->key() == Qt::Key_3 ) {
        mCurrentParticleColor = mBGParticleColor;
        mCurrentBlendingMode = BM_NORMAL;
        RedrawParticle();
    }

    if( event->key() == Qt::Key_Plus ) {
        mParticleSize = FMath::Min( 32, mParticleSize + 1 );
        RedrawParticle();
    }

    if( event->key() == Qt::Key_Minus ) {
        mParticleSize = FMath::Max( 1, mParticleSize - 1 );
        RedrawParticle();
    }

    if( event->key() == Qt::Key_P ) {
        mMul = mMul + 0.1f;
    }

    if( event->key() == Qt::Key_M ) {
        mMul = FMath::Max( 0.3f, mMul - 0.1f );
    }
}

void
SWindow::tickEvent() {
    std::cout << mParticles.size() << std::endl;
    if( mLeftButtonDown ) {
        for( int i = 0; i < 400 * mMul; ++i ) {
            float dir = ( rand() % 3600 ) / 1800.f * 3.14159265359;
            float vel = ( sqrtf( ( rand() % 1000 ) / 1000.f ) ) * 2 * mMul + 1;
            mParticles.push_back( { FVec2F( mPos.x(), mPos.y() ), FVec2F( cos( dir ) * vel, sin( dir ) * vel ) } );
        }
    }

    //Clear( mPool, ULIS_BLOCKING, ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, mWetCanvas, mWetCanvas->Rect() );
    Copy( mPool, ULIS_BLOCKING, ULIS_PERF_MT | ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, mDryCanvas, mWetCanvas, mDryCanvas->Rect(), FVec2I() );

    FRectI sourceRect = mParticle->Rect();
    for( size_t i = 0; i < mParticles.size(); ++i ) {
        mParticles[i].p.x += mParticles[i].v.x = mParticles[i].v.x * 0.9f;
        mParticles[i].p.y += mParticles[i].v.y = mParticles[i].v.y * 0.9f;
        Blend( mPool, ULIS_BLOCKING, ULIS_PERF_SSE42, mHost, ULIS_NOCB, mParticle, mWetCanvas, sourceRect, mParticles[i].p, ULIS_NOAA, mCurrentBlendingMode, AM_NORMAL, mCurrentOpacity );

        if( abs( mParticles[i].v.x ) + abs( mParticles[i].v.y ) <= 1 ) {
            Blend( mPool, ULIS_BLOCKING, ULIS_PERF_SSE42, mHost, ULIS_NOCB, mParticle, mDryCanvas, sourceRect, mParticles[i].p, ULIS_NOAA, mCurrentBlendingMode, AM_NORMAL, mCurrentOpacity );
            mParticles.erase( mParticles.begin() + i );
            --i;
        }
    }

    mPixmap->convertFromImage( *mImage );
    mLabel->setPixmap( *mPixmap );
}


void
SWindow::RedrawParticle() {
    if( mParticle )
        delete  mParticle;

    mParticle = new FBlock( mParticleSize, mParticleSize, ULIS_FORMAT_RGBA8 );
    ClearRaw( mParticle, ULIS_NOCB );
    Fill( mPool, ULIS_BLOCKING, ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, mParticle, *mCurrentParticleColor, mParticle->Rect() );

    if( mParticleSize > 1 ) {
        float midx = mParticle->Width() / 2.f;
        float midy = mParticle->Height() / 2.f;
        float ray2 = midx * midx;
        for( uint32 i = 0; i < mParticle->Width(); ++i ) {
            for( uint32 j = 0; j < mParticle->Height(); ++j ) {
                float dx = midx - i;
                float dy = midy - j;
                float dist2 = dx * dx + dy * dy;
                FPixel prox = mParticle->Pixel( i, j );
                float alpha = 1.f - FMath::Min( dist2 / ray2, 1.f );
                prox.SetAlphaF( alpha );
            }
        }
    }

}
