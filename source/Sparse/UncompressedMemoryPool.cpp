// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         UncompressedMemoryPool.cpp
* @author       Clement Berthaud
* @brief        This file provides declaration for the UncompressedMemoryPool class.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/UncompressedMemoryPool.h"

ULIS_NAMESPACE_BEGIN
FUncompressedMemoryPool::~FUncompressedMemoryPool() {
    bStopWorker = true;
    mWorker->join();
    PurgeAllNow();
}

FUncompressedMemoryPool::FUncompressedMemoryPool(
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
    , mMutexAvailableTilesLock()
    , mBytesPerTile( iTileSize )
    , bStopWorker( false )
    , mWorker( new std::thread( &FUncompressedMemoryPool::WorkProcess, this ) )
    , mWorkerRelaxTime_ms( iWorkerRelaxTime_ms )
    , mDeallocBatchSize( iDeallocBatchSize )
    , mAllocBatchSize( iAllocBatchSize )
{
    ULIS_ASSERT( mBackground, "Bad background data" );
    std::lock_guard< std::mutex > lock( mMutexAvailableTilesLock );
    mAllocPool.AllocArenasToReachTargetMemory();
}

void
FUncompressedMemoryPool::PurgeAllNow() {
    std::lock_guard< std::mutex > lock( mMutexAvailableTilesLock );
    for( auto it : mAvailableTiles )
        FFixedAllocMemoryPool::Free( it );
    mAvailableTiles.clear();
}

void
FUncompressedMemoryPool::SanitizeNow() {
    uint64 used = mAllocPool.UsedMemory();
    const uint64 target = mAllocPool.TargetMemoryUsage();
    std::lock_guard< std::mutex > lock( mMutexAvailableTilesLock );
    while( ( used > target ) && !( mAvailableTiles.empty() ) ) {
        tClient cli = mAvailableTiles.front();
        mAvailableTiles.pop_front();
        FFixedAllocMemoryPool::Free( cli );
        used -= mBytesPerTile;
    }
    mAllocPool.DefragIfNecessary();
}

tClient
FUncompressedMemoryPool::QueryOne() {
    std::lock_guard< std::mutex > lock( mMutexAvailableTilesLock );
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
FUncompressedMemoryPool::SetBackground( const uint8* iBackground ) {
    std::lock_guard< std::mutex > lock( mMutexAvailableTilesLock );
    ULIS_ASSERT( mBackground, "Bad background data" );
    mBackground = iBackground;
}

void
FUncompressedMemoryPool::Release( tClient iClient ) {
    std::lock_guard< std::mutex > lock( mMutexAvailableTilesLock );
    FFixedAllocMemoryPool::Free( iClient );
}

void
FUncompressedMemoryPool::Release( const std::list< tClient > iList ) {
    std::lock_guard< std::mutex > lock( mMutexAvailableTilesLock );
    for( auto it : iList )
        FFixedAllocMemoryPool::Free( it );
}

void
FUncompressedMemoryPool::PrintDiagnosis() {
    std::lock_guard< std::mutex > lock( mMutexAvailableTilesLock );
    std::cout << "====== FUncompressedMemoryPool" << std::endl;
    std::cout << "Num Available Tiles: " << std::distance( mAvailableTiles.begin(), mAvailableTiles.end() ) << std::endl;
    std::cout << "Replicated Bytes Per Tiles: " << mBytesPerTile << std::endl;
    std::cout << "Requested Stop ?: " << bStopWorker << std::endl;
    std::cout << "Local relax Time: " << mWorkerRelaxTime_ms << std::endl;
    std::cout << "Dealloc Batch Size: " << mDeallocBatchSize << std::endl;
    std::cout << "Alloc Batch Size: " << mAllocBatchSize << std::endl;

    std::cout << "====== FFixedAllocMemoryPool DebugPrint" << std::endl;
    std::cout << "Alloc Size: " << (double)mAllocPool.AllocSize() << std::endl;
    std::cout << "Arena Size: " << (double)mAllocPool.ArenaSize() << std::endl;
    std::cout << "Defrag Threshold: " << mAllocPool.DefragThreshold() << std::endl;
    std::cout << "Fragmentation: " << mAllocPool.Fragmentation() << std::endl;
    std::cout << "Num Cells: " << mAllocPool.NumCells() << std::endl;
    std::cout << "Num Free Cells: " << mAllocPool.NumFreeCells() << std::endl;
    std::cout << "Num Used Cells: " << mAllocPool.NumUsedCells() << std::endl;
    std::cout << "Target Memory Usage: " << (double)mAllocPool.TargetMemoryUsage() << std::endl;
    std::cout << "Total Memory: " << (double)mAllocPool.TotalMemory() << std::endl;
    std::cout << "Free Memory: " << (double)mAllocPool.FreeMemory() << std::endl;
    std::cout << "Used Memory: " << (double)mAllocPool.UsedMemory() << std::endl;
}

void
FUncompressedMemoryPool::AllocBatch() {
    uint64 used = mAllocPool.UsedMemory();
    const uint64 target = mAllocPool.TargetMemoryUsage();
    std::lock_guard< std::mutex > lock( mMutexAvailableTilesLock );
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
FUncompressedMemoryPool::DeallocBatch() {
    uint64 used = mAllocPool.UsedMemory();
    const uint64 target = mAllocPool.TargetMemoryUsage();
    std::lock_guard< std::mutex > lock( mMutexAvailableTilesLock );
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
FUncompressedMemoryPool::WorkProcess() {
    while( !bStopWorker ) {
        DeallocBatch();
        mAllocPool.FreeOneArenaIfNecessary();
        mAllocPool.AllocOneArenaIfNecessary();
        AllocBatch();
        std::this_thread::sleep_for( std::chrono::duration< double, std::milli >( mWorkerRelaxTime_ms ) );
    }
}

ULIS_NAMESPACE_END

