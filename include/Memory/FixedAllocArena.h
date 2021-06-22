// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FixedAllocMemoryPool.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for FixedAllocArena.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Format of Fixed Alloc
//
//          [meta][data]
//          [data]: fixed size ( alloc size ), e.g: 16384 bytes for a 64� RGBA8 tile.
//          [meta]: [adress of arena][adress of client alloc]
//          [client alloc]: stores the adress of the [data]
//                  e.g:
//                  [0xf4f568654][0xf54ez4ae4][data]
//                                |->[points do data]
//
//  Ex arena 1024� for 64� RGBA8:
//  mArenaSize: 4194304 bytes
//  mAllocSize: 16384 bytes
//  mNumCells = arenaSize / allocSize = 256
//  metaPadSize = sizeof( ptr )
class ULIS_API FFixedAllocArena {
public:
    typedef uint8** tAlloc;
    typedef uint8* tData;

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
    uint32 TotalCells() const;
    uint32 NumAvailableCells() const;
    uint32 NumUsedCells() const;
    uint8* Malloc();
    void Free( uint8* iAlloc );
    float LocalFragmentation() const;

private:
    uint64 LowBlockAdress() const;
    uint64 HighBlockAdress() const;
    uint64 BlockSize() const;
    uint32 LargestFreeChunk() const;
    uint8* Chunk( uint32 iIndex );
    const uint8* Chunk( uint32 iIndex ) const;
    bool IsChunkAvailable( const uint8* iChunk ) const;
    void SetChunkData( uint8* iChunk, bool iValue = true );

private:
    const uint64 mArenaSize;
    const uint32 mAllocSize;
    const uint32 mNumCells;
    uint32 mNumAvailableCells;
    uint8* const mBlock;

    static constexpr const uint8 smMetaPadSize = sizeof( tAlloc );
};

ULIS_NAMESPACE_END

