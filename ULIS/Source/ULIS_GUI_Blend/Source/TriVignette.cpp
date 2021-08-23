// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        TriVignette.h
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include "TriVignette.h"
#include "Vignette.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <ULIS_CORE>

/////////////////////////////////////////////////////
// cTriVignette
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
cTriVignette::cTriVignette( ::ULIS::IBlock* iSurface, ::ULIS::IBlock* iComp, QWidget* iParent )
    : tSuperClass   ( iParent )
    , mSurface      ( iSurface )
    , mComp         ( iComp )
    , mDiffVignette ( nullptr )
{
    Init();
    Build();
}


cTriVignette::~cTriVignette()
{
    Destroy();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
//virtual
void
cTriVignette::resizeEvent(QResizeEvent *event)
{
    Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ GUI utilities
void
cTriVignette::Init()
{
    mUlisVignette = new cVignette( mSurface, this );
    mPsVignette = new cVignette( mComp, this );

    ::ULIS::IBlock* diffblock = ::ULIS::FMakeContext::MakeBlock( mSurface->Width(), mSurface->Height(), mSurface->Id() );
    for( int y = 0; y < diffblock->Height(); ++y )
    {
        for( int x = 0; x < diffblock->Width(); ++x )
        {
            ::ULIS::CColor A = mSurface->PixelColor( x, y );
            ::ULIS::CColor B = mComp->PixelColor( x, y );
            int diffR = abs( A.Red() - B.Red() );
            int diffG = abs( A.Green() - B.Green() );
            int diffB = abs( A.Blue() - B.Blue() );
            diffblock->SetPixelColor( x, y, ::ULIS::CColor::FromRGB( diffR, diffG, diffB, 255 ) );
        }
    }

    mDiffVignette = new cVignette( diffblock, this );
}


void
cTriVignette::Build()
{
}


void
cTriVignette::Compose()
{
    int w = width();
    int h = height();
    mUlisVignette->setGeometry( 0, 0, 200, 150 );
    mPsVignette->setGeometry( 200, 0, 200, 150 );
    mDiffVignette->setGeometry( 400, 0, 200, 150 );
}


void
cTriVignette::Destroy()
{
    if( mUlisVignette ) delete  mUlisVignette;
    if( mPsVignette )   delete  mPsVignette;
    if( mDiffVignette ) delete  mDiffVignette;
}

