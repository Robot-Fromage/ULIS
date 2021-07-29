// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BusyMemoryDriver.cpp
* @author       Clement Berthaud
* @brief        This file provides declaration for the BusyMemoryDriver class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/BusyMemoryDriver.h"
#include "Memory/FixedAllocMemoryPool.h"
#include "Sparse/Tile.h"
#include "String/CRC32.h"

ULIS_NAMESPACE_BEGIN
FBusyMemoryDriver::~FBusyMemoryDriver() {
    bStopWorker = true;
    mWorker->join();
    PurgeAllNow();
}

FBusyMemoryDriver::FBusyMemoryDriver(
      uint32 iBackgroundHash
    , byte_t iTileSize
    , double iWorkerRelaxTime_ms
    , uint32 iDirtyHashedBatchSize
    , uint32 iCorrectlyHashedBatchSize
)
    : mBackgroundHash( iBackgroundHash )
    , mBytesPerTile( iTileSize )
    , mClientsScheduledForRelease()
    , mCorrectlyHashedTiles()
    , mDirtyHashedTiles()
    , mMutexClientsScheduledForReleaseLock()
    , mMutexCorrectlyHashedTilesLock()
    , mMutexDirtyHashedTilesLock()
    , bStopWorker( false )
    , mWorker( new std::thread( &FBusyMemoryDriver::WorkProcess, this ) )
    , mWorkerRelaxTime_ms( iWorkerRelaxTime_ms )
    , mCorrectlyHashedBatchSize( iCorrectlyHashedBatchSize )
    , mDirtyHashedBatchSize( iDirtyHashedBatchSize )
{}

void
FBusyMemoryDriver::PurgeAllNow() {
    std::lock_guard< std::mutex > lock( mMutexDirtyHashedTilesLock );
    std::lock_guard< std::mutex > lock( mMutexCorrectlyHashedTilesLock );
    std::lock_guard< std::mutex > lock( mMutexClientsScheduledForReleaseLock );

    // Cannot just free the clients ptrs, because the FixedAllocMemoryPool
    // isn't thread-safe.
    for( auto it : mDirtyHashedTiles ) {
        mClientsScheduledForRelease.push_front( it->mClient );
        delete  it;
    }

    for( auto it : mCorrectlyHashedTiles ) {
        mClientsScheduledForRelease.push_front( it.second->mClient );
        delete  it.second;
    }

    mDirtyHashedTiles.clear();
    mCorrectlyHashedTiles.clear();
}

void
FBusyMemoryDriver::SanitizeNow() {
}

void
FBusyMemoryDriver::Submit( FTile* iTile ) {
    std::lock_guard< std::mutex > lock( mMutexDirtyHashedTilesLock );
    mDirtyHashedTiles.push_front( iTile );
}

FTile*
FBusyMemoryDriver::RedundantHashMerge( FTile* iTile ) {
}

void
FBusyMemoryDriver::SetBackgroundHash( uint32 iBackgroundHash ) {
    // No contention on background hash a priori.
    // Should be driven on synchronously by the main tile pool.
    mBackgroundHash = iBackgroundHash;
}

std::list< tClient >&
FBusyMemoryDriver::ClientsScheduledForRelease() {
    return  mClientsScheduledForRelease;
}

void
FBusyMemoryDriver::LockScheduleRelease() {
    mMutexClientsScheduledForReleaseLock.lock();
}
void
FBusyMemoryDriver::UnlockScheduleRelease() {
    mMutexClientsScheduledForReleaseLock.unlock();
}

void
FBusyMemoryDriver::SanitizeDirtyHashedBatch() {
    // Just process the dirty ones
    // Refcounted ones: compute their hashes
    // Others: delete
    std::lock_guard<std::mutex> lock( mMutexDirtyHashedTilesLock );
    auto it = mDirtyHashedTiles.begin();
    uint32 count = 0;
    while( true ) {
        if(
               ( it == mDirtyHashedTiles.end() )
            || ( ( count == mDirtyHashedBatchSize ) && ( mDirtyHashedBatchSize != 0 ) )
        )
            break;

        FTile* tile = *it;
        if( tile->mRefCount == 0 ) {
            mMutexClientsScheduledForReleaseLock.lock();
            mClientsScheduledForRelease.push_front( tile->mClient );
            mMutexClientsScheduledForReleaseLock.unlock();
            delete  tile;
            it = mDirtyHashedTiles.erase( it );
        }
        else
        {
            if( tile->mDirty ) {
                tile->mHash = CRC32( *( tile->mClient ), mBytesPerTile );
                tile->mDirty = false;
            }
            ++it;
        }
        ++count;
    }
}

void
FBusyMemoryDriver::SanitizeCorrectlyHashedBatch() {
    // Process the correct ones
    // Eliminate non-referenced ones
    // Do NOT merge redundant, this is the responsibility of the block
    // sanitization, because we cannot ensure synchronisation with them.
    std::lock_guard<std::mutex> lock( mMutexCorrectlyHashedTilesLock );
    auto it = mCorrectlyHashedTiles.begin();
    uint32 count = 0;
    while( true ) {
        if(
               ( it == mCorrectlyHashedTiles.end() )
            || ( ( count == mCorrectlyHashedBatchSize ) && ( mCorrectlyHashedBatchSize != 0 ) )
        )
            break;

        FTile* tile = it->second;
        if( tile->mRefCount.load() == 0 ) {
            mMutexClientsScheduledForReleaseLock.lock();
            mClientsScheduledForRelease.push_front( tile->mClient );
            mMutexClientsScheduledForReleaseLock.unlock();
            delete  tile;
            it = mCorrectlyHashedTiles.erase( it );
        }
        else
        {
            ++it;
        }
    }
}

void
FBusyMemoryDriver::WorkProcess() {
    while( !bStopWorker ) {
        SanitizeDirtyHashedBatch();
        SanitizeCorrectlyHashedBatch();
        std::this_thread::sleep_for( std::chrono::duration< double, std::milli >( mWorkerRelaxTime_ms ) );
    }
}

ULIS_NAMESPACE_END

