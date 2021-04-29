// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Canvas.cpp
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Canvas.h"
#include "ULISLoader.h"
#include <QImage>
#include <QLabel>
#include <QVector>
#include <QPixmap>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>

SCanvas::~SCanvas() {
    FContext& ctx = mHandle.ContextCanvas();
    ctx.Finish();
    delete  mTemp;
    delete  mLive;
    delete  mCanvas;
    delete  mProxy;
    delete  mImage;
    delete  mPixmap;
    delete  mLabel;
    delete  mTimer;
}

SCanvas::SCanvas( QWidget* iParent, FULISLoader& iHandle )
    : QWidget( iParent )
    , mHandle( iHandle )
    , mTemp( nullptr )
    , mLive( nullptr )
    , mCanvas( nullptr )
    , mProxy( nullptr )
    , mRasterOp( kLine )
    , mRasterMode( kNone )
    , mBlendMode( Blend_Normal )
    , mAlphaMode( Alpha_Normal )
    , mFilled( false )
    , mImage( nullptr )
    , mPixmap( nullptr )
    , mLabel( nullptr )
    , mTimer( nullptr )
    , mPoints( 3 )
    , mPtIndex( 0 )
    , mOpMaxPoints( 2 )
{
    mPixmap = new QPixmap();
    mLabel = new QLabel( this );
    mLabel->setPixmap( *mPixmap );

    mTimer = new QTimer();
    mTimer->setInterval( 1000.0 / 60.0 );
    QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( tickEvent() ) );
    mTimer->start();
    setFocusPolicy( Qt::FocusPolicy::StrongFocus );
    ReallocInternalBuffers( size() );
}

void
SCanvas::SetFormat( int iIndex ) {
    eFormat newFormat = eDocumentFormaMatchingTable[ iIndex ];
    mHandle.SetContextCanvasFormat( newFormat );
    ReallocInternalBuffers( size() );
}

void
SCanvas::SetRaster( eRasterOp iRasterOp, eRasterMode iRasterMode ) {
    mRasterOp = iRasterOp;
    mRasterMode = iRasterMode;
}

void
SCanvas::SetBlend( int iIndex )
{
    mBlendMode = static_cast< eBlendMode >( iIndex );
}

void
SCanvas::SetAlpha( int iIndex )
{
    mAlphaMode = static_cast< eAlphaMode >( iIndex );
}

void
SCanvas::SetFilled( int iState )
{
    mFilled = static_cast< bool >( iState );
}

void
SCanvas::resizeEvent( QResizeEvent* event )
{
    ReallocInternalBuffers( event->size() );
}

void
SCanvas::mousePressEvent( QMouseEvent* event )
{
    mPoints[0] = event->pos();
    mPoints[1] = event->pos();
    mPoints[2] = event->pos();
    Update();
    Refresh();
}

void
SCanvas::mouseMoveEvent( QMouseEvent* event )
{
    mPoints[1] = event->pos();
    Update();
    Refresh();
}

void
SCanvas::mouseReleaseEvent( QMouseEvent* event )
{
    mPoints[1] = event->pos();
    Update();
    Commit();
}

void
SCanvas::keyPressEvent( QKeyEvent* event )
{
    if( event->key() == 0x41) // "A" key
        mPoints[2] = QWidget::mapFromGlobal(QCursor::pos());
}

void
SCanvas::tickEvent()
{
}

void
SCanvas::ReallocInternalBuffers( const QSize& iNewSize )
{
    eFormat fmt_c = mHandle.FormatCanvas();
    eFormat fmt_g = mHandle.FormatGUI();
    FContext& ctx_c = mHandle.ContextCanvas();
    FContext& ctx_g = mHandle.ContextGUI();

    uint16 w = static_cast< uint16 >( iNewSize.width() );
    uint16 h = static_cast< uint16 >( iNewSize.height() );
    FRectI roi( 0, 0, w, h );
    FBlock* newTemp = new FBlock( w, h, fmt_c );
    FBlock* newLive = new FBlock( w, h, fmt_c );
    FBlock* newCanvas = new FBlock( w, h, fmt_c );
    FBlock* newProxy = new FBlock( w, h, fmt_g );

    {
        FEvent eventClearCanvas;
        ctx_c.Clear( *newTemp, roi, FSchedulePolicy::CacheEfficient );
        ctx_c.Clear( *newLive, roi, FSchedulePolicy::CacheEfficient );
        ctx_c.Clear( *newCanvas, roi, FSchedulePolicy::CacheEfficient, 0, nullptr, &eventClearCanvas );
        ctx_g.Clear( *newProxy, roi, FSchedulePolicy::CacheEfficient );

        if( mCanvas )
            ctx_c.ConvertFormat( *mCanvas, *newCanvas, roi, FVec2I(), FSchedulePolicy::MultiScanlines, 1, &eventClearCanvas );
    }
    ctx_c.Finish();
    ctx_g.Finish();

    if( mTemp ) delete mTemp;
    if( mLive ) delete mLive;
    if( mCanvas ) delete mCanvas;
    if( mProxy ) delete mProxy;

    mTemp = newTemp;
    mLive = newLive;
    mCanvas = newCanvas;
    mProxy = newProxy;

    {
        ctx_c.ConvertFormat( *mCanvas, *mProxy, roi, FVec2I(), FSchedulePolicy::MultiScanlines );
    }
    ctx_c.Finish();

    if( mImage ) delete mImage;
    mImage = new QImage( mProxy->Bits(), w, h, mProxy->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    mPixmap->convertFromImage( *mImage );
    mLabel->setPixmap( *mPixmap );
}

void
SCanvas::Update()
{   
    FContext& ctx = mHandle.ContextCanvas();

    double dist = 0;
    int angle1 = 0;
    int angle2 = 0;

    switch (mRasterOp)
    {
        case kLine:
            if( mRasterMode == eRasterMode::kAA )
                ctx.DrawLineAA(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FVec2I(mPoints[1].x(), mPoints[1].y()), FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else if( mRasterMode == eRasterMode::kSP )
                ctx.DrawLineSP(*mTemp, FVec2F(mPoints[0].x() + 0.26, mPoints[0].y() + 0.11), FVec2F(mPoints[1].x(), mPoints[1].y()), FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else
                ctx.DrawLine(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FVec2I(mPoints[1].x(), mPoints[1].y()), FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            break;
        case kCircleAndres:
            if (mRasterMode == eRasterMode::kAA)
                ctx.DrawCircleAndresAA(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FMath::Dist(mPoints[0].x(), mPoints[0].y(), mPoints[1].x(), mPoints[1].y()), FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else if (mRasterMode == eRasterMode::kSP)
                ctx.DrawCircleAndresSP(*mTemp, FVec2F(mPoints[0].x(), mPoints[0].y()), FMath::Dist(float(mPoints[0].x() + 0.15), float(mPoints[0].y() + 0.06), float(mPoints[1].x()), float(mPoints[1].y())), FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else
                ctx.DrawCircleAndres(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FMath::Dist(mPoints[0].x(), mPoints[0].y(), mPoints[1].x(), mPoints[1].y()), FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            break;
        case kCircleBresenham:
            if (mRasterMode == eRasterMode::kAA)
                ctx.DrawCircleBresenhamAA(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FMath::Dist(mPoints[0].x(), mPoints[0].y(), mPoints[1].x(), mPoints[1].y()), FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else if (mRasterMode == eRasterMode::kSP)
                ctx.DrawCircleBresenhamSP(*mTemp, FVec2F(mPoints[0].x(), mPoints[0].y()), FMath::Dist(mPoints[0].x(), mPoints[0].y(), mPoints[1].x(), mPoints[1].y()), FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else
                ctx.DrawCircleBresenham(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FMath::Dist(mPoints[0].x(), mPoints[0].y(), mPoints[1].x(), mPoints[1].y()), FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            break;
        case kArcAndres:
            dist = FMath::Dist(mPoints[0].x(), mPoints[0].y(), mPoints[1].x(), mPoints[1].y());
            angle1 = int(540 + FMath::RadToDeg(atan2(mPoints[1].y() - mPoints[0].y(), mPoints[1].x() - mPoints[0].x()) - atan2(1, 0))) % 360;
            angle2 = int(540 + FMath::RadToDeg(atan2(mPoints[2].y() - mPoints[0].y(), mPoints[2].x() - mPoints[0].x()) - atan2(1, 0))) % 360;
            if (mRasterMode == eRasterMode::kAA)
                ctx.DrawArcAndresAA(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), dist, angle2, angle1, FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else if (mRasterMode == eRasterMode::kSP)
                ctx.DrawArcAndresSP(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), dist, angle2, angle1, FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else
                ctx.DrawArcAndres(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), dist, angle2, angle1, FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            break;
        case kArcBresenham:
            dist = FMath::Dist(mPoints[0].x(), mPoints[0].y(), mPoints[1].x(), mPoints[1].y());
            angle1 = int(540 + FMath::RadToDeg(atan2(mPoints[1].y() - mPoints[0].y(), mPoints[1].x() - mPoints[0].x()) - atan2(1, 0))) % 360;
            angle2 = int(540 + FMath::RadToDeg(atan2(mPoints[2].y() - mPoints[0].y(), mPoints[2].x() - mPoints[0].x()) - atan2(1, 0))) % 360;
            if (mRasterMode == eRasterMode::kAA)
                ctx.DrawArcBresenhamAA(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), dist, angle2, angle1, FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else if (mRasterMode == eRasterMode::kSP)
                ctx.DrawArcBresenhamSP(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), dist, angle2, angle1, FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else
                ctx.DrawArcBresenham(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), dist, angle2, angle1, FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            break;
        case kEllipse:
            if (mRasterMode == eRasterMode::kAA)
                ctx.DrawEllipseAA( *mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FMath::Abs( mPoints[0].x() - mPoints[1].x() ), FMath::Abs(mPoints[0].y() - mPoints[1].y() ), FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else if (mRasterMode == eRasterMode::kSP)
                ctx.DrawEllipseSP(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FMath::Abs(mPoints[0].x() - mPoints[1].x()), FMath::Abs(mPoints[0].y() - mPoints[1].y()), FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else
                ctx.DrawEllipse( *mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FMath::Abs( mPoints[0].x() - mPoints[1].x() ), FMath::Abs(mPoints[0].y() - mPoints[1].y() ), FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            break;
            //ctx.DrawCircleBresenhamAA(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), 500, FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
        case kRotatedEllipse:
            if (mRasterMode == eRasterMode::kAA)
                ;
            else if (mRasterMode == eRasterMode::kSP)
                ;
            else
                ;
            break;
            //ctx.DrawCircleBresenhamAA(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), 500, FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
        case kRectangle:
            ctx.DrawRectangle(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()) - (FVec2I(mPoints[0].x(), mPoints[0].y()) - FVec2I(mPoints[1].x(), mPoints[1].y())), FVec2I(mPoints[0].x(), mPoints[0].y()) + (FVec2I(mPoints[0].x(), mPoints[0].y()) - FVec2I(mPoints[1].x(), mPoints[1].y())), FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
        case kPolygon:
            if (mRasterMode == eRasterMode::kAA)
                ;
            else if (mRasterMode == eRasterMode::kSP)
                ;
            else
                ;
            break;
        case kQuadraticBezier:
            if (mRasterMode == eRasterMode::kAA)
                ctx.DrawQuadraticBezierAA(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FVec2I(mPoints[1].x(), mPoints[1].y()), FVec2I(mPoints[2].x(), mPoints[2].y()), 0.5, FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else if (mRasterMode == eRasterMode::kSP)
                ctx.DrawQuadraticBezierSP(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FVec2I(mPoints[1].x(), mPoints[1].y()), FVec2I(mPoints[2].x(), mPoints[2].y()), 0.5, FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            else
                ctx.DrawQuadraticBezier(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), FVec2I(mPoints[1].x(), mPoints[1].y()), FVec2I(mPoints[2].x(), mPoints[2].y()), 0.5, FColor::Black, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
            break;
            //ctx.DrawCircleBresenhamAA(*mTemp, FVec2I(mPoints[0].x(), mPoints[0].y()), 500, FColor::Black, mFilled, mTemp->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, nullptr);
        default:
            std::cout << "" << std::endl;
    }

    ctx.Finish();
}

void
SCanvas::Refresh()
{
    FContext& ctx = mHandle.ContextCanvas();
    {
        FEvent eventCopy;
        FEvent eventBlend;
        ctx.Copy( *mCanvas, *mLive, mCanvas->Rect(), FVec2I(), FSchedulePolicy::MultiScanlines, 0, nullptr, &eventCopy );
        ctx.Blend( *mTemp, *mLive, mTemp->Rect(), FVec2I( 0 ), mBlendMode, mAlphaMode, 1.f, FSchedulePolicy::MultiScanlines, 1, &eventCopy, &eventBlend );
        ctx.ConvertFormat( *mLive, *mProxy, mLive->Rect(), FVec2I(), FSchedulePolicy::MultiScanlines, 1, &eventBlend );
        ctx.Clear( *mTemp, mTemp->Rect(), FSchedulePolicy::CacheEfficient, 1, &eventBlend );
    }
    ctx.Finish();

    mPixmap->convertFromImage( *mImage );
    mLabel->setPixmap( *mPixmap );
}

void
SCanvas::Commit()
{
    FContext& ctx = mHandle.ContextCanvas();
    {
        FEvent eventBlend;
        ctx.Blend( *mTemp, *mCanvas, mTemp->Rect(), FVec2I( 0 ), mBlendMode, mAlphaMode, 1.f, FSchedulePolicy::MultiScanlines, 0, nullptr, &eventBlend );
        ctx.ConvertFormat( *mCanvas, *mProxy, mCanvas->Rect(), FVec2I(), FSchedulePolicy::MultiScanlines, 1, &eventBlend );
        ctx.Clear( *mTemp, mTemp->Rect(), FSchedulePolicy::CacheEfficient, 1, &eventBlend );
    }
    ctx.Finish();

    mPixmap->convertFromImage( *mImage );
    mLabel->setPixmap( *mPixmap );
}

