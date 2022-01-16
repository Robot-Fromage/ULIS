// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Tile.cpp
* @author       Clement Berthaud
* @brief        This file provides declaration for the FTile class.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/Tile.h"

ULIS_NAMESPACE_BEGIN
FTile::~FTile()
{
    ULIS_ASSERT( mRefCount.load() == 0, "Bad RefCount on Delete Tile" );
}

FTile::FTile()
    : mClient( nullptr )
    , mHash( 0 )
    , mRefCount( 0 )
    , mDirty( false )
{}

FTile::FTile( tClient iPtr )
    : mClient( iPtr )
    , mHash( 0 )
    , mRefCount( 0 )
    , mDirty( true )
{}

void
FTile::DecreaseRefCount() {
    mRefCount--;
};

void
FTile::IncreaseRefCount() {
    mRefCount++;
};

ULIS_NAMESPACE_END

