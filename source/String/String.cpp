// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         String.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FString class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "String/String.h"
#include "Memory/Array.h"
#include "Math/Math.h"
#include "String/CRC32.h"
#include "String/MD5.h"
#include "String/UUID.h"
#include "String/WString.h"
#include <string>
#include <cstring>

ULIS_NAMESPACE_BEGIN
FString::~FString() {
    if( mBulk )
        CleanupBulk();
}

FString::FString()
    : mBulk( new char_type[1] )
    , mCapacity( 1 )
    , mSize( 0 )
{
    mBulk[0] = '\0';
}

FString::FString( const char_type* iStr )
    : mBulk( nullptr )
    , mCapacity( 0 )
    , mSize( 0 )
{
    uint64 cap = strlen( iStr ) + 1;
    mBulk = new char_type[ cap ];
    memcpy( mBulk, iStr, sizeof( char_type ) * cap );
    mCapacity = cap;
    mSize = cap - 1;
    mBulk[ mSize ] = '\0';
}

FString::FString( const FString& iStr )
    : mBulk( nullptr )
    , mCapacity( 0 )
    , mSize( 0 )
{
    uint64 cap = iStr.Size() + 1;
    mBulk = new char_type[ cap ];
    memcpy( mBulk, iStr.mBulk, sizeof( char_type ) * cap );
    mCapacity = cap;
    mSize = cap - 1;
}

FString::FString( FString&& iStr )
    : mBulk( iStr.mBulk )
    , mCapacity( iStr.mCapacity )
    , mSize( iStr.mSize )
{
    iStr.mBulk = nullptr;
    iStr.mCapacity = 0;
    iStr.mSize = 0;
}

FString::FString( const FWString& iWStr )
    : mBulk( nullptr )
    , mCapacity( 0 )
    , mSize( 0 )
{
    ULIS_ASSERT( false, "TODO" );
}

FString&
FString::operator=( const FString& iStr ) {
    ULIS_ASSERT( iStr.mBulk != mBulk, "Cannot assign itself" );
    CleanupBulk();
    uint64 cap = iStr.Size() + 1;
    mBulk = new char_type[ cap ];
    memcpy( mBulk, iStr.mBulk, sizeof( char_type ) * cap );
    mCapacity = cap;
    mSize = cap - 1;
    return  (*this);
}

FString&
FString::operator=( FString&& iStr ) {
    ULIS_ASSERT( iStr.mBulk != mBulk, "Cannot assign itself" );
    mBulk = iStr.mBulk;
    mCapacity = iStr.mCapacity;
    mSize = iStr.mSize;
    iStr.mBulk = nullptr;
    iStr.mCapacity = 0;
    iStr.mSize = 0;
    return  (*this);
}

FString
FString::Copy() const {
    return  FString( *this );
}

FString
FString::SubString( uint64 iPos, uint64 iCount ) const {
    uint64 start = FMath::Min( iPos, mSize );
    uint64 end = FMath::Min( iPos + iCount, mSize );
    uint64 len = end - start;
    FString result( len );
    memcpy( result.mBulk, mBulk + start, len );
    result.mBulk[len] = '\0';

    return  result;
}

uint64
FString::Capacity() const {
    return  mCapacity;
}

uint64
FString::Size() const {
    return  mSize;
}

const FString::char_type*
FString::Data() const {
    return  mBulk;
}


FString::char_type&
FString::operator[]( uint64 iIndex ) {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( iIndex >= 0 && iIndex < mSize, "Bad Index" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ iIndex ];
}

const FString::char_type&
FString::operator[]( uint64 iIndex ) const {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( iIndex >= 0 && iIndex < mSize, "Bad Index" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ iIndex ];
}

FString::char_type&
FString::At( uint64 iIndex ) {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( iIndex >= 0 && iIndex < mSize, "Bad Index" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ iIndex ];
}

const FString::char_type&
FString::At( uint64 iIndex ) const {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( iIndex >= 0 && iIndex < mSize, "Bad Index" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ iIndex ];
}

FString::char_type&
FString::Front() {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ 0 ];
}

const
FString::char_type&
FString::Front() const {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ 0 ];
}

FString::char_type&
FString::Back() {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ mSize - 1 ];
}

const FString::char_type&
FString::Back() const {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ mSize - 1 ];
}

FWString
FString::ToWString() const {
    ULIS_ASSERT( false, "TODO" );
    FWString result;
    return  result;
}

FString&
FString::Append( const FString& iStr ) {
    uint64 len = mSize + iStr.mSize;
    GrowBulk( len );
    memcpy( mBulk + mSize, iStr.mBulk, iStr.mSize + 1 );
    mSize = len;
    return  (*this);
}

FString&
FString::Append( const char_type* iStr ) {
    uint64 olen = strlen( iStr );
    uint64 len = mSize + olen;
    GrowBulk( len );
    memcpy( mBulk + mSize, iStr, olen + 1 );
    mSize = len;
    return  (*this);
}

FString&
FString::Append( char_type iChar ) {
    uint64 len = mSize + 1;
    GrowBulk( len );
    mBulk[ mSize ] = iChar;
    mBulk[ len ] = '\0';
    mSize = len;
    return  (*this);
}

FString&
FString::Prepend( const FString& iStr ) {
    uint64 len = mSize + iStr.mSize;
    GrowBulk( len, iStr.mSize );
    mSize = len;
    memcpy( mBulk, iStr.mBulk, iStr.mSize );
    return  (*this);
}

FString&
FString::Prepend( const char_type* iStr ) {
    uint64 olen = strlen( iStr );
    uint64 len = mSize + olen;
    GrowBulk( len, olen );
    memcpy( mBulk, iStr, olen );
    mSize = len;
    return  (*this);
}

FString&
FString::Prepend( char_type iChar ) {
    uint64 len = mSize + 1;
    GrowBulk( len, 1 );
    mBulk[ 0 ] = iChar;
    mSize = len;
    return  (*this);
}

bool
FString::operator==( const FString& iOther ) const {
    if( mSize != iOther.mSize )
        return  false;

    for( uint64 i = 0; i < mSize; ++i )
        if( mBulk[i] != iOther.mBulk[i] )
            return  false;

    return  true;
}

bool
FString::operator!=( const FString& iOther ) const {
    return  !( (*this) == iOther );
}

int
FString::ToInt() const {
    return  std::atoi( mBulk );
}

unsigned int
FString::ToUnsignedInt() const {
    return  std::stoul( mBulk );
}


float
FString::ToFloat() const {
    return  std::stof( mBulk );
}

double
FString::ToDouble() const {
    return  std::stod( mBulk );
}

//static
FString
FString::FromInt( int iValue ) {
    std::string tmp = std::to_string( iValue );
    uint64 len = strlen( tmp.c_str() );
    FString result( len );
    memcpy( result.mBulk, tmp.c_str(), len + 1 );
    return  result;
}

//static
FString
FString::FromUnsignedInt( unsigned int iValue ) {
    std::string tmp = std::to_string( iValue );
    uint64 len = strlen( tmp.c_str() );
    FString result( len );
    memcpy( result.mBulk, tmp.c_str(), len + 1 );
    return  result;
}

//static
FString
FString::FromFloat( float iValue ) {
    std::string tmp = std::to_string( iValue );
    uint64 len = strlen( tmp.c_str() );
    FString result( len );
    memcpy( result.mBulk, tmp.c_str(), len + 1 );
    return  result;
}

//static
FString
FString::FromDouble( double iValue ) {
    std::string tmp = std::to_string( iValue );
    uint64 len = strlen( tmp.c_str() );
    FString result( len );
    memcpy( result.mBulk, tmp.c_str(), len + 1 );
    return  result;
}

//static
FString
FString::FromWString( const FWString& iValue ) {
    ULIS_ASSERT( false, "TODO" );
    return  FString();
}

FString&
FString::TransformUpperCase() {
    for( uint64 i = 0; i < mSize; ++i )
        mBulk[i] = toupper( mBulk[i] );
    return  (*this);
}

FString&
FString::TransformLowerCase() {
    for( uint64 i = 0; i < mSize; ++i )
        mBulk[i] = tolower( mBulk[i] );
    return  (*this);
}

FString
FString::ToUpper() const {
    FString result( *this );
    result.TransformUpperCase();
    return  result;
}

FString
FString::ToLower() const {
    FString result( *this );
    result.TransformLowerCase();
    return  result;
}

bool
FString::IsEmpty() const {
    return  mSize == 0;
}

void
FString::Reserve( uint64 iCapacity ) {
    if( iCapacity > mCapacity ) {
        char_type* tmp = new char_type[ iCapacity ];
        memcpy( tmp, mBulk, mSize + 1 );
        mCapacity = iCapacity;
        CleanupBulk();
        mBulk = tmp;
    }
}

void
FString::Shrink() {
    uint64 cap = mSize + 1;
    if( mCapacity > cap ) {
        char_type* tmp = new char_type[ cap ];
        memcpy( tmp, mBulk, cap );
        mCapacity = cap;
        CleanupBulk();
        mBulk = tmp;
    }
}

void
FString::Resize( uint64 iSize, char_type iChar ) {
    if( iSize == mSize )
        return;

    if( iSize == 0 ) {
        Clear();
    } else {
        GrowBulk( iSize );
        for( uint64 i = mSize; i < iSize; ++i ) {
            mBulk[i] = iChar;
        }

        mSize = iSize;
        mBulk[ mSize ] = '\0';
    }
}

void
FString::Clear() {
    CleanupBulk();
    mBulk = new char_type[1];
    mCapacity = 1;
    mSize = 0;
    mBulk[0] = '\0';
}

TArray< FString >
FString::Split( const FString& iSep ) const {
    TArray< FString > result;
    int64 prev = -1;
    while( 1 ) {
        int64 next = FindFrom( prev + 1, iSep );
        if( next == -1 ) {
            result.PushBack( SubString( prev, mSize - prev ) );
            break;
        } else {
            result.PushBack( SubString( prev, next - prev ) );
        }
        prev = next;
    }
    return  result;
}

FString&
FString::Erase( uint64 iPos, uint64 iCount ) {
    uint64 start = FMath::Min( iPos, mSize );
    uint64 end = FMath::Min( iPos + iCount, mSize );
    uint64 rem = mSize - end;
    uint64 len = rem + start;
    memmove( mBulk + start, mBulk + end, rem );
    mSize = len;
    mBulk[ mSize ] = '\0';
    return  (*this);
}

FString&
FString::Insert( uint64 iPos, const FString& iStr ) {
    // Implemented this way because i'm lazy.
    // Can be optimized.
    return  operator=( std::move( SubString( 0, iPos ).Append( iStr ).Append( SubString( iPos + iStr.Size(), mSize ) ) ) );
}

int64
FString::FindFirst( const FString& iStr ) const {
    return  FindFrom( 0, iStr );
}

int64
FString::FindLast( const FString& iStr ) const {
    int64 result = -1;
    while( 1 ) {
        int64 tmp = FindFrom( result + 1, iStr );
        if( tmp == -1 )
            return  result;
        result = tmp;
    }
}

int64
FString::FindFrom( uint64 iPos, const FString& iStr ) const {
    int64 result = -1;

    for( uint64 i = iPos; i < mSize; ++i ) {
        for( uint64 j = 0; j < iStr.mSize; ++j ) {
            if( i + j > mSize )
                goto end;

            if( mBulk[ i + j ] != iStr.mBulk[j] )
                goto next;
        }
        result = i;
        next:
        continue;
    }

    end:
    return  result;
}

bool
FString::ReplaceFirst( const FString& iA, const FString& iB ) {
    int64 index = FindFirst( iA );
    if( index != -1 ) {
        Erase( index, iA.Size() );
        Insert( index, iB );
        return  true;
    }
    return  false;
}

bool
FString::ReplaceLast( const FString& iA, const FString& iB ) {
    int64 index = FindLast( iA );
    if( index != -1 ) {
        Erase( index, iA.Size() );
        Insert( index, iB );
        return  true;
    }
    return  false;
}

bool
FString::ReplaceFrom( uint64 iPos, const FString& iA, const FString& iB ) {
    int64 index = FindFrom( iPos, iA );
    if( index != -1 ) {
        Erase( index, iA.Size() );
        Insert( index, iB );
        return  true;
    }
    return  false;
}

void
FString::ReplaceAll( const FString& iA, const FString& iB ) {
    while( ReplaceFirst( iA, iB ) ) {}
}

// Internal operators
FString&
FString::operator+=( const FString& iOther ) {
    return  Append( iOther );
}

FString&
FString::operator+=( const char_type* iOther ) {
    return  Append( iOther );
}

uint32
FString::CRC32() const
{
    return  ::ULIS::CRC32( reinterpret_cast< uint8* >( mBulk ), static_cast< int >( mSize ) );
}

// Private
void
FString::CleanupBulk() {
    ULIS_ASSERT( mBulk, "Bad State" );
    delete [] mBulk;
    mBulk = nullptr;
}

void
FString::GrowBulk( uint64 iSize, uint64 iCopyOffset ) {
    uint64 cap = iSize + 1;
    if( cap > mCapacity ) {
        char_type* tmp = new char_type[ cap ];
        memcpy( tmp + iCopyOffset, mBulk, mSize + 1 );
        mCapacity = cap;
        CleanupBulk();
        mBulk = tmp;
    }
}

FString::FString( uint64 iSize )
    : mBulk( new char_type[ iSize + 1 ] )
    , mCapacity( iSize + 1 )
    , mSize( iSize )
{
}

FString
operator+( const FString& iA, const FString& iB ) {
    return  iA.Copy().Append( iB );
}

FString
operator+( const FString& iA, const FString::char_type* iB ) {
    return  iA.Copy().Append( iB );
}

ULIS_NAMESPACE_END

