// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TilePool.ippp
* @author       Clement Berthaud
* @brief        This file provides the definition for the TilePool class.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/Device.h"
#include "Image/Block.h"
#include "Sparse/Tile.h"
#include <algorithm>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// TTilePool
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
template< uint8 _MICRO, uint8 _MACRO > TTilePool< _MICRO, _MACRO >::~TTilePool() {
    bRequestWorkersTerminationAtomic.store( true );

    mThreadDeallocatorAllocatorCleanerBackgroundWorker->join();
    mThreadHasherGarbageCollectorBackgroundWorker->join();

    delete  mThreadDeallocatorAllocatorCleanerBackgroundWorker;
    delete  mThreadHasherGarbageCollectorBackgroundWorker;

    PurgeAllNow();

    delete  mEmptyTile;
}

template< uint8 _MICRO, uint8 _MACRO > TTilePool< _MICRO, _MACRO >::TTilePool( eFormat iFormat, FColorSpace* iProfile )
    : mTileSize                                             ( scPixelDim, scPixelDim                                                                                        )
    , mTileFormat                                           ( iFormat                                                                                                       )
    , mTileColorProfile                                     ( iProfile                                                                                                      )
    , mEmptyTile                                            ( new FBlock( mTileSize.x, mTileSize.y, mTileFormat, mTileColorProfile )                                        )
    , mBytesPerTile                                         ( mEmptyTile->BytesTotal()                                                                                      )
    , mSWAPUsageCapTargetAtomic                             ( 0                                                                                                             )
    , mRAMUsageCapTargetAtomic                              ( 0                                                                                                             )
    , mCurrentRAMUsageAtomic                                ( 0                                                                                                             )
    , mTilesScheduledForClear                               ( std::forward_list< FBlock* >()                                                                                )
    , mFreshTilesAvailableForQuery                          ( std::forward_list< FBlock* >()                                                                                )
    , mDirtyHashedTilesCurrentlyInUse                       ( std::list< FTileElement* >()                                                                                  )
    , mCorrectlyHashedTilesCurrentlyInUse                   ( std::unordered_map< uint32, FTileElement* >()                                                                 )
    , mRegisteredTiledBlocks                                ( std::list< tTiledBlock* >()                                                                                   )
    , mMutexTilesScheduledForClearLock                      (                                                                                                               )
    , mMutexFreshTilesAvailableForQueryLock                 (                                                                                                               )
    , mMutexDirtyHashedTilesCurrentlyInUseLock              (                                                                                                               )
    , mMutexCorrectlyHashedTilesCurrentlyInUseLock          (                                                                                                               )
    , mMutexRegisteredTiledBlocksLock                       (                                                                                                               )
    , mNumTilesScheduledForClearAtomic                      ( 0                                                                                                             )
    , mNumFreshTilesAvailableForQueryAtomic                 ( 0                                                                                                             )
    , bRequestWorkersTerminationAtomic                      ( false                                                                                                         )
    , mThreadDeallocatorAllocatorCleanerBackgroundWorker    ( new std::thread( &TTilePool< _MICRO, _MACRO >::ThreadedDeallocatorAllocatorCleanerBackgroundWorker, this )    )
    , mThreadHasherGarbageCollectorBackgroundWorker         ( new std::thread( &TTilePool< _MICRO, _MACRO >::ThreadedHasherGarbageCollectorBackgroundWorker, this )         )
{
    ClearRaw( mEmptyTile );
    mEmptyCRC32Hash = mEmptyTile->CRC32();
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
template< uint8 _MICRO, uint8 _MACRO > const FVec2I&        TTilePool< _MICRO, _MACRO >::TileSize()                                 const   { return  mTileSize;                                    }
template< uint8 _MICRO, uint8 _MACRO > uint32               TTilePool< _MICRO, _MACRO >::EmptyCRC32Hash()                           const   { return  mEmptyCRC32Hash;                              }
template< uint8 _MICRO, uint8 _MACRO > const FBlock*        TTilePool< _MICRO, _MACRO >::EmptyTile()                                const   { return  mEmptyTile;                                   }
template< uint8 _MICRO, uint8 _MACRO > eFormat              TTilePool< _MICRO, _MACRO >::TileFormat()                               const   { return  mTileFormat;                                  }
template< uint8 _MICRO, uint8 _MACRO > const FFormatMetrics&   TTilePool< _MICRO, _MACRO >::TileFormatMetrics()                           const   { return  mEmptyTile->FormatMetrics();                     }
template< uint8 _MICRO, uint8 _MACRO > const FColorSpace* TTilePool< _MICRO, _MACRO >::TileColorProfile()                         const   { return  mTileColorProfile;                            }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::CurrentRAMUsage()                          const   { return  mCurrentRAMUsageAtomic.load();                }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::CurrentSwapUsage()                         const   { return  0;                                            }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::RAMUsageCapTarget()                        const   { return  mRAMUsageCapTargetAtomic.load();              }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::SWAPUsageCapTarget()                       const   { return  mSWAPUsageCapTargetAtomic.load();             }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::CurrentTotalMemoryUsage()                  const   { return  CurrentRAMUsage();                            }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::NumTilesScheduledForClear()                const   { return  mNumTilesScheduledForClearAtomic.load();      }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::NumFreshTilesAvailableForQuery()           const   { return  mNumFreshTilesAvailableForQueryAtomic.load(); }
template< uint8 _MICRO, uint8 _MACRO > void                 TTilePool< _MICRO, _MACRO >::SetRAMUsageCapTarget( uint64 iValue )              { mRAMUsageCapTargetAtomic.store( iValue );             }
template< uint8 _MICRO, uint8 _MACRO > void                 TTilePool< _MICRO, _MACRO >::SetSWAPUsageCapTarget( uint64 iValue )             { mSWAPUsageCapTargetAtomic.store( iValue );            }

template< uint8 _MICRO, uint8 _MACRO > uint64 TTilePool< _MICRO, _MACRO >::NumDirtyHashedTilesCurrentlyInUse() {
    const std::lock_guard<std::mutex> lock( mMutexDirtyHashedTilesCurrentlyInUseLock );
    return  mDirtyHashedTilesCurrentlyInUse.size();
}

template< uint8 _MICRO, uint8 _MACRO > uint64 TTilePool< _MICRO, _MACRO >::NumCorrectlyHashedTilesCurrentlyInUse() {
    const std::lock_guard<std::mutex> lock( mMutexCorrectlyHashedTilesCurrentlyInUseLock );
    return  mCorrectlyHashedTilesCurrentlyInUse.size();
}

template< uint8 _MICRO, uint8 _MACRO > uint64 TTilePool< _MICRO, _MACRO >::NumRegisteredTiledBlocks() {
    const std::lock_guard<std::mutex> lock( mMutexRegisteredTiledBlocksLock );
    return  mRegisteredTiledBlocks.size();
}

template< uint8 _MICRO, uint8 _MACRO > TTilePool< _MICRO, _MACRO >::tTiledBlock* TTilePool< _MICRO, _MACRO >::CreateNewTiledBlock() {
    const std::lock_guard<std::mutex> lock( mMutexRegisteredTiledBlocksLock );
    tTiledBlock* block = new tTiledBlock( this );
    mRegisteredTiledBlocks.emplace_back( block );
    return  block;
}

template< uint8 _MICRO, uint8 _MACRO > void TTilePool< _MICRO, _MACRO >::RequestTiledBlockDeletion( tTiledBlock* iBlock ) {
    const std::lock_guard<std::mutex> lock( mMutexRegisteredTiledBlocksLock );
    ULIS_ASSERT( iBlock, "Bad TiledBlock Deletion Request, this tiledblock is not the right type !" );
    auto it = std::find( mRegisteredTiledBlocks.begin(), mRegisteredTiledBlocks.end(), iBlock );
    ULIS_ASSERT( it != mRegisteredTiledBlocks.end(), "Bad TiledBlock Deletion Request, this tiledblock is not in this pool or has already been deleted !" );
    if( it != mRegisteredTiledBlocks.end() ) {
        mRegisteredTiledBlocks.erase( it );
        delete *it;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Core API
template< uint8 _MICRO, uint8 _MACRO >
void
TTilePool< _MICRO, _MACRO >::PurgeAllNow() {
    mMutexTilesScheduledForClearLock.lock();
    mMutexFreshTilesAvailableForQueryLock.lock();
    mMutexDirtyHashedTilesCurrentlyInUseLock.lock();
    mMutexCorrectlyHashedTilesCurrentlyInUseLock.lock();
    mMutexRegisteredTiledBlocksLock.lock();

    for( auto& it : mRegisteredTiledBlocks ) {
        delete it;
    }

    for( auto& it : mTilesScheduledForClear )
        delete it;

    for( auto& it : mFreshTilesAvailableForQuery )
        delete it;

    for( auto& it : mDirtyHashedTilesCurrentlyInUse ) {
        delete it->mBlock;
        delete it;
    }

    for( auto& it : mCorrectlyHashedTilesCurrentlyInUse ) {
        delete it.second->mBlock;
        delete it.second;
    }

    mTilesScheduledForClear.clear();
    mFreshTilesAvailableForQuery.clear();
    mDirtyHashedTilesCurrentlyInUse.clear();
    mCorrectlyHashedTilesCurrentlyInUse.clear();
    mRegisteredTiledBlocks.clear();
    mCurrentRAMUsageAtomic = 0;
    SetRAMUsageCapTarget( 0 );
    SetSWAPUsageCapTarget( 0 );
    mNumTilesScheduledForClearAtomic = 0;
    mNumTilesScheduledForClearAtomic = 0;

    mMutexTilesScheduledForClearLock.unlock();
    mMutexFreshTilesAvailableForQueryLock.unlock();
    mMutexDirtyHashedTilesCurrentlyInUseLock.unlock();
    mMutexCorrectlyHashedTilesCurrentlyInUseLock.unlock();
    mMutexRegisteredTiledBlocksLock.unlock();
}

template< uint8 _MICRO, uint8 _MACRO >
FTileElement*
TTilePool< _MICRO, _MACRO >::XQueryFreshTile() {

    mMutexFreshTilesAvailableForQueryLock.lock();

    if( mFreshTilesAvailableForQuery.empty() )
        ClearNowDirect_Unsafe( 1 );

    FBlock* block = mFreshTilesAvailableForQuery.front();
    mFreshTilesAvailableForQuery.pop_front();
    mMutexFreshTilesAvailableForQueryLock.unlock();
    --mNumFreshTilesAvailableForQueryAtomic;

    FTileElement* tile = new FTileElement( block );
    // Here we increase the refcount before inserting in the Dirty list,
    // so that parallel sanitize operation beetwen the allocation here,
    // and return from this function or ulterior usage can still safely
    // use the tile without it being deleted.
    tile->IncreaseRefCount();

    mMutexDirtyHashedTilesCurrentlyInUseLock.lock();
    mDirtyHashedTilesCurrentlyInUse.push_back( tile );
    mMutexDirtyHashedTilesCurrentlyInUseLock.unlock();

    return  tile;
}

template< uint8 _MICRO, uint8 _MACRO >
FTileElement*
TTilePool< _MICRO, _MACRO >::PerformRedundantHashMergeReturnCorrect( FTileElement* iElem ) {
    ULIS_ASSERT( iElem, "Bad Elem Query during Hash Merge Check" );

    // If the hashed tile is empty we return null ptr and decrease refcount
    if( iElem->mHash == mEmptyCRC32Hash ) {
        iElem->DecreaseRefCount();
        return  nullptr;
    }

    // Find the hashed tile in the map if not empty
    mMutexCorrectlyHashedTilesCurrentlyInUseLock.lock();
    std::unordered_map< uint32, FTileElement* >::iterator it = mCorrectlyHashedTilesCurrentlyInUse.find( iElem->mHash );
    std::unordered_map< uint32, FTileElement* >::iterator end = mCorrectlyHashedTilesCurrentlyInUse.end();
    mMutexCorrectlyHashedTilesCurrentlyInUseLock.unlock();

    if( it == end ) {
        mMutexFreshTilesAvailableForQueryLock.lock();
        if( mFreshTilesAvailableForQuery.empty() ) {
            FBlock* block = new FBlock( mTileSize.x, mTileSize.y, mTileFormat, mTileColorProfile );
            mCurrentRAMUsageAtomic.fetch_add( mBytesPerTile );
            ClearRaw( block );
            mFreshTilesAvailableForQuery.emplace_front( block );
            ++mNumFreshTilesAvailableForQueryAtomic;
        }

        FBlock* block = mFreshTilesAvailableForQuery.front();
        mFreshTilesAvailableForQuery.pop_front();
        mMutexFreshTilesAvailableForQueryLock.unlock();
        --mNumFreshTilesAvailableForQueryAtomic;
        FTileElement* tile = new FTileElement( block );
        tile->mDirty = false;
        tile->mHash = iElem->mHash;
        CopyRaw( iElem->mBlock, tile->mBlock, false );

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

template< uint8 _MICRO, uint8 _MACRO >
FTileElement*
TTilePool< _MICRO, _MACRO >::XPerformDataCopyForImminentMutableChangeIfNeeded( FTileElement* iElem ) {
    ULIS_ASSERT( iElem, "Bad Elem Query during Hash Merge Check" );
    const std::lock_guard<std::mutex> lock_correct( mMutexCorrectlyHashedTilesCurrentlyInUseLock );
    std::unordered_map< uint32, FTileElement* >::iterator it   = mCorrectlyHashedTilesCurrentlyInUse.find( iElem->mHash );
    std::unordered_map< uint32, FTileElement* >::iterator end  = mCorrectlyHashedTilesCurrentlyInUse.end();
    bool dirty  = iElem->mDirty.load();
    bool shared = iElem->mRefCount > 1;
    bool found  = it != end;
    bool same   = found ? it->second == iElem : false;

    if( shared ) {
        FTileElement* tile = XQueryFreshTile();
        CopyRaw( iElem->mBlock, tile->mBlock, false );
        iElem->DecreaseRefCount();
        return  tile;
    } else {
        if( same ) {
            const std::lock_guard<std::mutex> lock_dirty( mMutexDirtyHashedTilesCurrentlyInUseLock );
            mDirtyHashedTilesCurrentlyInUse.push_back( iElem );
            mCorrectlyHashedTilesCurrentlyInUse.erase( it );
            return  iElem;
        } else {
            return  iElem;
        }
    }
}

template< uint8 _MICRO, uint8 _MACRO > void TTilePool< _MICRO, _MACRO >::AllocateNow_Unsafe( int32 iNum ) {
    for( int i = 0; i < iNum; ++i )
        mTilesScheduledForClear.emplace_front( new FBlock( mTileSize.x, mTileSize.y, mTileFormat, mTileColorProfile ) );

    mNumTilesScheduledForClearAtomic.fetch_add( iNum );
    mCurrentRAMUsageAtomic.fetch_add( mBytesPerTile * iNum );
}

template< uint8 _MICRO, uint8 _MACRO > void TTilePool< _MICRO, _MACRO >::ClearNowDirect_Unsafe( int32 iNum ) {
    for( int i = 0; i < iNum; ++i ) {
        FBlock* block = new FBlock( mTileSize.x, mTileSize.y, mTileFormat, mTileColorProfile );
        ClearRaw( block );
        mFreshTilesAvailableForQuery.emplace_front( block );
    }

    mNumFreshTilesAvailableForQueryAtomic.fetch_add( iNum );
    mCurrentRAMUsageAtomic.fetch_add( mBytesPerTile * iNum );
}

template< uint8 _MICRO, uint8 _MACRO > void TTilePool< _MICRO, _MACRO >::DeallocOneInTilesScheduledForClearIfRAMOverflowsTarget() {
    if( mCurrentRAMUsageAtomic.load() > mRAMUsageCapTargetAtomic.load() )
    {
        const std::lock_guard<std::mutex> lock( mMutexTilesScheduledForClearLock );
        if( !( mTilesScheduledForClear.empty() ) )
        {
            FBlock* block = mTilesScheduledForClear.front();
            mTilesScheduledForClear.pop_front();
            delete  block;
            --mNumTilesScheduledForClearAtomic;
            mCurrentRAMUsageAtomic.fetch_sub( mBytesPerTile );
        }
    }
}

template< uint8 _MICRO, uint8 _MACRO > void TTilePool< _MICRO, _MACRO >::DeallocOneInFreshTilesAvailableForQueryIfRAMOverflowsTargetAndTilesScheduledForClearIsEmptyImprecise() {
    if( mCurrentRAMUsageAtomic.load() > mRAMUsageCapTargetAtomic.load() )
    {
        mMutexTilesScheduledForClearLock.lock();
        bool clearEmpty = mTilesScheduledForClear.empty();
        mMutexTilesScheduledForClearLock.unlock();

        const std::lock_guard<std::mutex> lock( mMutexFreshTilesAvailableForQueryLock );
        if( clearEmpty && !( mFreshTilesAvailableForQuery.empty() ) )
        {
            FBlock* block = mFreshTilesAvailableForQuery.front();
            mFreshTilesAvailableForQuery.pop_front();
            delete  block;
            --mNumFreshTilesAvailableForQueryAtomic;
            mCurrentRAMUsageAtomic.fetch_sub( mBytesPerTile );
        }
    }
}

template< uint8 _MICRO, uint8 _MACRO > void TTilePool< _MICRO, _MACRO >::AllocOneInTilesScheduledForClearIfRAMUnderflowsTarget() {
    if( mCurrentRAMUsageAtomic.load() < mRAMUsageCapTargetAtomic.load() )
    {
        const std::lock_guard<std::mutex> lock( mMutexTilesScheduledForClearLock );
        mTilesScheduledForClear.emplace_front( new FBlock( mTileSize.x, mTileSize.y, mTileFormat, mTileColorProfile ) );
        ++mNumTilesScheduledForClearAtomic;
        mCurrentRAMUsageAtomic.fetch_add( mBytesPerTile );
    }
}

template< uint8 _MICRO, uint8 _MACRO > void TTilePool< _MICRO, _MACRO >::ClearOneInTilesScheduledForClearAndMoveToFreshTilesAvailableForQueryIfNeeded() {
    const std::lock_guard<std::mutex> lock( mMutexTilesScheduledForClearLock );
    if( !( mTilesScheduledForClear.empty() ) )
    {
        FBlock* block = mTilesScheduledForClear.front();
        mTilesScheduledForClear.pop_front();
        --mNumTilesScheduledForClearAtomic;

        ClearRaw( block );

        mMutexFreshTilesAvailableForQueryLock.lock();
        mFreshTilesAvailableForQuery.emplace_front( block );
        ++mNumFreshTilesAvailableForQueryAtomic;
        mMutexFreshTilesAvailableForQueryLock.unlock();
    }
}

template< uint8 _MICRO, uint8 _MACRO > void TTilePool< _MICRO, _MACRO >::SanitizeAllDirtyTilesCurrentlyInUse() {
    std::list< FTileElement* >::iterator it = mDirtyHashedTilesCurrentlyInUse.end();
    bool first = true;
    while( true ) {

        const std::lock_guard<std::mutex> lock( mMutexDirtyHashedTilesCurrentlyInUseLock );

        if( first ) {
            it = mDirtyHashedTilesCurrentlyInUse.begin();
            first = false;
        }

        if( it == mDirtyHashedTilesCurrentlyInUse.end() )
            break;

        FTileElement* tile = *it;
        if( tile->mRefCount.load() == 0 ) {
            mMutexTilesScheduledForClearLock.lock();
            mTilesScheduledForClear.push_front( tile->mBlock );
            mMutexTilesScheduledForClearLock.unlock();
            ++mNumTilesScheduledForClearAtomic;
            delete  tile;
            it = mDirtyHashedTilesCurrentlyInUse.erase( it );
        }
        else
        {
            if( tile->mDirty.load() ) {
                tile->mHash = tile->mBlock->CRC32();
                tile->mDirty.store( false );
            }
            ++it;
        }
    }
}

template< uint8 _MICRO, uint8 _MACRO > void TTilePool< _MICRO, _MACRO >::SanitizeAllCorrectlyHashedTilesCurrentlyInUse() {
    std::unordered_map< uint32, FTileElement* >::iterator it = mCorrectlyHashedTilesCurrentlyInUse.end();
    bool first = true;
    while( true ) {

        const std::lock_guard<std::mutex> lock( mMutexCorrectlyHashedTilesCurrentlyInUseLock );

        if( first ) {
            it = mCorrectlyHashedTilesCurrentlyInUse.begin();
            first = false;
        }

        if( it == mCorrectlyHashedTilesCurrentlyInUse.end() )
            break;

        FTileElement* tile = it->second;
        if( tile->mRefCount.load() == 0 ) {
            mMutexTilesScheduledForClearLock.lock();
            mTilesScheduledForClear.push_front( tile->mBlock );
            mMutexTilesScheduledForClearLock.unlock();
            ++mNumTilesScheduledForClearAtomic;
            delete  tile;
            it = mCorrectlyHashedTilesCurrentlyInUse.erase( it );
        }
        else
        {
            ++it;
        }
    }
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Private Workers API
template< uint8 _MICRO, uint8 _MACRO > void TTilePool< _MICRO, _MACRO >::ThreadedDeallocatorAllocatorCleanerBackgroundWorker() {
    for(;;) {
        if( bRequestWorkersTerminationAtomic.load() ) return;

        DeallocOneInTilesScheduledForClearIfRAMOverflowsTarget();
        DeallocOneInFreshTilesAvailableForQueryIfRAMOverflowsTargetAndTilesScheduledForClearIsEmptyImprecise();
        AllocOneInTilesScheduledForClearIfRAMUnderflowsTarget();
        ClearOneInTilesScheduledForClearAndMoveToFreshTilesAvailableForQueryIfNeeded();

        // Relax
        std::this_thread::sleep_for( std::chrono::duration< double, std::pico >( 0 ) );
    }
}

template< uint8 _MICRO, uint8 _MACRO > void TTilePool< _MICRO, _MACRO >::ThreadedHasherGarbageCollectorBackgroundWorker() {
    for(;;) {
        if( bRequestWorkersTerminationAtomic.load() ) return;

        SanitizeAllDirtyTilesCurrentlyInUse();
        SanitizeAllCorrectlyHashedTilesCurrentlyInUse();

        // Relax
        std::this_thread::sleep_for( std::chrono::duration< double, std::milli >( 50.0 ) );
    }
}

ULIS_NAMESPACE_END

