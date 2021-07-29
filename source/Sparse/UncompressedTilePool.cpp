// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         UncompressedTilePool.cpp
* @author       Clement Berthaud
* @brief        This file provides declaration for the UncompressedTilePool class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/UncompressedTilePool.h"

ULIS_NAMESPACE_BEGIN
FUncompressedTilePool::~FUncompressedTilePool() {
    bStopWorker = true;
    mWorker->join();
    PurgeAllNow();
}

FUncompressedTilePool::FUncompressedTilePool(
      byte_t iTileSize
    , uint64 iNumCellPerArena
    , byte_t iTargetMemoryUsage
    , ufloat iDefragThreshold
    , double iWorkerRelaxTime_ms
    , uint32 iDeallocBatchSize
    , uint32 iAllocBatchSize
)
    : mAllocPool(
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
    , mWorker( new std::thread( &FUncompressedTilePool::WorkProcess, this ) )
    , mWorkerRelaxTime_ms( iWorkerRelaxTime_ms )
    , mDeallocBatchSize( iDeallocBatchSize )
    , mAllocBatchSize( iAllocBatchSize )
{
    mAllocPool.AllocArenasToReachTargetMemory();
}

void
FUncompressedTilePool::PurgeAllNow() {
    std::lock_guard< std::mutex > lock( mAvailableTilesMutex );
    for( auto it : mAvailableTiles )
        FFixedAllocMemoryPool::Free( it );
}

void
FUncompressedTilePool::SanitizeNow() {
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
FUncompressedTilePool::QueryOne() {
    std::lock_guard< std::mutex > lock( mAvailableTilesMutex );
    if( mAvailableTiles.empty() ) {
        tClient cli = mAllocPool.Malloc();
        memset( (*cli), 0, mBytesPerTile );
        return  cli;
    }

    tClient cli = mAvailableTiles.front();
    mAvailableTiles.pop_front();
    return  cli;
}

void
FUncompressedTilePool::AllocBatch() {
    uint64 used = mAllocPool.UsedMemory();
    const uint64 target = mAllocPool.TargetMemoryUsage();
    std::lock_guard< std::mutex > lock( mAvailableTilesMutex );
    for( uint32 i = 0; i < mAllocBatchSize; ++i ) {
        if( ( used < target ) ) {
            tClient cli = mAllocPool.Malloc();
            memset( (*cli), 0, mBytesPerTile );
            mAvailableTiles.push_front( cli );
            used += mBytesPerTile;
        } else {
            break;
        }
    }
}

void
FUncompressedTilePool::DeallocBatch() {
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
FUncompressedTilePool::WorkProcess() {
    while( !bStopWorker ) {
        DeallocBatch();
        mAllocPool.FreeOneArenaIfNecessary();
        mAllocPool.AllocOneArenaIfNecessary();
        AllocBatch();
        std::this_thread::sleep_for( std::chrono::duration< double, std::milli >( mWorkerRelaxTime_ms ) );
    }
}

ULIS_NAMESPACE_END

