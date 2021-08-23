// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Window.MainWindow.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definition for the SMainWindow class.
 */
#include "Application/Window/Application.Window.MainWindow.h"

#include "Application/Widgets/Application.Widgets.Canvas.h"
#include "Application/Widgets/Application.Widgets.Slider.h"
#include "Application/Widgets/Application.Widgets.Graph.h"

#include <QApplication>
#include <QStyle>
#include <QScrollArea>
#include <QScrollBar>

#include <ULIS_CORE>
#include <vector>

namespace  nApplication {
namespace  nWindow {
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
SMainWindow::~SMainWindow()
{
    tSelf::Destroy();
}


SMainWindow::SMainWindow( QWidget* iParent )
    : tSuperClass( iParent )
    , mScrollArea( nullptr )
    , mWrapper( nullptr )
    , mCanvasWrapper( nullptr )
    , mCanvasA( nullptr )
    , mCanvasB( nullptr )
{
    tSelf::Init();
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
void
SMainWindow::resizeEvent( QResizeEvent*  event )
{
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ GUI utilities
void
SMainWindow::Init()
{
    mScrollArea         = new QScrollArea( this );
    mWrapper            = new QWidget( mScrollArea );
    mCanvasWrapper      = new QWidget( mWrapper );
    mAnalyticsWrapper   = new QWidget( mWrapper );
    mCanvasA            = new ::nApplication::nWidgets::SCanvas( mCanvasWrapper );
    mCanvasB            = new ::nApplication::nWidgets::SCanvas( mCanvasWrapper );
    mGraphA             = new ::nApplication::nWidgets::SGraph( mAnalyticsWrapper );
    mGraphB             = new ::nApplication::nWidgets::SGraph( mAnalyticsWrapper );

    mScrollArea->setWidget( mWrapper );
    mScrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    mScrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
}


void
SMainWindow::Compose()
{
    int w = width();
    int h = height();
    int w2 = w / 2;
    int canvasHeight = 200;
    int analyticsHeight = 200;
    int scrollWidth = qApp->style()->pixelMetric( QStyle::PM_ScrollBarExtent );

    mScrollArea->setGeometry( 0, 0, w, h );
    mWrapper->setGeometry( 0, 0, w - scrollWidth, canvasHeight + analyticsHeight );
    mCanvasWrapper->setGeometry( 0, 0, w - scrollWidth, canvasHeight );
    mCanvasA->setGeometry( 0, 0, w2, canvasHeight );
    mCanvasB->setGeometry( w2, 0, w2, canvasHeight );
    mAnalyticsWrapper->setGeometry( 0, canvasHeight, w - scrollWidth, analyticsHeight );
    mGraphA->setGeometry( 0, 0, w2, analyticsHeight );
    mGraphB->setGeometry( w2, 0, w2, analyticsHeight );
}


void
SMainWindow::Destroy()
{
    delete  mCanvasA;
    delete  mCanvasB;
    delete  mCanvasWrapper;
    delete  mGraphA;
    delete  mGraphB;
    delete  mAnalyticsWrapper;
    delete  mWrapper;
    delete  mScrollArea;
}


} // namespace  nWindow
} // namespace  nApplication

