// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Widgets.GraphView.h
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definition for the SGraphView class.
 */
#pragma once

#include "Application/Widgets/Application.Widgets.GraphView.h"
#include "Config.h"

#include <QGuiApplication>
#include <QEasingCurve>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QScroller>
#include <QTimer>
#include <QCursor>
#include <QTransform>


#include <cmath>


#define DEFAULT_BACKGROUND_COLOR    QColor( 127, 127, 127 )
#define DEFAULT_GRID_COLOR          QColor( 60, 60, 60 )
#define DEFAULT_SUBGRID_COLOR       QColor( 80, 80, 80 )
#define DEFAULT_GRID_SIZE           20
#define DEFAULT_SUBGRID_COUNT       2


#define MIN_ZOOM    0.2
#define MAX_ZOOM    6.0

//#define SCALE_GRID


namespace  nApplication {
namespace  nWidgets {


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


SGraphView::~SGraphView()
{
    Destroy();
}


SGraphView::SGraphView( QWidget* iParent )
    : tSuperClass( iParent )
    , mScroller(                    nullptr )
    , mTimer(                       nullptr )
    , mLensCursor(                  nullptr )
    , mMoveModifierOn(              false )
    , mZoomModifierOn(              false )
    , mRightButtonMoveOn(           false )
    , mMiddleButtonMoveOn(          false )
    , mMouseMovedWhileMouseZoom(    false )
    , mInteractionState(            "none" )
    , mLastInteractionState(        "none" )
    , mLastMousePos(                QPoint() )
    , mTargetViewPos(               QPoint() )
    , mTargetScenePos(              QPointF() )
    , mVignette_TopLeft(            nullptr )
    , mVignette_Top(                nullptr )
    , mVignette_TopRight(           nullptr )
    , mVignette_Right(              nullptr )
    , mVignette_BotRight(           nullptr )
    , mVignette_Bot(                nullptr )
    , mVignette_BotLeft(            nullptr )
    , mVignette_Left(               nullptr )
{
    Init();
    Build();
    Compose();
}


//--------------------------------------------------------------------------------------
//----------------------------------------------- Protected Painting Behaviour Overrides


void
SGraphView::drawBackground( QPainter *painter, const QRectF &rect )
{
    QColor background_color = DEFAULT_BACKGROUND_COLOR;
    QColor grid_color       = DEFAULT_GRID_COLOR;

    // Painting background
    painter->setBrush( background_color );
    painter->setPen( Qt::NoPen );
    painter->drawRect( rect );

    // Painting grid
    int gridSizei = DEFAULT_GRID_SIZE;
    if( gridSizei <= 0 ) gridSizei = DEFAULT_GRID_SIZE;

    int subgridCounti = DEFAULT_SUBGRID_COUNT;
    if( subgridCounti <= 0 ) subgridCounti = 1;

    if( Scale() < 0.5f )
        gridSizei = gridSizei * 2;

    drawGrid( painter, rect, grid_color, DEFAULT_SUBGRID_COLOR, gridSizei, subgridCounti );
}


void
SGraphView::drawForeground(QPainter *painter, const QRectF &rect)
{
    painter->setRenderHint( QPainter::SmoothPixmapTransform, false );

    qreal scale = Scale();
    int corner_size = 100 / scale;
    int side_size   = 86 / scale;
    int rectLefti   = floor( rect.left() );
    int rectRighti  = ceil( rect.right() );
    int rectTopi    = floor( rect.top() );
    int rectBoti    = ceil( rect.bottom() );
    int x_corner_left   = rectLefti;
    int y_corner_top    = rectTopi;
    int x_corner_right  = rectRighti - corner_size;
    int y_corner_bot    = rectBoti - corner_size;
    int w = x_corner_right - x_corner_left - corner_size;
    int h = y_corner_bot - y_corner_top - corner_size;


    painter->drawImage( QRect( x_corner_left,   y_corner_top, corner_size, corner_size ), *mVignette_TopLeft    );
    painter->drawImage( QRect( x_corner_right,  y_corner_top, corner_size, corner_size ), *mVignette_TopRight   );
    painter->drawImage( QRect( x_corner_right,  y_corner_bot, corner_size, corner_size ), *mVignette_BotRight   );
    painter->drawImage( QRect( x_corner_left,   y_corner_bot, corner_size, corner_size ), *mVignette_BotLeft    );

    painter->drawImage( QRect( x_corner_left + corner_size, y_corner_top, w, side_size ), *mVignette_Top );
    painter->drawImage( QRect( rectRighti - side_size, y_corner_top + corner_size, side_size, h ), *mVignette_Right );
    painter->drawImage( QRect( x_corner_left + corner_size, rectBoti - side_size, w, side_size ), *mVignette_Bot );
    painter->drawImage( QRect( rectLefti, y_corner_top + corner_size, side_size, h ), *mVignette_Left );

    /*
    auto radial_gradient = QRadialGradient( rect.center(), rect.height() );
    radial_gradient.setColorAt(0.0, getVignetteGradientStartColor() );
    radial_gradient.setColorAt(1.0, getVignetteGradientStopColor() );
    painter->setBrush( radial_gradient );
    painter->setPen( Qt::NoPen );
    painter->drawRect( rect );
    */
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------- Protected Event Overrids


void
SGraphView::keyPressEvent( QKeyEvent*  event )
{
    // Custom modifier Listen: Space
    if( event->key() == Qt::Key_Space )
        mMoveModifierOn = true;

    // Custom modifier Listen: Control
    if( event->key() == Qt::Key_Control )
        mZoomModifierOn = true;

    // Pan Reset
    if( event->key() == Qt::Key_0 && mZoomModifierOn )
    {
        mScroller->stop();
        centerOn( sceneRect().center() );
    }

    // Zoom Reset
    if( event->key() == Qt::Key_1 && mZoomModifierOn )
    {
        mScroller->stop();
        SetScale( 1.0 );
    }

    // Zoom Plus
    if( event->key() == Qt::Key_Plus && mZoomModifierOn )
    {
        mScroller->stop();
        qreal scale = Scale();

        qreal delta = 0.1;
        if( scale >= 1.0 )
            delta = 1.0;
        if( scale >= 8.0 )
            delta = 4.0;
        if( scale >= 32.0 )
            delta = 0.0;

        DeltaScale( delta );
    }

    // Zoom Minus
    if( event->key() == Qt::Key_Minus && mZoomModifierOn )
    {
        mScroller->stop();
        qreal scale = Scale();

        qreal delta = -4.0;
        if( scale < 8.0 )
            delta = -1.0;
        if( scale < 2.0 )
            delta = -0.1;
        if( scale < 0.5 )
            delta = 0.0;

        DeltaScale( delta );
    }
}


void
SGraphView::keyReleaseEvent( QKeyEvent*  event )
{
    // Custom modifier Listen: Space
    if( event->key() == Qt::Key_Space && !event->isAutoRepeat() )
        mMoveModifierOn = false;

    // Custom modifier Listen: Control
    if( event->key() == Qt::Key_Control && !event->isAutoRepeat() )
        mZoomModifierOn = false;
}


void
SGraphView::mouseMoveEvent( QMouseEvent*  event )
{
    if( mInteractionState == "mouse_zoom" && event->buttons().testFlag( Qt::LeftButton ) )
    {
        mScroller->stop();
        mMouseMovedWhileMouseZoom = true;

        float lastx = mLastMousePos.x();
        float curx = event->pos().x();
        float deltax = curx - lastx;
        qreal delta = deltax / 500 * ( Scale() + 1.0 );

        DeltaScale( delta );

        centerOn( mTargetScenePos );
        QPointF delta_viewport_pos = mTargetViewPos - QPointF(  viewport()->width() / 2.0,
                                                                viewport()->height() / 2.0 );
        QPointF viewport_center = mapFromScene( mTargetScenePos ) - delta_viewport_pos;
        centerOn( mapToScene( viewport_center.toPoint() ) );
    }

    if( event->buttons().testFlag( Qt::MouseButton::RightButton ) )
    {
        mScroller->stop();
        auto delta = event->pos() - mLastMousePos;
        horizontalScrollBar()->setValue( horizontalScrollBar()->value() - delta.x() );
        verticalScrollBar()->setValue( verticalScrollBar()->value() - delta.y() );
        mRightButtonMoveOn = true;
        //setCursor( Qt::ClosedHandCursor );
    }

    if( event->buttons().testFlag( Qt::MouseButton::MiddleButton ) )
    {
        mScroller->stop();
        auto delta = event->pos() - mLastMousePos;
        horizontalScrollBar()->setValue( horizontalScrollBar()->value() + delta.x() );
        verticalScrollBar()->setValue( verticalScrollBar()->value() + delta.y() );
        mMiddleButtonMoveOn = true;
        //setCursor( Qt::CrossCursor );
    }

    mLastMousePos = event->pos();
}


void
SGraphView::mousePressEvent( QMouseEvent*  event )
{
    mTargetViewPos  = event->pos();
    mTargetScenePos = mapToScene( event->pos() );
    mLastMousePos   = event->pos();
}


void
SGraphView::mouseReleaseEvent( QMouseEvent*  event )
{
    if( mInteractionState == "mouse_zoom" && !mMouseMovedWhileMouseZoom )
    {
        mScroller->stop();
        qreal scale = Scale();

        qreal delta = 0.1;
        if( scale >= 1.0 )
            delta = 1.0;
        if( scale >= 8.0 )
            delta = 4.0;
        if( scale >= 32.0 )
            delta = 0.0;

        DeltaScale( delta );
    }

    mRightButtonMoveOn = false;
    mMiddleButtonMoveOn = false;
    mMouseMovedWhileMouseZoom = false;
}


void
SGraphView::wheelEvent( QWheelEvent*  event )
{
    mScroller->stop();

    //if( mInteractionState == "key_zoom" )
    //{
    mTargetViewPos = event->pos();
    mTargetScenePos = mapToScene( event->pos() );

    DeltaScale( double( event->delta() ) / 1000.0 * ( Scale() + 1.0 ) );

    centerOn( mTargetScenePos );
    QPointF delta_viewport_pos = mTargetViewPos - QPointF(  viewport()->width() / 2.0,
                                                            viewport()->height() / 2.0 );
    QPointF viewport_center = mapFromScene( mTargetScenePos ) - delta_viewport_pos;
    centerOn( mapToScene( viewport_center.toPoint() ) );
    return;
    //}

    tSuperClass::wheelEvent( event );
}


//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- TickSlot


void
SGraphView::tickEvent()
{
    QString decide_state = "none";

    // Decide
    if( mMoveModifierOn && !mZoomModifierOn )
        decide_state = "mouse_move";

    if( mZoomModifierOn )
        decide_state = "key_zoom";

    if( mMoveModifierOn && mZoomModifierOn )
        decide_state = "mouse_zoom";

    if( mLastInteractionState == "mouse_move" && QGuiApplication::mouseButtons().testFlag( Qt::MouseButton::LeftButton ) )
        decide_state = "mouse_move";

    if( mRightButtonMoveOn )
        decide_state = "right_button_move";

    if( mMiddleButtonMoveOn )
        decide_state = "middle_button_move";

    // Apply
    if( decide_state == "none" )
    {
        mScroller->ungrabGesture( viewport() );
        setCursor( Qt::ArrowCursor );
    }
    else if( decide_state == "mouse_move" )
    {
        mScroller->grabGesture( viewport(), QScroller::LeftMouseButtonGesture );

        if( QGuiApplication::mouseButtons().testFlag( Qt::MouseButton::LeftButton ) )
            setCursor( Qt::ClosedHandCursor );
        else
            setCursor( Qt::OpenHandCursor );
    }
    else if( decide_state == "mouse_zoom" )
    {
        mScroller->ungrabGesture( viewport() );
        setCursor( *mLensCursor );
    }
    else if( decide_state == "key_zoom" )
    {
        setCursor( Qt::CrossCursor );
    }
    else if( decide_state == "right_button_move" )
    {
        mScroller->ungrabGesture( viewport() );
        setCursor( Qt::ClosedHandCursor );
    }
    else if( decide_state == "middle_button_move" )
    {
        mScroller->ungrabGesture( viewport() );
        setCursor( Qt::SizeAllCursor );
    }

    // Update if necessary
    if( decide_state != mLastInteractionState )
        repaint();

    // Finally set flags
    mInteractionState = decide_state;
    mLastInteractionState = mInteractionState;
}


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Delegate Drawing Functions


void
SGraphView::drawGrid( QPainter* painter, const QRectF &rect, const QColor& iGridColor, const QColor& iSubGridColor, int iGridSize, int iSubGridCount)
{
    painter->setBrush( Qt::NoBrush );

    float sizei = iGridSize;
    float sizef = static_cast< float >( sizei );

    float startxf   = rect.left();
    float startyf   = rect.top();
    float stopxf    = rect.right();
    float stopyf    = rect.bottom();

    int startxi = static_cast< int >( floor(    startxf / sizef ) ) * sizei;
    int stopxi  = static_cast< int >( ceil(     stopxf  / sizef ) ) * sizei;
    int startyi = static_cast< int >( floor(    startyf / sizef ) ) * sizei;
    int stopyi  = static_cast< int >( ceil(     stopyf  / sizef ) ) * sizei;
    int wi = stopxi - startxi;
    int hi = stopyi - startyi;
    int nlh = wi / sizei;
    int nlv = hi / sizei;

    float shift = 0.5f;

    float startxfs = startxi - shift;
    float stopxfs  = stopxi  + shift;
    float startyfs = startyi - shift;
    float stopyfs  = stopyi  + shift;

    int substepsize = iGridSize / iSubGridCount;
    QPen Apen;
    Apen.setColor( iGridColor );

    QPen Bpen;
    Bpen.setColor( iSubGridColor );

#ifdef SCALE_GRID
    Apen.setWidth( 1 );
    Bpen.setWidth( 1 );
#else
    #ifdef ULIS_GRAPH_WITH_QOPENGLWIDGET
    Apen.setWidthF( 1 / Scale() );
    Bpen.setWidthF( 1 / Scale() );
    #else
    Apen.setWidthF( 1 / Scale() + 0.01 );
    Bpen.setWidthF( 1 / Scale() + 0.01 );
    #endif
#endif

    for( int i = 0; i < nlh; ++i )
    {
        painter->setPen( Apen );
        float x = ( startxi + i * sizei ) + shift;
        painter->drawLine( QPointF( x, startyfs ), QPointF( x, stopyfs ) );

        for( int j = 1; j < iSubGridCount; ++j )
        {
            painter->setPen( Bpen );
            float z = ( x + j * substepsize );
            painter->drawLine( QPointF( z, startyfs ), QPointF( z, stopyfs ) );
        }
    }

    for( int i = 0; i < nlv; ++i )
    {
        painter->setPen( Apen );
        float y = ( startyi + i * sizei ) + shift;
        painter->drawLine( QPointF( startxfs, y ), QPointF( stopxfs, y ) );

        for( int j = 1; j < iSubGridCount; ++j )
        {
            painter->setPen( Bpen );
            float z = ( y + j * substepsize );
            painter->drawLine( QPointF( startxfs, z ), QPointF( stopxfs, z ) );
        }
    }
}


//--------------------------------------------------------------------------------------
//-------------------------------------------------------- Custom Interaction Behaviours


qreal
SGraphView::Scale()
{
    return  transform().m11(); // same as t.m22()
}


void
SGraphView::DeltaScale( qreal iDelta )
{
    QTransform t = transform();
    qreal oldscale = t.m11(); // same as t.m22()
    qreal new_scale = oldscale + iDelta;

    if( new_scale >= MAX_ZOOM )
        new_scale = MAX_ZOOM;

    if( new_scale < MIN_ZOOM )
        new_scale = MIN_ZOOM;

    qreal factor = new_scale / oldscale;

    t.scale( factor, factor );
    setTransform( t );
}


void
SGraphView::SetScale( qreal iValue )
{
    QTransform t = transform();
    qreal old_scale = t.m11(); // both
    qreal new_scale = iValue;
    qreal factor = new_scale / old_scale;

    t.scale( factor, factor );
    setTransform( t );
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ GUI utilities


void
SGraphView::Init()
{
    mTimer = new QTimer( this );

    QString respath = QString::fromStdString( "Resources/" );
    mLensCursor = new QCursor( QPixmap( respath + "media/img/cursors/lens.png" ), 6, 6 );

    mVignette_TopLeft   = new QImage( respath + "media/img/graph/vignette/top_left.png" );
    mVignette_Top       = new QImage( respath + "media/img/graph/vignette/top.png" );
    mVignette_TopRight  = new QImage( respath + "media/img/graph/vignette/top_right.png" );
    mVignette_Right     = new QImage( respath + "media/img/graph/vignette/right.png" );
    mVignette_BotRight  = new QImage( respath + "media/img/graph/vignette/bot_right.png" );
    mVignette_Bot       = new QImage( respath + "media/img/graph/vignette/bot.png" );
    mVignette_BotLeft   = new QImage( respath + "media/img/graph/vignette/bot_left.png" );
    mVignette_Left      = new QImage( respath + "media/img/graph/vignette/left.png" );
}


void
SGraphView::Build()
{
    setObjectName( "SGraphView" );
    setSceneRect( 0, 0, 2000000, 2000000 );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setAutoFillBackground(false);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    centerOn( sceneRect().center() );

    setMouseTracking( true );

    float fps = 20.f;
    int mstime = int( 1000.f / fps );
    mTimer->setInterval( mstime );
    QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( tickEvent() ) );
    mTimer->start();
}


void
SGraphView::Compose()
{
}


void
SGraphView::Destroy()
{
    delete  mLensCursor;
    delete  mTimer;

    delete  mVignette_TopLeft;
    delete  mVignette_Top;
    delete  mVignette_TopRight;
    delete  mVignette_Right;
    delete  mVignette_BotRight;
    delete  mVignette_Bot;
    delete  mVignette_BotLeft;
    delete  mVignette_Left;
}


//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- Scroller Patch


void
SGraphView::InitScroller()
{
    mScroller = QScroller::scroller( this->viewport() );
    QScrollerProperties ScrollerProperties = mScroller->scrollerProperties();
    QVariant OvershootPolicyOff = QVariant::fromValue< QScrollerProperties::OvershootPolicy >( QScrollerProperties::OvershootAlwaysOff );
    QVariant OvershootPolicyOn  = QVariant::fromValue< QScrollerProperties::OvershootPolicy >( QScrollerProperties::OvershootAlwaysOn );
    ScrollerProperties.setScrollMetric( QScrollerProperties::VerticalOvershootPolicy,       OvershootPolicyOff );
    ScrollerProperties.setScrollMetric( QScrollerProperties::HorizontalOvershootPolicy,     OvershootPolicyOff );
    //ScrollerProperties.setScrollMetric( QScrollerProperties::ScrollingCurve,                QEasingCurve::Linear );
    ScrollerProperties.setScrollMetric( QScrollerProperties::DragStartDistance,             0 );
    // Uncomment the following in order to disable smooth panning:
    //ScrollerProperties.setScrollMetric( QScrollerProperties::DecelerationFactor,          0 );
    //ScrollerProperties.setScrollMetric( QScrollerProperties::MaximumVelocity,             0 );
    mScroller->setScrollerProperties( ScrollerProperties );

}


} // namespace  nWidgets
} // namespace  nApplication

