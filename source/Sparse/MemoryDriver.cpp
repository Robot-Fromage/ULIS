// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MemoryDriver.cpp
* @author       Clement Berthaud
* @brief        This file provides declaration for the MemoryDriver class.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/MemoryDriver.h"
#include "Memory/FixedAllocMemoryPool.h"
#include "Sparse/Tile.h"
#include "String/CRC32.h"

ULIS_NAMESPACE_BEGIN
FMemoryDriver::~FMemoryDriver() {
    bStopWorker = true;
    mWorker->join();
    PurgeAllNow();
    mUncompressedMemoryPool.PurgeAllNow();
}

FMemoryDriver::FMemoryDriver(
      const uint8* iBackground
    , uint32 iBackgroundHash
    , byte_t iTileSize
    , uint64 iNumCellPerArena
    , byte_t iTargetMemoryUsage
    , ufloat iDefragThreshold
    , double iUncompressedMemoryPoolRelaxTime
    , uint32 iDeallocBatchSize
    , uint32 iAllocBatchSize
    , double iHashingRelaxTime_ms
    , uint32 iDirtyHashedBatchSize
    , uint32 iCorrectlyHashedBatchSize
)
    : mUncompressedMemoryPool(
          iBackground
        , iTileSize
        , iNumCellPerArena
        , iTargetMemoryUsage
        , iDefragThreshold
        , iUncompressedMemoryPoolRelaxTime
        , iDeallocBatchSize
        , iAllocBatchSize
    )
    , mBackgroundHash( iBackgroundHash )
    , mBytesPerTile( iTileSize )
    , mCorrectlyHashedTiles()
    , mDirtyHashedTiles()
    , mMutexCorrectlyHashedTilesLock()
    , mMutexDirtyHashedTilesLock()
    , bStopWorker( false )
    , mWorker( new std::thread( &FMemoryDriver::WorkProcess, this ) )
    , mWorkerRelaxTime_ms( iHashingRelaxTime_ms )
    , mCorrectlyHashedBatchSize( iCorrectlyHashedBatchSize )
    , mDirtyHashedBatchSize( iDirtyHashedBatchSize )
{}

void
FMemoryDriver::PurgeAllNow() {
    std::list< tClient > scheduledForRelease;
    {
        std::lock_guard< std::mutex > lockA( mMutexDirtyHashedTilesLock );
        std::lock_guard< std::mutex > lockB( mMutexCorrectlyHashedTilesLock );
        // Cannot just free the clients ptrs, because the FixedAllocMemoryPool
        // isn't thread-safe.
        for( auto it : mDirtyHashedTiles ) {
            scheduledForRelease.push_front( it->mClient );
            delete  it;
        }

        for( auto it : mCorrectlyHashedTiles ) {
            scheduledForRelease.push_front( it.second->mClient );
            delete  it.second;
        }
        mDirtyHashedTiles.clear();
        mCorrectlyHashedTiles.clear();
    }
    mUncompressedMemoryPool.Release( scheduledForRelease );
}

void
FMemoryDriver::SanitizeNow() {
    mUncompressedMemoryPool.SanitizeNow();
}

FTile*
FMemoryDriver::QueryOne() {
    // Here we increase the refcount before inserting in the Dirty list,
    // so that parallel sanitize operation beetwen the allocation here,
    // and return from this function or ulterior usage can still safely
    // use the tile without it being deleted.
    FTile* tile = new FTile( mUncompressedMemoryPool.QueryOne() );
    tile->IncreaseRefCount();
    std::lock_guard< std::mutex > lock( mMutexDirtyHashedTilesLock );
    mDirtyHashedTiles.push_front( tile );
    return  tile;
}

FTile*
FMemoryDriver::RedundantHashMerge( FTile* iTile ) {
    if( !iTile )
        return  nullptr;

    // Process only clean tiles
    if( iTile->mDirty )
        return  iTile;

    // If the hashed tile is empty we return null ptr and decrease refcount
    if( iTile->mHash == mBackgroundHash ) {
        iTile->DecreaseRefCount();
        return  nullptr;
    }

    // Find the hashed tile in the map if not empty
    mMutexCorrectlyHashedTilesLock.lock();
    auto it = mCorrectlyHashedTiles.find( iTile->mHash );
    auto end = mCorrectlyHashedTiles.end();
    mMutexCorrectlyHashedTilesLock.unlock();

    // If the hash wasn't found, the tile might come from Dirty
    if( it == end ) {
        FTile* tile = new FTile( mUncompressedMemoryPool.QueryOne() );
        tile->IncreaseRefCount();
        tile->mDirty = false;
        tile->mHash = iTile->mHash;
        memcpy( *( tile->mClient ), *( iTile->mClient ), mBytesPerTile );

        mMutexCorrectlyHashedTilesLock.lock();
        mCorrectlyHashedTiles[ tile->mHash ] = tile;
        mMutexCorrectlyHashedTilesLock.unlock();

        iTile->DecreaseRefCount();
        return  tile;
    }

    // Found same, no need to merge
    if( it->second == iTile )
        return  iTile;

    // Found correct, merge one
    it->second->IncreaseRefCount();
    iTile->DecreaseRefCount();
    return  it->second;
}

FTile*
FMemoryDriver::SplitMutable( FTile* iElem ) {
    if( !iElem ) {
        return  QueryOne();
    } else {
        if( iElem->mRefCount == 1 ) {
            return  iElem;
        } else {
            FTile* tile = new FTile( mUncompressedMemoryPool.QueryOne() );
            tile->IncreaseRefCount();
            memcpy( *( tile->mClient ), *( iElem->mClient ), mBytesPerTile );
            std::lock_guard< std::mutex > lock( mMutexDirtyHashedTilesLock );
            mDirtyHashedTiles.push_front( tile );
            return  tile;
        }
    }
}

void
FMemoryDriver::SetBackground( const uint8* iBackground, uint32 iBackgroundHash ) {
    mUncompressedMemoryPool.SetBackground( iBackground );
    // No contention on background hash a priori.
    // Should be driven on synchronously by the main tile pool.
    mBackgroundHash = iBackgroundHash;
}

void
FMemoryDriver::PrintDiagnosis() {
    std::lock_guard< std::mutex > lockA( mMutexDirtyHashedTilesLock );
    std::lock_guard< std::mutex > lockB( mMutexCorrectlyHashedTilesLock );
    std::cout << "====== FMemoryDriver" << std::endl;
    std::cout << "Replicated Background Hash: " << mBackgroundHash << std::endl;
    std::cout << "Replicated Bytes Per Tile: " << mBytesPerTile << std::endl;
    std::cout << "Requested Stop? : " << bStopWorker << std::endl;
    std::cout << "Relax Time: " << mWorkerRelaxTime_ms << std::endl;
    std::cout << "Num Dirty Hashed Tiles: " << mDirtyHashedTiles.size() << std::endl;
    std::cout << "Num Correctly Hashed Tiles: " << mCorrectlyHashedTiles.size() << std::endl;
    std::cout << "Replicated Num Dirty Hashed Tiles: " << mDirtyHashedBatchSize << std::endl;
    std::cout << "Replicated Num Correctly Hashed Tiles: " << mCorrectlyHashedBatchSize << std::endl;
    mUncompressedMemoryPool.PrintDiagnosis();
}

void
FMemoryDriver::SanitizeDirtyHashedBatch() {
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
            mUncompressedMemoryPool.Release( tile->mClient );
            delete  tile;
            it = mDirtyHashedTiles.erase( it );
        }
        else
        {
            if( tile->mDirty ) {
                tile->mHash = CRC32( *( tile->mClient ), static_cast< int >( mBytesPerTile ) );
                tile->mDirty = false;
            }
            ++it;
        }
        ++count;
    }
}

void
FMemoryDriver::SanitizeCorrectlyHashedBatch() {
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
            mUncompressedMemoryPool.Release( tile->mClient );
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
FMemoryDriver::WorkProcess() {
    while( !bStopWorker ) {
        SanitizeDirtyHashedBatch();
        SanitizeCorrectlyHashedBatch();
        std::this_thread::sleep_for( std::chrono::duration< double, std::milli >( mWorkerRelaxTime_ms ) );
    }
}

ULIS_NAMESPACE_END

