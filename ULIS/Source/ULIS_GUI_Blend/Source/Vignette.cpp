// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Vignette.h
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include "Vignette.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <ULIS_CORE>

/////////////////////////////////////////////////////
// cVignette
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
cVignette::cVignette( ::ULIS::IBlock* iSurface, QWidget* iParent )
    : tSuperClass   ( iParent )
    , mSurface      ( iSurface )
    , mDisplay      ( nullptr )
    , mPixmap       ( nullptr )
    , mLabel        ( nullptr )
{
    Init();
    Build();
}


cVignette::~cVignette()
{
    Destroy();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
//virtual
void
cVignette::resizeEvent(QResizeEvent *event)
{
    Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ GUI utilities
void
cVignette::Init()
{
    mDisplay = new QImage( mSurface->DataPtr(), mSurface->Width(), mSurface->Height(), mSurface->BytesPerScanLine(), QImage::Format::Format_ARGB32 );
    mPixmap  = new::QPixmap();
    mLabel   = new QLabel( this );
}


void
cVignette::Build()
{
    *mPixmap = QPixmap::fromImage( *mDisplay );
    mLabel->setPixmap( *mPixmap );
    setFixedSize( mPixmap->size() );
}


void
cVignette::Compose()
{
    int w = width();
    int h = height();
    mLabel->resize( w, h );
}


void
cVignette::Destroy()
{
    if( mDisplay )  delete  mDisplay;
    if( mSurface )  delete  mSurface;
    if( mPixmap )   delete  mPixmap;
    if( mLabel )    delete  mLabel;
}

