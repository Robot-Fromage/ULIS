// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         TiledBlock.ipp
* @author       Clement Berthaud
* @brief        This file provides the definition for the TTiledBlock class.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Data/Sparse/TiledBlock.h"


#define ULIS2_SPARSE_MASK_X 0x00000000FFFFFFFF
#define ULIS2_SPARSE_MASK_Y 0xFFFFFFFF00000000

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// TTiledBlock
template< uint8 _MICRO, uint8 _MACRO > const FVec2I64 TTiledBlock< _MICRO, _MACRO >::modLeaf = FVec2I64( static_cast< int64 >( micro_chunk_size_as_pixels ) );
template< uint8 _MICRO, uint8 _MACRO > const FVec2I64 TTiledBlock< _MICRO, _MACRO >::modRoot = FVec2I64( static_cast< int64 >( macro_chunk_size_as_pixels ) );
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
template< uint8 _MICRO, uint8 _MACRO > TTiledBlock< _MICRO, _MACRO >::~TTiledBlock() {
    Clear();
}


template< uint8 _MICRO, uint8 _MACRO > TTiledBlock< _MICRO, _MACRO >::TTiledBlock( tTilePool* iPool )
    : tSuperClass()
    , mTilePool( iPool )
    , mShift()
    , mGeometry()
{
    static_assert( _MICRO > 0, "_MICRO template argument cannot be 0" );
    static_assert( _MACRO > 0, "_MACRO template argument cannot be 0" );
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Core API
template< uint8 _MICRO, uint8 _MACRO >
size_t
TTiledBlock< _MICRO, _MACRO >::NumRootEntries() const {
    return  mSparseMap.size();
}


template< uint8 _MICRO, uint8 _MACRO >
void
TTiledBlock< _MICRO, _MACRO >::Purge() {
    for( auto& i : mSparseMap )
        delete  i.second;
    mSparseMap.clear();
}


template< uint8 _MICRO, uint8 _MACRO >
void
TTiledBlock< _MICRO, _MACRO >::GatherRootEntries( std::vector< tRootChunk* >* oVector ) {
    oVector->clear();
    oVector->reserve( mSparseMap.size() );
    for( auto& i : mSparseMap )
        oVector->push_back( i.second );
}


template< uint8 _MICRO, uint8 _MACRO >
bool
TTiledBlock< _MICRO, _MACRO >::IsValidPixelCoordRange( int64 iValue )  const {
    return  iValue > pixel_min_coord && iValue < pixel_max_coord; 
}


template< uint8 _MICRO, uint8 _MACRO >
FVec2I32
TTiledBlock< _MICRO, _MACRO >::ChunkCoordinatesFromPixelCoordinates( const FVec2I64& iPos ) const {
    ULIS2_ASSERT( IsValidPixelCoordRange( iPos.x ) && IsValidPixelCoordRange( iPos.y ), "Bad coordinates" );
    int32 X = static_cast< int32 >( FMaths::RoundToNegativeInfinity( iPos.x / static_cast< float >( macro_chunk_size_as_pixels ) ) );
    int32 Y = static_cast< int32 >( FMaths::RoundToNegativeInfinity( iPos.y / static_cast< float >( macro_chunk_size_as_pixels ) ) );
    return  FVec2I32( X, Y );
}


template< uint8 _MICRO, uint8 _MACRO >
FVec2I64
TTiledBlock< _MICRO, _MACRO >::PixelCoordinatesFromChunkCoordinates( const FVec2I32& iPos ) const {
    int64 X = static_cast< int64 >( iPos.x * static_cast< int64 >( macro_chunk_size_as_pixels ) );
    int64 Y = static_cast< int64 >( iPos.y * static_cast< int64 >( macro_chunk_size_as_pixels ) );
    return  FVec2I64( X, Y );
}


template< uint8 _MICRO, uint8 _MACRO >
uint64
TTiledBlock< _MICRO, _MACRO >::KeyFromChunkCoordinates( const FVec2I32& iPos ) const {
    return  uint64( uint32( iPos.x ) ) | ( uint64( uint32( iPos.y ) ) << 32 );
}


template< uint8 _MICRO, uint8 _MACRO >
uint64
TTiledBlock< _MICRO, _MACRO >::KeyFromPixelCoordinates( const FVec2I64& iPos ) const {
    FVec2I32 pos = ChunkCoordinatesFromPixelCoordinates( iPos );
    return  KeyFromChunkCoordinates( pos );
}


template< uint8 _MICRO, uint8 _MACRO >
FVec2I32
TTiledBlock< _MICRO, _MACRO >::ChunkCoordinatesFromKey( uint64 iKey ) const {
    int32 X = int32( iKey & ULIS2_SPARSE_MASK_X );
    int32 Y = int32( ( iKey & ULIS2_SPARSE_MASK_Y ) >> 32 );
    return  FVec2I32( X, Y );
}


template< uint8 _MICRO, uint8 _MACRO >
FVec2I64
TTiledBlock< _MICRO, _MACRO >::PixelCoordinatesFromKey( uint64 iKey ) const {
    FVec2I32 pos = ChunkCoordinatesFromKey( iKey );
    return  PixelCoordinatesFromChunkCoordinates( pos );
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Block API
template< uint8 _MICRO, uint8 _MACRO >
const FVec2I&
TTiledBlock< _MICRO, _MACRO >::GetShift() const {
    return  mShift;
}


template< uint8 _MICRO, uint8 _MACRO >
void
TTiledBlock< _MICRO, _MACRO >::SetShift( const FVec2I& iShift ) {
    mShift = iShift;
}

template< uint8 _MICRO, uint8 _MACRO >
const FRect&
TTiledBlock< _MICRO, _MACRO >::GetGeometry() const {
    return  mGeometry;
}

template< uint8 _MICRO, uint8 _MACRO >
void
TTiledBlock< _MICRO, _MACRO >::ExtendRegionAfterMutableChange( const FRect& iRect ) {
    mGeometry = mGeometry | iRect;
}

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- Private API
template< uint8 _MICRO, uint8 _MACRO >
typename TTiledBlock< _MICRO, _MACRO >::tRootChunk*
TTiledBlock< _MICRO, _MACRO >::CreateRootEntryAtPixelSectorIfNotExistAndReturnPtr( const FVec2I64& iPos ) {
    return  CreateRootEntryAtChunkSectorIfNotExistAndReturnPtr( ChunkCoordinatesFromPixelCoordinates( iPos ) );
}

template< uint8 _MICRO, uint8 _MACRO >
typename TTiledBlock< _MICRO, _MACRO >::tRootChunk*
TTiledBlock< _MICRO, _MACRO >::CreateRootEntryAtChunkSectorIfNotExistAndReturnPtr( const FVec2I32& iPos ) {
    uint64 key = KeyFromChunkCoordinates( iPos );
    auto it = mSparseMap.find( key );

    if( it != mSparseMap.end() )
        return  it->second;

    tRootChunk* tmp = new tRootChunk();
    mSparseMap[ key ] = tmp;
    return  tmp;
}


template< uint8 _MICRO, uint8 _MACRO >
typename TTiledBlock< _MICRO, _MACRO >::tRootChunk*
TTiledBlock< _MICRO, _MACRO >::QueryRootEntryAtPixelSector( const FVec2I64& iPos ) const {
    return  QueryRootEntryAtChunkSector( ChunkCoordinatesFromPixelCoordinates( iPos ) );
}

template< uint8 _MICRO, uint8 _MACRO >
typename TTiledBlock< _MICRO, _MACRO >::tRootChunk*
TTiledBlock< _MICRO, _MACRO >::QueryRootEntryAtChunkSector( const FVec2I32& iPos ) const {
    uint64 key = KeyFromChunkCoordinates( iPos );
    auto it = mSparseMap.find( key );

    if( it != mSparseMap.end() )
        return  it->second;

    return  nullptr;
}


//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Tile API
template< uint8 _MICRO, uint8 _MACRO >
const FBlock*
TTiledBlock< _MICRO, _MACRO >::QueryConstBlockAtPixelCoordinates( FVec2I64 iPos, FVec2I64* oLocalCoords ) const {
    static FVec2I64 modLeaf( static_cast< int64 >( micro_chunk_size_as_pixels ) );
    static FVec2I64 modRoot( static_cast< int64 >( macro_chunk_size_as_pixels ) );
    *oLocalCoords = FMaths::PyModulo( iPos, modLeaf );
    FVec2I64 pixelCoordsInRoot = FMaths::PyModulo( iPos, modRoot );
    tRootChunk* root = QueryRootEntryAtPixelSector( iPos );
    return  root ? mTilePool->EmptyTile() : root->QueryConstBlockAtPixelCoordinates( mTilePool, pixelCoordsInRoot );
}


template< uint8 _MICRO, uint8 _MACRO >
FTileElement**
TTiledBlock< _MICRO, _MACRO >::QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( FVec2I64 iPos, FVec2I64* oLocalCoords  ) {
    *oLocalCoords = FMaths::PyModulo( iPos, modLeaf );
    return  CreateRootEntryAtPixelSectorIfNotExistAndReturnPtr( iPos )->QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( mTilePool, FMaths::PyModulo( iPos, modRoot ) );
}

template< uint8 _MICRO, uint8 _MACRO >
void
TTiledBlock< _MICRO, _MACRO >::DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) {
    for( auto& it : mSparseMap ) {
        auto pos = PixelCoordinatesFromKey( it.first );
        pos.x *= iScale;
        pos.y *= iScale;
        it.second->DrawDebugWireframe( iDst, iPos + pos, iScale );
    }
}

template< uint8 _MICRO, uint8 _MACRO >
void
TTiledBlock< _MICRO, _MACRO >::DrawDebugTileContent( FBlock* iDst, const FVec2I64& iPos ) {
    for( auto& it : mSparseMap ) {
        auto pos = PixelCoordinatesFromKey( it.first );
        it.second->DrawDebugTileContent( iDst, iPos + pos );
    }
}

template< uint8 _MICRO, uint8 _MACRO >
void
TTiledBlock< _MICRO, _MACRO >::Clear() {
    for( auto& i : mSparseMap )
        delete  i.second;
    mSparseMap.clear();
}

template< uint8 _MICRO, uint8 _MACRO >
void
TTiledBlock< _MICRO, _MACRO >::SanitizeNow() {
    std::vector< typename tMap::iterator > to_delete;
    typename tMap::iterator it = mSparseMap.begin();
    while( it != mSparseMap.end() ) {
        it->second->SanitizeNow( mTilePool );
        if( it->second->Child() == nullptr )
            to_delete.push_back( it );
        ++it;
    }

    for( auto it : to_delete )
        mSparseMap.erase( it );
}

template< uint8 _MICRO, uint8 _MACRO >
const typename TTiledBlock< _MICRO, _MACRO >::tMap&
TTiledBlock< _MICRO, _MACRO >::GetSparseMap() const {
    return  mSparseMap;
}

ULIS2_NAMESPACE_END

