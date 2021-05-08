// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Code.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Code.h"
#include "SyntaxHighlight.h"
#include <QEvent>
#include <QKeyEvent>

SCode::~SCode() {
    delete  mHighlighter;
}

SCode::SCode( QWidget* iParent )
    : QPlainTextEdit( iParent )
{
    QFont font = QFontDatabase::systemFont( QFontDatabase::FixedFont );
    //font.setFamily( "Courier" );
    //font.setHintingPreference( QFont::PreferNoHinting );
    //font.setStyleStrategy( QFont::NoAntialias );
    font.setStyleHint( QFont::Monospace );
    font.setFixedPitch( true );
    font.setPointSize( 11 );

    this->setFrameStyle( QFrame::NoFrame );
    this->setPlainText(
        "from pyULIS4 import *\n"
        "\n"
        "pool   = FThreadPool()\n"
        "queue  = FCommandQueue( pool )\n"
        "fmt    = Format_RGBA8\n"
        "ctx    = FContext( pool, fmt )\n"
        "canvas = FBlock( 800, 600, fmt )\n"
        "\n"
        "def start():\n"
        "   ctx.Clear( canvas )\n"
        "   ctx.Finish()\n"
        "\n"
        "def update( delta ):\n"
        "   ctx.Fill( canvas, FColor.Black )\n"
        "   ctx.Finish()\n"
        "\n"
        "\n"
    );
    this->setLineWrapMode( QPlainTextEdit::LineWrapMode::NoWrap );
    this->setObjectName( "Code" );
    this->setFont(font);
    QFontMetrics metrics( font );
    this->setTabStopWidth( 4 * metrics.width(' ') );
    mHighlighter = new FPythonSyntaxHighlighter( this->document() );
    QTextOption opts = this->document()->defaultTextOption();
    opts.setFlags( opts.flags() | QTextOption::ShowTabsAndSpaces );
    this->document()->setDefaultTextOption( opts );
}

void
SCode::keyPressEvent( QKeyEvent* e )
{
    if( e->key() == Qt::Key_Tab ) {
        int amount = 4 - this->textCursor().positionInBlock() % 4;
        QString str = " ";
        this->insertPlainText( str.repeated( amount ) );
    }
    else
    {
        QPlainTextEdit::keyPressEvent( e );
    }
}

