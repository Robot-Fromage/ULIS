// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         UncompressedMemoryDriver.cpp
* @author       Clement Berthaud
* @brief        This file provides declaration for the UncompressedMemoryDriver class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/UncompressedMemoryDriver.h"

ULIS_NAMESPACE_BEGIN
FUncompressedMemoryDriver::~FUncompressedMemoryDriver() {
    bStopWorker = true;
    mWorker->join();
    PurgeAllNow();
}

FUncompressedMemoryDriver::FUncompressedMemoryDriver(
      const uint8* iBackground
    , byte_t iTileSize
    , uint64 iNumCellPerArena
    , byte_t iTargetMemoryUsage
    , ufloat iDefragThreshold
    , double iWorkerRelaxTime_ms
    , uint32 iDeallocBatchSize
    , uint32 iAllocBatchSize
)
    : mBackground( iBackground )
    , mAllocPool(
          iTileSize
        , iNumCellPerArena
        , iTargetMemoryUsage
        , iDefragThreshold
        , FMemoryPoolPolicy::PermissiveGrowth
    )
    , mAvailableTiles()
    , mAvailableTilesMutex()
    , mBytesPerTile( iTileSize )
    , bStopWorker( false )
    , mWorker( new std::thread( &FUncompressedMemoryDriver::WorkProcess, this ) )
    , mWorkerRelaxTime_ms( iWorkerRelaxTime_ms )
    , mDeallocBatchSize( iDeallocBatchSize )
    , mAllocBatchSize( iAllocBatchSize )
{
    ULIS_ASSERT( mBackground, "Bad background data" );
    std::lock_guard< std::mutex > lock( mAvailableTilesMutex );
    mAllocPool.AllocArenasToReachTargetMemory();
}

void
FUncompressedMemoryDriver::PurgeAllNow() {
    std::lock_guard< std::mutex > lock( mAvailableTilesMutex );
    for( auto it : mAvailableTiles )
        FFixedAllocMemoryPool::Free( it );
}

void
FUncompressedMemoryDriver::SanitizeNow() {
    uint64 used = mAllocPool.UsedMemory();
    const uint64 target = mAllocPool.TargetMemoryUsage();
    std::lock_guard< std::mutex > lock( mAvailableTilesMutex );
    while( ( used > target ) && !( mAvailableTiles.empty() ) ) {
        tClient cli = mAvailableTiles.front();
        mAvailableTiles.pop_front();
        FFixedAllocMemoryPool::Free( cli );
        used -= mBytesPerTile;
    }
    mAllocPool.DefragIfNecessary();
}

tClient
FUncompressedMemoryDriver::QueryOne() {
    std::lock_guard< std::mutex > lock( mAvailableTilesMutex );
    if( mAvailableTiles.empty() ) {
        tClient cli = mAllocPool.Malloc();
        memcpy( (*cli), mBackground, mBytesPerTile );
        return  cli;
    }

    tClient cli = mAvailableTiles.front();
    mAvailableTiles.pop_front();
    return  cli;
}

void
FUncompressedMemoryDriver::SetBackground( const uint8* iBackground ) {
    std::lock_guard< std::mutex > lock( mAvailableTilesMutex );
    ULIS_ASSERT( mBackground, "Bad background data" );
    mBackground = iBackground;
}

void
FUncompressedMemoryDriver::AllocBatch() {
    uint64 used = mAllocPool.UsedMemory();
    const uint64 target = mAllocPool.TargetMemoryUsage();
    std::lock_guard< std::mutex > lock( mAvailableTilesMutex );
    for( uint32 i = 0; i < mAllocBatchSize; ++i ) {
        if( ( used < target ) ) {
            tClient cli = mAllocPool.Malloc();
            memcpy( (*cli), mBackground, mBytesPerTile );
            mAvailableTiles.push_front( cli );
            used += mBytesPerTile;
        } else {
            break;
        }
    }
}

void
FUncompressedMemoryDriver::DeallocBatch() {
    uint64 used = mAllocPool.UsedMemory();
    const uint64 target = mAllocPool.TargetMemoryUsage();
    std::lock_guard< std::mutex > lock( mAvailableTilesMutex );
    for( uint32 i = 0; i < mDeallocBatchSize; ++i ) {
        if( ( used > target ) && !( mAvailableTiles.empty() ) ) {
            tClient cli = mAvailableTiles.front();
            mAvailableTiles.pop_front();
            FFixedAllocMemoryPool::Free( cli );
            used -= mBytesPerTile;
        } else {
            break;
        }
    }
}

void
FUncompressedMemoryDriver::WorkProcess() {
    while( !bStopWorker ) {
        DeallocBatch();
        mAllocPool.FreeOneArenaIfNecessary();
        mAllocPool.AllocOneArenaIfNecessary();
        AllocBatch();
        std::this_thread::sleep_for( std::chrono::duration< double, std::milli >( mWorkerRelaxTime_ms ) );
    }
}

ULIS_NAMESPACE_END

