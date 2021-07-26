// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TilePool.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the TilePool class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Sparse/TiledBlock.h"
#include "Image/Format.h"
#include "Image/ColorSpace.h"

#include <atomic>
#include <forward_list>
#include <list>
#include <mutex>
#include <thread>
#include <unordered_map>

ULIS_NAMESPACE_BEGIN
#pragma warning(push)
#pragma warning(disable : 4251) // Shut warning C4251 dll export of stl classes
class ULIS_API FTilePool
    : public IHasFormat
    , public IHasColorSpace
{
public:
    // Construction / Destruction
    /*! Destructor, cleanup chunks. */
    ~FTilePool();

    /*! Constructor, initialize with zero chunks. */
    FTilePool( eFormat iFormat, FColorSpace* iProfile );

    /*! Explicitely deleted copy constructor */
    FTilePool( const FTiledBlock& ) = delete;

    /*! Explicitely deleted copy assignment operator */
    FTilePool& operator=( const FTiledBlock& ) = delete;

public:
    // Public API
    const FVec2I& TileSize() const;
    uint32 EmptyCRC32Hash() const;
    const uint8* EmptyTile() const;
    uint64 CurrentRAMUsage() const;
    uint64 CurrentSwapUsage() const;
    uint64 RAMUsageCapTarget() const;
    uint64 SWAPUsageCapTarget() const;
    uint64 CurrentTotalMemoryUsage() const;
    uint64 NumTilesScheduledForClear() const;
    uint64 NumFreshTilesAvailableForQuery() const;
    uint64 NumDirtyHashedTilesCurrentlyInUse();
    uint64 NumCorrectlyHashedTilesCurrentlyInUse();
    uint64 NumRegisteredTiledBlocks();
    FTiledBlock* CreateNewTiledBlock();
    void RequestTiledBlockDeletion( FTiledBlock* iBlock );
    void SetRAMUsageCapTarget( uint64 iValue );
    void SetSWAPUsageCapTarget( uint64 iValue );

public:
    // Core API
    void PurgeAllNow();
    FTile* XQueryFreshTile();
    FTile* PerformRedundantHashMergeReturnCorrect( FTile* iElem );
    FTile* XPerformDataCopyForImminentMutableChangeIfNeeded( FTile* iElem );

private:
    // Internal API
    void AllocateNow_Unsafe( int32 iNum );
    void ClearNowDirect_Unsafe( int32 iNum );
    void DeallocOneInTilesScheduledForClearIfRAMOverflowsTarget();
    void DeallocOneInFreshTilesAvailableForQueryIfRAMOverflowsTargetAndTilesScheduledForClearIsEmptyImprecise();
    void AllocOneInTilesScheduledForClearIfRAMUnderflowsTarget();
    void ClearOneInTilesScheduledForClearAndMoveToFreshTilesAvailableForQueryIfNeeded();
    void SanitizeAllDirtyTilesCurrentlyInUse();
    void SanitizeAllCorrectlyHashedTilesCurrentlyInUse();

private:
    // Private Workers API
    void ThreadedDeallocatorAllocatorCleanerBackgroundWorker();
    void ThreadedHasherGarbageCollectorBackgroundWorker();

    // Empty Tile Ref
    FBlock* const mEmptyTile;
    uint32 mEmptyCRC32Hash;
    const uint64 mBytesPerTile;

    // Atomic memory Intent
    std::atomic< uint64 > mSWAPUsageCapTargetAtomic;
    std::atomic< uint64 > mRAMUsageCapTargetAtomic;
    std::atomic< uint64 > mCurrentRAMUsageAtomic;

    // Memory Pools
    std::forward_list< FBlock* > mTilesScheduledForClear;
    std::forward_list< FBlock* > mFreshTilesAvailableForQuery;

    // Usage Pools
    std::list< FTile* > mDirtyHashedTilesCurrentlyInUse;
    std::unordered_map< uint32, FTile* > mCorrectlyHashedTilesCurrentlyInUse;

    // Registered Blocks
    std::list< FTiledBlock* > mRegisteredTiledBlocks;

    // Parallel Concurrency Tools Mutex
    std::mutex mMutexTilesScheduledForClearLock;
    std::mutex mMutexFreshTilesAvailableForQueryLock;
    std::mutex mMutexDirtyHashedTilesCurrentlyInUseLock;
    std::mutex mMutexCorrectlyHashedTilesCurrentlyInUseLock;
    std::mutex mMutexRegisteredTiledBlocksLock;

    // Atomic Hack for Memory Pools Sizing
    std::atomic< uint32 > mNumTilesScheduledForClearAtomic;
    std::atomic< uint32 > mNumFreshTilesAvailableForQueryAtomic;

    // Threading Workers Tools
    std::atomic< bool > bRequestWorkersTerminationAtomic;
    std::thread * const mThreadDeallocatorAllocatorCleanerBackgroundWorker;
    std::thread * const mThreadHasherGarbageCollectorBackgroundWorker;
};
#pragma warning(pop)

ULIS_NAMESPACE_END

