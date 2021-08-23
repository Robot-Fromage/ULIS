// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Widgets.Slider.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definition for the SSlider class.
 */
#include "Application/Widgets/Application.Widgets.Slider.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>

namespace  nApplication {
namespace  nWidgets {
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
SSlider::~SSlider()
{
}


SSlider::SSlider( QWidget* iParent )
    : tSuperClass( iParent )
    , bIsLeftMouseDown( false )
    , mValue( 0.f )
    , mTimer( nullptr )
    , bSwapDir( false )
{
    mTimer = new QTimer();
    mTimer->setInterval( 1000.0 / 60.0 );
    QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( AutoAnimSlider() ) );
    mTimer->start();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
void
SSlider::paintEvent( QPaintEvent* event )
{
    QPainter painter(this);
    painter.setRenderHint( QPainter::Antialiasing, false );
    int w = width();
    int h = height();
    int x = w * mValue;

    painter.setPen( QPen( QColor( 0, 0, 0 ), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin ) );
    painter.drawRect( 0, 0, w, h );

    painter.setPen( Qt::NoPen );
    painter.setBrush( QColor( 0, 127, 255 ) );
    painter.drawRect( 1, 0, x, h - 2 );
}


void
SSlider::mousePressEvent( QMouseEvent*  event )
{
    if( event->button() == Qt::LeftButton )
        bIsLeftMouseDown = true;
}


void
SSlider::mouseMoveEvent( QMouseEvent*  event )
{
    QPoint pos = event->pos();
    int w = width();
    mValue = float( pos.x() ) / float( w );
    if( mValue < 0.f ) mValue = 0.f;
    if( mValue > 1.f ) mValue = 1.f;

    repaint();
    emit ValueChanged( mValue );
}


void
SSlider::mouseReleaseEvent( QMouseEvent*  event )
{
    if( event->button() == Qt::LeftButton )
        bIsLeftMouseDown = false;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
void
SSlider::AutoAnimSlider()
{
    float step = 0.01f;

    if( bSwapDir )
        mValue -= step;
    else
        mValue += step;

    if( mValue < 0.f )
    {
        mValue = 0.f;
        bSwapDir = !bSwapDir;
    }

    if( mValue > 1.f )
    {
        mValue = 1.f;
        bSwapDir = !bSwapDir;
    }

    emit ValueChanged( mValue );
    repaint();
}


} // namespace  nWidgets
} // namespace  nApplication

