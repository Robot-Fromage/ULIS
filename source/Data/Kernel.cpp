// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Kernel.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FKernel class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Data/Kernel.h"
#include "Maths/Maths.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FKernel
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FKernel::~FKernel()
{
}

FKernel::FKernel( const FVec2I& iSize, float iValue )
    : tSuperClass( iSize.x, iSize.y, ULIS3_FORMAT_GF )
    , mPivot( iSize / 2 )
{
    float* data = reinterpret_cast< float* >( mData );
    for( tSize i = 0; i < Length(); ++i )
        data[ i ] = iValue;

}

FKernel::FKernel( const FVec2I& iSize, std::initializer_list< float > iNums )
    : tSuperClass( iSize.x, iSize.y, ULIS3_FORMAT_GF )
    , mPivot( iSize / 2 )
{
    ULIS3_ASSERT( Length() == iNums.size(), "Bad input initialized list for Kernel" );
    float* data = reinterpret_cast< float* >( mData );
    for( int i = 0; i < iNums.size(); ++i )
        data[ i ] = *( iNums.begin() + i );
}

FKernel::FKernel( const FKernel& iOther )
    : tSuperClass( iOther.Width(), iOther.Height(), ULIS3_FORMAT_GF )
    , mPivot( iOther.Size() / 2 )
{
    memcpy( mData, iOther.mData, BytesTotal() );
}

FKernel::FKernel( FKernel&& iOther )
    : tSuperClass( iOther.mData, iOther.Width(), iOther.Height(), ULIS3_FORMAT_GF )
    , mPivot( iOther.Size() / 2 )
{
    iOther.ReleaseOwnership();
    iOther.mData = nullptr;
    TakeOwnership();
}

FKernel&
FKernel::operator=( const FKernel& iOther ) {
    mPivot = iOther.Size() / 2;
    delete [] mData;
    mWidth = iOther.Width();
    mHeight = iOther.Height();
    mBPS = mWidth * FormatInfo().BPP;
    mBTT = mHeight * mBPS;
    mData = new tByte[ mBTT ];
    memcpy( mData, iOther.mData, BytesTotal() );
    return  *this;
}

FKernel&
FKernel::operator=( FKernel&& iOther ) {
    mPivot = iOther.Size() / 2;
    delete [] mData;
    mWidth = iOther.Width();
    mHeight = iOther.Height();
    mBPS = mWidth * FormatInfo().BPP;
    mBTT = mHeight * mBPS;
    iOther.ReleaseOwnership();
    mData = iOther.mData;
    iOther.mData = nullptr;
    TakeOwnership();
    return  *this;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void
FKernel::Set( std::initializer_list< float > iNums ) {
    ULIS3_ASSERT( Length() == iNums.size(), "Bad input initialized list for Kernel" );
    float* data = reinterpret_cast< float* >( mData );
    for( int i = 0; i < iNums.size(); ++i )
        data[ i ] = *( iNums.begin() + i );
}

float
FKernel::At( int iX, int iY ) const {
    return  *reinterpret_cast< const float* >( PixelPtr( iX, iY ) );
}

float
FKernel::At( FVec2I iPoint ) const {
    return  *reinterpret_cast< const float* >( PixelPtr( iPoint.x, iPoint.y ) );
}

void
FKernel::SetAt( int iX, int iY, float iValue ) {
    *reinterpret_cast< float* >( PixelPtr( iX, iY ) ) = iValue;
}

void
FKernel::SetAt( FVec2I iPoint, float iValue ) {
    *reinterpret_cast< float* >( PixelPtr( iPoint.x, iPoint.y ) ) = iValue;
}

void
FKernel::Clear() {
    Fill( 0.f );
}

void
FKernel::Fill( float iValue ) {
    float* data = reinterpret_cast< float* >( mData );
    for( tSize i = 0; i < Length(); ++i )
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
    float* data = reinterpret_cast< float* >( mData );
    for( tSize i = 0; i < Length(); ++i )
        res += data[ i ];
    return  res;
}

void
FKernel::Add( float iValue ) {
    float* data = reinterpret_cast< float* >( mData );
    for( tSize i = 0; i < Length(); ++i )
        data[ i ] += iValue;
}

void
FKernel::Mul( float iValue ) {
    float* data = reinterpret_cast< float* >( mData );
    for( tSize i = 0; i < Length(); ++i )
        data[ i ] *= iValue;
}

void
FKernel::Normalize() {
    float sum = Sum();
    float* data = reinterpret_cast< float* >( mData );
    for( tSize i = 0; i < Length(); ++i )
        data[ i ] /= sum;
}

bool
FKernel::IsNormalized() const {
    return  FMaths::Abs( Sum() - 1.f ) < FMaths::kEpsilonf;
}

void
FKernel::FlipX() {
    const tSize w = Width() - 1;
    FKernel ret( FVec2I( mWidth, mHeight ) );
    for( tSize x = 0; x < mWidth; ++x )
        for( tSize y = 0; y < mHeight; ++y )
            ret.SetAt( x, y, At( w - x, y ) );
    *this = std::move( ret );
}

void
FKernel::FlipY() {
    const tSize h = Height() - 1;
    FKernel ret( FVec2I( mWidth, mHeight ) );
    for( tSize x = 0; x < mWidth; ++x )
        for( tSize y = 0; y < mHeight; ++y )
            ret.SetAt( x, y, At( x, h - y ) );
    *this = std::move( ret );
}

void
FKernel::Rotate90CW() {
    FKernel ret( FVec2I( mHeight, mWidth ) );
    const int w = Height() - 1;
    const int h = Width() - 1;
    for( tSize x = 0; x < mHeight; ++x )
        for( tSize y = 0; y < mWidth; ++y )
            ret.SetAt( x, y, At( y, w - x ) );
    *this = std::move( ret );
}

void
FKernel::Rotate90CCW() {
    FKernel ret( FVec2I( mHeight, mWidth ) );
    const int w = Height() - 1;
    const int h = Width() - 1;
    for( tSize x = 0; x < mHeight; ++x )
        for( tSize y = 0; y < mWidth; ++y )
            ret.SetAt( x, y, At( h - y, x ) );
    *this = std::move( ret );
}

void
FKernel::Rotate180() {
    FKernel ret( FVec2I( mWidth, mHeight ) );
    const int w = Width() - 1;
    const int h = Height() - 1;
    for( tSize x = 0; x < mWidth; ++x )
        for( tSize y = 0; y < mHeight; ++y )
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

ULIS3_NAMESPACE_END

