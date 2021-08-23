// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Canvas.cpp
* @author       Clement Berthaud
* @brief        TiledBlock application for ULIS2.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
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

int Mo2O( int iO ) {
    return  iO * 1000 * 1000;
}

int O2Mo( int iMo ) {
    return  iMo / 1000 / 1000;
}

SCanvas::~SCanvas() {
    delete  mTimer;
    delete  mTilePool;
    delete  mImage;
    delete  mPixmap;
    delete  mLabel;
    delete  mCanvas;
}


SCanvas::SCanvas()
    : mHost( FHostDeviceInfo::Detect() )
    , mPool()
    , mCanvas(      nullptr )
    , mFontEngine()
    , mFontReg( mFontEngine )
    , mFont( mFontReg, "Courrier New", "Bold" )
    , mTilePool(    nullptr )
    , mImage(       nullptr )
    , mPixmap(      nullptr )
    , mLabel(       nullptr )
    , mTimer(       nullptr )
{
    uint32 perfIntent = ULIS2_PERF_MT | ULIS2_PERF_TSPEC | ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2;

    mCanvas = new  FBlock( 320, 600, ULIS2_FORMAT_RGBA8 );

    Clear( &mPool, ULIS2_BLOCKING, perfIntent, mHost, ULIS2_NOCB, mCanvas, mCanvas->Rect() );

    mImage  = new QImage( mCanvas->DataPtr(), mCanvas->Width(), mCanvas->Height(), mCanvas->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    mPixmap = new QPixmap( QPixmap::fromImage( *mImage ) );
    mLabel  = new QLabel( this );
    mLabel->setPixmap( *mPixmap );
    this->QWidget::setFixedSize( mPixmap->size() );

    mTimer = new QTimer();
    mTimer->setInterval( 1000.0 / 24.0 );
    QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( tickEvent() ) );
    mTimer->start();

    uint64 maxRAM   = Mo2O( 100 );
    int timeoutms   = 10;
    mTilePool = new TTilePool< MICRO_16, MACRO_16 >( ULIS2_FORMAT_RGBA8, nullptr );
    mTiledBlock = mTilePool->CreateNewTiledBlock();

    mRAMUSAGEBLOCK1 = new FBlock( 300, 100, ULIS2_FORMAT_RGBA8 );
    mRAMUSAGEBLOCK2 = new FBlock( 300, 100, ULIS2_FORMAT_RGBA8 );
    mRAMUSAGESWAPBUFFER = mRAMUSAGEBLOCK1;
    Fill( &mPool, ULIS2_BLOCKING, ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2, mHost, ULIS2_NOCB, mRAMUSAGEBLOCK1, FPixelValue( ULIS2_FORMAT_G8, { 15 } ), mRAMUSAGEBLOCK1->Rect() );
}


void
SCanvas::mousePressEvent( QMouseEvent* event ) {
}

void
SCanvas::mouseMoveEvent( QMouseEvent* event ) {
    if ( event->buttons() & Qt::LeftButton ) {
        int HH = ( mRAMUSAGESWAPBUFFER->Height() - 1 );
        int WW = ( mRAMUSAGESWAPBUFFER->Width() - 1 );
        float scale = 1.f;
        FVec2I64 ref( 10, 80+HH+10 );
        FVec2I64 pos( event->x(), event->y() );
        int size = 8;
        for( int i = -size; i < size; ++i ) {
            for( int j = -size; j < size; ++j ) {
                FVec2I64 res( ( pos.x - ref.x + i ) / scale, ( pos.y - ref.y + j ) / scale );
                if( res.x >= 0 && res.x < 256 && res.y >= 0 && res.y < 256 ) {
                    FTileElement** tileptr = mTiledBlock->QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( res, &res );
                    DrawDotNoAA( (*tileptr)->mBlock, FPixelValue( ULIS2_FORMAT_G8, {0} ), FVec2I( res.x, res.y ) );
                }
            }
        }
    }

    if( event->buttons() & Qt::RightButton ) {
        int HH = ( mRAMUSAGESWAPBUFFER->Height() - 1 );
        int WW = ( mRAMUSAGESWAPBUFFER->Width() - 1 );
        float scale = 1.f;
        FVec2I64 ref( 10, 80+HH+10 );
        FVec2I64 pos( event->x(), event->y() );
        int size = 16;
        for( int i = -size; i < size; ++i ) {
            for( int j = -size; j < size; ++j ) {
                FVec2I64 res( ( pos.x - ref.x + i ) / scale, ( pos.y - ref.y + j ) / scale );
                if( res.x >= 0 && res.x < 256 && res.y >= 0 && res.y < 256 ) {
                    FTileElement** tileptr = mTiledBlock->QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( res, &res );
                    DrawDotNoAA( (*tileptr)->mBlock, FPixelValue( ULIS2_FORMAT_GA8, {0,0} ), FVec2I( res.x, res.y ) );
                }
            }
        }
    }
}

void
SCanvas::mouseReleaseEvent( QMouseEvent* event ) {
}

void
SCanvas::keyPressEvent( QKeyEvent* event ) {
    if( event->key() == Qt::Key::Key_0 )
        mTilePool->SetRAMUsageCapTarget( 0 );

    if( event->key() == Qt::Key::Key_1 )
        mTilePool->SetRAMUsageCapTarget( Mo2O( 2 ) );

    if( event->key() == Qt::Key::Key_2 )
        mTilePool->SetRAMUsageCapTarget( Mo2O( 200 ) );

    if( event->key() == Qt::Key::Key_3 )
        mTilePool->SetRAMUsageCapTarget( Mo2O( 400 ) );

    if( event->key() == Qt::Key::Key_4 )
        mTilePool->SetRAMUsageCapTarget( Mo2O( 1000 ) );

    if( event->key() == Qt::Key::Key_5 )
        mTilePool->SetRAMUsageCapTarget( Mo2O( 2000 ) );

    if( event->key() == Qt::Key::Key_A ) {
        mTiledBlock->Clear();
    }

    if( event->key() == Qt::Key::Key_B ) {
        mTiledBlock->SanitizeNow();
    }

}

void
SCanvas::tickEvent() {
    int HH = ( mRAMUSAGESWAPBUFFER->Height() - 1 );
    int WW = ( mRAMUSAGESWAPBUFFER->Width() - 1 );
    auto cramu = mTilePool->CurrentRAMUsage();
    float maxramu = Mo2O( 2000 );
    float tramu = cramu / maxramu;
    int iramu = FMaths::Min( HH, int( tramu * HH ) );
    for( int i = 0; i < iramu; ++i ) {
        FPixelProxy prox = mRAMUSAGESWAPBUFFER->PixelProxy( WW, HH - i );
        prox.SetR8( 20 );
        prox.SetG8( 80 );
        prox.SetB8( 200 );
        prox.SetA8( 255 );
    }

    Fill( &mPool, ULIS2_BLOCKING, ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2, mHost, ULIS2_NOCB, mCanvas, FPixelValue( ULIS2_FORMAT_G8, { 40 } ), mCanvas->Rect() );
    RenderText( &mPool, ULIS2_BLOCKING, 0, mHost, ULIS2_NOCB, mCanvas, L"Target  RAM                : " + std::to_wstring( mTilePool->RAMUsageCapTarget() ),                        mFont, 12, FPixelValue( ULIS2_FORMAT_G8, { 220 } ), FTransform2D( MakeTranslationMatrix( 10, 10 ) ), ULIS2_NOAA );
    RenderText( &mPool, ULIS2_BLOCKING, 0, mHost, ULIS2_NOCB, mCanvas, L"Current RAM                : " + std::to_wstring( mTilePool->CurrentRAMUsage() ),                          mFont, 12, FPixelValue( ULIS2_FORMAT_G8, { 220 } ), FTransform2D( MakeTranslationMatrix( 10, 20 ) ), ULIS2_NOAA );
    RenderText( &mPool, ULIS2_BLOCKING, 0, mHost, ULIS2_NOCB, mCanvas, L"Num Scheduled For Clear    : " + std::to_wstring( mTilePool->NumTilesScheduledForClear() ),                mFont, 12, FPixelValue( ULIS2_FORMAT_G8, { 220 } ), FTransform2D( MakeTranslationMatrix( 10, 30 ) ), ULIS2_NOAA );
    RenderText( &mPool, ULIS2_BLOCKING, 0, mHost, ULIS2_NOCB, mCanvas, L"Num Available For Query    : " + std::to_wstring( mTilePool->NumFreshTilesAvailableForQuery() ),           mFont, 12, FPixelValue( ULIS2_FORMAT_G8, { 220 } ), FTransform2D( MakeTranslationMatrix( 10, 40 ) ), ULIS2_NOAA );
    RenderText( &mPool, ULIS2_BLOCKING, 0, mHost, ULIS2_NOCB, mCanvas, L"Num Dirty In Use           : " + std::to_wstring( mTilePool->NumDirtyHashedTilesCurrentlyInUse() ),        mFont, 12, FPixelValue( ULIS2_FORMAT_G8, { 220 } ), FTransform2D( MakeTranslationMatrix( 10, 50 ) ), ULIS2_NOAA );
    RenderText( &mPool, ULIS2_BLOCKING, 0, mHost, ULIS2_NOCB, mCanvas, L"Num Correct In Use         : " + std::to_wstring( mTilePool->NumCorrectlyHashedTilesCurrentlyInUse() ),    mFont, 12, FPixelValue( ULIS2_FORMAT_G8, { 220 } ), FTransform2D( MakeTranslationMatrix( 10, 60 ) ), ULIS2_NOAA );
    RenderText( &mPool, ULIS2_BLOCKING, 0, mHost, ULIS2_NOCB, mCanvas, L"Num Registered Blocks      : " + std::to_wstring( mTilePool->NumRegisteredTiledBlocks() ),                 mFont, 12, FPixelValue( ULIS2_FORMAT_G8, { 220 } ), FTransform2D( MakeTranslationMatrix( 10, 70 ) ), ULIS2_NOAA );
    Copy( &mPool, ULIS2_BLOCKING, ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2, mHost, ULIS2_NOCB, mRAMUSAGESWAPBUFFER, mCanvas, mRAMUSAGESWAPBUFFER->Rect(), FVec2I( 10, 80 ) );

    FBlock* oldram = mRAMUSAGESWAPBUFFER;
    FBlock* newram = mRAMUSAGESWAPBUFFER == mRAMUSAGEBLOCK1 ? mRAMUSAGEBLOCK2 : mRAMUSAGEBLOCK1;
    Fill( &mPool, ULIS2_BLOCKING, ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2, mHost, ULIS2_NOCB, newram, FPixelValue( ULIS2_FORMAT_G8, { 15 } ), newram->Rect() );
    Copy( &mPool, ULIS2_BLOCKING, ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2, mHost, ULIS2_NOCB, oldram, newram, oldram->Rect(), FVec2I( -1, 0 ) );
    mRAMUSAGESWAPBUFFER = newram;

    Clear( &mPool, ULIS2_BLOCKING, ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2, mHost, ULIS2_NOCB, mCanvas, FRect( 10, 80+HH+10, 256, 256 ) );
    mTiledBlock->DrawDebugTileContent( mCanvas, FVec2I64( 10, 80+HH+10 ) );
    mTiledBlock->DrawDebugWireframe( mCanvas, FVec2I64( 10, 80+HH+10 ), 1.f );
    mPixmap->convertFromImage( *mImage );
    mLabel->setPixmap( *mPixmap );
}

