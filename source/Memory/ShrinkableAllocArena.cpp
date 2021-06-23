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
    XFree( mBlock );
}

FShrinkableAllocArena::FShrinkableAllocArena(
      uint64 iArenaSize
    , uint32 iMaxAllocSize
)
    : mArenaSize( iArenaSize )
    , mMaxAllocSize( iMaxAllocSize )
    , mUsedMemory( smMetaTotalPadSize )
    , mBlock( reinterpret_cast< uint8* >( XMalloc( mArenaSize ) ) )
{
    ULIS_ASSERT( mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mMaxAllocSize, "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    *((uint8***)(mBlock)) = nullptr;
    *((uint32*)(mBlock + smMetaClientPadSize)) = static_cast< uint32 >( mArenaSize - smMetaTotalPadSize );
}

bool
FShrinkableAllocArena::IsEmpty() const
{
    return  *((uint8***)(mBlock)) == nullptr && *((uint32*)(mBlock + smMetaClientPadSize)) == mArenaSize - smMetaTotalPadSize;
}

bool
FShrinkableAllocArena::IsInRange( const uint8* iPtr ) const
{
    uint64 adress = reinterpret_cast< uint64 >( iPtr );
    return  adress >= LowBlockAdress() && adress < HighBlockAdress();
}

uint64
FShrinkableAllocArena::ArenaSize() const
{
    return  mArenaSize;
}

uint32
FShrinkableAllocArena::MaxAllocSize() const
{
    return  mMaxAllocSize;
}

uint64
FShrinkableAllocArena::AvailableMemory() const
{
    return  mArenaSize - mUsedMemory;
}

uint64
FShrinkableAllocArena::UsedMemory() const
{
    return  mUsedMemory;
}

FShrinkableAllocArena::tClient
FShrinkableAllocArena::Malloc( uint32 iSizeBytes )
{
    ULIS_ASSERT( iSizeBytes > 0, "Cannot alloc 0 bytes" );
    uint32 size = FMath::Min( MaxAllocSize(), iSizeBytes );
    uint8* metaBase = FirstEmptyMetaBaseMinAlloc( size );
    *((uint8***)(mBlock)) = nullptr;
    *((uint32*)(mBlock + smMetaClientPadSize)) = static_cast< uint32 >( mArenaSize - smMetaTotalPadSize );
    return  nullptr;
}

void
FShrinkableAllocArena::Free( tClient iClient )
{
    ULIS_ASSERT( iClient, "Cannot free null client" );
    uint8* alloc = *iClient;
    ULIS_ASSERT( alloc, "Cannot free null alloc" );
    /*
    ULIS_ASSERT( !IsFree( alloc ), "Already Free" );
    uint8* metaBase = alloc - smMetaPadSize;
    ULIS_ASSERT( IsInRange( metaBase ), "Bad alloc, not in this arena !" );

    // Cleanup client
    tClient* client_ptr = reinterpret_cast< tClient* >( metaBase );
    tClient client = *client_ptr;
    delete client;
    *client_ptr = nullptr;
    mNumAvailableCells++;
    */
}

float
FShrinkableAllocArena::LocalFragmentation() const
{
    return  0.5f;
}

uint64
FShrinkableAllocArena::LowBlockAdress() const
{
    return  reinterpret_cast< uint64 >( mBlock );
}

uint64
FShrinkableAllocArena::HighBlockAdress() const
{
    return  reinterpret_cast< uint64 >( mBlock ) + mArenaSize;
}

void
FShrinkableAllocArena::Print() const
{
    std::cout << "[";
    //for( uint32 i = 0; i < mNumCells; ++i )
    //    if( IsChunkMetaBaseAvailable( ChunkMetaBase( i ) ) )
    //        std::cout << "-";
    //    else
    //        std::cout << "+";
    std::cout << "] " << FMath::CeilToInt( LocalFragmentation() * 100 ) <<"%\n";
}

void
FShrinkableAllocArena::DefragSelf()
{
    //uint32 lindex = 0;
    //uint32 rindex = ULIS_UINT32_MAX;
    //while( LargestFreeChunk() != mNumAvailableCells )
    //    Swap( LastFullChunkMetaBase( rindex, &rindex ), FirstEmptyChunkMetaBase( lindex, &lindex ), mAllocSize );
}

//static
bool
FShrinkableAllocArena::IsFree( const uint8* iAlloc )
{
    return  IsMetaBaseAvailable( iAlloc - smMetaTotalPadSize );
}

//static
bool
FShrinkableAllocArena::IsFree( const uint8** iClient )
{
    return  IsFree( *iClient );
}

//static
bool
FShrinkableAllocArena::IsMetaBaseAvailable( const uint8* iChunk )
{
    ULIS_ASSERT( iChunk, "Bad input" );
    return  ((const uint8**)(iChunk));
}

FShrinkableAllocArena::tMetaBase
FShrinkableAllocArena::FirstEmptyMetaBaseMinAlloc( uint32 iMinimumSizeBytes, tMetaBase iFrom )
{
    ULIS_ASSERT( iMinimumSizeBytes > 0, "Cannot alloc 0 bytes" );
    ULIS_ASSERT( IsInRange( iFrom ), "From param not in range" );
    uint32 size = FMath::Min( MaxAllocSize(), iMinimumSizeBytes );
    tMetaBase metaBase = iFrom ? iFrom : mBlock;
    while( !IsMetaBaseAvailable( metaBase ) && MetaBaseSize( metaBase ) < size ) {
        metaBase = AdvanceMetaBase( metaBase );
    }
    if( IsInRange( metaBase ) )
        return  metaBase;
    return  nullptr;
}

//static
FShrinkableAllocArena::tMetaBase
FShrinkableAllocArena::AdvanceMetaBase( const tMetaBase iMetaBase )
{
    return  iMetaBase + smMetaClientPadSize + MetaBaseSize( iMetaBase );
}

//static
uint32
FShrinkableAllocArena::MetaBaseSize( const tMetaBase iMetaBase )
{
    return  *( (uint32*)( iMetaBase + smMetaClientPadSize ) );
}

ULIS_NAMESPACE_END

