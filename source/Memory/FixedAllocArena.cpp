// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FixedAllocMemoryPool.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for FixedAllocArena.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Memory/FixedAllocArena.h"

ULIS_NAMESPACE_BEGIN
FFixedAllocArena::~FFixedAllocArena()
{
    delete  mBlock;
}

FFixedAllocArena::FFixedAllocArena(
      uint64 iArenaSize
    , uint32 iAllocSize
)
    : mArenaSize( iArenaSize )
    , mAllocSize( iAllocSize )
    , mTotalCells( static_cast< uint32 >( mArenaSize / mAllocSize ) )
    , mNumAvailableCells( mTotalCells )
    , mBlock( new uint8[ iArenaSize ] )
    , mUsed( mTotalCells, false )
{
    ULIS_ASSERT( mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mAllocSize, "Bad Size !" );
    ULIS_ASSERT( mTotalCells, "Bad Size !" );
    ULIS_ASSERT( mNumAvailableCells, "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
}

bool
FFixedAllocArena::IsFull() const
{
    return  mNumAvailableCells == 0;
}

bool
FFixedAllocArena::IsEmpty() const
{
    return  mNumAvailableCells == mTotalCells;
}

uint64
FFixedAllocArena::ArenaSize() const
{
    return  mArenaSize;
}

uint32
FFixedAllocArena::AllocSize() const
{
    return  mAllocSize;
}

uint32
FFixedAllocArena::TotalCells() const
{
    return  mTotalCells;
}

uint32
FFixedAllocArena::NumAvailableCells() const
{
    return  mNumAvailableCells;
}

uint32
FFixedAllocArena::NumUsedCells() const
{
    return  mTotalCells - mNumAvailableCells;
}

uint8*
FFixedAllocArena::Malloc()
{
    for( uint64 i = 0; i < mTotalCells; ++i ) {
        if( !mUsed[i] ) {
            mUsed[i] = true;
            return  mBlock + i * mAllocSize;
        }
    }
    // Alloc failed, return nullptr:
    return  nullptr;
}

void
FFixedAllocArena::Free( const uint8* iAlloc )
{
    ULIS_ASSERT( IsInRange( iAlloc ), "Bad alloc, not in this arena !" );
    uint64 rel_adress = reinterpret_cast< uint64 >( iAlloc ) - LowAdress();
    uint32 cell = static_cast< uint32 >( rel_adress / mAllocSize );
    ULIS_ASSERT( mUsed[cell] == true, "Bad Free, already free !" );
    mUsed[cell] = false;
}

bool
FFixedAllocArena::IsInRange( const uint8* iAlloc ) const
{
    uint64 adress = reinterpret_cast< uint64 >( iAlloc );
    return  adress >= LowAdress() && adress < HighAdress();
}

uint64
FFixedAllocArena::LowAdress() const
{
    return  reinterpret_cast< uint64 >( mBlock );
}

uint64
FFixedAllocArena::HighAdress() const
{
    return  reinterpret_cast< uint64 >( mBlock ) + mArenaSize;
}

ULIS_NAMESPACE_END

