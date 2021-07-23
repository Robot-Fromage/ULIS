// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TiledBlock.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the TiledBlock class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Sparse/LinearQuadtree.h"
#include <unordered_map>

ULIS_NAMESPACE_BEGIN
class FTilePool;
class FLQTree;
#pragma warning(push)
#pragma warning(disable : 4251) // Shut warning C4251 dll export of stl classes
/////////////////////////////////////////////////////
/// @class      FTiledBlock
/// @brief      The FTiledBlock class provides a mean of storing very large tiled images,
///             powered by a large number of complex algorithms allowing efficient memory
///             usage and performances, as well as disk streaming facilities, compression
///             systems, redundant data reuse by hashing pixel data, low overall rates in
///             memory fragmentation, infinite or unbounded images in space and time, within
///             reasonable range ( depends on the user available disk space and ram space ),
///             and multithreaded operations.
class ULIS_API FTiledBlock
{
public:
    ~FTiledBlock();
    FTiledBlock( FTilePool* iTilePool );

public:
    // Core API
    uint64 NumChunks() const;
    void GatherChunks( TArray< FLQTree* >* oVec );
    bool IsValidPixelCoordRange( int64 iValue ) const;
    FVec2I ChunkCoordinatesFromPixelCoordinates( const FVec2I& iPos ) const;
    FVec2I PixelCoordinatesFromChunkCoordinates( const FVec2I& iPos ) const;
    uint64 KeyFromChunkCoordinates( const FVec2I& iPos ) const;
    uint64 KeyFromPixelCoordinates( const FVec2I& iPos ) const;
    FVec2I ChunkCoordinatesFromKey( uint64 iKey ) const;
    FVec2I PixelCoordinatesFromKey( uint64 iKey ) const;

public:
    // Block API
    const FRectI& OperativeGeometry() const;
    const FRectI& RootGeometry() const;
    const FRectI& LeafGeometry() const;
    void ExtendOperativeGeometryAfterMutableChange( const FRectI& iRect );
    void SubstractOperativeGeometryAfterMutableChange( const FRectI& iRect );
    void RecomputeRootGeometry();
    void RecomputeLeafGeometry();

private:
    // Private API
    FLQTree* QueryChunkRW( const FVec2I& iPos );
    const FLQTree* QueryChunkR( const FVec2I& iPos ) const;

public:
    // Tile API
    const uint8* QueryConstTile( const FVec2I& iPos ) const;
    FTile** QueryMutableTile( const FVec2I& iPos );
    void Clear();
    void SanitizeNow();

private:
    // Private Data Members
    // key uint64
    // 1) 32bits: packed int32 Y
    // 2) 32bits: packed int32 X
    std::unordered_map< uint64, FLQTree* > mChunks;
    FTilePool* mTilePool;
    FRectI mOperativeGeometry;
    FRectI mRootGeometry;
    FRectI mLeafGeometry;

    static constexpr int32 sm_chunk_min_coord = std::numeric_limits< int32 >::min();
    static constexpr int32 sm_chunk_max_coord = std::numeric_limits< int32 >::max();
    static constexpr int64 sm_pixel_min_coord = static_cast< int64 >( sm_chunk_min_coord ) * FLQTree::sm_root_size_as_pixels;
    static constexpr int64 sm_pixel_max_coord = static_cast< int64 >( sm_chunk_max_coord ) * FLQTree::sm_root_size_as_pixels;
};
#pragma warning(pop)

ULIS_NAMESPACE_END

