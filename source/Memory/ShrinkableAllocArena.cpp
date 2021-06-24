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
    tMetaBase metaBase = mBlock;
    while( IsInRange( metaBase ) ) {
        if( !IsMetaBaseAvailable( metaBase ) ) {
            tClient* client_ptr = reinterpret_cast< tClient* >( metaBase );
            tClient client = *client_ptr;
            delete client;
            *client_ptr = nullptr;
        }
        metaBase = AdvanceMetaBase( metaBase );
    }
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
    ULIS_ASSERT( mMaxAllocSize + static_cast< uint64 >( smMetaTotalPadSize ) < mArenaSize, "Bad Size !" );
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
    uint32 requestedSize = FMath::Min( MaxAllocSize(), iSizeBytes );
    uint8* metaBase = FirstEmptyMetaBaseMinAlloc( requestedSize );
    if( !metaBase )
        return  nullptr;
    // Easy case
    // [••][----------------------]
    // [••][+++++++++[••]---------]
    // Harder case
    // [••][-----------[••][++++++]
    // [••][++++++++++][••][++++++]
    // client alloc points to data
    uint8* data = metaBase + smMetaTotalPadSize;
    uint8** client = new uint8*( data );
    *((uint8***)(metaBase)) = client;
    uint32 actualChunkSize = MetaBaseSize( metaBase );
    if( actualChunkSize > requestedSize + smMetaTotalPadSize ) {
        *((uint32*)(metaBase + smMetaClientPadSize)) = static_cast< uint32 >( requestedSize );
        uint8* nextMetaBase = AdvanceMetaBase( metaBase );
        *((uint8***)(nextMetaBase)) = nullptr;
        *((uint32*)(nextMetaBase + smMetaClientPadSize)) = static_cast< uint32 >( actualChunkSize - requestedSize - smMetaTotalPadSize );
        mUsedMemory += requestedSize + static_cast< uint64 >( smMetaTotalPadSize );
    } else {
        *((uint32*)(metaBase + smMetaClientPadSize)) = static_cast< uint32 >( actualChunkSize );
        mUsedMemory += actualChunkSize;
    }
    return  client;
}

void
FShrinkableAllocArena::Free( tClient iClient )
{
    ULIS_ASSERT( iClient, "Cannot free null client" );
    uint8* alloc = *iClient;
    ULIS_ASSERT( alloc, "Cannot free null alloc" );
    ULIS_ASSERT( !IsFree( alloc ), "Already Free" );
    uint8* metaBase = alloc - smMetaTotalPadSize;
    ULIS_ASSERT( IsInRange( metaBase ), "Bad alloc, not in this arena !" );
    uint32 freeSize = MetaBaseSize(  metaBase );

    // Cleanup client
    tClient* client_ptr = reinterpret_cast< tClient* >( metaBase );
    tClient client = *client_ptr;
    delete client;
    *client_ptr = nullptr;
    mUsedMemory -= freeSize;
    tMetaBase previousMetaBase = PreviousMetaBase( metaBase );
    if( previousMetaBase == nullptr ) // Beginning of block
        return;

    // Notify previous alloc that space is available if it is free ( merge free )
    if( IsMetaBaseAvailable( previousMetaBase ) )
        *((uint32*)(previousMetaBase + smMetaClientPadSize)) += static_cast< uint32 >( freeSize + smMetaTotalPadSize );
}

float
FShrinkableAllocArena::LocalFragmentation() const
{
    tMetaBase metaBase = mBlock;
    uint32 runFree = 0;
    uint32 maxFree = 0;
    uint32 numFree = 0;
    uint32 runUsed = 0;
    uint32 maxUsed = 0;
    while( IsInRange( metaBase ) ) {
        bool avail = IsMetaBaseAvailable( metaBase );
        uint32 size = MetaBaseSize( metaBase );
        if( avail ) {
            runFree += size;
            runUsed = 0;
            numFree++;
        } else {
            runUsed += size + static_cast< uint64 >( smMetaTotalPadSize );
            runFree = 0;
        }
        maxFree = FMath::Max( runFree, maxFree );
        maxUsed = FMath::Max( runUsed, maxUsed );
        metaBase = AdvanceMetaBase( metaBase );
    }
    maxUsed = static_cast< uint32 >( FMath::Min( mUsedMemory, maxUsed + numFree * static_cast< uint64 >( smMetaTotalPadSize ) ) );

    if( AvailableMemory() == 0 )
        return  0.f;

    float frag = ( AvailableMemory() - maxFree ) / static_cast< float >( AvailableMemory() );

    if( UsedMemory() == 0 )
        return  0.f;

    float pack = ( UsedMemory() - maxUsed ) / static_cast< float >( UsedMemory() );

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
    return  reinterpret_cast< uint64 >( mBlock ) + mArenaSize;
}

void
FShrinkableAllocArena::Print() const
{
    std::cout << "[";
    tMetaBase metaBase = mBlock;
    while( IsInRange( metaBase ) ) {
        bool avail = IsMetaBaseAvailable( metaBase );
        uint32 size = MetaBaseSize( metaBase );
        char disp = avail ? '-' : '+';
        char beg = avail ? '0' : '@';
        std::cout << beg;
        for( uint32 i = 0; i < size + smMetaTotalPadSize - 1; ++i )
            std::cout << disp;
        metaBase = AdvanceMetaBase( metaBase );
    }
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
    return  !*((const uint8***)(iChunk));
}

FShrinkableAllocArena::tMetaBase
FShrinkableAllocArena::FirstEmptyMetaBaseMinAlloc( uint32 iMinimumSizeBytes, tMetaBase iFrom )
{
    ULIS_ASSERT( iMinimumSizeBytes > 0, "Cannot alloc 0 bytes" );
    tMetaBase metaBase = iFrom ? iFrom : mBlock;
    ULIS_ASSERT( IsInRange( metaBase ), "From param not in range" );
    uint32 size = FMath::Min( MaxAllocSize(), iMinimumSizeBytes );
    while( true ) {
        if( IsMetaBaseAvailable( metaBase ) && MetaBaseSize( metaBase ) >= size )
            break;
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
    return  iMetaBase + smMetaTotalPadSize + MetaBaseSize( iMetaBase );
}

FShrinkableAllocArena::tMetaBase
FShrinkableAllocArena::PreviousMetaBase( const tMetaBase iMetaBase )
{
    ULIS_ASSERT( IsInRange( iMetaBase ), "From param not in range" );
    if( iMetaBase == mBlock )
        return  nullptr;

    tMetaBase previousMetaBase = mBlock;
    tMetaBase it = mBlock;
    while( ( it = AdvanceMetaBase( it ) ) != iMetaBase )
        previousMetaBase = AdvanceMetaBase( previousMetaBase );
    return  previousMetaBase;
}

//static
uint32
FShrinkableAllocArena::MetaBaseSize( const tMetaBase iMetaBase )
{
    return  *( (uint32*)( iMetaBase + smMetaClientPadSize ) );
}

ULIS_NAMESPACE_END

