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
)
    : mArenaSize( iArenaSize )
    , mAllocSize( iAllocSize )
    , mDefragThreshold( FMath::Clamp( iDefragThreshold, 0.f, 1.f ) )
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
    , uint32 iNumCellPerArena
    , byte_t iTargetMemoryUsage
    , ufloat iDefragThreshold
)
    : mArenaSize(
          static_cast< uint64 >( iAllocSize )
        * static_cast< uint64 >( iNumCellPerArena )
    )
    , mAllocSize( iAllocSize )
    , mDefragThreshold( FMath::Clamp( iDefragThreshold, 0.f, 1.f ) )
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
    uint32 avail = 0;
    for( auto it : mArenaPool )
        avail += it->NumFreeCells();
    return  avail;
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
    return  double(TotalMemory()) - double(FreeMemory());
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
    ufloat sum = 0.f;
    for( auto it : mArenaPool )
        sum += it->OccupationRate();

    float globalOccupationRate = sum / mArenaPool.size();
    return  1.f - globalOccupationRate;
}

void
FFixedAllocMemoryPool::DefragIfNecessary()
{
    if( Fragmentation() < mDefragThreshold )
        return;
    DefragForce();
}

void
FFixedAllocMemoryPool::DefragForce()
{
    if( mArenaPool.size() <= 1 )
        return;

    mArenaPool.sort(
        []( FFixedAllocArena* iLhs, FFixedAllocArena* iRhs ) {
            return  iLhs->OccupationRate() > iRhs->OccupationRate();
        }
    );

    auto left = mArenaPool.begin();
    auto right = --mArenaPool.end();
    uint32 numCells =
        static_cast< uint32 >(
              static_cast< uint64 >( mArenaSize )
            / static_cast< uint64 >( mAllocSize )
        );

    tMetaBase lfrom = nullptr;
    tMetaBase rfrom = nullptr;
    uint32 leftUsed = (*left)->NumUsedCells();
    uint32 rightFree = (*right)->NumFreeCells();
    while( left != right ) {
        while( rightFree != numCells ) {
            if( leftUsed == numCells ) {
                if( left == right )
                    return;
                ++left;
                lfrom = nullptr;
                leftUsed = (*left)->NumUsedCells();
            }
            lfrom = (*left)->FirstFullCellMetaBase( lfrom );
            rfrom = (*right)->FirstFullCellMetaBase( rfrom );
            FFixedAllocArena::MoveAlloc(
                  rfrom
                , lfrom
                , mAllocSize
            );
            rightFree++;
            leftUsed++;
        }
        --right;
        rfrom = nullptr;
        rightFree = (*right)->NumFreeCells();
    }
}

tClient
FFixedAllocMemoryPool::Malloc()
{
    tClient alloc = nullptr;
    for( auto it : mArenaPool ) {
        alloc = it->Malloc();
        if( alloc )
            return  alloc;
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
    if( uint64(TotalMemory()) < uint64(TargetMemoryUsage()) ) {
        mArenaPool.push_back( new FFixedAllocArena( byte_t( mArenaSize ), byte_t( mAllocSize ) ) );
        return  true;
    }
    return  false;
}

bool
FFixedAllocMemoryPool::FreeOneArenaIfNecessary()
{
    if( uint64(TotalMemory()) > uint64(TargetMemoryUsage()) ) {
        for( auto it = mArenaPool.begin(); it != mArenaPool.end(); ++it ) {
            if( (*it)->IsEmpty() ) {
                mArenaPool.erase( it );
                return  true;
            }
        }
    }
    return  false;
}

void
FFixedAllocMemoryPool::DebugPrint() const
{
    std::cout << "=== " << FMath::CeilToInt( Fragmentation() * 100 ) << "%\n";
    for( auto it : mArenaPool )
        it->DebugPrint();
}

ULIS_NAMESPACE_END

