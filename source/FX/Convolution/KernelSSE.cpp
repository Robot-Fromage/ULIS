// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         KernelSSE.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FKernelSSE class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "FX/Convolution/KernelSSE.h"
#include "Data/Kernel.h"
#include "Maths/Maths.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FKernelSSE
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FKernelSSE::~FKernelSSE()
{
    if( mData )
        delete [] mData;
}

FKernelSSE::FKernelSSE( const FVec2I& iSize, float iValue )
    : mSize( iSize )
    , mPivot( iSize / 2 )
{
    mData = new Vec4f[ Area() ];
    for( int i = 0; i < Area(); ++i )
        mData[ i ] = iValue;
}

FKernelSSE::FKernelSSE( const FVec2I& iSize, std::initializer_list< float > iNums )
    : mSize( iSize )
    , mPivot( iSize / 2 )
{
    ULIS3_ASSERT( Area() == iNums.size(), "Bad input initialized list for KernelSSE" );
    mData = new Vec4f[ Area() ];
    for( int i = 0; i < iNums.size(); ++i )
        mData[ i ] = *( iNums.begin() + i );
}

FKernelSSE::FKernelSSE( const FKernelSSE& iOther )
    : mSize( iOther.mSize )
    , mPivot( iOther.mPivot )
{
    mData = new Vec4f[ Area() ];
    memcpy( mData, iOther.mData, Area() * sizeof( Vec4f ) );
}

FKernelSSE::FKernelSSE( const FKernel& iOther )
    : mSize( iOther.Size() )
    , mPivot( iOther.Pivot() )
{
    mData = new Vec4f[ Area() ];
    const float* data = reinterpret_cast< const float* >( iOther.DataPtr() );
    for( int i = 0; i < Area(); ++i )
        mData[ i ] = data[i];
}

FKernelSSE::FKernelSSE( FKernelSSE&& iOther )
    : mSize( iOther.Size() )
    , mPivot( iOther.Pivot() )
{
    mData = iOther.mData;
    iOther.mData = nullptr;
}

FKernelSSE&
FKernelSSE::operator=( const FKernelSSE& iOther ) {
    mSize = iOther.mSize;
    mPivot = iOther.mPivot;

    if( mData )
        delete [] mData;

    mData = new Vec4f[ Area() ];

    memcpy( mData, iOther.mData, Area() * sizeof( Vec4f ) );
    return  *this;
}

FKernelSSE&
FKernelSSE::operator=( FKernelSSE&& iOther ) {
    mPivot = iOther.mSize;
    mPivot = iOther.mPivot;

    if( mData )
        delete [] mData;

    mData = iOther.mData;
    iOther.mData = nullptr;

    return  *this;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void
FKernelSSE::Set( std::initializer_list< float > iNums ) {
    ULIS3_ASSERT( Area() == iNums.size(), "Bad input initialized list for KernelSSE" );
    for( int i = 0; i < iNums.size(); ++i )
        mData[ i ] = *( iNums.begin() + i );
}

const Vec4f*
FKernelSSE::Data() const {
    return  mData;
}

float
FKernelSSE::At( int iX, int iY ) const {
    return  mData[ iY * mSize.x  + iX ].extract( 0 );
}

float
FKernelSSE::At( FVec2I iPoint ) const {
    return  mData[ iPoint.y * mSize.x  + iPoint.x ].extract( 0 );
}

void
FKernelSSE::SetAt( int iX, int iY, float iValue ) {
    mData[ iY * mSize.x  + iX ] = iValue;
}

void
FKernelSSE::SetAt( FVec2I iPoint, float iValue ) {
    mData[ iPoint.y * mSize.x  + iPoint.x ] = iValue;
}

void
FKernelSSE::Clear() {
    Fill( 0.f );
}

void
FKernelSSE::Fill( float iValue ) {
    for( int i = 0; i < Area(); ++i )
        mData[ i ] = iValue;
}

void
FKernelSSE::SetZeroes() {
    Clear();
}

void
FKernelSSE::SetOnes() {
    Fill( 1.f );
}

float
FKernelSSE::Sum() const {
    Vec4f res = 0.f;
    for( int i = 0; i < Area(); ++i )
        res += mData[ i ];
    return  res.extract( 0 ) / 4.f;
}

void
FKernelSSE::Add( float iValue ) {
    for( int i = 0; i < Area(); ++i )
        mData[ i ] += iValue;
}

void
FKernelSSE::Mul( float iValue ) {
    for( int i = 0; i < Area(); ++i )
        mData[ i ] *= iValue;
}

void
FKernelSSE::Normalize() {
    float sum = Sum();
    for( int i = 0; i < Area(); ++i )
        mData[ i ] /= sum;
}

bool
FKernelSSE::IsNormalized() const {
    return  FMaths::Abs( Sum() - 1.f ) < FMaths::kEpsilonf;
}

void
FKernelSSE::FlipX() {
    const int w = Width() - 1;
    FKernelSSE ret( FVec2I( mSize.x, mSize.y ) );
    for( int x = 0; x < mSize.x; ++x )
        for( int y = 0; y < mSize.y; ++y )
            ret.SetAt( x, y, At( w - x, y ) );
    *this = std::move( ret );
}

void
FKernelSSE::FlipY() {
    const int h = Height() - 1;
    FKernelSSE ret( FVec2I( mSize.x, mSize.y ) );
    for( int x = 0; x < mSize.x; ++x )
        for( int y = 0; y < mSize.y; ++y )
            ret.SetAt( x, y, At( x, h - y ) );
    *this = std::move( ret );
}

void
FKernelSSE::Rotate90CW() {
    FKernelSSE ret( FVec2I( mSize.y, mSize.x ) );
    const int w = Height() - 1;
    const int h = Width() - 1;
    for( int x = 0; x < mSize.y; ++x )
        for( int y = 0; y < mSize.x; ++y )
            ret.SetAt( x, y, At( y, w - x ) );
    *this = std::move( ret );
}

void
FKernelSSE::Rotate90CCW() {
    FKernelSSE ret( FVec2I( mSize.y, mSize.x ) );
    const int w = Height() - 1;
    const int h = Width() - 1;
    for( int x = 0; x < mSize.y; ++x )
        for( int y = 0; y < mSize.x; ++y )
            ret.SetAt( x, y, At( h - y, x ) );
    *this = std::move( ret );
}

void
FKernelSSE::Rotate180() {
    FKernelSSE ret( FVec2I( mSize.x, mSize.y ) );
    const int w = Width() - 1;
    const int h = Height() - 1;
    for( int x = 0; x < mSize.x; ++x )
        for( int y = 0; y < mSize.y; ++y )
            ret.SetAt( x, y, At( w - x, h - y ) );
    *this = std::move( ret );
}

FKernelSSE
FKernelSSE::Normalized() const {
    FKernelSSE ret( *this );
    ret.Normalize();
    return  ret;
}

FKernelSSE
FKernelSSE::FlippedX() const {
    FKernelSSE ret( *this );
    ret.FlipX();
    return  ret;
}

FKernelSSE
FKernelSSE::FlippedY() const {
    FKernelSSE ret( *this );
    ret.FlipY();
    return  ret;
}

FKernelSSE
FKernelSSE::Rotated90CW() const {
    FKernelSSE ret( *this );
    ret.Rotate90CW();
    return  ret;
}

FKernelSSE
FKernelSSE::Rotated90CCW() const {
    FKernelSSE ret( *this );
    ret.Rotate90CCW();
    return  ret;
}

FKernelSSE
FKernelSSE::Rotated180() const {
    FKernelSSE ret( *this );
    ret.Rotate180();
    return  ret;
}

const FVec2I&
FKernelSSE::Size() const {
    return  mSize;
}

int
FKernelSSE::Area() const {
    return  mSize.x * mSize.y;
}

const FVec2I&
FKernelSSE::Pivot() const {
    return  mPivot;
}

void
FKernelSSE::SetPivot( const FVec2I& iPivot ) {
    mPivot = iPivot;
}

int
FKernelSSE::Width() const {
    return  mSize.x;
}

int
FKernelSSE::Height() const {
    return  mSize.y;
}

ULIS3_NAMESPACE_END

