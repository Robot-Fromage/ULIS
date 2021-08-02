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
using namespace units_literals;
// Construction / Destruction
FTilePool::~FTilePool() {
    PurgeAllNow();
    delete  mMemoryDriver;
    delete  mEmptyTile;
}

FTilePool::FTilePool(
          eFormat iFormat
        , FColorSpace* iColorSpace
    )
    : IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , mRegisteredTiledBlocks( std::list< FTiledBlock* >() )
    , mEmptyTile( new FBlock( FLQTree::sm_leaf_size_as_pixels, FLQTree::sm_leaf_size_as_pixels, iFormat, iColorSpace ) )
    , mEmptyCRC32Hash( 0 )
    , mBytesPerTile( mEmptyTile->BytesTotal() )
    , mMemoryDriver( nullptr )
{
    memset( mEmptyTile->Bits(), 0, mBytesPerTile );
    mEmptyCRC32Hash = mEmptyTile->CRC32();
    mMemoryDriver = new FMemoryDriver(
          mEmptyTile->Bits()
        , mEmptyCRC32Hash
        , mBytesPerTile
        , 256
        , 1_Gio
        , 1/3.f
        , 0.0
        , 1
        , 1
        , 5.0
        , 0
        , 0
    );
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
    return  mEmptyTile->Bits();
}

// Core API
void
FTilePool::PurgeAllNow() {
    for( auto& it : mRegisteredTiledBlocks )
        delete it;
    mRegisteredTiledBlocks.clear();
    mMemoryDriver->PurgeAllNow();
}

FTile*
FTilePool::QueryOne() {
    return  mMemoryDriver->QueryOne();
}

FTile*
FTilePool::RedundantHashMerge( FTile* iElem ) {
    return  mMemoryDriver->RedundantHashMerge( iElem );
}

FTile*
FTilePool::SplitMutable( FTile* iElem ) {
    return  nullptr;
}

ULIS_NAMESPACE_END

