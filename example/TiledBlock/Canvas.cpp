// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Canvas.cpp
* @author       Clement Berthaud
* @brief        TiledBlock application for ULIS3.
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
    XDeleteThreadPool( mPool );
}


SCanvas::SCanvas()
    : mHost(        FHardwareMetrics::Detect()           )
    , mPool(        XCreateThreadPool()                 )
    , mCanvas(      nullptr                             )
    , mFontEngine(                                      )
    , mFontReg(     mFontEngine                         )
    , mFont(        mFontReg, "Courrier New", "Bold"    )
    , mTilePool(    nullptr                             )
    , mImage(       nullptr                             )
    , mPixmap(      nullptr                             )
    , mLabel(       nullptr                             )
    , mTimer(       nullptr                             )
{
    uint32 perfIntent = ULIS_PERF_MT | ULIS_PERF_SSE42 | ULIS_PERF_AVX2;

    mCanvas = new  FBlock( 320, 600, ULIS_FORMAT_RGBA8 );

    Clear( mPool, ULIS_BLOCKING, perfIntent, mHost, ULIS_NOCB, mCanvas, mCanvas->Rect() );

    mImage  = new QImage( mCanvas->Bits(), mCanvas->Width(), mCanvas->Height(), mCanvas->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    mPixmap = new QPixmap( QPixmap::fromImage( *mImage ) );
    mLabel  = new QLabel( this );
    mLabel->setPixmap( *mPixmap );
    this->QWidget::setFixedSize( mPixmap->size() );

    mTimer = new QTimer();
    mTimer->setInterval( 1000.0 / 24.0 );
    QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( tickEvent() ) );
    mTimer->start();

    uint64 maxRAM   = Mo2O( 100 );
    mTilePool   = ITilePool::XCreateTilePool( ULIS_FORMAT_RGBA8, nullptr, eMicro::MICRO_128, eMacro::MACRO_16 );
    mTiledBlock = mTilePool->CreateNewTiledBlock();

    mRAMUSAGEBLOCK1 = new FBlock( 300, 100, ULIS_FORMAT_RGBA8 );
    mRAMUSAGEBLOCK2 = new FBlock( 300, 100, ULIS_FORMAT_RGBA8 );
    mRAMUSAGESWAPBUFFER = mRAMUSAGEBLOCK1;
    Fill( mPool, ULIS_BLOCKING, ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, mRAMUSAGEBLOCK1, FColor( ULIS_FORMAT_G8, { 15 } ), mRAMUSAGEBLOCK1->Rect() );
}

void
SCanvas::mouseMoveEvent( QMouseEvent* event ) {
    if ( event->buttons() & Qt::LeftButton ) {
        int HH = ( mRAMUSAGESWAPBUFFER->Height() - 1 );
        int WW = ( mRAMUSAGESWAPBUFFER->Width() - 1 );
        float scale = 1.f;
        FVec2I64 ref( 10, 80+HH+10 );
        FVec2I64 pos( event->x(), event->y() );
        int size = 10;
        for( int i = -size; i < size; ++i ) {
            for( int j = -size; j < size; ++j ) {
                FVec2I64 res( ( pos.x - ref.x + i ) / scale, ( pos.y - ref.y + j ) / scale );
                if( res.x >= 0 && res.x < 256 && res.y >= 0 && res.y < 256 ) {
                    FTileElement** tileptr = mTiledBlock->QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( res, &res );
                    DrawDotNoAA( (*tileptr)->mBlock, FColor( ULIS_FORMAT_G8, {0} ), FVec2I( res.x, res.y ) );
                }
            }
        }
        int x = FMath::Clamp( int( pos.x -ref.x - size ), 0, 256 );
        int y = FMath::Clamp( int( pos.y -ref.y - size ), 0, 256 );
        int x2 = FMath::Clamp( x + size * 2, 0, 256 );
        int y2 = FMath::Clamp( y + size * 2, 0, 256 );
        FRectI rect = FRectI::FromMinMax( x, y, x2, y2 );
        mTiledBlock->ExtendOperativeGeometryAfterMutableChange( FRectI::FromMinMax( x, y, x2, y2 ) );
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
                    DrawDotNoAA( (*tileptr)->mBlock, FColor( ULIS_FORMAT_GA8, {0,0} ), FVec2I( res.x, res.y ) );
                }
            }
        }
        mTiledBlock->SubstractOperativeGeometryAfterMutableChange( FRectI( pos.x -ref.x - size, pos.y -ref.y - size, size * 2, size * 2 ) );
    }
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

    if( event->key() == Qt::Key::Key_A )
        mTiledBlock->Clear();

    if( event->key() == Qt::Key::Key_B ) {
        mTiledBlock->SanitizeNow();
    }

    if( event->key() == Qt::Key::Key_C ) {
        mTiledBlock->SanitizeNow();
        mTiledBlock->RecomputeRoughRootGeometry();
    }

    if( event->key() == Qt::Key::Key_D ) {
        mTiledBlock->SanitizeNow();
        mTiledBlock->RecomputeRoughLeafGeometry();
    }
}

void
SCanvas::tickEvent() {
    int HH = ( mRAMUSAGESWAPBUFFER->Height()    - 1 );
    int WW = ( mRAMUSAGESWAPBUFFER->Width()     - 1 );
    auto cramu = mTilePool->CurrentRAMUsage();
    float maxramu = Mo2O( 2000 );
    float tramu = cramu / maxramu;
    int iramu = FMath::Min( HH, int( tramu * HH ) );
    for( int i = 0; i < iramu; ++i ) {
        FPixel prox = mRAMUSAGESWAPBUFFER->Pixel( WW, HH - i );
        prox.SetR8( 20 );
        prox.SetG8( 80 );
        prox.SetB8( 200 );
        prox.SetA8( 255 );
    }

    Fill( mPool, ULIS_BLOCKING, ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, mCanvas, FColor( ULIS_FORMAT_G8, { 40 } ), mCanvas->Rect() );
    RenderText( mPool, ULIS_BLOCKING, 0, mHost, ULIS_NOCB, mCanvas, L"Target  RAM                : " + std::to_wstring( mTilePool->RAMUsageCapTarget() ),                        mFont, 12, FColor( ULIS_FORMAT_G8, { 220 } ), FTransformation2D::MakeTranslationTransform( 10, 10 ), ULIS_NOAA );
    RenderText( mPool, ULIS_BLOCKING, 0, mHost, ULIS_NOCB, mCanvas, L"Current RAM                : " + std::to_wstring( mTilePool->CurrentRAMUsage() ),                          mFont, 12, FColor( ULIS_FORMAT_G8, { 220 } ), FTransformation2D::MakeTranslationTransform( 10, 20 ), ULIS_NOAA );
    RenderText( mPool, ULIS_BLOCKING, 0, mHost, ULIS_NOCB, mCanvas, L"Num Scheduled For Clear    : " + std::to_wstring( mTilePool->NumTilesScheduledForClear() ),                mFont, 12, FColor( ULIS_FORMAT_G8, { 220 } ), FTransformation2D::MakeTranslationTransform( 10, 30 ), ULIS_NOAA );
    RenderText( mPool, ULIS_BLOCKING, 0, mHost, ULIS_NOCB, mCanvas, L"Num Available For Query    : " + std::to_wstring( mTilePool->NumFreshTilesAvailableForQuery() ),           mFont, 12, FColor( ULIS_FORMAT_G8, { 220 } ), FTransformation2D::MakeTranslationTransform( 10, 40 ), ULIS_NOAA );
    RenderText( mPool, ULIS_BLOCKING, 0, mHost, ULIS_NOCB, mCanvas, L"Num Dirty In Use           : " + std::to_wstring( mTilePool->NumDirtyHashedTilesCurrentlyInUse() ),        mFont, 12, FColor( ULIS_FORMAT_G8, { 220 } ), FTransformation2D::MakeTranslationTransform( 10, 50 ), ULIS_NOAA );
    RenderText( mPool, ULIS_BLOCKING, 0, mHost, ULIS_NOCB, mCanvas, L"Num Correct In Use         : " + std::to_wstring( mTilePool->NumCorrectlyHashedTilesCurrentlyInUse() ),    mFont, 12, FColor( ULIS_FORMAT_G8, { 220 } ), FTransformation2D::MakeTranslationTransform( 10, 60 ), ULIS_NOAA );
    RenderText( mPool, ULIS_BLOCKING, 0, mHost, ULIS_NOCB, mCanvas, L"Num Registered Blocks      : " + std::to_wstring( mTilePool->NumRegisteredTiledBlocks() ),                 mFont, 12, FColor( ULIS_FORMAT_G8, { 220 } ), FTransformation2D::MakeTranslationTransform( 10, 70 ), ULIS_NOAA );
    Copy( mPool, ULIS_BLOCKING, ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, mRAMUSAGESWAPBUFFER, mCanvas, mRAMUSAGESWAPBUFFER->Rect(), FVec2I( 10, 80 ) );

    FBlock* oldram = mRAMUSAGESWAPBUFFER;
    FBlock* newram = mRAMUSAGESWAPBUFFER == mRAMUSAGEBLOCK1 ? mRAMUSAGEBLOCK2 : mRAMUSAGEBLOCK1;
    Fill( mPool, ULIS_BLOCKING, ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, newram, FColor( ULIS_FORMAT_G8, { 15 } ), newram->Rect() );
    Copy( mPool, ULIS_BLOCKING, ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, oldram, newram, oldram->Rect(), FVec2I( -1, 0 ) );
    mRAMUSAGESWAPBUFFER = newram;

    Clear( mPool, ULIS_BLOCKING, ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, mCanvas, FRectI( 10, 80+HH+10, 256, 256 ) );

    mTiledBlock->DrawDebugTileContent(  mCanvas, FVec2I64( 10, 80+HH+10 ) );
    mTiledBlock->DrawDebugWireframe(    mCanvas, FVec2I64( 10, 80+HH+10 ), 1.f );

    FBlock* shade = new FBlock( 256, 256, ULIS_FORMAT_RGBA8 );
    Fill( mPool, ULIS_BLOCKING,  ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, shade, FColor( ULIS_FORMAT_G8, { 0 } ), shade->Rect() );
    FRectI outline = mTiledBlock->GetOperativeGeometry();
    Clear( mPool, ULIS_BLOCKING, ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, shade, outline );
    Blend( mPool, ULIS_BLOCKING,  ULIS_PERF_SSE42 | ULIS_PERF_AVX2, mHost, ULIS_NOCB, shade, mCanvas, shade->Rect(), FVec2F( 10, 80+HH+10 ), ULIS_NOAA, BM_NORMAL, AM_NORMAL, 0.5f );
    delete shade;
    outline.x += 10;
    outline.y += 80+HH+10;
    DrawRectOutlineNoAA( mCanvas, FColor( ULIS_FORMAT_RGB8, { 255, 0, 255 } ), outline );

    mPixmap->convertFromImage( *mImage );
    mLabel->setPixmap( *mPixmap );
}

