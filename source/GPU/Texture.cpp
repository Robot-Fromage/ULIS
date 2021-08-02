// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Texture.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FTexture class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#ifdef ULIS_FEATURE_GPU_ENABLED

#include "Image/Texture.h"
#include "Image/Texture_Private.h"

ULIS_NAMESPACE_BEGIN
FTexture::~FTexture()
{
    delete  d;
}

FTexture::FTexture(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , const FOnInvalidTexture& iOnInvalid
    , const FOnCleanupData& iOnCleanup
    )
    : IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , d( nullptr )
{
    d = new FTexture_Private(
          iWidth
        , iHeight
        , iOnInvalid
        , iOnCleanup
        , *this
    );
}

uint16
FTexture::Width() const
{
    return  d->Width();
}


uint16
FTexture::Height() const
{
    return  d->Height();
}

uint32
FTexture::Area() const
{
    return  d->Area();
}

FRectI
FTexture::Rect() const
{
    return  d->Rect();
}

uint32
FTexture::BytesPerScanLine() const
{
    return  d->BytesPerScanLine();
}


uint64
FTexture::BytesTotal() const
{
    return  d->BytesTotal();
}

void
FTexture::Dirty( bool iCall ) const
{
    d->Dirty( iCall );
}


void
FTexture::Dirty( const FRectI& iRect, bool iCall ) const
{
    d->Dirty( iRect, iCall );
}

void
FTexture::Dirty( const FRectI* iRectList, const uint32 iNumRects, bool iCall ) const
{
    d->Dirty( iRectList, iNumRects, iCall );
}

void
FTexture::OnInvalid( const FOnInvalidTexture& iOnInvalid )
{
    d->OnInvalid( iOnInvalid );
}

void
FTexture::OnCleanup( const FOnCleanupData& iOnCleanup )
{
    d->OnCleanup( iOnCleanup );
}

ULIS_NAMESPACE_END

#endif //ULIS_FEATURE_GPU_ENABLED

