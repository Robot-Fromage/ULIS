// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Widgets.Graph.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definition for the SGraph class.
 */

#include "Application/Widgets/Application.Widgets.Graph.h"

#include "Application/Widgets/Application.Widgets.GraphScene.h"
#include "Application/Widgets/Application.Widgets.GraphView.h"
#include "Config.h"


#ifdef ULIS_GRAPH_WITH_QOPENGLWIDGET
#include <QOpenGLWidget>
#endif


namespace  nApplication {
namespace  nWidgets {


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


SGraph::~SGraph()
{
    Destroy();
}


SGraph::SGraph( QWidget* iParent ) :
    tSuperClass( iParent ),
    mView(  NULL ),
    mScene( NULL )
{
    Init();
    Build();
    Compose();
}


//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Event


void
SGraph::resizeEvent( QResizeEvent* event )
{
    tSuperClass::resizeEvent( event );
    Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ GUI utilities


void
SGraph::Init()
{
    mView   = new SGraphView();
    mScene  = new SGraphScene();
}


void
SGraph::Build()
{
    setObjectName( "SGraph" );

#ifdef ULIS_GRAPH_WITH_QOPENGLWIDGET
    mView->update();
    QOpenGLWidget* gl = new QOpenGLWidget;
    QSurfaceFormat fmt;
    fmt.setSamples( 8 );
    gl->setFormat(fmt);
    mView->setViewport(gl);
    mView->setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
    mView->setRenderHint(QPainter::Antialiasing);
    mView->setAttribute(Qt::WA_OpaquePaintEvent);
    mView->setAttribute(Qt::WA_NoSystemBackground);
    mView->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    mView->viewport()->setAttribute(Qt::WA_NoSystemBackground);
#endif

    mView->setParent( this );
    mView->setScene( mScene );
    mView->InitScroller();

    /*
    // TMP LEAK NODE
    auto testnode = new ::DraftsmanGUI::Node();
    testnode->setPos( mView->sceneRect().center() );
    mScene->addItem( testnode );
    */
}


void
SGraph::Compose()
{
    int w = width();
    int h = height();
    mView->resize( w, h );
}


void
SGraph::Destroy()
{
    delete mScene;
    delete mView;
}


} // namespace  nWidgets
} // namespace  nApplication
