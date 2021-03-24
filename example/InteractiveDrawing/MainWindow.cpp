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
#include "ToolBar.h"
#include "ULISLoader.h"

SMainWindow::~SMainWindow() {
    delete  mToolBar;
}

SMainWindow::SMainWindow( FULISLoader& iHandle )
    : QWidget()
    , mHandle( iHandle )
    , mToolBar( new SToolBar( this, mHandle ) )
{
    setFixedSize( 1280, 800 );
    mToolBar->setFixedSize( 116, 800 );
}

