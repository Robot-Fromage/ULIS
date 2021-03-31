// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Kernel.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FKernel class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Image/Kernel.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FKernel
// Static common kernels
const FKernel FKernel::Identity =
    FKernel( FVec2I( 3, 3 ), {
          0, 0, 0
        , 0, 1, 0
        , 0, 0, 0
    } );
const FKernel FKernel::Edge4 =
    FKernel( FVec2I( 3, 3 ), {
          +0, -1, +0
        , -1, +4, -1
        , +0, -1, +0
    } );
const FKernel FKernel::Edge8 =
    FKernel( FVec2I( 3, 3 ), {
          -1, -1, -1
        , -1, +8, -1
        , -1, -1, -1
    } );
const FKernel FKernel::Sharpen =
    FKernel( FVec2I( 3, 3 ), {
          +0, -1, +0
        , -1, +5, -1
        , +0, -1, +0
    } );
const FKernel FKernel::BoxBlur =
    FKernel( FVec2I( 3, 3 ), {
          1, 1, 1
        , 1, 1, 1
        , 1, 1, 1
    } );
const FKernel FKernel::GaussianBlur =
    FKernel( FVec2I( 3, 3 ), {
          1, 2, 1
        , 2, 4, 1
        , 1, 2, 1
    } ).Normalized();
const FKernel FKernel::UnsharpMask =
    FKernel( FVec2I( 5, 5 ), {
          1,  4,  6,  4, 1
        , 4, 16, 24, 16, 4
        , 6, 24, 36, 24, 6
        , 4, 16, 24, 16, 4
        , 1,  4,  6,  4, 1
    } ).Normalized();
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FKernel::~FKernel()
{
}

FKernel::FKernel( const FVec2I& iSize, float iValue )
    : tSuperClass( iSize.x, iSize.y, eFormat::Format_GF )
    , mPivot( iSize / 2 )
{
    float* data = reinterpret_cast< float* >( mBitmap );
    for( uint32 i = 0; i < Area(); ++i )
        data[ i ] = iValue;

}

FKernel::FKernel( const FVec2I& iSize, std::initializer_list< float > iNums )
    : tSuperClass( iSize.x, iSize.y, eFormat::Format_GF )
    , mPivot( iSize / 2 )
{
    ULIS_ASSERT( Area() == iNums.size(), "Bad input initialized list for Kernel" );
    float* data = reinterpret_cast< float* >( mBitmap );
    for( int i = 0; i < iNums.size(); ++i )
        data[ i ] = *( iNums.begin() + i );
}

FKernel::FKernel( const FKernel& iOther )
    : tSuperClass( iOther.Width(), iOther.Height(), eFormat::Format_GF )
    , mPivot( iOther.mPivot )
{
    memcpy( mBitmap, iOther.mBitmap, BytesTotal() );
}

FKernel::FKernel( FKernel&& iOther )
    : tSuperClass( iOther.mBitmap, iOther.Width(), iOther.Height(), eFormat::Format_GF )
    , mPivot( iOther.mPivot )
{
    iOther.OnCleanup( FOnCleanupData() );
    iOther.mBitmap = nullptr;
    OnCleanup( FOnCleanupData( &OnCleanup_FreeMemory ) );
}

FKernel&
FKernel::operator=( const FKernel& iOther ) {
    mPivot = iOther.mPivot;
    delete [] mBitmap;
    mWidth = iOther.Width();
    mHeight = iOther.Height();
    mBytesPerScanline = mWidth * BytesPerPixel();
    mBytesTotal = mHeight * mBytesPerScanline;
    mBitmap = new uint8[ mBytesTotal ];
    memcpy( mBitmap, iOther.mBitmap, BytesTotal() );
    return  *this;
}

FKernel&
FKernel::operator=( FKernel&& iOther ) {
    mPivot = iOther.mPivot;
    delete [] mBitmap;
    mWidth = iOther.Width();
    mHeight = iOther.Height();
    mBytesPerScanline = mWidth * BytesPerPixel();
    mBytesTotal = mHeight * mBytesPerScanline;
    iOther.OnCleanup( FOnCleanupData() );
    mBitmap = iOther.mBitmap;
    iOther.mBitmap = nullptr;
    OnCleanup( FOnCleanupData( &OnCleanup_FreeMemory ) );
    return  *this;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void
FKernel::Set( std::initializer_list< float > iNums ) {
    ULIS_ASSERT( Area() == iNums.size(), "Bad input initialized list for Kernel" );
    float* data = reinterpret_cast< float* >( mBitmap );
    for( int i = 0; i < iNums.size(); ++i )
        data[ i ] = *( iNums.begin() + i );
}

float
FKernel::At( int iX, int iY ) const {
    return  *reinterpret_cast< const float* >( PixelBits( iX, iY ) );
}

float
FKernel::At( FVec2I iPoint ) const {
    return  *reinterpret_cast< const float* >( PixelBits( iPoint.x, iPoint.y ) );
}

void
FKernel::SetAt( int iX, int iY, float iValue ) {
    *reinterpret_cast< float* >( PixelBits( iX, iY ) ) = iValue;
}

void
FKernel::SetAt( FVec2I iPoint, float iValue ) {
    *reinterpret_cast< float* >( PixelBits( iPoint.x, iPoint.y ) ) = iValue;
}

void
FKernel::Clear() {
    Fill( 0.f );
}

void
FKernel::Fill( float iValue ) {
    float* data = reinterpret_cast< float* >( mBitmap );
    for( uint32 i = 0; i < Area(); ++i )
        data[ i ] = iValue;
}

void
FKernel::SetZeroes() {
    Clear();
}

void
FKernel::SetOnes() {
    Fill( 1.f );
}

float
FKernel::Sum() const {
    float res = 0.f;
    float* data = reinterpret_cast< float* >( mBitmap );
    for( uint32 i = 0; i < Area(); ++i )
        res += data[ i ];
    return  res;
}

void
FKernel::Add( float iValue ) {
    float* data = reinterpret_cast< float* >( mBitmap );
    for( uint32 i = 0; i < Area(); ++i )
        data[ i ] += iValue;
}

void
FKernel::Mul( float iValue ) {
    float* data = reinterpret_cast< float* >( mBitmap );
    for( uint32 i = 0; i < Area(); ++i )
        data[ i ] *= iValue;
}

void
FKernel::Normalize() {
    float sum = Sum();
    float* data = reinterpret_cast< float* >( mBitmap );
    for( uint32 i = 0; i < Area(); ++i )
        data[ i ] /= sum;
}

bool
FKernel::IsNormalized() const {
    return  FMath::Abs( Sum() - 1.f ) < FMath::kEpsilonf;
}

void
FKernel::FlipX() {
    const uint32 w = Width() - 1;
    FKernel ret( FVec2I( mWidth, mHeight ) );
    for( uint32 x = 0; x < mWidth; ++x )
        for( uint32 y = 0; y < mHeight; ++y )
            ret.SetAt( x, y, At( w - x, y ) );
    *this = std::move( ret );
}

void
FKernel::FlipY() {
    const uint32 h = Height() - 1;
    FKernel ret( FVec2I( mWidth, mHeight ) );
    for( uint32 x = 0; x < mWidth; ++x )
        for( uint32 y = 0; y < mHeight; ++y )
            ret.SetAt( x, y, At( x, h - y ) );
    *this = std::move( ret );
}

void
FKernel::Rotate90CW() {
    FKernel ret( FVec2I( mHeight, mWidth ) );
    const int w = Height() - 1;
    const int h = Width() - 1;
    for( uint32 x = 0; x < mHeight; ++x )
        for( uint32 y = 0; y < mWidth; ++y )
            ret.SetAt( x, y, At( y, w - x ) );
    *this = std::move( ret );
}

void
FKernel::Rotate90CCW() {
    FKernel ret( FVec2I( mHeight, mWidth ) );
    const int w = Height() - 1;
    const int h = Width() - 1;
    for( uint32 x = 0; x < mHeight; ++x )
        for( uint32 y = 0; y < mWidth; ++y )
            ret.SetAt( x, y, At( h - y, x ) );
    *this = std::move( ret );
}

void
FKernel::Rotate180() {
    FKernel ret( FVec2I( mWidth, mHeight ) );
    const int w = Width() - 1;
    const int h = Height() - 1;
    for( uint32 x = 0; x < mWidth; ++x )
        for( uint32 y = 0; y < mHeight; ++y )
            ret.SetAt( x, y, At( w - x, h - y ) );
    *this = std::move( ret );
}

FKernel
FKernel::Normalized() const {
    FKernel ret( *this );
    ret.Normalize();
    return  ret;
}

FKernel
FKernel::FlippedX() const {
    FKernel ret( *this );
    ret.FlipX();
    return  ret;
}

FKernel
FKernel::FlippedY() const {
    FKernel ret( *this );
    ret.FlipY();
    return  ret;
}

FKernel
FKernel::Rotated90CW() const {
    FKernel ret( *this );
    ret.Rotate90CW();
    return  ret;
}

FKernel
FKernel::Rotated90CCW() const {
    FKernel ret( *this );
    ret.Rotate90CCW();
    return  ret;
}

FKernel
FKernel::Rotated180() const {
    FKernel ret( *this );
    ret.Rotate180();
    return  ret;
}

const FVec2I
FKernel::Size() const {
    return  FVec2I( Width(), Height() );
}

const FVec2I&
FKernel::Pivot() const {
    return  mPivot;
}

void
FKernel::SetPivot( const FVec2I& iPivot ) {
    mPivot = iPivot;
}

ULIS_NAMESPACE_END

