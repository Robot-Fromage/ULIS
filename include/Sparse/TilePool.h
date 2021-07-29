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
#include "Memory/FixedAllocMemoryPool.h"
#include "Memory/ShrinkableAllocMemoryPool.h"

#include <atomic>
#include <forward_list>
#include <list>
#include <mutex>
#include <thread>
#include <unordered_map>

ULIS_NAMESPACE_BEGIN
#pragma warning(push)
#pragma warning(disable : 4251) // Shut warning C4251 dll export of stl classes
/////////////////////////////////////////////////////
/// @class      FTilePool
/// @brief      The FTilePool class stores redundant tiles and manages tiled blocks.
class ULIS_API FTilePool
    : public IHasFormat
    , public IHasColorSpace
{
public:
    // Construction / Destruction
    /*! Destructor. */
    ~FTilePool();

    /*! Constructor. */
    FTilePool(
          eFormat iFormat = eFormat::Format_RGBA8
        , FColorSpace* iColorSpace = nullptr
    );

    /*! Explicitely deleted copy constructor */
    FTilePool( const FTilePool& ) = delete;

    /*! Explicitely deleted copy assignment operator */
    FTilePool& operator=( const FTilePool& ) = delete;

public:
    // Public API
    FVec2I TileSize() const;
    uint32 EmptyCRC32Hash() const;
    const uint8* EmptyTile() const;

    FTiledBlock* XNewTiledBlock();
    void XDeleteTiledBlock( FTiledBlock* iBlock );

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

private:
    // Private Data Members
    // Registered Blocks
    std::list< FTiledBlock* > mRegisteredTiledBlocks;

    // Empty Tile
    FBlock* const mEmptyTile;
    uint32 mEmptyCRC32Hash;
    const uint64 mBytesPerTile;

    // Memory Pools
    // Uncompressed
    FFixedAllocMemoryPool mUncompressedTilesMemoryPool;
    std::forward_list< tClient > mUncompressedTilesAvailableForQuery;
    std::mutex mMutexUncompressedTilesAvailableForQuery;

    // Usage Pools
    std::list< FTile* > mDirtyHashedTilesCurrentlyInUse;
    std::unordered_map< uint32, FTile* > mCorrectlyHashedTilesCurrentlyInUse;

    // Parallel Concurrency Tools Mutex
    std::mutex mMutexDirtyHashedTilesCurrentlyInUseLock;
    std::mutex mMutexCorrectlyHashedTilesCurrentlyInUseLock;
    std::mutex mMutexRegisteredTiledBlocksLock;

    // Threading Workers Tools
    std::atomic< bool > bRequestWorkersTerminationAtomic;
    std::thread* const mMemoryDriverWorker;
    std::thread* const mSanitizationDriverWorker;
};
#pragma warning(pop)

ULIS_NAMESPACE_END

