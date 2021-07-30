// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Block.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FBlock class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Image/Block.h"
#include "String/CRC32.h"
#include <new>

ULIS_NAMESPACE_BEGIN
FBlock::~FBlock()
{
    mOnCleanup.ExecuteIfBound( mBitmap );
}

FBlock::FBlock(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , const FOnInvalidBlock& iOnInvalid
    , const FOnCleanupData& iOnCleanup
    )
    : IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , IHasSize2D( FVec2UI16( iWidth, iHeight ) )
    , mBitmap( nullptr )
    , mBytesPerScanline( 0 )
    , mBytesPerPlane( 0 )
    , mBytesTotal( 0 )
    , mOnInvalid( iOnInvalid )
    , mOnCleanup( iOnCleanup )
{
    ULIS_ASSERT( iWidth  > 0, "Width must be greater than zero" );
    ULIS_ASSERT( iHeight > 0, "Height must be greater than zero" );
    mBytesPerScanline = Width() * FormatMetrics().BPP;
    mBytesPerPlane = Planar() ? Area() * BytesPerSample() : BytesPerSample();
    mBytesTotal = Height() * mBytesPerScanline;

    ULIS_ASSERT( mBytesTotal != 0, "Cannot allocate a buffer of size 0" );

    mBitmap = new  ( std::nothrow )  uint8[ mBytesTotal ];
    ULIS_ASSERT( mBitmap, "Allocation failed with requested size: " << mBytesTotal << " bytes" );
}

FBlock::FBlock(
      uint8* iData
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , const FOnInvalidBlock& iOnInvalid
    , const FOnCleanupData& iOnCleanup
    )
    : IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , IHasSize2D( FVec2UI16( iWidth, iHeight ) )
    , mBitmap( iData )
    , mBytesPerScanline( 0 )
    , mBytesPerPlane( 0 )
    , mBytesTotal( 0 )
    , mOnInvalid( iOnInvalid )
    , mOnCleanup( iOnCleanup )
{
    //ULIS_ASSERT( iWidth  > 0, "Width must be greater than zero" );
    //ULIS_ASSERT( iHeight > 0, "Height must be greater than zero" );
    mBytesPerScanline = Width() * FormatMetrics().BPP;
    mBytesTotal = Height() * mBytesPerScanline;
    mBytesPerPlane = Planar() ? Area() * BytesPerSample() : BytesPerSample();
}

FBlock
FBlock::MakeHollow()
{
    return  FBlock();
}

bool
FBlock::IsHollow() const
{
    return  mBitmap == nullptr;
}

uint8*
FBlock::Bits()
{
    return  mBitmap;
}

uint8*
FBlock::ScanlineBits( uint16 iRow )
{
    ULIS_ASSERT( iRow >= 0 && iRow < Height(), "Index out of range" );
    return  mBitmap + ( iRow * mBytesPerScanline );
}

uint8*
FBlock::PlaneBits( uint16 iPlane )
{
    ULIS_ASSERT( iPlane >= 0 && iPlane < SamplesPerPixel(), "Index out of range" );
    return  mBitmap + ( iPlane * mBytesPerPlane );
}

uint8*
FBlock::PixelBits( uint16 iX, uint16 iY )
{
    ULIS_ASSERT( iX < Width(), "Index out of range: " << iX << " " << Width() );
    ULIS_ASSERT( iY < Height(), "Index out of range: " << iY << " " << Height() );
    return  mBitmap + ( iX * FormatMetrics().BPP + iY * mBytesPerScanline );
}

const uint8*
FBlock::Bits() const
{
    return  mBitmap;
}

const uint8*
FBlock::ScanlineBits( uint16 iRow ) const
{
    ULIS_ASSERT( iRow >= 0 && iRow < Height(), "Index out of range" );
    return  mBitmap + ( iRow * mBytesPerScanline );
}

const uint8*
FBlock::PlaneBits( uint16 iPlane ) const
{
    ULIS_ASSERT( iPlane >= 0 && iPlane < SamplesPerPixel(), "Index out of range" );
    return  mBitmap + ( iPlane * mBytesPerPlane );
}

const uint8*
FBlock::PixelBits( uint16 iX, uint16 iY ) const
{
    ULIS_ASSERT( iX >= 0 && iX < Width(), "Index out of range" );
    ULIS_ASSERT( iY >= 0 && iY < Height(), "Index out of range" );
    return  mBitmap + ( iX * FormatMetrics().BPP + iY * mBytesPerScanline );
}

uint32
FBlock::BytesPerScanLine() const
{
    return  mBytesPerScanline;
}

uint32
FBlock::BytesPerPlane() const
{
    return  mBytesPerPlane;
}

uint64
FBlock::BytesTotal() const
{
    return  mBytesTotal;
}

void
FBlock::Dirty( bool iCall ) const
{
    FRectI rect= Rect();
    Dirty( &rect, 1, iCall );
}

void
FBlock::Dirty( const FRectI& iRect, bool iCall ) const
{
    Dirty( &iRect, 1, iCall );
}

void
FBlock::Dirty( const FRectI* iRectList, const uint32 iNumRects, bool iCall ) const
{
    if( !iCall )
        return;

#ifdef ULIS_ASSERT_ENABLED
    {
        int w = static_cast< int >( Width() );
        int h = static_cast< int >( Height() );
        for( uint32 i = 0; i < iNumRects; ++i ) {
            int x1 = iRectList[i].x;
            int y1 = iRectList[i].y;
            int x2 = iRectList[i].x + iRectList[i].w;
            int y2 = iRectList[i].y + iRectList[i].h;
            ULIS_ASSERT( iRectList[i].w >= 0, "Bad dirty geometry out of range" );
            ULIS_ASSERT( iRectList[i].h >= 0, "Bad dirty geometry out of range" );
            ULIS_ASSERT( x1 >= 0 && x1 < w, "Bad dirty geometry out of range" );
            ULIS_ASSERT( y1 >= 0 && y1 < h, "Bad dirty geometry out of range" );
            ULIS_ASSERT( x2 >= 0 && x2 <= w, "Bad dirty geometry out of range" );
            ULIS_ASSERT( y2 >= 0 && y2 <= h, "Bad dirty geometry out of range" );
        }
    }
#endif

    mOnInvalid.ExecuteIfBound( this, iRectList, iNumRects );
}

FColor
FBlock::Color( uint16 iX, uint16 iY ) const
{
    return  FColor( PixelBits( iX, iY ), Format(), ColorSpace() );
}

FColor
FBlock::Sample( int16 iX, int16 iY, eBorderMode iBorderMode, const FColor& iConstant ) const
{
    switch( iBorderMode ) {
        case eBorderMode::Border_Transparent : {
            if( Rect().HitTest( FVec2I( iX, iY ) ) )
                return  Color( iX, iY );
            else
                return  FColor::Transparent;
        }
        case eBorderMode::Border_Constant : {
            if( Rect().HitTest( FVec2I( iX, iY ) ) )
                return  Color( iX, iY );
            else
                return  iConstant;
        }
        case eBorderMode::Border_Extend : {
            return  Color( FMath::Clamp( iX, int16(0), int16( Width() ) ), FMath::Clamp( iY, int16(0), int16( Height() ) ) );
        }
        case eBorderMode::Border_Wrap : {
            return  Color( FMath::PyModulo( iX, int16( Width() ) ), FMath::PyModulo( iY, int16( Height() ) ) );
        }
        default : {
            return  FColor::Transparent;
        }
    }
}

FColor
FBlock::SampleSubpixel( float iX, float iY, eBorderMode iBorderMode, const FColor& iColor, bool iCompensateBlackDrifting ) const
{
    int16 x0 = static_cast< int16 >( FMath::RoundToNegativeInfinity( iX ) );
    int16 y0 = static_cast< int16 >( FMath::RoundToNegativeInfinity( iY ) );
    int16 x1 = x0 + 1;
    int16 y1 = y0 + 1;
    ufloat t = iX - x0;
    ufloat u = iY - y0;
    FColor m00 = Sample( x0, y0, iBorderMode, iColor );
    FColor m10 = Sample( x1, y0, iBorderMode, iColor );
    FColor m01 = Sample( x0, y1, iBorderMode, iColor );
    FColor m11 = Sample( x1, y1, iBorderMode, iColor );
    if( iCompensateBlackDrifting ) {
        m00.Premultiply();
        m10.Premultiply();
        m01.Premultiply();
        m11.Premultiply();
    }
    FColor row0 = ISample::MixFormat( m00, m10, Format(), t );
    FColor row1 = ISample::MixFormat( m01, m11, Format(), t );
    FColor col0 = ISample::MixFormat( row0, row1, Format(), u );
    if( iCompensateBlackDrifting )
        col0.Unpremultiply();
    return  col0;
}

FPixel
FBlock::Pixel( uint16 iX, uint16 iY )
{
    return  FPixel( PixelBits( iX, iY ), Format(), ColorSpace(), BytesPerPlane() );
}

const FPixel
FBlock::Pixel( uint16 iX, uint16 iY ) const
{
    return  FPixel( PixelBits( iX, iY ), Format(), ColorSpace(), BytesPerPlane() );
}

void
FBlock::SetPixel( uint16 iX, uint16 iY, const ISample& iSample )
{
    FPixel pixel = Pixel( iX, iY );
    ISample::ConvertFormat( iSample, pixel );
}

void
FBlock::SetPixelSafe(uint16 iX, uint16 iY, const ISample& iSample)
{
    if( iX < 0 || iY < 0 || iX >= Width() || iY >= Height() )
        return;

    FPixel pixel = Pixel(iX, iY);
    ISample::ConvertFormat(iSample, pixel);
}

void
FBlock::OnInvalid( const FOnInvalidBlock& iOnInvalid )
{
    mOnInvalid = iOnInvalid;
}

void
FBlock::OnCleanup( const FOnCleanupData& iOnCleanup )
{
    mOnCleanup = iOnCleanup;
}

void
FBlock::LoadFromData(
      uint8* iData
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , const FOnInvalidBlock& iOnInvalid
    , const FOnCleanupData& iOnCleanup
    )
{
    ULIS_ASSERT( iWidth  > 0, "Width must be greater than zero" );
    ULIS_ASSERT( iHeight > 0, "Height must be greater than zero" );

    mOnCleanup.ExecuteIfBound( mBitmap );

    ReinterpretFormat( iFormat );
    AssignColorSpace( iColorSpace );
    ReinterpretSize( FVec2UI16( iWidth, iHeight ) );
    mBitmap = iData;
    mOnInvalid = iOnInvalid;
    mOnCleanup = iOnCleanup;

    mBytesPerScanline = Width() * FormatMetrics().BPP;
    mBytesPerPlane = Planar() ? Area() * BytesPerSample() : BytesPerSample();
    mBytesTotal = Height() * mBytesPerScanline;
}

void
FBlock::ReallocInternalData(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , const FOnInvalidBlock& iOnInvalid
    , const FOnCleanupData& iOnCleanup
)
{
    ULIS_ASSERT( iWidth  > 0, "Width must be greater than zero" );
    ULIS_ASSERT( iHeight > 0, "Height must be greater than zero" );

    mOnCleanup.ExecuteIfBound( mBitmap );

    ReinterpretFormat( iFormat );
    AssignColorSpace( iColorSpace );
    ReinterpretSize( FVec2UI16( iWidth, iHeight ) );

    mBitmap = new  ( std::nothrow )  uint8[ mBytesTotal ];
    ULIS_ASSERT( mBitmap, "Allocation failed with requested size: " << mBytesTotal << " bytes" );
    mOnInvalid = iOnInvalid;
    mOnCleanup = iOnCleanup;

    mBytesPerScanline = Width() * FormatMetrics().BPP;
    mBytesPerPlane = Planar() ? Area() * BytesPerSample() : BytesPerSample();
    mBytesTotal = Height() * mBytesPerScanline;
}

uint32
FBlock::CRC32() const
{
    return  ULIS::CRC32( mBitmap, static_cast< int >( mBytesTotal ) );
}

ULIS_NAMESPACE_END

