// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TilePool.cpp
* @author       Clement Berthaud
* @brief        This file provides declaration for the TilePool class.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/TilePool.h"
#include "Image/Block.h"
#include <algorithm>

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

void
FTilePool::PrintDiagnosis() {
    std::cout << "====== FTilePool" << std::endl;
    std::cout << "Num registered TileBlocks: " << mRegisteredTiledBlocks.size() << std::endl;
    std::cout << "Empty Tile Hash: " << mEmptyCRC32Hash << std::endl;
    std::cout << "Bytes Per Tile: " << mBytesPerTile << "o" << std::endl;
    std::cout << "Tile Size: " << TileSize().x << " * " << TileSize().y << std::endl;
    mMemoryDriver->PrintDiagnosis();
}

void
FTilePool::RegisterTiledBlock( FTiledBlock* iBlock ) {
    mRegisteredTiledBlocks.push_back( iBlock );
}

void
FTilePool::UnregisterTiledBlock( FTiledBlock* iBlock ) {
    auto f = std::find( mRegisteredTiledBlocks.begin(), mRegisteredTiledBlocks.end(), iBlock );
    if( f != mRegisteredTiledBlocks.end() )
        mRegisteredTiledBlocks.erase( f );
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
    return  mMemoryDriver->SplitMutable( iElem );
}

ULIS_NAMESPACE_END

