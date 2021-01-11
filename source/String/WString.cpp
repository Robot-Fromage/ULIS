// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         String.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FWString class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "String/WString.h"
#include "Memory/Array.h"
#include "Math/Math.h"
#include "String/CRC32.h"
#include "String/MD5.h"
#include "String/String.h"
#include "String/UUID.h"
#include <string>
#include <cstring>

ULIS_NAMESPACE_BEGIN
FWString::~FWString() {
    if( mBulk )
        CleanupBulk();
}

FWString::FWString()
    : mBulk( new char_type[1] )
    , mCapacity( 1 )
    , mSize( 0 )
{
    mBulk[0] = L'\0';
}

FWString::FWString( const char_type* iStr )
    : mBulk( nullptr )
    , mCapacity( 0 )
    , mSize( 0 )
{
    uint64 cap = wcslen( iStr ) + 1;
    mBulk = new char_type[ cap ];
    memcpy( mBulk, iStr, sizeof( char_type ) * cap );
    mCapacity = cap;
    mSize = cap - 1;
    mBulk[ mSize ] = '\0';
}

FWString::FWString( const FWString& iStr )
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

FWString::FWString( FWString&& iStr )
    : mBulk( iStr.mBulk )
    , mCapacity( iStr.mCapacity )
    , mSize( iStr.mSize )
{
    iStr.mBulk = nullptr;
    iStr.mCapacity = 0;
    iStr.mSize = 0;
}

FWString::FWString( const FString& iWStr )
    : mBulk( nullptr )
    , mCapacity( 0 )
    , mSize( 0 )
{
    ULIS_ASSERT( false, "TODO" );
}

FWString&
FWString::operator=( const FWString& iStr ) {
    CleanupBulk();
    uint64 cap = iStr.Size() + 1;
    mBulk = new char_type[ cap ];
    memcpy( mBulk, iStr.mBulk, sizeof( char_type ) * cap );
    mCapacity = cap;
    mSize = cap - 1;
    return  (*this);
}

FWString&
FWString::operator=( FWString&& iStr ) {
    mBulk = iStr.mBulk;
    mCapacity = iStr.mCapacity;
    mSize = iStr.mSize;
    iStr.mBulk = nullptr;
    iStr.mCapacity = 0;
    iStr.mSize = 0;
    return  (*this);
}

FWString
FWString::Copy() const {
    return  FWString( *this );
}

FWString
FWString::SubString( uint64 iPos, uint64 iCount ) const {
    uint64 start = FMath::Min( iPos, mSize );
    uint64 end = FMath::Min( iPos + iCount, mSize );
    uint64 len = end - start;
    FWString result( len );
    memcpy( result.mBulk, mBulk + start, len );
    result.mBulk[len] = L'\0';

    return  result;
}

uint64
FWString::Capacity() const {
    return  mCapacity;
}

uint64
FWString::Size() const {
    return  mSize;
}

const FWString::char_type*
FWString::Data() const {
    return  mBulk;
}


FWString::char_type&
FWString::operator[]( uint64 iIndex ) {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( iIndex >= 0 && iIndex < mSize, "Bad Index" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ iIndex ];
}

const FWString::char_type&
FWString::operator[]( uint64 iIndex ) const {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( iIndex >= 0 && iIndex < mSize, "Bad Index" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ iIndex ];
}

FWString::char_type&
FWString::At( uint64 iIndex ) {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( iIndex >= 0 && iIndex < mSize, "Bad Index" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ iIndex ];
}

const FWString::char_type&
FWString::At( uint64 iIndex ) const {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( iIndex >= 0 && iIndex < mSize, "Bad Index" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ iIndex ];
}

FWString::char_type&
FWString::Front() {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ 0 ];
}

const
FWString::char_type&
FWString::Front() const {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ 0 ];
}

FWString::char_type&
FWString::Back() {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ mSize - 1 ];
}

const FWString::char_type&
FWString::Back() const {
    ULIS_ASSERT( mSize > 0, "Bad call, string is empty" );
    ULIS_ASSERT( mBulk != nullptr, "Error, no bulk, string is uninitialized" );
    return  mBulk[ mSize - 1 ];
}

FString
FWString::ToString() const {
    ULIS_ASSERT( false, "TODO" );
    FString result;
    return  result;
}

FWString&
FWString::Append( const FWString& iStr ) {
    uint64 len = mSize + iStr.mSize;
    GrowBulk( len );
    memcpy( mBulk + mSize, iStr.mBulk, iStr.mSize + 1 );
    mSize = len;
    return  (*this);
}

FWString&
FWString::Append( const char_type* iStr ) {
    uint64 olen = wcslen( iStr );
    uint64 len = mSize + olen;
    GrowBulk( len );
    memcpy( mBulk + mSize, iStr, olen + 1 );
    mSize = len;
    return  (*this);
}

FWString&
FWString::Append( char_type iChar ) {
    uint64 len = mSize + 1;
    GrowBulk( len );
    mBulk[ mSize ] = iChar;
    mSize = len;
    return  (*this);
}

FWString&
FWString::Prepend( const FWString& iStr ) {
    uint64 len = mSize + iStr.mSize;
    GrowBulk( len, iStr.mSize );
    mSize = len;
    memcpy( mBulk, iStr.mBulk, iStr.mSize );
    return  (*this);
}

FWString&
FWString::Prepend( const char_type* iStr ) {
    uint64 olen = wcslen( iStr );
    uint64 len = mSize + olen;
    GrowBulk( len, olen );
    memcpy( mBulk, iStr, olen + 1 );
    mSize = len;
    return  (*this);
}

FWString&
FWString::Prepend( char_type iChar ) {
    uint64 len = mSize + 1;
    GrowBulk( len, 1 );
    mBulk[ 0 ] = iChar;
    mSize = len;
    return  (*this);
}

bool
FWString::operator==( const FWString& iOther ) {
    if( mSize != iOther.mSize )
        return  false;

    for( uint64 i = 0; i < mSize; ++i )
        if( mBulk[i] != iOther.mBulk[i] )
            return  false;

    return  true;
}

bool
FWString::operator!=( const FWString& iOther ) {
    return  !( (*this) == iOther );
}

int
FWString::ToInt() const {
    return  std::wcstol( mBulk, nullptr, 10 );
}

unsigned int
FWString::ToUnsignedInt() const {
    return  std::wcstoul( mBulk, nullptr, 10 );
}


float
FWString::ToFloat() const {
    return  std::wcstof( mBulk, nullptr );
}

double
FWString::ToDouble() const {
    return  std::wcstod( mBulk, nullptr );
}

//static
FWString
FWString::FromInt( int iValue ) {
    std::wstring tmp = std::to_wstring( iValue );
    uint64 len = wcslen( tmp.c_str() );
    FWString result( len );
    memcpy( result.mBulk, tmp.c_str(), len + 1 );
    return  result;
}

//static
FWString
FWString::FromUnsignedInt( unsigned int iValue ) {
    std::wstring tmp = std::to_wstring( iValue );
    uint64 len = wcslen( tmp.c_str() );
    FWString result( len );
    memcpy( result.mBulk, tmp.c_str(), len + 1 );
    return  result;
}

//static
FWString
FWString::FromFloat( float iValue ) {
    std::wstring tmp = std::to_wstring( iValue );
    uint64 len = wcslen( tmp.c_str() );
    FWString result( len );
    memcpy( result.mBulk, tmp.c_str(), len + 1 );
    return  result;
}

//static
FWString
FWString::FromDouble( double iValue ) {
    std::wstring tmp = std::to_wstring( iValue );
    uint64 len = wcslen( tmp.c_str() );
    FWString result( len );
    memcpy( result.mBulk, tmp.c_str(), len + 1 );
    return  result;
}

//static
FWString
FWString::FromString( const FString& iValue ) {
    ULIS_ASSERT( false, "TODO" );
    return  FWString();
}

FWString&
FWString::TransformUpperCase() {
    for( uint64 i = 0; i < mSize; ++i )
        mBulk[i] = toupper( mBulk[i] );
    return  (*this);
}

FWString&
FWString::TransformLowerCase() {
    for( uint64 i = 0; i < mSize; ++i )
        mBulk[i] = tolower( mBulk[i] );
    return  (*this);
}

FWString
FWString::ToUpper() const {
    FWString result( *this );
    result.TransformUpperCase();
    return  result;
}

FWString
FWString::ToLower() const {
    FWString result( *this );
    result.TransformLowerCase();
    return  result;
}

bool
FWString::IsEmpty() const {
    return  mSize == 0;
}

void
FWString::Reserve( uint64 iCapacity ) {
    if( iCapacity > mCapacity ) {
        char_type* tmp = new char_type[ iCapacity ];
        memcpy( tmp, mBulk, mSize + 1 );
        mCapacity = iCapacity;
        CleanupBulk();
        mBulk = tmp;
    }
}

void
FWString::Shrink() {
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
FWString::Resize( uint64 iSize, char_type iChar ) {
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
        mBulk[ mSize ] = L'\0';
    }
}

void
FWString::Clear() {
    CleanupBulk();
    mBulk = new char_type[1];
    mCapacity = 1;
    mSize = 0;
    mBulk[0] = L'\0';
}

TArray< FWString >
FWString::Split( const FWString& iSep ) const {
    TArray< FWString > result;
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

FWString&
FWString::Erase( uint64 iPos, uint64 iCount ) {
    uint64 start = FMath::Min( iPos, mSize );
    uint64 end = FMath::Min( iPos + iCount, mSize );
    uint64 rem = mSize - end;
    uint64 len = rem + start;
    memmove( mBulk + start, mBulk + end, rem );
    mSize = len;
    mBulk[ mSize ] = L'\0';
    return  (*this);
}

FWString&
FWString::Insert( uint64 iPos, const FWString& iStr ) {
    // Implemented this way because i'm lazy.
    // Can be optimized.
    return  operator=( std::move( SubString( 0, iPos ).Append( iStr ).Append( SubString( iPos + iStr.Size(), mSize ) ) ) );
}

int64
FWString::FindFirst( const FWString& iStr ) const {
    return  FindFrom( 0, iStr );
}

int64
FWString::FindLast( const FWString& iStr ) const {
    int64 result = -1;
    while( 1 ) {
        int64 tmp = FindFrom( result + 1, iStr );
        if( tmp == -1 )
            return  result;
        result = tmp;
    }
}

int64
FWString::FindFrom( uint64 iPos, const FWString& iStr ) const {
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
FWString::ReplaceFirst( const FWString& iA, const FWString& iB ) {
    int64 index = FindFirst( iA );
    if( index != -1 ) {
        Erase( index, iA.Size() );
        Insert( index, iB );
        return  true;
    }
    return  false;
}

bool
FWString::ReplaceLast( const FWString& iA, const FWString& iB ) {
    int64 index = FindLast( iA );
    if( index != -1 ) {
        Erase( index, iA.Size() );
        Insert( index, iB );
        return  true;
    }
    return  false;
}

bool
FWString::ReplaceFrom( uint64 iPos, const FWString& iA, const FWString& iB ) {
    int64 index = FindFrom( iPos, iA );
    if( index != -1 ) {
        Erase( index, iA.Size() );
        Insert( index, iB );
        return  true;
    }
    return  false;
}

void
FWString::ReplaceAll( const FWString& iA, const FWString& iB ) {
    while( ReplaceFirst( iA, iB ) ) {}
}

// Private
void
FWString::CleanupBulk() {
    ULIS_ASSERT( mBulk, "Bad State" );
    delete [] mBulk;
    mBulk = nullptr;
}

void
FWString::GrowBulk( uint64 iSize, uint64 iCopyOffset ) {
    uint64 cap = iSize + 1;
    if( cap > mCapacity ) {
        char_type* tmp = new char_type[ cap ];
        memcpy( tmp + iCopyOffset, mBulk, mSize + 1 );
        mCapacity = cap;
        CleanupBulk();
        mBulk = tmp;
    }
}

FWString::FWString( uint64 iSize )
    : mBulk( new char_type[ iSize + 1 ] )
    , mCapacity( iSize + 1 )
    , mSize( iSize )
{
}

ULIS_NAMESPACE_END

