// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TiledBlock.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TTiledBlock class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Sparse/Tile.h"
#include "Sparse/TiledBlock.h"
#include "Sparse/Chunk.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <thread>

ULIS_NAMESPACE_BEGIN

//----------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------- eScale
enum eMicro {
    Micro_32  = 5,
    Micro_64  = 6,
    Micro_128 = 7,
    Micro_256 = 8,
};

enum eMacro {
    Macro_256   = 2,
    Macro_512   = 3,
    Macro_1024  = 4,
    Macro_2048  = 5,
};


/////////////////////////////////////////////////////
/// TTiledBlock
template< uint8 _MICRO, uint8 _MACRO >
class TTiledBlock : public ITiledBlock
{
    typedef ITiledBlock tSuperClass;
    typedef TRootChunk< _MICRO, _MACRO, _MACRO > tRootChunk;
    typedef std::unordered_map< uint64, tRootChunk* > tMap;
    typedef TTilePool< _MICRO, _MACRO > tTilePool;

public:
    // Construction / Destruction
    virtual ~TTiledBlock();
    TTiledBlock( tTilePool* iPool );

public:
    // Core API
    virtual size_t NumRootEntries() const override;
    virtual void GatherRootEntries( std::vector< tRootChunk* >* oVector );
    virtual bool IsValidPixelCoordRange( int64 iValue ) const override;
    virtual FVec2I32 ChunkCoordinatesFromPixelCoordinates( const FVec2I& iPos ) const override;
    virtual FVec2I PixelCoordinatesFromChunkCoordinates( const FVec2I32& iPos ) const override;
    virtual uint64 KeyFromChunkCoordinates( const FVec2I32& iPos ) const override;
    virtual uint64 KeyFromPixelCoordinates( const FVec2I& iPos ) const override;
    virtual FVec2I32 ChunkCoordinatesFromKey( uint64 iKey ) const override;
    virtual FVec2I PixelCoordinatesFromKey( uint64 iKey ) const override;

public:
    // Block API
    virtual const FRectI& GetOperativeGeometry() const override;
    virtual void ExtendOperativeGeometryAfterMutableChange( const FRectI& iRect ) override;
    virtual void SubstractOperativeGeometryAfterMutableChange( const FRectI& iRect ) override;
    virtual void RecomputeRoughRootGeometry() override;
    virtual void RecomputeRoughLeafGeometry() override;

private:
    // Private API
    tRootChunk* CreateRootEntryAtPixelSectorIfNotExistAndReturnPtr( const FVec2I& iPos );
    tRootChunk* CreateRootEntryAtChunkSectorIfNotExistAndReturnPtr( const FVec2I32& iPos );
    tRootChunk* QueryRootEntryAtPixelSector( const FVec2I& iPos ) const;
    tRootChunk* QueryRootEntryAtChunkSector( const FVec2I32& iPos ) const;

public:
    // Tile API
    virtual const FBlock* QueryConstBlockAtPixelCoordinates( FVec2I iPos, FVec2I* oLocalCoords ) const override;
    virtual FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( FVec2I iPos, FVec2I* oLocalCoords ) override;
    virtual void DrawDebugWireframe( FBlock* iDst, const FVec2I& iPos, float iScale ) override;
    virtual void DrawDebugTileContent( FBlock* iDst, const FVec2I& iPos ) override;
    virtual void Clear() override;
    virtual void SanitizeNow() override;
    const tMap& GetSparseMap() const;

private:
    // Private Data Members
    tMap mSparseMap;
    tTilePool* mTilePool; // Non-Owning
    FRectI mOperativeGeometry;
    FRectI mRoughRootGeometry;
    FRectI mRoughLeafGeometry;

    static constexpr uint8 micro_threshold = _MICRO;
    static constexpr uint8 macro_threshold = _MACRO;
    static constexpr uint16 micro_chunk_size_as_pixels = ::smath::pow( 2, micro_threshold );
    static constexpr uint16 macro_chunk_size_as_micro_chunks = ::smath::pow( 2, macro_threshold );
    static constexpr uint32 macro_chunk_size_as_pixels = micro_chunk_size_as_pixels * macro_chunk_size_as_micro_chunks;
    static constexpr int32 macro_chunk_min_coord = std::numeric_limits< int32 >::min();
    static constexpr int32 macro_chunk_max_coord = std::numeric_limits< int32 >::max();
    static constexpr int64 pixel_min_coord = static_cast< int64 >( macro_chunk_min_coord ) * static_cast< int64 >( macro_chunk_size_as_micro_chunks );
    static constexpr int64 pixel_max_coord = static_cast< int64 >( macro_chunk_max_coord ) * static_cast< int64 >( macro_chunk_size_as_micro_chunks );
    static const FVec2I modLeaf;
    static const FVec2I modRoot;
};

ULIS_NAMESPACE_END

#include "TiledBlock.ipp"

