// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         StructuringElement.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the FStructuringElement class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Image/StructuringElement.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FStructuringElement
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FStructuringElement::~FStructuringElement()
{
}

FStructuringElement::FStructuringElement( const FVec2I& iSize, eMorphologicalElementValue iValue )
    : tSuperClass( iSize.x, iSize.y, eFormat::Format_G8 )
    , mPivot( iSize / 2 )
{
    for( uint32 i = 0; i < Area(); ++i )
        mBitmap[ i ] = static_cast< uint8 >( iValue );

}

FStructuringElement::FStructuringElement( const FVec2I& iSize, std::initializer_list< eMorphologicalElementValue > iNums )
    : tSuperClass( iSize.x, iSize.y, eFormat::Format_G8 )
    , mPivot( iSize / 2 )
{
    ULIS_ASSERT( Area() == iNums.size(), "Bad input initialized list for Kernel" );
    for( int i = 0; i < iNums.size(); ++i )
        mBitmap[ i ] = static_cast< uint8 >( *( iNums.begin() + i ) );
}

FStructuringElement::FStructuringElement( const FStructuringElement& iOther )
    : tSuperClass( iOther.Width(), iOther.Height(), eFormat::Format_G8 )
    , mPivot( iOther.mPivot )
{
    memcpy( mBitmap, iOther.mBitmap, BytesTotal() );
}

FStructuringElement::FStructuringElement( FStructuringElement&& iOther )
    : tSuperClass( iOther.mBitmap, iOther.Width(), iOther.Height(), eFormat::Format_G8 )
    , mPivot( iOther.mPivot )
{
    iOther.OnCleanup( FOnCleanupData() );
    iOther.mBitmap = nullptr;
    OnCleanup( FOnCleanupData( &OnCleanup_FreeMemory ) );
}

FStructuringElement&
FStructuringElement::operator=( const FStructuringElement& iOther ) {
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

FStructuringElement&
FStructuringElement::operator=( FStructuringElement&& iOther ) {
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
FStructuringElement::Set( std::initializer_list< eMorphologicalElementValue > iNums ) {
    ULIS_ASSERT( Area() == iNums.size(), "Bad input initialized list for Kernel" );
    for( int i = 0; i < iNums.size(); ++i )
        mBitmap[ i ] = static_cast< uint8 >( *( iNums.begin() + i ) );
}

eMorphologicalElementValue
FStructuringElement::At( int iX, int iY ) const {
    return  static_cast< eMorphologicalElementValue >( *PixelBits( iX, iY ) );
}

eMorphologicalElementValue
FStructuringElement::At( FVec2I iPoint ) const {
    return  static_cast< eMorphologicalElementValue >( *PixelBits( iPoint.x, iPoint.y ) );
}

void
FStructuringElement::SetAt( int iX, int iY, eMorphologicalElementValue iValue ) {
    *PixelBits( iX, iY ) = static_cast< uint8 >( iValue );
}

void
FStructuringElement::SetAt( FVec2I iPoint, eMorphologicalElementValue iValue ) {
    *PixelBits( iPoint.x, iPoint.y ) = static_cast< uint8 >( iValue );
}

void
FStructuringElement::Clear() {
    Fill( MpE_Zero );
}

void
FStructuringElement::Fill( eMorphologicalElementValue iValue ) {
    for( uint32 i = 0; i < Area(); ++i )
        mBitmap[ i ] = static_cast< uint8 >( iValue );
}

void
FStructuringElement::SetZeroes() {
    Clear();
}

void
FStructuringElement::SetOnes() {
    Fill( MpE_One );
}

void
FStructuringElement::FlipX() {
    const uint32 w = Width() - 1;
    FStructuringElement ret( FVec2I( mWidth, mHeight ) );
    for( uint32 x = 0; x < mWidth; ++x )
        for( uint32 y = 0; y < mHeight; ++y )
            ret.SetAt( x, y, At( w - x, y ) );
    *this = std::move( ret );
}

void
FStructuringElement::FlipY() {
    const uint32 h = Height() - 1;
    FStructuringElement ret( FVec2I( mWidth, mHeight ) );
    for( uint32 x = 0; x < mWidth; ++x )
        for( uint32 y = 0; y < mHeight; ++y )
            ret.SetAt( x, y, At( x, h - y ) );
    *this = std::move( ret );
}

void
FStructuringElement::Rotate90CW() {
    FStructuringElement ret( FVec2I( mHeight, mWidth ) );
    const int w = Height() - 1;
    //const int h = Width() - 1;
    for( uint32 x = 0; x < mHeight; ++x )
        for( uint32 y = 0; y < mWidth; ++y )
            ret.SetAt( x, y, At( y, w - x ) );
    *this = std::move( ret );
}

void
FStructuringElement::Rotate90CCW() {
    FStructuringElement ret( FVec2I( mHeight, mWidth ) );
    //const int w = Height() - 1;
    const int h = Width() - 1;
    for( uint32 x = 0; x < mHeight; ++x )
        for( uint32 y = 0; y < mWidth; ++y )
            ret.SetAt( x, y, At( h - y, x ) );
    *this = std::move( ret );
}

void
FStructuringElement::Rotate180() {
    FStructuringElement ret( FVec2I( mWidth, mHeight ) );
    const int w = Width() - 1;
    const int h = Height() - 1;
    for( uint32 x = 0; x < mWidth; ++x )
        for( uint32 y = 0; y < mHeight; ++y )
            ret.SetAt( x, y, At( w - x, h - y ) );
    *this = std::move( ret );
}

FStructuringElement
FStructuringElement::FlippedX() const { 
    FStructuringElement ret( *this );
    ret.FlipX();
    return  ret;
}

FStructuringElement
FStructuringElement::FlippedY() const {
    FStructuringElement ret( *this );
    ret.FlipY();
    return  ret;
}

FStructuringElement
FStructuringElement::Rotated90CW() const {
    FStructuringElement ret( *this );
    ret.Rotate90CW();
    return  ret;
}

FStructuringElement
FStructuringElement::Rotated90CCW() const {
    FStructuringElement ret( *this );
    ret.Rotate90CCW();
    return  ret;
}

FStructuringElement
FStructuringElement::Rotated180() const {
    FStructuringElement ret( *this );
    ret.Rotate180();
    return  ret;
}

const FVec2I
FStructuringElement::Size() const {
    return  FVec2I( Width(), Height() );
}

const FVec2I&
FStructuringElement::Pivot() const {
    return  mPivot;
}

void
FStructuringElement::SetPivot( const FVec2I& iPivot ) {
    mPivot = iPivot;
}

ULIS_NAMESPACE_END

