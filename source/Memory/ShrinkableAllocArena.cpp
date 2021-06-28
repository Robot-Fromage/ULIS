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
#include "Core/Constants.h"

ULIS_NAMESPACE_BEGIN
FShrinkableAllocArena::~FShrinkableAllocArena()
{
    ULIS_ASSERT( IsEmpty(), "Error, trying to delete a non empty arena !" );
    XFree( mBlock );
}

FShrinkableAllocArena::FShrinkableAllocArena(
      byte_t iArenaSize
    , byte_t iAllocSize
)
    : mArenaSize( iArenaSize )
    , mMaxAllocSize( iMaxAllocSize )
    , mBlock( reinterpret_cast< uint8* >( XMalloc( mArenaSize ) ) )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mMaxAllocSize > 0, "Bad Size !" );
    ULIS_ASSERT( static_cast< uint64 >( mMaxAllocSize ) + static_cast< uint64 >( smMetaTotalPadSize ) < static_cast< uint64 >( mArenaSize ), "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    memset( mBlock, 0, smMetaClientPadSize );
    *( uint32* )( mBlock + smMetaClientPadSize ) = 0;
    *( uint32* )( mBlock + smMetaClientPadSize + smMetaPrevDeltaPadSize ) = static_cast< uint32 >( mArenaSize ) - static_cast< uint32 >( smMetaTotalPadSize );
}

FShrinkableAllocArena::FShrinkableAllocArena(
      byte_t iAllocSize
    , uint32 iNumCells
)
    : mArenaSize(
          static_cast< uint64 >( iAllocSize )
        * static_cast< uint64 >( iNumCells )
        + static_cast< uint64 >( smMetaTotalPadSize )
        * static_cast< uint64 >( iNumCells )
    )
    , mAllocSize( iAllocSize )
    , mBlock(
        reinterpret_cast< uint8* >( XMalloc( BlockSize() ) )
    )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mMaxAllocSize > 0, "Bad Size !" );
    ULIS_ASSERT( static_cast< uint64 >( mMaxAllocSize ) + static_cast< uint64 >( smMetaTotalPadSize ) < static_cast< uint64 >( mArenaSize ), "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    memset( mBlock, 0, smMetaClientPadSize );
    *( uint32* )( mBlock + smMetaClientPadSize ) = 0;
    *( uint32* )( mBlock + smMetaClientPadSize + smMetaPrevDeltaPadSize ) = static_cast< uint32 >( mArenaSize ) - static_cast< uint32 >( smMetaTotalPadSize );
}

bool
FShrinkableAllocArena::IsFull() const
{
    return  FreeMemory() == 0;
}

bool
FShrinkableAllocArena::IsEmpty() const
{
    return
           *(tClient)mBlock == nullptr
        && *(uint32*)( mBlock + smMetaClientPadSize + smMetaPrevDeltaPadSize ) == mArenaSize - smMetaTotalPadSize;
}

bool
FShrinkableAllocArena::IsResident( tClient iClient ) const
{
    return  IsMetaBaseResident( ( *iClient ) - smMetaTotalPadSize );
}

byte_t
FShrinkableAllocArena::ArenaSize() const
{
    return  mArenaSize;
}

byte_t
FShrinkableAllocArena::MaxAllocSize() const
{
    return  mMaxAllocSize;
}

const uint8*
FShrinkableAllocArena::LowBlockAdress() const
{
    return  mBlock;
}

const uint8*
FShrinkableAllocArena::HighBlockAdress() const
{
    return  mBlock + BlockSize();
}

byte_t
FShrinkableAllocArena::TotalMemory() const
{
    return  ArenaSize();
}

byte_t
FShrinkableAllocArena::FreeMemory() const
{
    return  0;
}

byte_t
FShrinkableAllocArena::UsedMemory() const
{
    return  TotalMemory() - FreeMemory();
}

tClient
FShrinkableAllocArena::Malloc( byte_t iSize )
{
    ULIS_ASSERT( uint32( iSizeBytes ) > 0, "Cannot alloc 0 bytes" );
    tMetaBase metaBase = FirstEmptyMetaBaseMinAlloc( requestedSize );
    // No alloc available, return null client
    if( !metaBase ) return  nullptr;
    // Setup client
    tClient client = *(tClient*)metaBase = new tAlloc( metaBase + smMetaTotalPadSize );
    // sizes: actual chunk size, requested alloc size, remaining size
    uint32 sizes[3];
    sizes[0] = MetaBaseSize( metaBase );
    sizes[1] = FMath::Min( uint32( MaxAllocSize() ), uint32( iSize ) );
    sizes[2] = sizes[0] > sizes[1] + smMetaTotalPadSize ? sizes[0] - ( sizes[1] + smMetaTotalPadSize ) : 0;
    // insufficient rem size, nothing to do, embed rem size.
    if( !bool( sizes[2] ) ) return client;
    // sufficient rem size: setup requested size and notify following sectors
     // own req
    memcpy( metaBase + smMetaClientPadSize + smMetaPrevDeltaPadSize, &sizes[1], sizeof( uint32 ) );
    // penultimate free, guaranteed to be in buffer range
    memcpy( metaBase + smMetaTotalPadSize + sizes[1] + smMetaClientPadSize, &sizes[1], sizeof( uint32 ) * 2 );
    // ultimate followup, could exceed block bounds
    tMetaBase ult = metaBase + smMetaTotalPadSize + sizes[0];
    if( !IsMetaBaseResident( ult ) ) return client; // exceed, abort
    // in range, notify ultimate prev about the new rem free sector
    memcpy( ult + smMetaClientPadSize, &sizes[2], sizeof( uint32 ) );
    // done.
    return  client;
}

//static
void
FShrinkableAllocArena::Free( tClient iClient )
{
    ULIS_ASSERT( iClient, "Cannot free null client" );
    tMetaBase metaBase = ( *iClient ) - smMetaTotalPadSize;
    uint32 freeSize = MetaBaseSize(  metaBase );

    // Cleanup client
    tClient* client_ptr = reinterpret_cast< tClient* >( metaBase );
    tClient client = *client_ptr;
    delete client;
    *client_ptr = nullptr;
    mUsedMemory -= freeSize + smMetaTotalPadSize;

    uint8* followingMetaBase = AdvanceMetaBase( metaBase );
    if( IsMetaBaseAvailable( followingMetaBase ) ) {
        *((uint32*)(metaBase + smMetaClientPadSize)) += MetaBaseSize( followingMetaBase ) + smMetaTotalPadSize;
    }

    tMetaBase previousMetaBase = PreviousMetaBase( metaBase );
    if( previousMetaBase == nullptr ) // Beginning of block
        return;

    // Notify previous alloc that space is available if it is free ( merge free )
    if( IsMetaBaseAvailable( previousMetaBase ) )
        *((uint32*)(previousMetaBase + smMetaClientPadSize)) += static_cast< uint32 >( MetaBaseSize( metaBase ) + smMetaTotalPadSize );
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
FShrinkableAllocArena::Print( bool iShort, int iCol ) const
{
    if( iShort )
    {
        uint8* raw_buf = new uint8[mArenaSize];
        int index = 0;
        std::cout << "[";
        tMetaBase metaBase = mBlock;
        while( IsInRange( metaBase ) ) {
            bool avail = IsMetaBaseAvailable( metaBase );
            uint32 size = MetaBaseSize( metaBase );
            uint8 disp = avail ? 0 : 1;
            uint32 i = index;
            for( ; i < index + ( size + smMetaTotalPadSize ); ++i )
                raw_buf[i] = disp;
            index = i;
            metaBase = AdvanceMetaBase( metaBase );
        }

        int display_size = 100;
        char* display_buf = new char[display_size];
        for( int i = 0; i < display_size; ++i ) {
            int j = (i / float(display_size)) * mArenaSize;
            int k = (( i + 1 ) / float(display_size)) * mArenaSize;
            float delta = k-j;
            uint64 accum = 0;
            for( j; j<k; ++j) {
                accum += raw_buf[j];
            }
            float avg = (accum / delta);
            std::cout << ( avg > 0.5f ? char( 219 ) : ' ' );
        }
        std::cout << "] " << FMath::CeilToInt( LocalFragmentation() * 100 ) <<"%\n";
    }
    else
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

bool
FShrinkableAllocArena::Shrink( uint8* iAlloc, uint32 iNewSizeBytes )
{
    ULIS_ASSERT( !IsFree( iAlloc ), "Don't shrink free parts !" );
    ULIS_ASSERT( iNewSizeBytes, "New size cannot be 0" );
    tMetaBase metaBase = iAlloc - smMetaTotalPadSize;
    uint32 size = MetaBaseSize( metaBase );
    ULIS_ASSERT( iNewSizeBytes <= size, "New size cannot be greater than current size." );
    if( size > iNewSizeBytes + smMetaTotalPadSize ) {
        *((uint32*)(metaBase + smMetaClientPadSize)) = static_cast< uint32 >( iNewSizeBytes );
        uint8* nextMetaBase = AdvanceMetaBase( metaBase );
        *((uint8***)(nextMetaBase)) = nullptr;
        *((uint32*)(nextMetaBase + smMetaClientPadSize)) = static_cast< uint32 >( size - iNewSizeBytes - smMetaTotalPadSize );
        mUsedMemory -= ( size - iNewSizeBytes ) + static_cast< uint64 >( smMetaTotalPadSize );
        uint8* followingMetaBase = AdvanceMetaBase( nextMetaBase );
        if( IsMetaBaseAvailable( followingMetaBase ) ) {
            *((uint32*)(nextMetaBase + smMetaClientPadSize)) += MetaBaseSize( followingMetaBase ) + smMetaTotalPadSize;
        }
        return true;
    } else {
        return  false;
    }
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
    while( true ) {
        it = AdvanceMetaBase( it );
        if( it == iMetaBase )
            break;
        previousMetaBase = AdvanceMetaBase( previousMetaBase );
    }
    return  previousMetaBase;
}

//static
uint32
FShrinkableAllocArena::MetaBaseSize( const tMetaBase iMetaBase )
{
    return  *( (uint32*)( iMetaBase + smMetaClientPadSize ) );
}

ULIS_NAMESPACE_END

