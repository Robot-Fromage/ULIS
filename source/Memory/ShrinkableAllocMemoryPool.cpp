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
      uint64 iArenaSize
    , uint32 iAllocSize
    , uint64 iTargetMemoryUsage
    , float iDefragThreshold
)
    : mArenaSize( iArenaSize )
    , mAllocSize( iAllocSize )
    , mDefragThreshold( FMath::Clamp( iDefragThreshold, 0.f, 1.f ) )
{
    ULIS_ASSERT( mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mAllocSize, "Bad Size !" );
    mTargetMemoryUsage = static_cast< uint64 >( FMath::Ceil( static_cast< double >( FMath::Max( iTargetMemoryUsage, uint64( 1 ) ) ) / mArenaSize ) * mArenaSize );
    ULIS_ASSERT( mTargetMemoryUsage, "Bad Size !" );
    ULIS_ASSERT( mTargetMemoryUsage % mArenaSize == 0, "Bad Computation !" );
    mArenaPool.push_back( new FShrinkableAllocArena( mArenaSize, mAllocSize ) );
}

uint64
FShrinkableAllocMemoryPool::ArenaSize() const
{
    return  mArenaSize;
}

uint32
FShrinkableAllocMemoryPool::AllocSize() const
{
    return  mAllocSize;
}

uint64
FShrinkableAllocMemoryPool::NumCells() const
{
    return  mArenaPool.size() * ( mArenaSize / mAllocSize );
}
uint64
FShrinkableAllocMemoryPool::NumAvailableCells() const
{
    uint32 avail = 0;
    for( auto it : mArenaPool )
        avail += it->NumAvailableCells();
    return  avail;
}

uint64
FShrinkableAllocMemoryPool::NumUsedCells() const
{
    return  NumCells() - NumAvailableCells();
}

uint64
FShrinkableAllocMemoryPool::TotalMemory() const
{
    return  mArenaPool.size() * mArenaSize;
}

uint64
FShrinkableAllocMemoryPool::AvailableMemory() const
{
    return  static_cast< uint64 >( NumAvailableCells() ) * mAllocSize;
}

uint64
FShrinkableAllocMemoryPool::UsedMemory() const
{
    return  TotalMemory() - AvailableMemory();
}

uint64
FShrinkableAllocMemoryPool::TargetMemoryUsage() const
{
    return  mTargetMemoryUsage;
}

void
FShrinkableAllocMemoryPool::SetTargetMemoryUsage( uint64 iValue )
{
    mTargetMemoryUsage = static_cast< uint64 >( FMath::Ceil( static_cast< double >( FMath::Max( iValue, uint64( 1 ) ) ) / mArenaSize ) * mArenaSize );
    ULIS_ASSERT( mTargetMemoryUsage, "Bad Size !" );
    ULIS_ASSERT( mTargetMemoryUsage % mArenaSize == 0, "Bad Computation !" );
}

float
FShrinkableAllocMemoryPool::DefragThreshold() const
{
    return  mDefragThreshold;
}

void
FShrinkableAllocMemoryPool::SetDefragThreshold( float iValue )
{
    mDefragThreshold = FMath::Clamp( iValue, 0.f, 1.f );
}

FShrinkableAllocArena::tClient
FShrinkableAllocMemoryPool::Malloc()
{
    FShrinkableAllocArena::tClient alloc = nullptr;
    for( auto it : mArenaPool ) {
        alloc = it->Malloc();
        if( alloc )
            return  alloc;
    }
    ULIS_ASSERT( false, "Bad alloc, no space available" );
    return  nullptr;
}

void
FShrinkableAllocMemoryPool::Free( FShrinkableAllocArena::tClient iClient )
{
    uint64 adress = reinterpret_cast< uint64 >( *iClient );
    for( auto it : mArenaPool )
        if( adress < it->LowBlockAdress() )
            continue;
        else if( adress < it->HighBlockAdress() )
            return  it->Free( iClient );
    ULIS_ASSERT( false, "Bad alloc, not from this pool !" );
}

float
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
    if( mArenaPool.empty() )
        return;

    mArenaPool.sort(
        []( FShrinkableAllocArena* iLhs, FShrinkableAllocArena* iRhs ) {
            return  iLhs->LocalFragmentation() < iRhs->LocalFragmentation();
        }
    );

    auto left = mArenaPool.begin();
    auto right = --mArenaPool.end();
    while( left != right ) {
        uint32 lindex = 0;
        uint32 rindex = ULIS_UINT32_MAX;
        while( !(*right)->IsEmpty() ) {
            if( (*left)->IsFull() ) {
                ++left;
                lindex = 0;
                if( left == right )
                    goto end;
            }
            FShrinkableAllocArena::Swap( (*right)->LastFullChunkMetaBase( rindex, &rindex ), (*left)->FirstEmptyChunkMetaBase( lindex, &lindex ), mAllocSize );
            (*right)->mNumAvailableCells++;
            (*left)->mNumAvailableCells--;
        }
        --right;
    }

end:
    (*left)->DefragSelf();
}

bool
FShrinkableAllocMemoryPool::AllocOneArenaIfNecessary()
{
    if( TotalMemory() < TargetMemoryUsage() ) {
        mArenaPool.push_back( new FShrinkableAllocArena( mArenaSize, mAllocSize ) );
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
                mArenaPool.erase( it );
                return  true;
            }
        }
    }
    return  false;
}

void
FShrinkableAllocMemoryPool::Print() const
{
    std::cout << "=== " << FMath::CeilToInt( Fragmentation() * 100 ) << "%\n";
    for( auto it : mArenaPool )
        it->Print();
}

ULIS_NAMESPACE_END

