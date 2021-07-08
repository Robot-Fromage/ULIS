// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ShrinkableAllocMemoryPool.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for FShrinkableAllocMemoryPool.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Memory/ShrinkableAllocMemoryPool.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
FShrinkableAllocMemoryPool::~FShrinkableAllocMemoryPool()
{
    for( auto it : mArenaPool )
        delete it;
}

FShrinkableAllocMemoryPool::FShrinkableAllocMemoryPool(
      byte_t iArenaSize
    , byte_t iMaxAllocSize
    , byte_t iTargetMemoryUsage
    , ufloat iDefragThreshold
    , const FMemoryPoolPolicy& iPolicy
)
    : mArenaSize( iArenaSize )
    , mMaxAllocSize( iMaxAllocSize )
    , mDefragThreshold( FMath::Clamp( iDefragThreshold, 0.f, 1.f ) )
    , mPolicy( iPolicy )
{
    ULIS_ASSERT( mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mMaxAllocSize, "Bad Size !" );
    mTargetMemoryUsage = static_cast< uint64 >( FMath::Ceil( static_cast< double >( FMath::Max( uint64( iTargetMemoryUsage ), uint64( 1 ) ) ) / mArenaSize ) * mArenaSize );
    ULIS_ASSERT( mTargetMemoryUsage, "Bad Size !" );
    ULIS_ASSERT( mTargetMemoryUsage % mArenaSize == 0, "Bad Computation !" );
}

FShrinkableAllocMemoryPool::FShrinkableAllocMemoryPool(
      byte_t iMaxAllocSize
    , uint64 iNumCellPerArena
    , byte_t iTargetMemoryUsage
    , ufloat iDefragThreshold
    , const FMemoryPoolPolicy& iPolicy
)
    : mArenaSize(
          // Room for allocs
          uint64( iMaxAllocSize )
        * uint64( iNumCellPerArena )
        // Room for meta Bases, including begin sentinel
        + uint64( FShrinkableAllocArena::smMetaTotalPad )
        * uint64( iNumCellPerArena )
        // Room for end sentinel
        + uint64( FShrinkableAllocArena::smMetaTotalPad )
    )
    , mMaxAllocSize( iMaxAllocSize )
    , mDefragThreshold( FMath::Clamp( iDefragThreshold, 0.f, 1.f ) )
    , mPolicy( iPolicy )
{
    ULIS_ASSERT( mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mMaxAllocSize, "Bad Size !" );
    // Round target memory usage to a multiple of arena size to avoid ping pong effect on unmatched size.
    // E.g: alloc / dealloc one arena constantly because the actual usage is either greater or lesser but never exact.
    mTargetMemoryUsage = static_cast< uint64 >( FMath::Ceil( static_cast< double >( FMath::Max( uint64( iTargetMemoryUsage ), uint64( 1 ) ) ) / mArenaSize ) * mArenaSize );
    ULIS_ASSERT( mTargetMemoryUsage, "Bad Size !" );
    ULIS_ASSERT( mTargetMemoryUsage % mArenaSize == 0, "Bad Computation !" );
}

byte_t
FShrinkableAllocMemoryPool::ArenaSize() const
{
    return  mArenaSize;
}

byte_t
FShrinkableAllocMemoryPool::MaxAllocSize() const
{
    return  mMaxAllocSize;
}

byte_t
FShrinkableAllocMemoryPool::TotalMemory() const
{
    return  mArenaPool.size() * mArenaSize;
}

byte_t
FShrinkableAllocMemoryPool::FreeMemory() const
{
    uint64 sum = 0;
    for( auto it : mArenaPool )
        sum += uint64( it->FreeMemory() );
    return  sum;
}

byte_t
FShrinkableAllocMemoryPool::UsedMemory() const
{
    return  TotalMemory() - FreeMemory();
}

byte_t
FShrinkableAllocMemoryPool::TargetMemoryUsage() const
{
    return  mTargetMemoryUsage;
}

void
FShrinkableAllocMemoryPool::SetTargetMemoryUsage( byte_t iValue )
{
    mTargetMemoryUsage = static_cast< uint64 >( FMath::Ceil( static_cast< double >( FMath::Max( uint64( iValue ), uint64( 1 ) ) ) / mArenaSize ) * mArenaSize );
    ULIS_ASSERT( mTargetMemoryUsage, "Bad Size !" );
    ULIS_ASSERT( mTargetMemoryUsage % mArenaSize == 0, "Bad Computation !" );
}

FMemoryPoolPolicy&
FShrinkableAllocMemoryPool::MemoryPolicy() {
    return  mPolicy;
}

const FMemoryPoolPolicy&
FShrinkableAllocMemoryPool::MemoryPolicy() const {
    return  mPolicy;
}

float
FShrinkableAllocMemoryPool::DefragThreshold() const
{
    return  mDefragThreshold;
}

void
FShrinkableAllocMemoryPool::SetDefragThreshold( ufloat iValue )
{
    mDefragThreshold = FMath::Clamp( iValue, 0.f, 1.f );
}

ufloat
FShrinkableAllocMemoryPool::Fragmentation() const
{
    float sum = 0.f;
    for( auto it : mArenaPool )
        sum += it->LocalFragmentation();
    return  sum / mArenaPool.size();
}

void
FShrinkableAllocMemoryPool::DefragIfNecessary()
{
    if( Fragmentation() < mDefragThreshold )
        return;
    DefragForce();
}

void
FShrinkableAllocMemoryPool::DefragForce()
{
    if( mArenaPool.size() < 1 )
        return;

    // Sort: in decreasing frag rate:
    // High frag rates are at the beginning
    // Low frag rates are at the end
    // That way we minimize the moves and skip full.
    // High work Self Defrag in beginning
    // Low work Self Defrag in end
    mArenaPool.sort(
        []( FShrinkableAllocArena* iLhs, FShrinkableAllocArena* iRhs ) {
            return  iLhs->LocalFragmentation() < iRhs->LocalFragmentation();
        }
    );

    // Defrag Self All to make room
    for( auto it : mArenaPool ) {
        it->DefragSelfForce();
    }

    auto highArena = mArenaPool.begin();
    auto lowArena = --mArenaPool.end();
    FShrinkableAllocArena::FIterator src_it = FShrinkableAllocArena::FIterator::MakeNull();
    FShrinkableAllocArena::FIterator dst_it = FShrinkableAllocArena::FIterator::MakeNull();
    uint64 initialFreeBufferSize = FShrinkableAllocArena::InitialFreeMemory( mArenaSize );
    uint64 srcFree = (*lowArena)->FreeMemory();
    uint64 dstUsed = (*highArena)->UsedMemory();
}

tClient
FShrinkableAllocMemoryPool::Malloc( byte_t iSize )
{
    tClient alloc = nullptr;
    // Reverse iteration, because after a defrag, arenas with a lot of free space are likely to be at the end
    for ( auto it = mArenaPool.rbegin(); it != mArenaPool.rend(); ++it ) {
        alloc = (*it)->Malloc( iSize );
        if( alloc )
            return  alloc;
    }

    if( mPolicy.AllocPageIfOutOfSpace() ) {
        FShrinkableAllocArena* page = new FShrinkableAllocArena( byte_t( mArenaSize ), byte_t( mMaxAllocSize ) );
        mArenaPool.push_back( page );
        return  page->Malloc( iSize );
    }

    return  nullptr;
}

//static
void
FShrinkableAllocMemoryPool::Free( tClient iClient )
{
    FShrinkableAllocArena::Free( iClient );
}

void
FShrinkableAllocMemoryPool::UnsafeFreeAll()
{
    for( auto it : mArenaPool )
        it->UnsafeFreeAll();
}

bool
FShrinkableAllocMemoryPool::AllocOneArenaIfNecessary()
{
    if( TotalMemory() < TargetMemoryUsage() ) {
        mArenaPool.push_back( new FShrinkableAllocArena( byte_t( mArenaSize ), byte_t( mMaxAllocSize ) ) );
        return  true;
    }
    return  false;
}

bool
FShrinkableAllocMemoryPool::FreeOneArenaIfNecessary()
{
    if( TotalMemory() > TargetMemoryUsage() ) {
        for( auto it = mArenaPool.begin(); it != mArenaPool.end(); ++it ) {
            if( (*it)->IsEmpty() ) {
                delete (*it);
                mArenaPool.erase( it );
                return  true;
            }
        }
    }
    return  false;
}

uint64
FShrinkableAllocMemoryPool::AllocArenasToReachTargetMemory()
{
    uint32 count = 0;
    while( TotalMemory() < TargetMemoryUsage() ) {
        mArenaPool.push_back( new FShrinkableAllocArena( byte_t( mArenaSize ), byte_t( mMaxAllocSize ) ) );
        count++;
    }
    return  count;
}

uint64
FShrinkableAllocMemoryPool::FreeEmptyArenasToReachTargetMemory() {
    return  FreeEmptyArenasAccordingToPredicate( [this](){ return  TotalMemory() > TargetMemoryUsage(); } );
}

uint64
FShrinkableAllocMemoryPool::FreeAllEmptyArenas() {
    return  FreeEmptyArenasAccordingToPredicate( [](){ return  true; } );
}

uint64
FShrinkableAllocMemoryPool::FreeEmptyArenasAccordingToPredicate( std::function< bool() > iPredicate ) {
    std::list< FShrinkableAllocArena* > toDelete;
    for( auto it = mArenaPool.begin(); it != mArenaPool.end(); ++it ) {
        if( (*it)->IsEmpty() ) {
            toDelete.push_back( (*it) );
        }
    }

    uint64 count = static_cast< uint64 >( toDelete.size() );
    while( toDelete.size() && iPredicate() ) {
        delete (*(toDelete.begin()));
        toDelete.erase( toDelete.begin() );
    }
    return  count;
}

void
FShrinkableAllocMemoryPool::DebugPrint( int iType, int iCol ) const
{
    std::cout << "=== " << FMath::FloorToInt( Fragmentation() * 100 ) << "%\n";
    for( auto it : mArenaPool )
        it->DebugPrint( iType, iCol );
}

ULIS_NAMESPACE_END

