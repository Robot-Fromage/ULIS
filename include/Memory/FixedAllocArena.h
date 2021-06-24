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

