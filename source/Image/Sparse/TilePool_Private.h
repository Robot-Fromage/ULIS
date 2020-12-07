// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TilePool.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TilePool class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Image/Sparse/TilePool.h"
#include "Image/Sparse/TiledBlock.h"
#include "Image/Sparse/TiledBlock_Private.h"

#include <static_math/static_math.h>

#include <atomic>
#include <forward_list>
#include <list>
#include <mutex>
#include <thread>
#include <unordered_map>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// TTilePool
template< uint8 _MICRO
        , uint8 _MACRO >
class TTilePool : public ITilePool
{
    typedef  TTiledBlock< _MICRO, _MACRO > tTiledBlock;

public:
    // Construction / Destruction
    virtual ~TTilePool();
    TTilePool( eFormat          iFormat
             , FColorSpace*   iProfile );

public:
    // Public API
    const FVec2I&                               TileSize()                              const;
    uint32                                      EmptyCRC32Hash()                        const;
    const FBlock*                               EmptyTile()                             const;
    eFormat                                     TileFormat()                            const;
    const FFormatMetrics&                          TileFormatMetrics()                        const;
    const FColorSpace*                        TileColorProfile()                      const;
    uint64                                      CurrentRAMUsage()                       const;
    uint64                                      CurrentSwapUsage()                      const;
    uint64                                      RAMUsageCapTarget()                     const;
    uint64                                      SWAPUsageCapTarget()                    const;
    uint64                                      CurrentTotalMemoryUsage()               const;
    uint64                                      NumTilesScheduledForClear()             const;
    uint64                                      NumFreshTilesAvailableForQuery()        const;
    uint64                                      NumDirtyHashedTilesCurrentlyInUse();
    uint64                                      NumCorrectlyHashedTilesCurrentlyInUse();
    uint64                                      NumRegisteredTiledBlocks();
    ITiledBlock*                                CreateNewTiledBlock();
    void                                        RequestTiledBlockDeletion( ITiledBlock* iBlock );
    void                                        SetRAMUsageCapTarget( uint64 iValue );
    void                                        SetSWAPUsageCapTarget( uint64 iValue );

public:
    // Core API
    void                                        PurgeAllNow();
    FTileElement*                               XQueryFreshTile();
    FTileElement*                               PerformRedundantHashMergeReturnCorrect( FTileElement* iElem );
    FTileElement*                               XPerformDataCopyForImminentMutableChangeIfNeeded( FTileElement* iElem );

private:
    // Internal API
    void    AllocateNow_Unsafe( int32 iNum );
    void    ClearNowDirect_Unsafe( int32 iNum );
    void    DeallocOneInTilesScheduledForClearIfRAMOverflowsTarget();
    void    DeallocOneInFreshTilesAvailableForQueryIfRAMOverflowsTargetAndTilesScheduledForClearIsEmptyImprecise();
    void    AllocOneInTilesScheduledForClearIfRAMUnderflowsTarget();
    void    ClearOneInTilesScheduledForClearAndMoveToFreshTilesAvailableForQueryIfNeeded();
    void    SanitizeAllDirtyTilesCurrentlyInUse();
    void    SanitizeAllCorrectlyHashedTilesCurrentlyInUse();

private:
    // Private Workers API
    void  ThreadedDeallocatorAllocatorCleanerBackgroundWorker();
    void  ThreadedHasherGarbageCollectorBackgroundWorker();

private:
    // Private Data Members
    // Static sizing
    static constexpr int                                            scPixelDim = ::smath::pow( 2, _MICRO );

    // Runtime Tile Info
    const FVec2I                                                    mTileSize;
    const eFormat                                                   mTileFormat;
    FColorSpace         * const                                   mTileColorProfile;

    // Empty Tile Ref
    FBlock                * const                                   mEmptyTile;
    uint32                                                          mEmptyCRC32Hash;
    const uint64                                                    mBytesPerTile;

    // Atomic memory Intent
    std::atomic< uint64 >                                           mSWAPUsageCapTargetAtomic;
    std::atomic< uint64 >                                           mRAMUsageCapTargetAtomic;
    std::atomic< uint64 >                                           mCurrentRAMUsageAtomic;

    // Memory Pools
    std::forward_list< FBlock* >                                    mTilesScheduledForClear;
    std::forward_list< FBlock* >                                    mFreshTilesAvailableForQuery;

    // Usage Pools
    std::list< FTileElement* >                                      mDirtyHashedTilesCurrentlyInUse;
    std::unordered_map< uint32, FTileElement* >                     mCorrectlyHashedTilesCurrentlyInUse;

    // Registered Blocks
    std::list< tTiledBlock* >                                       mRegisteredTiledBlocks;

    // Parallel Concurrency Tools Mutex
    std::mutex                                                      mMutexTilesScheduledForClearLock;
    std::mutex                                                      mMutexFreshTilesAvailableForQueryLock;
    std::mutex                                                      mMutexDirtyHashedTilesCurrentlyInUseLock;
    std::mutex                                                      mMutexCorrectlyHashedTilesCurrentlyInUseLock;
    std::mutex                                                      mMutexRegisteredTiledBlocksLock;

    // Atomic Hack for Memory Pools Sizing
    std::atomic< uint32 >                                           mNumTilesScheduledForClearAtomic;
    std::atomic< uint32 >                                           mNumFreshTilesAvailableForQueryAtomic;

    // Threading Workers Tools
    std::atomic< bool >                                             bRequestWorkersTerminationAtomic;
    std::thread * const                                             mThreadDeallocatorAllocatorCleanerBackgroundWorker;
    std::thread * const                                             mThreadHasherGarbageCollectorBackgroundWorker;
};

ULIS_NAMESPACE_END

#include "TilePool_Private.ipp"

