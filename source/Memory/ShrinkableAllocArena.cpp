// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ShrinkableAllocArena.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for ShrinkableAllocArena.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Memory/ShrinkableAllocArena.h"
#include "Memory/Memory.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
FShrinkableAllocArena::~FShrinkableAllocArena()
{
    //ULIS_ASSERT( IsEmpty(), "Error, trying to delete a non empty arena !" );
    for( uint32 i = 0; i < mNumCells; ++i ) {
        uint8* metaBase = ChunkMetaBase( i );
        if( !IsChunkMetaBaseAvailable( metaBase ) ) {
            uint8*** client_ptr = reinterpret_cast< uint8*** >( metaBase );
            uint8** client = *client_ptr;
            delete client;
            client_ptr = nullptr;
            mNumAvailableCells++;
        }
    }
    XFree( mBlock );
}

FShrinkableAllocArena::FShrinkableAllocArena(
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
FShrinkableAllocArena::IsFull() const
{
    return  mNumAvailableCells == 0;
}

bool
FShrinkableAllocArena::IsEmpty() const
{
    return  mNumAvailableCells == mNumCells;
}

bool
FShrinkableAllocArena::IsInRange( const uint8* iAlloc ) const
{
    uint64 adress = reinterpret_cast< uint64 >( iAlloc );
    return  adress >= LowBlockAdress() && adress < HighBlockAdress();
}

uint64
FShrinkableAllocArena::ArenaSize() const
{
    return  mArenaSize;
}

uint32
FShrinkableAllocArena::AllocSize() const
{
    return  mAllocSize;
}

uint32
FShrinkableAllocArena::NumCells() const
{
    return  mNumCells;
}

uint32
FShrinkableAllocArena::NumAvailableCells() const
{
    return  mNumAvailableCells;
}

uint32
FShrinkableAllocArena::NumUsedCells() const
{
    return  mNumCells - mNumAvailableCells;
}

FShrinkableAllocArena::tClient
FShrinkableAllocArena::Malloc()
{
    for( uint32 i = 0; i < mNumCells; ++i ) {
        uint8* metaBase = ChunkMetaBase( i );
        if( IsChunkMetaBaseAvailable( metaBase ) ) {
            // client alloc points to data
            uint8* data = metaBase + smMetaPadSize;
            uint8** client = new uint8*( data );
            ULIS_ASSERT( *client == data, "Bad" );

            // meta stores adress of client
            uint8*** client_ptr = reinterpret_cast< uint8*** >( metaBase );
            *client_ptr = client;
            mNumAvailableCells--;
            return  client;
        }
    }
    return  nullptr;
}

void
FShrinkableAllocArena::Free( tClient iClient )
{
    ULIS_ASSERT( iClient, "Cannot free null client" );
    uint8* alloc = *iClient;
    ULIS_ASSERT( alloc, "Cannot free null alloc" );
    ULIS_ASSERT( !IsFree( alloc ), "Already Free" );
    uint8* metaBase = alloc - smMetaPadSize;
    ULIS_ASSERT( IsInRange( metaBase ), "Bad alloc, not in this arena !" );

    // Cleanup client
    tClient* client_ptr = reinterpret_cast< tClient* >( metaBase );
    tClient client = *client_ptr;
    delete client;
    *client_ptr = nullptr;
    mNumAvailableCells++;
}

float
FShrinkableAllocArena::LocalFragmentation() const
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
FShrinkableAllocArena::LowBlockAdress() const
{
    return  reinterpret_cast< uint64 >( mBlock );
}

uint64
FShrinkableAllocArena::HighBlockAdress() const
{
    return  reinterpret_cast< uint64 >( mBlock ) + BlockSize();
}

void
FShrinkableAllocArena::Print() const
{
    std::cout << "[";
    for( uint32 i = 0; i < mNumCells; ++i )
        if( IsChunkMetaBaseAvailable( ChunkMetaBase( i ) ) )
            std::cout << "-";
        else
            std::cout << "+";
    std::cout << "] " << FMath::CeilToInt( LocalFragmentation() * 100 ) <<"%\n";
}

void
FShrinkableAllocArena::DefragSelf()
{
    uint32 lindex = 0;
    uint32 rindex = ULIS_UINT32_MAX;
    while( LargestFreeChunk() != mNumAvailableCells )
        Swap( LastFullChunkMetaBase( rindex, &rindex ), FirstEmptyChunkMetaBase( lindex, &lindex ), mAllocSize );
}

//static
bool
FShrinkableAllocArena::IsFree( const uint8* iAlloc )
{
    return  IsChunkMetaBaseAvailable( iAlloc - smMetaPadSize ); 
}

//static
bool
FShrinkableAllocArena::IsFree( const tClient iClient )
{
    return  IsFree( *iClient );
}

//static
void
FShrinkableAllocArena::Swap( uint8* iFromMetaBase, uint8* iToMetaBase, uint32 iAllocSize )
{
    ULIS_ASSERT( !IsChunkMetaBaseAvailable( iFromMetaBase ), "Bad swap, iFrom should be unavailable !" );
    ULIS_ASSERT( IsChunkMetaBaseAvailable( iToMetaBase ), "Bad swap, iTo should be available !" );
    memcpy( iToMetaBase, iFromMetaBase, iAllocSize + static_cast< uint64 >( smMetaPadSize ) );
    memset( iFromMetaBase, 0, smMetaPadSize );
    uint8*** client_ptr = reinterpret_cast< uint8*** >( iToMetaBase );
    uint8** client = *client_ptr;
    uint8* data = iToMetaBase + smMetaPadSize;
    *client = data;
}

uint32
FShrinkableAllocArena::LargestFreeChunk() const
{
    int run = 0;
    int max = 0;
    for( uint32 i = 0; i < mNumCells; ++i ) {
        if( IsChunkMetaBaseAvailable( ChunkMetaBase( i ) ) ) {
            ++run;
        } else {
            run = 0;
        }
        max = FMath::Max( run, max );
    }
    return  max;
}

uint32
FShrinkableAllocArena::LargestUsedChunk() const
{
    int run = 0;
    int max = 0;
    for( uint32 i = 0; i < mNumCells; ++i ) {
        if( !IsChunkMetaBaseAvailable( ChunkMetaBase( i ) ) ) {
            ++run;
        } else {
            run = 0;
        }
        max = FMath::Max( run, max );
    }
    return  max;
}

uint64
FShrinkableAllocArena::BlockSize() const
{
    return  static_cast< uint64 >( mArenaSize ) + static_cast< uint64 >( smMetaPadSize ) * static_cast< uint64 >( mNumCells );
}

uint8*
FShrinkableAllocArena::ChunkMetaBase( uint32 iIndex )
{
    return  mBlock + static_cast< uint64 >( iIndex ) * ( mAllocSize + static_cast< uint64 >( smMetaPadSize ) );
}

const uint8*
FShrinkableAllocArena::ChunkMetaBase( uint32 iIndex ) const
{
    return  mBlock + static_cast< uint64 >( iIndex ) * ( mAllocSize + static_cast< uint64 >( smMetaPadSize ) );
}

//static
bool
FShrinkableAllocArena::IsChunkMetaBaseAvailable( const uint8* iChunk )
{
    ULIS_ASSERT( iChunk, "Bad input" );
    return  !(*(const uint8***)(iChunk));
}

uint8*
FShrinkableAllocArena::FirstEmptyChunkMetaBase( uint32 iFrom, uint32* oIndex )
{
    for( uint32 i = iFrom; i < mNumCells; ++i ) {
        uint8* metaBase = ChunkMetaBase( i );
        if( IsChunkMetaBaseAvailable( metaBase ) ) {
            if( oIndex )
                *oIndex = i;
            return  metaBase;
        }
    }
    return  nullptr;
}

uint8*
FShrinkableAllocArena::FirstFullChunkMetaBase( uint32 iFrom, uint32* oIndex )
{
    for( uint32 i = iFrom; i < mNumCells; ++i ) {
        uint8* metaBase = ChunkMetaBase( i );
        if( !IsChunkMetaBaseAvailable( metaBase ) ) {
            if( oIndex )
                *oIndex = i;
            return  metaBase;
        }
    }
    return  nullptr;
}

uint8*
FShrinkableAllocArena::LastFullChunkMetaBase( uint32 iFrom, uint32* oIndex )
{
    for( int32 i = FMath::Min( iFrom, mNumCells - 1 ); i >= 0; --i ) {
        uint8* metaBase = ChunkMetaBase( i );
        if( !IsChunkMetaBaseAvailable( metaBase ) ) {
            if( oIndex )
                *oIndex = i;
            return  metaBase;
        }
    }
    return  nullptr;
}

ULIS_NAMESPACE_END

