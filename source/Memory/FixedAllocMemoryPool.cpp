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
    mArenaPool.push_back( new FFixedAllocArena( mArenaSize, mAllocSize ) );
}

uint64
FFixedAllocMemoryPool::ArenaSize() const
{
    return  mArenaSize;
}

uint32
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
FFixedAllocMemoryPool::NumAvailableCells() const
{
    uint32 avail = 0;
    for( auto it : mArenaPool )
        avail += it->NumAvailableCells();
    return  avail;
}

uint64
FFixedAllocMemoryPool::NumUsedCells() const
{
    return  NumCells() - NumAvailableCells();
}

uint64
FFixedAllocMemoryPool::TotalMemory() const
{
    return  mArenaPool.size() * mArenaSize;
}

uint64
FFixedAllocMemoryPool::AvailableMemory() const
{
    return  static_cast< uint64 >( NumAvailableCells() ) * mAllocSize;
}

uint64
FFixedAllocMemoryPool::UsedMemory() const
{
    return  TotalMemory() - AvailableMemory();
}

uint64
FFixedAllocMemoryPool::TargetMemoryUsage() const
{
    return  mTargetMemoryUsage;
}

void
FFixedAllocMemoryPool::SetTargetMemoryUsage( uint64 iValue )
{
    mTargetMemoryUsage = static_cast< uint64 >( FMath::Ceil( static_cast< double >( FMath::Max( iValue, uint64( 1 ) ) ) / mArenaSize ) * mArenaSize );
    ULIS_ASSERT( mTargetMemoryUsage, "Bad Size !" );
    ULIS_ASSERT( mTargetMemoryUsage % mArenaSize == 0, "Bad Computation !" );
}

float
FFixedAllocMemoryPool::DefragThreshold() const
{
    return  mDefragThreshold;
}

void
FFixedAllocMemoryPool::SetDefragThreshold( float iValue )
{
    mDefragThreshold = FMath::Clamp( iValue, 0.f, 1.f );
}

float
FFixedAllocMemoryPool::ExpectedThresholdAfterDefrag()
{
    return  1.f / mArenaPool.size();
}

void
FFixedAllocMemoryPool::SetAutoDefragThreshold( float iValue )
{
    SetDefragThreshold( FMath::Clamp( ExpectedThresholdAfterDefrag() * 2, 0.f, 0.75f ) );
}

uint8*
FFixedAllocMemoryPool::Malloc()
{
    uint8* alloc = nullptr;
    for( auto it : mArenaPool ) {
        alloc = it->Malloc();
        if( alloc )
            return  alloc;
    }
    ULIS_ASSERT( false, "Bad alloc, no space available" );
    return  nullptr;
}

void
FFixedAllocMemoryPool::Free( uint8* iAlloc )
{
    uint64 adress = reinterpret_cast< uint64 >( iAlloc );
    for( auto it : mArenaPool )
        if( adress < it->LowBlockAdress() )
            continue;
        else if( adress < it->HighBlockAdress() )
            return  it->Free( iAlloc );
    ULIS_ASSERT( false, "Bad alloc, not from this pool !" );
}

float
FFixedAllocMemoryPool::Fragmentation() const
{
    float sum = 0.f;
    for( auto it : mArenaPool )
        sum += it->LocalFragmentation();
    return  sum / mArenaPool.size();
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
    mArenaPool.sort(
        []( FFixedAllocArena* iLhs, FFixedAllocArena* iRhs ) {
            return  iLhs->LocalFragmentation() < iRhs->LocalFragmentation();
        }
    );

    auto left = mArenaPool.begin();
    auto right = --mArenaPool.end();
    while( left != right ) {
        uint32 lindex = 0;
        uint32 rindex = 0;
        while( !(*right)->IsEmpty() ) {
            if( (*left)->IsFull() ) {
                ++left;
                lindex = 0;
                if( left == right )
                    return;
            }
            uint8* metaBaseFull = (*right)->FirstFull( rindex, &rindex );
            uint8* metaBaseEmpty = (*left)->FirstEmpty( lindex, &lindex );
            memcpy( metaBaseEmpty, metaBaseFull, mAllocSize + static_cast< uint64 >( FFixedAllocArena::smMetaPadSize ) );
            memset( metaBaseFull, 0, FFixedAllocArena::smMetaPadSize );
            uint8*** client_ptr = reinterpret_cast< uint8*** >( metaBaseEmpty );
            uint8** client = *client_ptr;
            uint8* data = metaBaseEmpty + FFixedAllocArena::smMetaPadSize;
            *client = data;
            (*right)->mNumAvailableCells++;
            (*left)->mNumAvailableCells--;
        }
        --right;
    }
}

bool
FFixedAllocMemoryPool::AllocOneArenaIfNecessary()
{
    if( TotalMemory() < TargetMemoryUsage() ) {
        mArenaPool.push_back( new FFixedAllocArena( mArenaSize, mAllocSize ) );
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
                mArenaPool.erase( it );
                return  true;
            }
        }
    }
    return  false;
}

void
FFixedAllocMemoryPool::Print() const
{
    std::cout << "=== " << FMath::CeilToInt( Fragmentation() * 100 ) << "%\n";
    for( auto it : mArenaPool )
        it->Print();
}

ULIS_NAMESPACE_END

