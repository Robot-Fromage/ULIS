// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TiledBlock.cpp
* @author       Clement Berthaud
* @brief        This file provides declaration for the TiledBlock class.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/TiledBlock.h"
#include "Sparse/TilePool.h"
#include "Math/Math.h"

#define ULIS_SPARSE_MASK_X 0x00000000FFFFFFFF
#define ULIS_SPARSE_MASK_Y 0xFFFFFFFF00000000

ULIS_NAMESPACE_BEGIN
FTiledBlock::~FTiledBlock() {
    mTilePool.UnregisterTiledBlock( this );
    Clear();
}

FTiledBlock::FTiledBlock( FTilePool& iTilePool )
    : mChunks()
    , mTilePool( iTilePool )
    , mOperativeGeometry()
    , mRootGeometry()
    , mLeafGeometry()
{
    mTilePool.RegisterTiledBlock( this );
}

uint64
FTiledBlock::NumChunks() const {
    return  mChunks.size();
}

void
FTiledBlock::GatherChunks( TArray< FLQTree* >* oVec ) {
    oVec->Clear();
    oVec->Reserve( NumChunks() );
    for( auto i: mChunks )
        oVec->PushBack( i.second );
}

//static
bool
FTiledBlock::IsValidPixelCoordRange( int64 iValue ) {
    return  iValue > sm_pixel_min_coord && iValue < sm_pixel_max_coord;
}

FVec2I
FTiledBlock::ChunkCoordinatesFromPixelCoordinates( const FVec2I& iPos ) const {
    ULIS_ASSERT( IsValidPixelCoordRange( iPos.x ) && IsValidPixelCoordRange( iPos.y ), "Bad coordinates" );
    int32 X = static_cast< int32 >( FMath::RoundToNegativeInfinity( iPos.x / static_cast< float >( FLQTree::sm_root_size_as_pixels ) ) );
    int32 Y = static_cast< int32 >( FMath::RoundToNegativeInfinity( iPos.y / static_cast< float >( FLQTree::sm_root_size_as_pixels ) ) );
    return  FVec2I( X, Y );
}

FVec2I
FTiledBlock::PixelCoordinatesFromChunkCoordinates( const FVec2I& iPos ) const {
    int64 X = static_cast< int64 >( iPos.x * static_cast< int64 >( FLQTree::sm_root_size_as_pixels ) );
    int64 Y = static_cast< int64 >( iPos.y * static_cast< int64 >( FLQTree::sm_root_size_as_pixels ) );
    return  FVec2I( X, Y );
}

uint64
FTiledBlock::KeyFromChunkCoordinates( const FVec2I& iPos ) const {
    return  uint64( uint32( iPos.x ) ) | ( uint64( uint32( iPos.y ) ) << 32 );
}

uint64
FTiledBlock::KeyFromPixelCoordinates( const FVec2I& iPos ) const {
    FVec2I pos = ChunkCoordinatesFromPixelCoordinates( iPos );
    return  KeyFromChunkCoordinates( pos );
}

FVec2I
FTiledBlock::ChunkCoordinatesFromKey( uint64 iKey ) const {
    int32 X = int32( iKey & ULIS_SPARSE_MASK_X );
    int32 Y = int32( ( iKey & ULIS_SPARSE_MASK_Y ) >> 32 );
    return  FVec2I( X, Y );
}

FVec2I
FTiledBlock::PixelCoordinatesFromKey( uint64 iKey ) const {
    FVec2I pos = ChunkCoordinatesFromKey( iKey );
    return  PixelCoordinatesFromChunkCoordinates( pos );
}

const FRectI&
FTiledBlock::OperativeGeometry() const {
    return  mOperativeGeometry;
}

const FRectI&
FTiledBlock::RootGeometry() const {
    return  mRootGeometry;
}

const FRectI&
FTiledBlock::LeafGeometry() const {
    return  mLeafGeometry;
}

void
FTiledBlock::ExtendOperativeGeometryAfterMutableChange( const FRectI& iRect ) {
    if( iRect.Area() == 0 )
        return;

    if( mOperativeGeometry.Area() == 0 )
        mOperativeGeometry = iRect;
    else
        mOperativeGeometry = mOperativeGeometry | iRect;
}

void
FTiledBlock::SubstractOperativeGeometryAfterMutableChange( const FRectI& iRect ) {
    mOperativeGeometry = mOperativeGeometry - iRect;
}

void
FTiledBlock::RecomputeRootGeometry() {
    if( mChunks.size() == 0 ) {
        mOperativeGeometry = FRectI();
        mRootGeometry = FRectI();
        mLeafGeometry = FRectI();
        return;
    }

    mRootGeometry = FRectI();
    for( auto it = mChunks.begin(); it != mChunks.end(); ++it ) {
        auto pos = PixelCoordinatesFromKey( it->first );
        mRootGeometry = mRootGeometry.UnionLeaveEmpty(
            FRectI(
                  static_cast< int >( pos.x )
                , static_cast< int >( pos.y )
                , static_cast< int >( FLQTree::sm_root_size_as_pixels )
                , static_cast< int >( FLQTree::sm_root_size_as_pixels )
            )
        );
    }

    mOperativeGeometry = mRootGeometry & mOperativeGeometry;
    mLeafGeometry = mRootGeometry & mLeafGeometry;
}

void
FTiledBlock::RecomputeLeafGeometry() {
    if( mChunks.size() == 0 ) {
        mOperativeGeometry = FRectI();
        mRootGeometry = FRectI();
        mLeafGeometry = FRectI();
        return;
    }

    mLeafGeometry = FRectI();
    for( auto it = mChunks.begin(); it != mChunks.end(); ++it )
        mLeafGeometry = mLeafGeometry.UnionLeaveEmpty(
            it->second->LeafGeometry().Shift( PixelCoordinatesFromKey( it->first ) )
        );

    mOperativeGeometry = mOperativeGeometry & mLeafGeometry;
    mRootGeometry = mRootGeometry & mLeafGeometry;
}

FLQTree*
FTiledBlock::QueryChunkRW( const FVec2I& iPos ) {
    uint64 key = KeyFromPixelCoordinates( iPos );
    auto it = mChunks.find( key );

    if( it != mChunks.end() )
        return  it->second;

    FLQTree* tmp = new FLQTree();
    mChunks[ key ] = tmp;
    return  tmp;
}

const FLQTree*
FTiledBlock::QueryChunkR( const FVec2I& iPos ) const {
    auto it = mChunks.find( KeyFromPixelCoordinates( iPos ) );
    return  it != mChunks.end() ? it->second : nullptr;
    if( it != mChunks.end() )
        return  it->second;
}

const uint8*
FTiledBlock::QueryConstTile( const FVec2I& iPos ) const {
    FVec2I mod = FVec2I::PyModulo( iPos, FVec2I( static_cast< int64 >( FLQTree::sm_root_size_as_pixels ) ) );
    const FLQTree* chunk = QueryChunkR( iPos );
    return  chunk ? chunk->QueryConst( mTilePool, mod.x, mod.y ) : mTilePool.EmptyTile();
}

FTile**
FTiledBlock::QueryMutableTile( const FVec2I& iPos ) {
    FVec2I mod = FVec2I::PyModulo( iPos, FVec2I( static_cast< int64 >( FLQTree::sm_root_size_as_pixels ) ) );
    return  QueryChunkRW( iPos )->QueryMutable( mTilePool, mod.x, mod.y );
}

void
FTiledBlock::Clear() {
    for( auto i : mChunks )
        delete  i.second;
    mChunks.clear();
    mOperativeGeometry = FRectI();
    mRootGeometry = FRectI();
    mLeafGeometry = FRectI();
}

void
FTiledBlock::SanitizeNow() {
    std::vector< std::unordered_map< uint64, FLQTree* >::iterator > to_delete;
    typename std::unordered_map< uint64, FLQTree* >::iterator it = mChunks.begin();
    while( it != mChunks.end() ) {
        it->second->SanitizeNow( mTilePool );
        if( it->second->IsEmpty() )
            to_delete.push_back( it );
        ++it;
    }

    for( auto ittd : to_delete )
        mChunks.erase( ittd );
}


ULIS_NAMESPACE_END

