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
namespace details {
static constexpr char sgBlockChar = char( 219 );
} // namespace details

FShrinkableAllocArena::~FShrinkableAllocArena()
{
    ULIS_ASSERT( IsEmpty(), "Error, trying to delete a non empty arena !" );
    XFree( mBlock );
}

FShrinkableAllocArena::FShrinkableAllocArena(
      byte_t iArenaSize
    , byte_t iMaxAllocSize
)
    : mArenaSize( iArenaSize )
    , mMaxAllocSize( iMaxAllocSize )
    , mBlock( reinterpret_cast< uint8* >( XMalloc( mArenaSize ) ) )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mMaxAllocSize > 0, "Bad Size !" );
    ULIS_ASSERT( static_cast< uint64 >( mMaxAllocSize ) + static_cast< uint64 >( smMetaTotalPadSize ) < static_cast< uint64 >( mArenaSize ), "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    uint32 initialFreeBufferSize = static_cast< uint32 >( mArenaSize ) - static_cast< uint32 >( smMetaTotalPadSize ) * 2;
    tMetaBase beginSentinel = mBlock;
    tMetaBase endSentinel = mBlock + initialFreeBufferSize;

    // Begin Sentinel
    memset( beginSentinel, 0, smMetaClientPadSize );
    *( uint32* )( beginSentinel + smMetaClientPadSize ) = 0;
    *( uint32* )( beginSentinel + smMetaClientPadSize + smMetaPrevDeltaPadSize ) = initialFreeBufferSize;

    // End Sentinel
    memset( endSentinel, 0, smMetaClientPadSize );
    *( uint32* )( endSentinel + smMetaClientPadSize ) = initialFreeBufferSize;
    *( uint32* )( endSentinel + smMetaClientPadSize + smMetaPrevDeltaPadSize ) = 0;
}

FShrinkableAllocArena::FShrinkableAllocArena(
      byte_t iMaxAllocSize
    , uint32 iNumCells
)
    : mArenaSize(
          static_cast< uint64 >( iMaxAllocSize )
        * static_cast< uint64 >( iNumCells )
        + static_cast< uint64 >( smMetaTotalPadSize )
        * static_cast< uint64 >( iNumCells )
    )
    , mMaxAllocSize( iMaxAllocSize )
    , mBlock(
        reinterpret_cast< uint8* >( XMalloc( ArenaSize() ) )
    )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mMaxAllocSize > 0, "Bad Size !" );
    ULIS_ASSERT( static_cast< uint64 >( mMaxAllocSize ) + static_cast< uint64 >( smMetaTotalPadSize ) < static_cast< uint64 >( mArenaSize ), "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    uint32 initialFreeBufferSize = static_cast< uint32 >( mArenaSize ) - static_cast< uint32 >( smMetaTotalPadSize ) * 2;
    tMetaBase beginSentinel = mBlock;
    tMetaBase endSentinel = mBlock + initialFreeBufferSize;

    // Begin Sentinel
    memset( beginSentinel, 0, smMetaClientPadSize );
    *( uint32* )( beginSentinel + smMetaClientPadSize ) = 0;
    *( uint32* )( beginSentinel + smMetaClientPadSize + smMetaPrevDeltaPadSize ) = initialFreeBufferSize;

    // End Sentinel
    memset( endSentinel, 0, smMetaClientPadSize );
    *( uint32* )( endSentinel + smMetaClientPadSize ) = initialFreeBufferSize;
    *( uint32* )( endSentinel + smMetaClientPadSize + smMetaPrevDeltaPadSize ) = 0;
}

bool
FShrinkableAllocArena::IsFull() const
{
    return  uint64( FreeMemory() ) == 0;
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
    return  byte_t( static_cast< double >( mArenaSize ) );
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
    return  mBlock + static_cast< uint64 >( ArenaSize() );
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
    ULIS_ASSERT( uint32( iSize ) > 0, "Cannot alloc 0 bytes" );
    uint32 req = FMath::Min( uint32( MaxAllocSize() ), uint32( iSize ) );
    tMetaBase metaBase = FirstEmptyMetaBaseMinAlloc( req );

    if( !metaBase )
        return  nullptr;

    tClient client = *(tClient*)metaBase = new tAlloc( metaBase + smMetaTotalPadSize );
    uint32 chunkSize = static_cast< uint32 >( MetaBaseSize( metaBase ) );
    uint32 rem = chunkSize > req + smMetaTotalPadSize ? chunkSize - ( req + smMetaTotalPadSize ) : 0;

    if( rem == 0 )
        return client;

    // sufficient rem size: setup requested size and notify following sectors
    *(uint32*)( metaBase + smMetaClientPadSize + smMetaPrevDeltaPadSize ) = req; // current meta next
    *(uint32*)( metaBase + smMetaClientPadSize + req + smMetaClientPadSize ) = req; // next meta prev
    *(uint32*)( metaBase + smMetaClientPadSize + req + smMetaClientPadSize + smMetaPrevDeltaPadSize ) = rem; // next meta next

    // notify following about rem free sector
    tMetaBase ult = metaBase + smMetaTotalPadSize + chunkSize;
    if( !IsMetaBaseResident( ult ) )
        return client;
    *(uint32*)( ult + smMetaClientPadSize ) = rem; // next next meta prev
    return  client;
}

//static
void
FShrinkableAllocArena::Free( tClient iClient )
{
    ULIS_ASSERT( iClient, "Cannot free null client" );
    tMetaBase metaBase = ( *iClient ) - smMetaTotalPadSize;
    tMetaBase prev = PrevMetaBase( metaBase );
    tMetaBase next = NextMetaBase( metaBase );
    memset( metaBase, 0, smMetaClientPadSize );
    delete iClient;

    // Next free, extend current free sector ( merge )
    if( IsMetaBaseFree( next ) )
        *(uint32*)( metaBase + smMetaClientPadSize + smMetaPrevDeltaPadSize ) +=
            static_cast< uint32 >(
                MetaBaseSize( metaBase ) + smMetaTotalPadSize
            );

    // Prev free, extend prev free sector ( merge )
    if( IsMetaBaseFree( prev ) )
        *(uint32*)( prev + smMetaClientPadSize + smMetaPrevDeltaPadSize ) +=
            static_cast< uint32 >(
                MetaBaseSize( metaBase ) + smMetaTotalPadSize
            );
}

//static
void
FShrinkableAllocArena::Shrink( tClient iClient, byte_t iNewSize )
{
    ULIS_ASSERT( iClient, "Cannot shrink null client" );

    tMetaBase metaBase = ( *iClient ) - smMetaTotalPadSize;
    ULIS_ASSERT( !IsMetaBaseFree( metaBase ), "Don't shrink free parts !" );

    uint32 req = iNewSize;
    ULIS_ASSERT( req > 0, "New size cannot be 0" );

    uint32 chunkSize = static_cast< uint32 >( MetaBaseSize( metaBase ) );
    ULIS_ASSERT( req <= chunkSize, "New size cannot be greater than current size." );

    uint32 rem = chunkSize > req + smMetaTotalPadSize ? chunkSize - ( req + smMetaTotalPadSize ) : 0;

    if( rem == 0 )
        return;

    *(uint32*)( metaBase + smMetaClientPadSize + smMetaPrevDeltaPadSize ) = req; // current meta next
    tMetaBase next = NextMetaBase( metaBase );

    memset( next, 0, smMetaClientPadSize );
    *(uint32*)( next + smMetaClientPadSize ) = req; // next meta prev
    *(uint32*)( next + smMetaClientPadSize + smMetaPrevDeltaPadSize ) = rem; // next meta next

    tMetaBase next2 = NextMetaBase( next );
    //TODO: Check, cannot modify last meta base resident
    //if( !IsMetaBaseResident( next2 ) )
    //    return;

    *(uint32*)( next2 + smMetaClientPadSize ) = rem; // next next meta prev
    if( IsMetaBaseFree( next2 ) )
        *(uint32*)( next + smMetaClientPadSize + smMetaPrevDeltaPadSize ) += rem + smMetaTotalPadSize;
}

float
FShrinkableAllocArena::LocalFragmentation() const
{
    tMetaBase metaBase = mBlock;
    uint32 runFree, maxFree, numFree, runUsed, maxUsed, totalFree, totalUsed;
    runFree = maxFree = numFree = runUsed = maxUsed = totalFree = totalUsed = 0;

    while( IsMetaBaseResident( metaBase ) ) {
        uint32 size = static_cast< uint32 >( MetaBaseSize( metaBase ) );
        if( IsMetaBaseFree( metaBase ) ) {
            runFree += size;
            totalFree += size;
            runUsed = 0;
            numFree++;
        } else {
            runUsed += size + static_cast< uint64 >( smMetaTotalPadSize );
            totalUsed += size + static_cast< uint64 >( smMetaTotalPadSize );
            runFree = 0;
        }
        maxFree = FMath::Max( runFree, maxFree );
        maxUsed = FMath::Max( runUsed, maxUsed );
        metaBase = NextMetaBase( metaBase );
    }
    maxUsed += numFree * smMetaTotalPadSize;
    totalUsed += numFree * smMetaTotalPadSize;

    if( totalFree == 0 )
        return  0.f;

    float frag = ( totalFree - maxFree ) / static_cast< float >( totalFree );
    float pack = ( totalUsed - maxUsed ) / static_cast< float >( totalUsed );
    return  ( frag + pack ) / 2;
}

void
FShrinkableAllocArena::DefragSelfForce()
{
    // Align all allocs to the left of the buffer.
    // Use memmove to avoid overlapping self sector.
    // pass contiguous blocks.
    // first find a src ( first empty after contiguous used ).
    // then find a dst ( first [next] used after src. ).
    tMetaBase metaBase = mBlock;
    tMetaBase src = nullptr;
    tMetaBase dst = nullptr;
    while( IsMetaBaseResident( metaBase ) ) {
        bool bFree = IsMetaBaseFree( metaBase );
        if( bFree ) {
            dst = metaBase;
            src = metaBase;
            uint32 dstChunkSize = *(uint32*)( dst + smMetaClientPadSize + smMetaPrevDeltaPadSize );
            uint32 ctgRun = 0;
            while( IsMetaBaseResident( src ) ) {
                ctgRun += static_cast< uint32 >( MetaBaseSize( src ) ) + static_cast< uint32 >( smMetaTotalPadSize );
                src = NextMetaBase( src );
            }
            memmove( dst, src, ctgRun + smMetaTotalPadSize );
            *(uint32*)( src + smMetaClientPadSize + smMetaPrevDeltaPadSize ) += dstChunkSize;
            metaBase = src;
        } else {
            metaBase = NextMetaBase( metaBase );
        }
    }
}

void
FShrinkableAllocArena::DebugPrint( bool iShort, int iCol ) const
{
    if( iShort )
    {
        // Print short version, scaled down with rough estimate of sector status.
        std::cout << "[";
        uint8* raw_buf = new uint8[mArenaSize];
        int index = 0;
        tMetaBase metaBase = mBlock;
        while( IsMetaBaseResident( metaBase ) ) {
            bool bFree = IsMetaBaseFree( metaBase );
            uint32 size = MetaBaseSize( metaBase );
            uint32 i = index;
            for( ; i < index + ( size + smMetaTotalPadSize ); ++i )
                raw_buf[i] = bFree;
            index = i;
            metaBase = NextMetaBase( metaBase );
        }

        char* display_buf = new char[iCol];
        for( int i = 0; i < iCol; ++i ) {
            int j = ( i / float( iCol ) ) * mArenaSize;
            int k = ( ( i + 1 ) / float( iCol ) ) * mArenaSize;
            float delta = k-j;
            uint64 sum = 0;
            for( j; j<k; ++j) {
                sum += raw_buf[j];
            }
            std::cout << ( ( sum / delta ) > 0.5f ? details::sgBlockChar : ' ' );
        }
        std::cout << "] " << LocalFragmentation() * 100 <<"%\n";
    }
    else
    {
        // Print long version with all octets and initial sector status char.
        std::cout << "[";
        tMetaBase metaBase = mBlock;
        while( IsMetaBaseResident( metaBase ) ) {
            bool bFree = IsMetaBaseFree( metaBase );
            uint32 size = MetaBaseSize( metaBase );
            char disp   = bFree ? '-' : '+';
            char beg    = bFree ? '0' : '@';
            std::cout << beg;
            for( uint32 i = 0; i < size + smMetaTotalPadSize - 1; ++i )
                std::cout << disp;
            metaBase = NextMetaBase( metaBase );
        }
        std::cout << "] " << LocalFragmentation() * 100 <<"%\n";
    }
}

bool
FShrinkableAllocArena::IsMetaBaseResident( tMetaBase iMetaBase ) const
{
    return  iMetaBase >= LowBlockAdress() && iMetaBase < HighBlockAdress();
}

//static
bool
FShrinkableAllocArena::IsMetaBaseFree( tMetaBase iMetaBase )
{
    ULIS_ASSERT( iMetaBase, "Bad input" );
    return  *reinterpret_cast< tClient* >( iMetaBase ) == nullptr;
}

//static
bool
FShrinkableAllocArena::IsBeginSentinel( tMetaBase iMetaBase )
{
    return  *(uint32*)( iMetaBase + smMetaClientPadSize ) == 0;
}

//static
bool
FShrinkableAllocArena::IsEndSentinel( tMetaBase iMetaBase )
{
    return  *(uint32*)( iMetaBase + smMetaClientPadSize + smMetaPrevDeltaPadSize ) == 0;
}


tMetaBase
FShrinkableAllocArena::FirstEmptyMetaBaseMinAlloc( byte_t iMinimumSizeBytes, tMetaBase iFrom )
{
    uint32 size = FMath::Min( MaxAllocSize(), iMinimumSizeBytes );
    ULIS_ASSERT( size > 0, "Cannot alloc 0 bytes" );

    tMetaBase metaBase = iFrom ? iFrom : mBlock;
    ULIS_ASSERT( IsMetaBaseResident( metaBase ), "From param not in range" );

    while( true ) {
        if( IsMetaBaseFree( metaBase ) && MetaBaseSize( metaBase ) >= size )
            break;
        metaBase = NextMetaBase( metaBase );
    }

    return  IsMetaBaseResident( metaBase ) ? metaBase : nullptr;
}

//static
tMetaBase
FShrinkableAllocArena::NextMetaBase( const tMetaBase iMetaBase )
{
    uint32 size = *(uint32*)( iMetaBase + smMetaClientPadSize + smMetaPrevDeltaPadSize );
    return  size == 0 ? nullptr : iMetaBase + smMetaTotalPadSize + size;
}

//static
tMetaBase
FShrinkableAllocArena::PrevMetaBase( const tMetaBase iMetaBase )
{
    uint32 size = *(uint32*)( iMetaBase + smMetaClientPadSize );
    return  size == 0 ? nullptr : iMetaBase - smMetaTotalPadSize - size;
}

//static
uint64
FShrinkableAllocArena::MetaBaseSize( const tMetaBase iMetaBase )
{
    return  static_cast< uint64 >( *(uint32*)( iMetaBase + smMetaClientPadSize + smMetaPrevDeltaPadSize ) );
}

ULIS_NAMESPACE_END

