// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Console.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Console.h"
#include <ULIS>
#include <QScrollBar>
#include <QEvent>
#include <QKeyEvent>
#include <QFontDatabase>

SConsole::~SConsole() {
}

SConsole::SConsole( QWidget* iParent )
    : QTextEdit( iParent )
{
    QFont font = QFontDatabase::systemFont( QFontDatabase::FixedFont );
    //font.setFamily( "Courier" );
    //font.setHintingPreference( QFont::PreferNoHinting );
    //font.setStyleStrategy( QFont::NoAntialias );
    font.setStyleHint( QFont::Monospace );
    font.setFixedPitch( true );
    font.setPointSize( 11 );
    QFontMetrics metrics( font );
    this->setTabStopWidth( 4 * metrics.width(' ') );
    this->setFrameStyle( QFrame::NoFrame );
    this->setReadOnly( true );
    this->setLineWrapMode( QTextEdit::LineWrapMode::NoWrap );
    this->setOverwriteMode( true );
    LogCPP( QString( ::ULIS::FullLibraryInformationString().Data() ) );
}

void
SConsole::LogCPP( const QString& iStr )
{
    Log( "[cpp]> " + iStr );
}

void
SConsole::LogPy( const QString& iStr )
{
    Log( "[py]> " + iStr );
}

void
SConsole::Log( const QString& iStr )
{
    this->append( iStr );
    ScrollBottom();
}

void
SConsole::ScrollBottom()
{
    this->verticalScrollBar()->setValue( this->verticalScrollBar()->maximum() );
}

void
SConsole::keyPressEvent( QKeyEvent* e )
{
    if( e->key() == Qt::Key_Escape ) {
        setText( "" );
    }
}

