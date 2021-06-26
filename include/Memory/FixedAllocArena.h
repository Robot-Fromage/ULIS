// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FixedAllocArena.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for FixedAllocArena.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/Units.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FFixedAllocArena
/// @brief      The FFixedAllocArena class is a class that provides a single
///             configurable Arena buffer that manages allocations of fixed
///             size. It is meant to be used for tiles or any objects of fixed
///             size.
/// @details    FFixedAllocArena allocs fixed allocations inside its block, and
///             returns "clients" to the allocation, that is, a pointer to an
///             allocation.
///             Using such clients allows us to regularly defragment the Arena,
///             or multiple Arenas if used in a pool, without losing the data
///             if moved to another memory sector.
///
///             The Arena block allocates extra bytes to store some information
///             about the memory status and allocation status, to ensure a fixed
///             number of fixed allocations cells are available.
///
///             The layout of the buffer is like so:
///             - [meta][data]
///             - [data] contains the real data used by the allocation or tile.
///             - [meta] stores meta information about the allocation such as:
///                 - [arena][client]
///                 - [arena] is a pointer to the arena where the allocation resides
///                 [client] is a pointer to the allocation, that allows tracking a moving alloc in case of a defrag
///
///             FFixedAllocArena has public methods to estimate the local
///             fragmentation of the memory. To be precise, there is no issue
///             with fragmentation within a single Arena, but it is still possible
///             to measure the sparsity of the allocations. Fragmentation
///             becomes a problem when many arenas are sparse, hence wasting a
///             lot of space.
///
///             Terminology:
///             byte_t,     a size in bytes
///             tByte,      a byte value ( uint8 )
///             tAlloc,     an allocation ( uint8* )
///             Arena,      the whole system
///             block,      the underlying page buffer
///             metaBase,   the meta info before an alloc
///             client,     a pointer to an alloc that can change position
///             cell,       a fixed element in the area that can host an alloc
///             chunk,      a collection of cells, either free or used
class ULIS_API FFixedAllocArena {
    friend class FFixedAllocMemoryPool;
public:
    ~FFixedAllocArena();
    FFixedAllocArena(
          byte_t iArenaSize
        , byte_t iAllocSize
    );
    FFixedAllocArena( const FFixedAllocArena& ) = delete;
    FFixedAllocArena& operator=( const FFixedAllocArena& ) = delete;

public:
    bool IsFull() const;
    bool IsEmpty() const;
    bool IsInRange( const uint8* iAlloc ) const;

    byte_t ArenaSize() const;
    byte_t AllocSize() const;

    uint32 NumCells() const;
    uint32 NumFreeCells() const;
    uint32 NumUsedCells() const;

    tClient Malloc();
    void Free( tClient iClient );
    float LocalFragmentation() const;

    byte_t LowBlockAdress() const;
    byte_t HighBlockAdress() const;

    void Print() const;
    void DefragSelf();

    static bool IsFree( const uint8* iAlloc );
    static bool IsFree( const tClient iClient );

private:
    static void Swap( tMetaBase iFrom, tMetaBase iTo, byte_t iAllocSize );
    byte_t LargestFreeChunk() const;
    byte_t LargestUsedChunk() const;
    byte_t BlockSize() const;
    tMetaBase ChunkMetaBase( uint32 iCellIndex );
    tConstMetaBase CellMetaBase( uint32 iCellIndex ) const;
    static bool IsCellMetaBaseAvailable( tConstMetaBase iChunk );
    tMetaBase FirstEmptyCellMetaBase( uint32 iFromCellIndex = 0, uint32* oFoundCellIndex = nullptr );
    tMetaBase FirstFullCellMetaBase( uint32 iFromCellIndex = 0, uint32* oFoundCellIndex = nullptr );
    tMetaBase LastFullCellMetaBase( uint32 iFromCellIndex = ULIS_UINT32_MAX, uint32* oFoundCellIndex = nullptr );

private:
    const byte_t mArenaSize;
    const byte_t mAllocSize;
    const uint32 mNumCells;
    uint32 mNumFreeCells;
    tByte* const mBlock;

    static constexpr const uint8 smMetaPadSize = sizeof( tClient );
};

ULIS_NAMESPACE_END

