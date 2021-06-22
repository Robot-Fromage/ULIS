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
    TForwardListNode< FFixedAllocArena* >* it = mArenaPool.NodeFront();
    while( it ) {
        delete it->mValue;
        it = it->mNext;
    }
}

FFixedAllocMemoryPool::FFixedAllocMemoryPool(
      uint64 iArenaSize
    , uint32 iAllocSize
    , uint64 iTargetMemoryUsage
)
    : mArenaSize( iArenaSize )
    , mAllocSize( iAllocSize )
{
    ULIS_ASSERT( mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mAllocSize, "Bad Size !" );
    mTargetMemoryUsage = static_cast< uint64 >( FMath::Ceil( static_cast< double >( FMath::Max( iTargetMemoryUsage, uint64( 1 ) ) ) / mArenaSize ) * mArenaSize );
    ULIS_ASSERT( mTargetMemoryUsage, "Bad Size !" );
    ULIS_ASSERT( mTargetMemoryUsage % mArenaSize == 0, "Bad Computation !" );
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

uint8*
FFixedAllocMemoryPool::Malloc()
{
    uint8* alloc = nullptr;
    TForwardListNode< FFixedAllocArena* >* it = mArenaPool.NodeFront();
    while( it ) {
        alloc = it->mValue->Malloc();
        if( alloc )
            return  alloc;
        it = it->mNext;
    }
    ULIS_ASSERT( false, "Bad alloc, no space available" );
    return  nullptr;
}

void
FFixedAllocMemoryPool::Free( const uint8* iAlloc )
{
    uint64 adress = reinterpret_cast< uint64 >( iAlloc );
    TForwardListNode< FFixedAllocArena* >* it = mArenaPool.NodeFront();
    while( it ) {
        if( adress < it->mValue->LowAdress() ) {
            it = it->mNext;
            continue;
        }

        if( adress < it->mValue->HighAdress() )
            return  it->mValue->Free( iAlloc );

        it = it->mNext;
    }
    ULIS_ASSERT( false, "Bad alloc, not from this pool !" );
}

uint64
FFixedAllocMemoryPool::TotalMemory() const
{
    return  mArenaPool.Size() * mAllocSize;
}

uint64
FFixedAllocMemoryPool::TotalCells() const
{
    return  mArenaPool.Size();
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

uint32
FFixedAllocMemoryPool::NumAvailableCells() const
{
    uint32 avail = 0;
    const TForwardListNode< FFixedAllocArena* >* it = mArenaPool.NodeFront();
    while( it ) {
        avail += it->mValue->NumAvailableCells();
        it = it->mNext;
    }
    return  avail;
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

bool
FFixedAllocMemoryPool::AllocArenaIfNecessary()
{
    if( TotalMemory() < TargetMemoryUsage() ) {
        mArenaPool.PushBack( new FFixedAllocArena( mArenaSize, mAllocSize ) );
        return  true;
    }
    return  false;
}

bool
FFixedAllocMemoryPool::FreeArenaIfNecessary()
{
    if( TotalMemory() > TargetMemoryUsage() ) {
        const TForwardListNode< FFixedAllocArena* >* it = mArenaPool.NodeFront();
        while( it ) {
            if( it->mValue->IsEmpty() ) {
                mArenaPool.Erase( it );
                return  true;
            }
            it = it->mNext;
        }
    }
    return  false;
}

ULIS_NAMESPACE_END

