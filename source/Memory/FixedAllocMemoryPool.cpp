// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FixedAllocMemoryPool.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for FFixedAllocMemoryPool.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Memory/FixedAllocMemoryPool.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
FFixedAllocMemoryPool::~FFixedAllocMemoryPool()
{
    for( auto it : mArenaPool )
        delete it;
}

FFixedAllocMemoryPool::FFixedAllocMemoryPool(
      byte_t iArenaSize
    , byte_t iAllocSize
    , byte_t iTargetMemoryUsage
    , ufloat iDefragThreshold
    , const FMemoryPoolPolicy& iPolicy
)
    : mArenaSize( iArenaSize )
    , mAllocSize( iAllocSize )
    , mDefragThreshold( FMath::Clamp( iDefragThreshold, 0.f, 1.f ) )
    , mPolicy( iPolicy )
{
    ULIS_ASSERT( mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mAllocSize, "Bad Size !" );
    // Round target memory usage to a multiple of arena size to avoid ping pong effect on unmatched size.
    // E.g: alloc / dealloc one arena constantly because the actual usage is either greater or lesser but never exact.
    mTargetMemoryUsage = static_cast< uint64 >( FMath::Ceil( static_cast< double >( FMath::Max( uint64( iTargetMemoryUsage ), uint64( 1 ) ) ) / mArenaSize ) * mArenaSize );
    ULIS_ASSERT( mTargetMemoryUsage, "Bad Size !" );
    ULIS_ASSERT( mTargetMemoryUsage % mArenaSize == 0, "Bad Computation !" );
}

FFixedAllocMemoryPool::FFixedAllocMemoryPool(
      byte_t iAllocSize
    , uint64 iNumCellPerArena
    , byte_t iTargetMemoryUsage
    , ufloat iDefragThreshold
    , const FMemoryPoolPolicy& iPolicy
)
    : mArenaSize(
          static_cast< uint64 >( iAllocSize )
        * static_cast< uint64 >( iNumCellPerArena )
    )
    , mAllocSize( iAllocSize )
    , mDefragThreshold( FMath::Clamp( iDefragThreshold, 0.f, 1.f ) )
    , mPolicy( iPolicy )
{
    ULIS_ASSERT( mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mAllocSize, "Bad Size !" );
    // Round target memory usage to a multiple of arena size to avoid ping pong effect on unmatched size.
    // E.g: alloc / dealloc one arena constantly because the actual usage is either greater or lesser but never exact.
    mTargetMemoryUsage = static_cast< uint64 >( FMath::Ceil( static_cast< double >( FMath::Max( uint64( iTargetMemoryUsage ), uint64( 1 ) ) ) / mArenaSize ) * mArenaSize );
    ULIS_ASSERT( mTargetMemoryUsage, "Bad Size !" );
    ULIS_ASSERT( mTargetMemoryUsage % mArenaSize == 0, "Bad Computation !" );
}

byte_t
FFixedAllocMemoryPool::ArenaSize() const
{
    return  mArenaSize;
}

byte_t
FFixedAllocMemoryPool::AllocSize() const
{
    return  mAllocSize;
}

uint64
FFixedAllocMemoryPool::NumCells() const
{
    return  mArenaPool.size() * ( mArenaSize / mAllocSize );
}
uint64
FFixedAllocMemoryPool::NumFreeCells() const
{
    uint64 numFree = 0;
    for( auto it : mArenaPool )
        numFree += it->NumFreeCells();
    return  numFree;
}

uint64
FFixedAllocMemoryPool::NumUsedCells() const
{
    return  NumCells() - NumFreeCells();
}

byte_t
FFixedAllocMemoryPool::TotalMemory() const
{
    return  mArenaPool.size() * mArenaSize;
}

byte_t
FFixedAllocMemoryPool::FreeMemory() const
{
    return  NumFreeCells() * mAllocSize;
}

byte_t
FFixedAllocMemoryPool::UsedMemory() const
{
    return  TotalMemory() - FreeMemory();
}

byte_t
FFixedAllocMemoryPool::TargetMemoryUsage() const
{
    return  mTargetMemoryUsage;
}

void
FFixedAllocMemoryPool::SetTargetMemoryUsage( byte_t iValue )
{
    mTargetMemoryUsage = static_cast< uint64 >( FMath::Ceil( static_cast< double >( FMath::Max( uint64( iValue ), uint64( 1 ) ) ) / mArenaSize ) * mArenaSize );
    ULIS_ASSERT( mTargetMemoryUsage, "Bad Size !" );
    ULIS_ASSERT( mTargetMemoryUsage % mArenaSize == 0, "Bad Computation !" );
}

FMemoryPoolPolicy&
FFixedAllocMemoryPool::MemoryPolicy() {
    return  mPolicy;
}

const FMemoryPoolPolicy&
FFixedAllocMemoryPool::MemoryPolicy() const {
    return  mPolicy;
}

ufloat
FFixedAllocMemoryPool::DefragThreshold() const
{
    return  mDefragThreshold;
}

void
FFixedAllocMemoryPool::SetDefragThreshold( ufloat iValue )
{
    mDefragThreshold = FMath::Clamp( iValue, 0.f, 1.f );
}

ufloat
FFixedAllocMemoryPool::Fragmentation() const
{
    if( mArenaPool.size() <= 1 )
        return 0.f;

    ufloat sum = 0.f;
    uint64 count = 0;
    for( auto it : mArenaPool ) {
        float rate = it->OccupationRate();
        if( rate != 1.f ) {
            sum += rate;
            count++;
        }
    }
    if( count <= 1 )
        return  0.f;

    float globalOccupationRate = count == 0 ? 0.f : sum / count;
    return  globalOccupationRate;
}

void
FFixedAllocMemoryPool::DefragIfNecessary()
{
    if( Fragmentation() > mDefragThreshold )
        return;
    DefragForce();
}

void
FFixedAllocMemoryPool::DefragForce()
{
    if( mArenaPool.size() <= 1 )
        return;

    // Sort: in decreasing occupation rate:
    // High occupation rates are at the beginning
    // Low occupation rates are at the end
    // That way we minimize the moves and skip full.
    mArenaPool.sort(
        []( FFixedAllocArena* iLhs, FFixedAllocArena* iRhs ) {
            return  iLhs->OccupationRate() > iRhs->OccupationRate();
        }
    );

    auto highArena = mArenaPool.begin();
    auto lowArena = --mArenaPool.end();
    const uint64 numCells =
        static_cast< uint64 >(
              static_cast< uint64 >( mArenaSize )
            / static_cast< uint64 >( mAllocSize )
        );

    FFixedAllocArena::FIterator src_it = FFixedAllocArena::FIterator::MakeNull();
    FFixedAllocArena::FIterator dst_it = FFixedAllocArena::FIterator::MakeNull();
    uint64 srcFree = (*lowArena)->NumFreeCells();
    uint64 dstUsed = (*highArena)->NumUsedCells();
    while( highArena != lowArena ) {
        while( srcFree != numCells ) {
            if( dstUsed == numCells ) {
                if( highArena == lowArena )
                    return;

                ++highArena;
                dst_it = FFixedAllocArena::FIterator::MakeNull();
                dstUsed = (*highArena)->NumUsedCells();
            }

            src_it = (*lowArena)->FindFirst( true, src_it );
            dst_it = (*highArena)->FindFirst( false, dst_it );
            FFixedAllocArena::MoveAlloc( src_it, dst_it );
            srcFree++;
            dstUsed++;
        }
        if( highArena == lowArena )
            return;
        --lowArena;
        src_it = FFixedAllocArena::FIterator::MakeNull();
        srcFree = (*lowArena)->NumFreeCells();
    }
}

tClient
FFixedAllocMemoryPool::Malloc()
{
    tClient alloc = nullptr;
    // Reverse iteration, because after a defrag, arenas with a lot of free space are likely to be at the end
    for ( auto it = mArenaPool.rbegin(); it != mArenaPool.rend(); ++it ) {
        alloc = (*it)->Malloc();
        if( alloc )
            return  alloc;
    }

    if( mPolicy.AllocPageIfOutOfSpace() ) {
        FFixedAllocArena* page = new FFixedAllocArena( byte_t( mArenaSize ), byte_t( mAllocSize ) );
        mArenaPool.push_back( page );
        return  page->Malloc();
    }

    return  nullptr;
}

//static
void
FFixedAllocMemoryPool::Free( tClient iClient )
{
    FFixedAllocArena::Free( iClient );
}

void
FFixedAllocMemoryPool::UnsafeFreeAll()
{
    for( auto it : mArenaPool )
        it->UnsafeFreeAll();
}

bool
FFixedAllocMemoryPool::AllocOneArenaIfNecessary()
{
    if( TotalMemory() < TargetMemoryUsage() ) {
        mArenaPool.push_back( new FFixedAllocArena( byte_t( mArenaSize ), byte_t( mAllocSize ) ) );
        return  true;
    }
    return  false;
}

bool
FFixedAllocMemoryPool::FreeOneArenaIfNecessary()
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
FFixedAllocMemoryPool::AllocArenasToReachTargetMemory()
{
    uint32 count = 0;
    while( TotalMemory() < TargetMemoryUsage() ) {
        mArenaPool.push_back( new FFixedAllocArena( byte_t( mArenaSize ), byte_t( mAllocSize ) ) );
        count++;
    }
    return  count;
}

uint64
FFixedAllocMemoryPool::FreeEmptyArenasToReachTargetMemory() {
    return  FreeEmptyArenasAccordingToPredicate( [this](){ return  TotalMemory() > TargetMemoryUsage(); } );
}

uint64
FFixedAllocMemoryPool::FreeAllEmptyArenas() {
    return  FreeEmptyArenasAccordingToPredicate( [](){ return  true; } );
}

uint64
FFixedAllocMemoryPool::FreeEmptyArenasAccordingToPredicate( std::function< bool() > iPredicate ) {
    std::list< FFixedAllocArena* > toDelete;
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
FFixedAllocMemoryPool::DebugPrint() const
{
    std::cout << "=== " << FMath::FloorToInt( Fragmentation() * 100 ) << "%\n";
    for( auto it : mArenaPool )
        it->DebugPrint();
}

ULIS_NAMESPACE_END

