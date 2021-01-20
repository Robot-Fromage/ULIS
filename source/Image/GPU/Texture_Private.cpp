// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Texture_Private.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FTexture_Private class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#ifdef ULIS_FEATURE_GPU_ENABLED

#include "Image/GPU/Texture_Private.h"

std::string gKernel_Clear =
#include "Kernel/OpenCL/Clear.cl"
;

ULIS_NAMESPACE_BEGIN
FTexture_Private::~FTexture_Private()
{
    mOnCleanup.ExecuteIfBound( nullptr );
}

FTexture_Private::FTexture_Private(
      uint16 iWidth
    , uint16 iHeight
    , const FOnInvalidTexture& iOnInvalid
    , const FOnCleanupData& iOnCleanup
    , FTexture& iParent
    )
    : mWidth( iWidth )
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

    ULIS_ASSERT( mBytesTotal != 0, "Cannot allocate a buffer of size 0" );
    ULIS_ASSERT( mBytesTotal < CL_DEVICE_MAX_MEM_ALLOC_SIZE , "Cannot allocate a buffer that big on device." );
    ULIS_ASSERT( mBytesTotal < CL_DEVICE_GLOBAL_MEM_SIZE, "Cannot allocate a buffer that big on device" );

    cl_int err = 0;
    mBitmap = cl::Buffer( CL_MEM_READ_WRITE, mBytesTotal, nullptr, &err );
    ULIS_ASSERT( err == 0, "Error during buffer creation in device memory." );
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
    Dirty( &Rect(), 1, iCall );
}

void
FTexture_Private::Dirty( const FRectI& iRect, bool iCall ) const
{
    Dirty( &iRect, 1, iCall );
}

void
FTexture_Private::Dirty( const FRectI* iRectList, const uint32 iNumRects, bool iCall ) const
{
    if( !iCall )
        return;

#ifdef ULIS_ASSERT_ENABLED
    {
        int w = static_cast< int >( mWidth );
        int h = static_cast< int >( mHeight );
        for( uint32 i = 0; i < iNumRects; ++i ) {
            int x1 = iRectList[i].x;
            int y1 = iRectList[i].y;
            int x2 = iRectList[i].x + iRectList[i].w;
            int y2 = iRectList[i].y + iRectList[i].h;
            ULIS_ASSERT( iRectList[i].w >= 0, "Bad dirty geometry out of range" );
            ULIS_ASSERT( iRectList[i].h >= 0, "Bad dirty geometry out of range" );
            ULIS_ASSERT( x1 >= 0 && x1 < w, "Bad dirty geometry out of range" );
            ULIS_ASSERT( y1 >= 0 && y1 < h, "Bad dirty geometry out of range" );
            ULIS_ASSERT( x2 >= 0 && x2 < w, "Bad dirty geometry out of range" );
            ULIS_ASSERT( y2 >= 0 && y2 < h, "Bad dirty geometry out of range" );
        }
    }
#endif

    mOnInvalid.ExecuteIfBound( &mParent, iRectList, iNumRects );
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

#endif // ULIS_FEATURE_GPU_ENABLED

