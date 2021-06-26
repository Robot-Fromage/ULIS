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
    ULIS_ASSERT( IsEmpty(), "Error, trying to delete a non empty arena !" );
    XFree( mBlock );
}

FFixedAllocArena::FFixedAllocArena(
      byte_t iArenaSize
    , byte_t iAllocSize
)
    : mArenaSize( iArenaSize )
    , mAllocSize( iAllocSize )
    , mBlock(
        reinterpret_cast< uint8* >( XMalloc( BlockSize() ) )
    )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mAllocSize > 0 && mAllocSize <= mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    InitializeCleanCellsMetaBase();
}

FFixedAllocArena::FFixedAllocArena(
      byte_t iAllocSize
    , uint32 iNumCells
)
    : mArenaSize(
          static_cast< uint64 >( iAllocSize )
        * static_cast< uint64 >( iNumCells )
    )
    , mAllocSize( iAllocSize )
    , mBlock(
        reinterpret_cast< uint8* >( XMalloc( BlockSize() ) )
    )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mAllocSize > 0 && mAllocSize <= mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    InitializeCleanCellsMetaBase();
}

bool
FFixedAllocArena::IsFull() const
{
    return  NumFreeCells() == 0;
}

bool
FFixedAllocArena::IsEmpty() const
{
    return  NumFreeCells() == NumCells();
}

bool
FFixedAllocArena::IsResident( tConstClient iClient ) const
{
    return  IsResident( *iClient );
}

//static
bool
FFixedAllocArena::IsFree( tConstClient iClient )
{
    return  IsFree( *iClient );
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
    return
        static_cast< uint32 >(
              static_cast< uint64 >( mArenaSize )
            / static_cast< uint64 >( mAllocSize )
        );
}

uint32
FFixedAllocArena::NumFreeCells() const
{
    return  0;
}

uint32
FFixedAllocArena::NumUsedCells() const
{
    return  NumCells() - NumFreeCells();
}

const uint8*
FFixedAllocArena::LowBlockAdress() const
{
    return  mBlock;
}

const uint8*
FFixedAllocArena::HighBlockAdress() const
{
    return  mBlock + BlockSize();
}

FFixedAllocArena::tClient
FFixedAllocArena::Malloc()
{
    for( uint32 i = 0; i < mNumCells; ++i ) {
        uint8* metaBase = ChunkMetaBase( i );
        if( IsCellMetaBaseFree( metaBase ) ) {
            // client alloc points to data
            uint8* data = metaBase + smMetaPadSize;
            uint8** client = new uint8*( data );

            // meta stores adress of client
            uint8*** client_ptr = reinterpret_cast< uint8*** >( metaBase );
            *client_ptr = client;
            mNumAvailableCells--;
            return  client;
        }
    }
    return  nullptr;
}

// static
void
FFixedAllocArena::Free( tClient iClient )
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

void
FFixedAllocArena::UnsafeFreeAll()
{
    uint8 const * const highAdress = HighBlockAdress();
    const uint64 cellPadding = CellPadding();
    for( tMetaBase metaBase = LowBlockAdress(); metaBase < highAdress; metaBase += cellPadding; ) {
        if( !IsCellMetaBaseFree( metaBase ) ) {
            delete reinterpret_cast< tClient >( metaBase );
            memset( metaBase, 0, smMetaPadSize );
        }
    }
}

float
FFixedAllocArena::OccupationRate() const
{
    return  NumUsedCells() / NumCells();
}

void
FFixedAllocArena::Print() const
{
    std::cout << "[";
    for( uint32 i = 0; i < mNumCells; ++i )
        if( IsCellMetaBaseFree( ChunkMetaBase( i ) ) )
            std::cout << "-";
        else
            std::cout << "+";
    std::cout << "] " << FMath::CeilToInt( OccupationRate() * 100 ) <<"%\n";
}

//static
bool
FFixedAllocArena::IsCellMetaBaseFree( tConstMetaBase iMetaBase )
{
    ULIS_ASSERT( iMetaBase, "Bad input" );
    return  *reinterpret_cast< tConstClient* >( iMetaBase ) == nullptr;
}

uint8*
FFixedAllocArena::LowBlockAdress()
{
    return  mBlock;
}

uint8*
FFixedAllocArena::HighBlockAdress()
{
    return  mBlock + BlockSize();
}

uint64
FFixedAllocArena::BlockSize() const
{
    return  mArenaSize + static_cast< uint64 >( smMetaPadSize ) * static_cast< uint64 >( NumCells() );
}

uint64
FFixedAllocArena::CellPadding() const
{
    return  static_cast< uint64 >( mAllocSize ) + static_cast< uint64 >( smMetaPadSize );
}

FFixedAllocArena::tMetaBase
FFixedAllocArena::FirstEmptyChunkMetaBase( uint32 iFrom, uint32* oIndex )
{
    for( uint32 i = iFrom; i < mNumCells; ++i ) {
        uint8* metaBase = ChunkMetaBase( i );
        if( IsCellMetaBaseFree( metaBase ) ) {
            if( oIndex )
                *oIndex = i;
            return  metaBase;
        }
    }
    return  nullptr;
}

FFixedAllocArena::tMetaBase
FFixedAllocArena::FirstFullChunkMetaBase( uint32 iFrom, uint32* oIndex )
{
    for( uint32 i = iFrom; i < mNumCells; ++i ) {
        uint8* metaBase = ChunkMetaBase( i );
        if( !IsCellMetaBaseFree( metaBase ) ) {
            if( oIndex )
                *oIndex = i;
            return  metaBase;
        }
    }
    return  nullptr;
}

FFixedAllocArena::tMetaBase
FFixedAllocArena::LastFullChunkMetaBase( uint32 iFrom, uint32* oIndex )
{
    for( int32 i = FMath::Min( iFrom, mNumCells - 1 ); i >= 0; --i ) {
        uint8* metaBase = ChunkMetaBase( i );
        if( !IsCellMetaBaseFree( metaBase ) ) {
            if( oIndex )
                *oIndex = i;
            return  metaBase;
        }
    }
    return  nullptr;
}

//static
void
FFixedAllocArena::MoveAlloc( tMetaBase iSrcMetaBase, tMetaBase iDstMetaBase, byte_t iAllocSize )
{
    ULIS_ASSERT( !IsCellMetaBaseFree( iSrcMetaBase ), "Bad move, source should not be free !" );
    ULIS_ASSERT( IsCellMetaBaseFree( iDstMetaBase ), "Bad move, destination should be free !" );
    memcpy( iDstMetaBase, iSrcMetaBase, static_cast< uint64 >( iAllocSize ) + static_cast< uint64 >( smMetaPadSize ) );
    memset( iSrcMetaBase, 0, smMetaPadSize );
    tClient* clientPtr = reinterpret_cast< tClient* >( iDstMetaBase );
    **clientPtr = iDstMetaBase + smMetaPadSize;
}

void
FFixedAllocArena::InitializeCleanCellsMetaBase()
{
    uint8 const * const highAdress = HighBlockAdress();
    const uint64 cellPadding = CellPadding();
    for( tMetaBase metaBase = LowBlockAdress(); metaBase < highAdress; metaBase += cellPadding; )
        memset( metaBase, 0, smMetaPadSize );
}

ULIS_NAMESPACE_END

