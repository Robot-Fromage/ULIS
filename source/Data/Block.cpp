// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Block.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FBlock class.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Data/Block.h"
//#include "Base/UUID.h"
#include "Maths/Geometry.h"
#include "Base/CRC32.h"
#include "Base/MD5.h"

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FBlock
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FBlock::~FBlock()
{
    mOnCleanup.ExecuteIfBound( mData );
}


FBlock::FBlock( int iWidth
              , int iHeight
              , tFormat iFormat
              , FColorProfile* iProfile
              , const FOnInvalid& iOnInvalid
              , const FOnCleanup& iOnCleanup )
    : mData( nullptr )
    , mWidth( iWidth )
    , mHeight( iHeight )
    , mOnInvalid( iOnInvalid )
    , mOnCleanup( iOnCleanup )
    , mProfile( iProfile )
    //, mUUID( GenerateWeakUUID( 16 ) )
    , mInfo( iFormat )
{
    ULIS2_ASSERT( iWidth  > 0, "Width must be greater than zero" );
    ULIS2_ASSERT( iHeight > 0, "Height must be greater than zero" );
    mBPS = mWidth * mInfo.BPP;
    mBTT = mHeight * mBPS;

    tSize num = mWidth * mHeight * mInfo.SPP;
    ULIS2_ASSERT( num != 0, "Cannot allocate an image bulk buffer of size 0" )

    // For proper alignment
    switch( Type() ) {
        case TYPE_UINT8     :   mData = reinterpret_cast< tByte* >( new uint8   [ num ] ); break;
        case TYPE_UINT16    :   mData = reinterpret_cast< tByte* >( new uint16  [ num ] ); break;
        case TYPE_UINT32    :   mData = reinterpret_cast< tByte* >( new uint32  [ num ] ); break;
        case TYPE_UFLOAT    :   mData = reinterpret_cast< tByte* >( new ufloat  [ num ] ); break;
        case TYPE_UDOUBLE   :   mData = reinterpret_cast< tByte* >( new udouble [ num ] ); break;
    }
}


FBlock::FBlock( tByte* iData
              , int iWidth
              , int iHeight
              , tFormat iFormat
              , FColorProfile* iProfile
              , const FOnInvalid& iOnInvalid
              , const FOnCleanup& iOnCleanup )
    : mData( iData )
    , mWidth( iWidth )
    , mHeight( iHeight )
    , mOnInvalid( iOnInvalid )
    , mOnCleanup( iOnCleanup )
    , mProfile( iProfile )
    //, mUUID( GenerateWeakUUID( 16 ) )
    , mInfo( iFormat )
{
    ULIS2_ASSERT( iWidth  > 0, "Width must be greater than zero" );
    ULIS2_ASSERT( iHeight > 0, "Height must be greater than zero" );
    mBPS = mWidth * mInfo.BPP;
    mBTT = mHeight * mBPS;

}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
tByte*
FBlock::DataPtr()
{
    return  mData;
}


tByte*
FBlock::PixelPtr( int iX, int iY )
{
    ULIS2_ASSERT( iX >= 0 && iX < static_cast< int >( mWidth ),     "Index out of range" );
    ULIS2_ASSERT( iY >= 0 && iY < static_cast< int >( mHeight ),    "Index out of range" );
    return  mData + ( iX * mInfo.BPP + iY * mBPS );
}


tByte*
FBlock::ScanlinePtr( int iRow )
{
    ULIS2_ASSERT( iRow >= 0 && iRow < static_cast< int >( mHeight ), "Index out of range" );
    return  mData + ( iRow * mBPS );
}


const tByte*
FBlock::DataPtr() const
{
    return  mData;
}


void
FBlock::AssignProfile( FColorProfile* iProfile )
{
    mProfile = iProfile;
}


const tByte*
FBlock::PixelPtr( int iX, int iY ) const
{
    ULIS2_ASSERT( iX >= 0 && iX < static_cast< int >( mWidth ),     "Index out of range" )
    ULIS2_ASSERT( iY >= 0 && iY < static_cast< int >( mHeight ),    "Index out of range" )
    return  mData + ( iX * mInfo.BPP + iY * mBPS );
}


const tByte*
FBlock::ScanlinePtr( int iRow ) const
{
    ULIS2_ASSERT( iRow >= 0 && iRow < static_cast< int >( mHeight ), "Index out of range" )
    return  mData + ( iRow * mBPS );
}


tSize
FBlock::Width() const
{
    return  mWidth;
}


tSize
FBlock::Height() const
{
    return  mHeight;
}


tSize
FBlock::BytesPerSample() const
{
    return  mInfo.BPC;
}


tSize
FBlock::BytesPerPixel() const
{
    return  mInfo.BPP;
}


tSize
FBlock::BytesPerScanLine() const
{
    return  mBPS;
}


tSize
FBlock::BytesTotal() const
{
    return  mBTT;
}


tFormat
FBlock::Format() const
{
    return  mInfo.FMT;
}


eColorModel
FBlock::Model() const
{
    return  mInfo.CM;
}


eType
FBlock::Type() const
{
    return  mInfo.TP;
}


bool
FBlock::HasAlpha() const
{
    return  mInfo.HEA;
}


bool
FBlock::Swapped() const
{
    return  static_cast< bool >( mInfo.SWA );
}


bool
FBlock::Reversed() const
{
    return  static_cast< bool >( mInfo.REV );
}


uint8
FBlock::SamplesPerPixel() const
{
    return  mInfo.SPP;
}


uint8
FBlock::NumColorChannels() const
{
    return  mInfo.NCC;
}


FColorProfile*
FBlock::Profile() const
{
    return  mProfile;
}


uint8
FBlock::RedirectedIndex( uint8 iIndex ) const
{
    ULIS2_ASSERT( iIndex >= 0 && iIndex < mInfo.SPP, "Bad Index" );
    return  mInfo.IDT[ iIndex ];
}


uint8
FBlock::AlphaIndex() const
{
    ULIS2_ASSERT( mInfo.HEA, "Bad Call" );
    return  mInfo.AID;
}


void
FBlock::Invalidate( bool iCall ) const
{
    Invalidate( FRect( 0, 0, Width(), Height() ), iCall );
}


void
FBlock::Invalidate( const FRect& iRect, bool iCall ) const
{
    if( !iCall )
        return;

    ULIS2_ASSERT( iRect.x >= 0 && iRect.x < (int)mWidth,                            "Index out of range" );
    ULIS2_ASSERT( iRect.y >= 0 && iRect.y < (int)mHeight,                           "Index out of range" );
    ULIS2_ASSERT( iRect.x + iRect.w >= 1 && iRect.x + iRect.w <= (int)mWidth,       "Index out of range" );
    ULIS2_ASSERT( iRect.y + iRect.h >= 1 && iRect.y + iRect.h <= (int)mHeight,      "Index out of range" );
    mOnInvalid.ExecuteIfBound( this, iRect );
}


FRect
FBlock::Rect() const
{
    return  FRect( 0, 0, mWidth, mHeight );
}

FPixelValue
FBlock::PixelValue( int iX, int iY ) const
{
    return  FPixelValue( PixelPtr( iX, iY ), Format(), Profile() );
}


FPixelProxy
FBlock::PixelProxy( int iX, int iY )
{
    return  FPixelProxy( PixelPtr( iX, iY ), Format(), Profile() );
}


const FPixelProxy
FBlock::PixelProxy( int iX, int iY ) const
{
    return  FPixelProxy( PixelPtr( iX, iY ), Format(), Profile() );
}


uint32
FBlock::CRC32() const
{
    return  ::ULIS2::CRC32( mData, BytesTotal() );
}


std::string
FBlock::MD5() const
{
    return  ::ULIS2::MD5( mData, BytesTotal() );
}


/*
std::string
FBlock::UUID() const
{
    return  mUUID;
}
*/

uint8*
FBlock::IndexTable() const
{
    return  mInfo.IDT;
}


const FFormatInfo&
FBlock::FormatInfo() const
{
    return  mInfo;
}

void
FBlock::TweakFormat( tFormat iFormat )
{
    FFormatInfo newInfo( iFormat );
    ULIS2_ASSERT( newInfo.BPP == mInfo.BPP, "Bad tweak operation" );
    mInfo = newInfo;
    mBPS = mWidth * mInfo.BPP;
    mBTT = mHeight * mBPS;
}

void
FBlock::SetOnInvalid( const FOnInvalid& iOnInvalid )
{
    mOnInvalid = iOnInvalid;
}

void
FBlock::SetOnCleanup( const FOnCleanup& iOnCleanup )
{
    mOnCleanup = iOnCleanup;
}

void
FBlock::TakeOwnership() {
    mOnCleanup = FOnCleanup( &OnCleanup_FreeMemory );
}

void
FBlock::ReleaseOwnership() {
    mOnCleanup = FOnCleanup( &OnCleanup_DoNothing );
}

ULIS2_NAMESPACE_END

