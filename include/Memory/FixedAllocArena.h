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
#include "Memory/Alloc.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Format of Fixed Alloc
//
//          [meta][data]
//          [data]: fixed size ( alloc size ), e.g: 16384 bytes for a 64² RGBA8 tile.
//          [meta]: [adress of arena][adress of client alloc]
//          [client alloc]: stores the adress of the [data]
//                  e.g:
//                  [0xf4f568654][0xf54ez4ae4][data]
//                                |->[points do data]
//
//  Ex arena 1024² for 64² RGBA8:
//  mArenaSize: 4194304 bytes
//  mAllocSize: 16384 bytes
//  mNumCells = arenaSize / allocSize = 256
//  metaPadSize = sizeof( ptr )
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
class ULIS_API FFixedAllocArena {
    friend class FFixedAllocMemoryPool;

public:
    typedef uint8* tAlloc;
    typedef uint8* tMetaBase;
    typedef const uint8* tConstMetaBase;
    typedef uint8** tClient;

public:
    ~FFixedAllocArena();
    FFixedAllocArena(
          uint64 iArenaSize
        , uint32 iAllocSize
    );
    FFixedAllocArena( const FFixedAllocArena& ) = delete;
    FFixedAllocArena& operator=( const FFixedAllocArena& ) = delete;

public:
    bool IsFull() const;
    bool IsEmpty() const;
    bool IsInRange( const uint8* iAlloc ) const;

    uint64 ArenaSize() const;
    uint32 AllocSize() const;

    uint32 NumCells() const;
    uint32 NumAvailableCells() const;
    uint32 NumUsedCells() const;

    tClient Malloc();
    void Free( tClient iClient );
    float LocalFragmentation() const;

    uint64 LowBlockAdress() const;
    uint64 HighBlockAdress() const;

    void Print() const;
    void DefragSelf();

    static bool IsFree( const uint8* iAlloc );
    static bool IsFree( const tClient iClient );

private:
    static void Swap( uint8* iFromMetaBase, uint8* iToMetaBase, uint32 iAllocSize );
    uint32 LargestFreeChunk() const;
    uint32 LargestUsedChunk() const;
    uint64 BlockSize() const;
    uint8* ChunkMetaBase( uint32 iIndex );
    const uint8* ChunkMetaBase( uint32 iIndex ) const;
    static bool IsChunkMetaBaseAvailable( const uint8* iChunk );
    uint8* FirstEmptyChunkMetaBase( uint32 iFrom = 0, uint32* oIndex = nullptr );
    uint8* FirstFullChunkMetaBase( uint32 iFrom = 0, uint32* oIndex = nullptr );
    uint8* LastFullChunkMetaBase( uint32 iFrom = ULIS_UINT32_MAX, uint32* oIndex = nullptr );

private:
    const uint64 mArenaSize;
    const uint32 mAllocSize;
    const uint32 mNumCells;
    uint32 mNumAvailableCells;
    uint8* const mBlock;

    static constexpr const uint8 smMetaPadSize = sizeof( uint8** );
};

ULIS_NAMESPACE_END

