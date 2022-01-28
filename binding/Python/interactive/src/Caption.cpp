// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Caption.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#include "Caption.h"
#include <QLabel>

SCaption::~SCaption() {
    delete  mTitle;
}

SCaption::SCaption()
    : FDefaultCaption()
    , mTitle( nullptr )
{
    mTitle = new QLabel( this );
    mTitle->setText( "pyULIS4 Interactive Demo" );
    mTitle->setStyleSheet("QLabel { color: white; }");
    mTitle->setFont( QFont( "Arial Black" ) );
    mTitle->adjustSize();
    ActiveBackgroundColor( QColor( 50, 50, 50 ) );
    InactiveBackgroundColor( QColor( 20, 20, 20 ) );
    Compose();
}

void
SCaption::Compose()
{
    tSuperClass::Compose();
    mTitle->resize( mTitle->width(), height() );
    mTitle->move( 10, 0 );
}

