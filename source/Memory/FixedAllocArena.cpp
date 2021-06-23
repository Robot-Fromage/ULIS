// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FixedAllocMemoryPool.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for FixedAllocArena.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Memory/FixedAllocArena.h"
#include "Memory/Memory.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
FFixedAllocArena::~FFixedAllocArena()
{
    //ULIS_ASSERT( IsEmpty(), "Error, trying to delete a non empty arena !" );
    for( uint32 i = 0; i < mNumCells; ++i ) {
        uint8* metaBase = Chunk( i );
        if( !IsChunkAvailable( metaBase ) ) {
            uint8*** client_ptr = reinterpret_cast< uint8*** >( metaBase );
            uint8** client = *client_ptr;
            delete client;
            client_ptr = nullptr;
            mNumAvailableCells++;
        }
    }
    XFree( mBlock );
}

FFixedAllocArena::FFixedAllocArena(
      uint64 iArenaSize
    , uint32 iAllocSize
)
    : mArenaSize( iArenaSize )
    , mAllocSize( iAllocSize )
    , mNumCells( static_cast< uint32 >( mArenaSize / mAllocSize ) )
    , mNumAvailableCells( mNumCells )
    , mBlock( reinterpret_cast< uint8* >( XMalloc( BlockSize() ) ) )
{
    ULIS_ASSERT( mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mAllocSize, "Bad Size !" );
    ULIS_ASSERT( mNumCells, "Bad Size !" );
    ULIS_ASSERT( mNumAvailableCells, "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    for( uint64 i = 0; i < mNumCells; ++i )
        memset( mBlock + i * ( mAllocSize + smMetaPadSize ), 0, smMetaPadSize );

    auto dummy = 0;
}

bool
FFixedAllocArena::IsFull() const
{
    return  mNumAvailableCells == 0;
}

bool
FFixedAllocArena::IsEmpty() const
{
    return  mNumAvailableCells == mNumCells;
}

bool
FFixedAllocArena::IsInRange( const uint8* iAlloc ) const
{
    uint64 adress = reinterpret_cast< uint64 >( iAlloc );
    return  adress >= LowBlockAdress() && adress < HighBlockAdress();
}

uint64
FFixedAllocArena::ArenaSize() const
{
    return  mArenaSize;
}

uint32
FFixedAllocArena::AllocSize() const
{
    return  mAllocSize;
}

uint32
FFixedAllocArena::NumCells() const
{
    return  mNumCells;
}

uint32
FFixedAllocArena::NumAvailableCells() const
{
    return  mNumAvailableCells;
}

uint32
FFixedAllocArena::NumUsedCells() const
{
    return  mNumCells - mNumAvailableCells;
}

uint8*
FFixedAllocArena::Malloc()
{
    for( uint32 i = 0; i < mNumCells; ++i ) {
        uint8* metaBase = Chunk( i );
        if( IsChunkAvailable( metaBase ) ) {
            // client alloc points to data
            uint8* data = metaBase + smMetaPadSize;
            uint8** client = new uint8*( data );

            // meta stores adress of client
            uint8*** client_ptr = reinterpret_cast< uint8*** >( metaBase );
            *client_ptr = client;
            mNumAvailableCells--;
            return  data;
        }
    }
    return  nullptr;
}

void
FFixedAllocArena::Free( uint8* iAlloc )
{
    ULIS_ASSERT( iAlloc, "Cannot free null alloc" );
    uint8* metaBase = iAlloc - smMetaPadSize;
    ULIS_ASSERT( IsInRange( metaBase ), "Bad alloc, not in this arena !" );

    // Cleanup client
    uint8*** client_ptr = reinterpret_cast< uint8*** >( metaBase );
    uint8** client = *client_ptr;
    delete client;
    *client_ptr = nullptr;
    mNumAvailableCells++;
}

float
FFixedAllocArena::LocalFragmentation() const
{
    // Rough Estimate
    if( mNumAvailableCells == 0 )
        return  0.f;

    float frag = ( mNumAvailableCells - LargestFreeChunk() ) / static_cast< float >( mNumAvailableCells );

    uint32 used = NumUsedCells();
    if( used == 0 )
        return  0.f;

    float pack = ( used - LargestUsedChunk() ) / static_cast< float >( used );

    return  ( frag + pack ) / 2;
}

uint64
FFixedAllocArena::LowBlockAdress() const
{
    return  reinterpret_cast< uint64 >( mBlock );
}

uint64
FFixedAllocArena::HighBlockAdress() const
{
    return  reinterpret_cast< uint64 >( mBlock ) + BlockSize();
}

void
FFixedAllocArena::Print() const
{
    std::cout << "[";
    for( uint32 i = 0; i < mNumCells; ++i )
        if( IsChunkAvailable( Chunk( i ) ) )
            std::cout << "-";
        else
            std::cout << "+";
    std::cout << "] " << FMath::CeilToInt( LocalFragmentation() * 100 ) <<"%\n";
}

void
FFixedAllocArena::DefragSelf()
{
    uint32 lindex = 0;
    uint32 rindex = ULIS_UINT32_MAX;
    while( LargestFreeChunk() != mNumAvailableCells ) {
        uint8* metaBaseFull = LastFull( rindex, &rindex );
        uint8* metaBaseEmpty = FirstEmpty( lindex, &lindex );
        memcpy( metaBaseEmpty, metaBaseFull, mAllocSize + static_cast< uint64 >( smMetaPadSize ) );
        memset( metaBaseFull, 0, smMetaPadSize );
        uint8*** client_ptr = reinterpret_cast< uint8*** >( metaBaseEmpty );
        uint8** client = *client_ptr;
        uint8* data = metaBaseEmpty + smMetaPadSize;
        *client = data;
    }
}

uint32
FFixedAllocArena::LargestFreeChunk() const
{
    int run = 0;
    int max = 0;
    for( uint32 i = 0; i < mNumCells; ++i ) {
        if( IsChunkAvailable( Chunk( i ) ) ) {
            ++run;
        } else {
            run = 0;
        }
        max = FMath::Max( run, max );
    }
    return  max;
}

uint32
FFixedAllocArena::LargestUsedChunk() const
{
    int run = 0;
    int max = 0;
    for( uint32 i = 0; i < mNumCells; ++i ) {
        if( !IsChunkAvailable( Chunk( i ) ) ) {
            ++run;
        } else {
            run = 0;
        }
        max = FMath::Max( run, max );
    }
    return  max;
}

uint64
FFixedAllocArena::BlockSize() const
{
    return  static_cast< uint64 >( mArenaSize ) + static_cast< uint64 >( smMetaPadSize ) * static_cast< uint64 >( mNumCells );
}

uint8*
FFixedAllocArena::Chunk( uint32 iIndex )
{
    return  mBlock + static_cast< uint64 >( iIndex ) * ( mAllocSize + static_cast< uint64 >( smMetaPadSize ) );
}

const uint8*
FFixedAllocArena::Chunk( uint32 iIndex ) const
{
    return  mBlock + static_cast< uint64 >( iIndex ) * ( mAllocSize + static_cast< uint64 >( smMetaPadSize ) );
}

bool
FFixedAllocArena::IsChunkAvailable( const uint8* iChunk ) const
{
    return  !(*(const uint8***)(iChunk));
}

uint8*
FFixedAllocArena::FirstEmpty( uint32 iFrom, uint32* oIndex )
{
    for( uint32 i = iFrom; i < mNumCells; ++i ) {
        uint8* metaBase = Chunk( i );
        if( IsChunkAvailable( metaBase ) ) {
            if( oIndex )
                *oIndex = i;
            return  metaBase;
        }
    }
    return  nullptr;
}

uint8*
FFixedAllocArena::FirstFull( uint32 iFrom, uint32* oIndex )
{
    for( uint32 i = iFrom; i < mNumCells; ++i ) {
        uint8* metaBase = Chunk( i );
        if( !IsChunkAvailable( metaBase ) ) {
            if( oIndex )
                *oIndex = i;
            return  metaBase;
        }
    }
    return  nullptr;
}

uint8*
FFixedAllocArena::LastFull( uint32 iFrom, uint32* oIndex )
{
    for( int32 i = FMath::Min( iFrom, mNumCells - 1 ); i >= 0; --i ) {
        uint8* metaBase = Chunk( i );
        if( !IsChunkAvailable( metaBase ) ) {
            if( oIndex )
                *oIndex = i;
            return  metaBase;
        }
    }
    return  nullptr;
}

ULIS_NAMESPACE_END

