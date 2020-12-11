// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Texture_Private.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FTexture_Private class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Image/Texture_Private.h"
#include <CL/cl.h>

ULIS_NAMESPACE_BEGIN
FTexture_Private::~FTexture_Private()
{
    mOnCleanup.ExecuteIfBound( mHostPtr );
}

FTexture_Private::FTexture_Private(
      uint16 iWidth
    , uint16 iHeight
    , const FOnInvalidTexture& iOnInvalid
    , const FOnCleanupData& iOnCleanup
    , FTexture& iParent
    )
    : mHostPtr( nullptr )
    , mWidth( iWidth )
    , mHeight( iHeight )
    , mBytesPerScanline( 0 )
    , mBytesTotal( 0 )
    , mOnInvalid( iOnInvalid )
    , mOnCleanup( iOnCleanup )
    , mParent( iParent )
{
    ULIS_ASSERT( iWidth  > 0, "Width must be greater than zero" );
    ULIS_ASSERT( iHeight > 0, "Height must be greater than zero" );
    const FFormatMetrics& fmt = mParent.FormatMetrics();
    mBytesPerScanline = mWidth * fmt.BPP;
    mBytesTotal = mHeight * mBytesPerScanline;

    uint32 num = mWidth * mHeight * fmt.SPP;
    ULIS_ASSERT( num != 0, "Cannot allocate a buffer of size 0" );
}

uint16
FTexture_Private::Width() const
{
    return  mWidth;
}


uint16
FTexture_Private::Height() const
{
    return  mHeight;
}

uint32
FTexture_Private::Area() const
{
    return  mWidth * mHeight;
}

FRectI
FTexture_Private::Rect() const
{
    return  FRectI( 0, 0, mWidth, mHeight );
}

uint32
FTexture_Private::BytesPerScanLine() const
{
    return  mBytesPerScanline;
}


uint64
FTexture_Private::BytesTotal() const
{
    return  mBytesTotal;
}

void
FTexture_Private::Dirty( bool iCall ) const
{
    Dirty( Rect(), iCall );
}


void
FTexture_Private::Dirty( const FRectI& iRect, bool iCall ) const
{
    if( !iCall )
        return;

#ifdef ULIS_ASSERT_ENABLED
    int w = static_cast< int >( mWidth );
    int h = static_cast< int >( mHeight );
    int x1 = iRect.x;
    int y1 = iRect.y;
    int x2 = iRect.x + iRect.w;
    int y2 = iRect.y + iRect.h;
    ULIS_ASSERT( iRect.w >= 0, "Bad dirty geometry out of range" );
    ULIS_ASSERT( iRect.h >= 0, "Bad dirty geometry out of range" );
    ULIS_ASSERT( x1 >= 0 && x1 < w, "Bad dirty geometry out of range" );
    ULIS_ASSERT( y1 >= 0 && y1 < h, "Bad dirty geometry out of range" );
    ULIS_ASSERT( x2 >= 0 && x2 < w, "Bad dirty geometry out of range" );
    ULIS_ASSERT( y2 >= 0 && y2 < h, "Bad dirty geometry out of range" );
#endif

    mOnInvalid.ExecuteIfBound( &mParent, iRect );
}

void
FTexture_Private::OnInvalid( const FOnInvalidTexture& iOnInvalid )
{
    mOnInvalid = iOnInvalid;
}

void
FTexture_Private::OnCleanup( const FOnCleanupData& iOnCleanup )
{
    mOnCleanup = iOnCleanup;
}

ULIS_NAMESPACE_END

