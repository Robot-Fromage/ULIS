// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MainWindow.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "MainWindow.h"
#include "Caption.h"
#include <QSplitter>

FMainWindow::~FMainWindow()
{
    // Do not delete mCaption, parent has ownership.
    // Do not delete mCenter, parent has ownership.
    delete  mMetrics;
    delete  mViewport;
    delete  mSplitterMetricsViewport;
    delete  mTop;
    delete  mConsole;
    delete  mSplitterTopConsole;
    delete  mLeft;
    delete  mCode;
    delete  mSplitterLeftCode;
}

FMainWindow::FMainWindow()
    : FCustomMainWindow( nullptr )
    , mCaption( nullptr )
    , mCenter( nullptr )
    , mSplitterLeftCode( nullptr )
    , mLeft( nullptr )
    , mCode( nullptr )
{
    mCaption = new FCaption();
    mCenter = new QWidget();
    SetCaptionWidget( mCaption ); // Transfer ownership
    SetCenterWidget( mCenter ); // Transfer ownership
    mSplitterLeftCode = new QSplitter( mCenter );
    mCode = new QWidget();
    mLeft = new QWidget();
    mSplitterLeftCode->addWidget( mLeft );
    mSplitterLeftCode->addWidget( mCode );

    mSplitterLeftCode->resize( 500, 500 );
    mLeft->resize( 200, 500 );

    mSplitterTopConsole = new QSplitter( mLeft );
    mSplitterTopConsole->setOrientation( Qt::Orientation::Vertical );
    mTop = new QWidget();
    mConsole = new QWidget();
    mSplitterTopConsole->addWidget( mTop );
    mSplitterTopConsole->addWidget( mConsole );

    mSplitterTopConsole->resize( 200, 500 );
    mTop->resize( 200, 200 );

    mSplitterMetricsViewport = new QSplitter( mTop );
    mMetrics = new QWidget();
    mViewport = new QWidget();
    mSplitterMetricsViewport->addWidget( mMetrics );
    mSplitterMetricsViewport->addWidget( mViewport );

    mSplitterMetricsViewport->resize( 200, 200 );
    mMetrics->resize( 100, 200 );
}

