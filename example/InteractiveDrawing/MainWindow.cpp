// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MainWindow.cpp
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "MainWindow.h"
#include "Canvas.h"
#include "ToolBar.h"
#include "ULISLoader.h"

SMainWindow::~SMainWindow() {
    delete  mToolBar;
    delete  mCanvas;
}

SMainWindow::SMainWindow( FULISLoader& iHandle )
    : QWidget()
    , mHandle( iHandle )
    , mToolBar( new SToolBar( this, mHandle ) )
    , mCanvas( new SCanvas( this, mHandle ) )
{
    setFixedSize( 1280, 800 );
    mToolBar->setFixedSize( 180, 800 );
    mCanvas->setFixedSize( 1280 - 180, 800 );
    mCanvas->move( 180, 0 );

    QObject::connect( mToolBar, SIGNAL( FormatChanged( int ) ), mCanvas, SLOT( SetFormat( int ) ) );
    QObject::connect( mToolBar, SIGNAL( RasterChanged( eRasterOp, eRasterMode ) ), mCanvas, SLOT( SetRaster( eRasterOp, eRasterMode ) ) );
    QObject::connect( mToolBar, SIGNAL( BlendChanged( int ) ), mCanvas, SLOT( SetBlend( int ) ) );
    QObject::connect( mToolBar, SIGNAL( AlphaChanged( int ) ), mCanvas, SLOT( SetAlpha( int ) ) );
    QObject::connect( mToolBar, SIGNAL( FilledChanged( int ) ), mCanvas, SLOT( SetFilled( int ) ) );
}

