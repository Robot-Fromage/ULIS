// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TilePool.cpp
* @author       Clement Berthaud
* @brief        This file provides declaration for the TilePool class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/TilePool.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
// Construction / Destruction
FTilePool::~FTilePool() {
    bRequestWorkersTerminationAtomic = true;
    mMemoryDriverWorker->join();
    mSanitizationDriverWorker->join();
    delete  mMemoryDriverWorker;
    delete  mSanitizationDriverWorker;
    PurgeAllNow();
    delete  mEmptyTile;
}

FTilePool::FTilePool(
          eFormat iFormat
        , FColorSpace* iColorSpace = nullptr
    )
    : IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , mEmptyTile( new FBlock( FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels, iFormat, iColorSpace ) )
    , mBytesPerTile( mEmptyTile->BytesTotal() )
    , mDirtyHashedTilesCurrentlyInUse( std::list< FTile* >() )
    , mCorrectlyHashedTilesCurrentlyInUse( std::unordered_map< uint32, FTile* >() )
    , mRegisteredTiledBlocks( std::list< FTiledBlock* >() )
    , mMutexDirtyHashedTilesCurrentlyInUseLock()
    , mMutexCorrectlyHashedTilesCurrentlyInUseLock()
    , mMutexRegisteredTiledBlocksLock()
    , bRequestWorkersTerminationAtomic( false )
    , mMemoryDriverWorker( new std::thread( &FTilePool::ThreadedDeallocatorAllocatorCleanerBackgroundWorker, this ) )
    , mSanitizationDriverWorker( new std::thread( &FTilePool::ThreadedHasherGarbageCollectorBackgroundWorker, this ) )
{
    memset( mEmptyTile->Bits(), 0, mBytesPerTile );
    mEmptyCRC32Hash = mEmptyTile->CRC32();
}



// Public API
FVec2I
FTilePool::TileSize() const {
    return  FVec2I( FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels );
}

uint32
FTilePool::EmptyCRC32Hash() const {
    return  mEmptyCRC32Hash;
}

const uint8*
FTilePool::EmptyTile() const {
    mEmptyTile;
}

FTiledBlock*
FTilePool::XNewTiledBlock() {
    const std::lock_guard<std::mutex> lock( mMutexRegisteredTiledBlocksLock );
    FTiledBlock* block = new FTiledBlock( *this );
    mRegisteredTiledBlocks.emplace_back( block );
    return  block;
}

void
FTilePool::XDeleteTiledBlock( FTiledBlock* iBlock ) {
    const std::lock_guard<std::mutex> lock( mMutexRegisteredTiledBlocksLock );
    ULIS_ASSERT( iBlock, "Bad TiledBlock Deletion Request, this tiledblock is not the right type !" );
    auto it = std::find( mRegisteredTiledBlocks.begin(), mRegisteredTiledBlocks.end(), iBlock );
    ULIS_ASSERT( it != mRegisteredTiledBlocks.end(), "Bad TiledBlock Deletion Request, this tiledblock is not in this pool or has already been deleted !" );
    if( it != mRegisteredTiledBlocks.end() ) {
        mRegisteredTiledBlocks.erase( it );
        delete *it;
    }
}



// Core API
void
FTilePool::PurgeAllNow() {
    mMutexUncompressedTilesAvailableForQuery.lock();
    mMutexDirtyHashedTilesCurrentlyInUseLock.lock();
    mMutexCorrectlyHashedTilesCurrentlyInUseLock.lock();
    mMutexRegisteredTiledBlocksLock.lock();

    for( auto& it : mRegisteredTiledBlocks ) {
        delete it;
    }

    for( auto& it : mDirtyHashedTilesCurrentlyInUse ) {
        delete it;
    }

    for( auto& it : mCorrectlyHashedTilesCurrentlyInUse ) {
        delete it.second;
    }

    mUncompressedTilesMemoryPool.UnsafeFreeAll();

    mRegisteredTiledBlocks.clear();
    mDirtyHashedTilesCurrentlyInUse.clear();
    mCorrectlyHashedTilesCurrentlyInUse.clear();
    mUncompressedTilesAvailableForQuery.clear();

    mMutexUncompressedTilesAvailableForQuery.unlock();
    mMutexDirtyHashedTilesCurrentlyInUseLock.unlock();
    mMutexCorrectlyHashedTilesCurrentlyInUseLock.unlock();
    mMutexRegisteredTiledBlocksLock.unlock();
}

FTile*
FTilePool::XQueryFreshTile() {
    // Here we increase the refcount before inserting in the Dirty list,
    // so that parallel sanitize operation beetwen the allocation here,
    // and return from this function or ulterior usage can still safely
    // use the tile without it being deleted.
    FTile* tile = new FTile( mUncompressedMemoryDriver.QueryOne() );
    tile->IncreaseRefCount();
    mBusyMemoryDriver.Submit( tile );
    return  tile;
}

FTile*
FTilePool::PerformRedundantHashMergeReturnCorrect( FTile* iElem ) {
    ULIS_ASSERT( iElem, "Bad Elem Query during Hash Merge Check" );

    // If the hashed tile is empty we return null ptr and decrease refcount
    if( iElem->mHash == mEmptyCRC32Hash ) {
        iElem->DecreaseRefCount();
        return  nullptr;
    }

    // Find the hashed tile in the map if not empty
    mMutexCorrectlyHashedTilesCurrentlyInUseLock.lock();
    std::unordered_map< uint32, FTile* >::iterator it = mCorrectlyHashedTilesCurrentlyInUse.find( iElem->mHash );
    std::unordered_map< uint32, FTile* >::iterator end = mCorrectlyHashedTilesCurrentlyInUse.end();
    mMutexCorrectlyHashedTilesCurrentlyInUseLock.unlock();

    if( it == end ) {
        mMutexUncompressedTilesAvailableForQuery.lock();
        if( mUncompressedTilesAvailableForQuery.empty() ) {
            tClient data = mUncompressedTilesMemoryPool.Malloc();
            memset( mEmptyTile->Bits(), 0, mBytesPerTile );
            mUncompressedTilesAvailableForQuery.emplace_front( data );
        }

        tClient data = mUncompressedTilesAvailableForQuery.front();
        mUncompressedTilesAvailableForQuery.pop_front();
        mMutexUncompressedTilesAvailableForQuery.unlock();
        FTile* tile = new FTile( data );
        tile->mDirty = false;
        tile->mHash = iElem->mHash;
        memcpy( *(iElem->mClient), *(tile->mClient), mBytesPerTile );

        mMutexCorrectlyHashedTilesCurrentlyInUseLock.lock();
        mCorrectlyHashedTilesCurrentlyInUse[ tile->mHash ] = tile;
        mMutexCorrectlyHashedTilesCurrentlyInUseLock.unlock();

        iElem->DecreaseRefCount();
        tile->IncreaseRefCount();
        return  tile;
    }

    if( it->second == iElem )
        return  iElem;

    it->second->IncreaseRefCount();
    iElem->DecreaseRefCount();
    return  it->second;
}

FTile*
FTilePool::XPerformDataCopyForImminentMutableChangeIfNeeded( FTile* iElem ) {
}

ULIS_NAMESPACE_END

