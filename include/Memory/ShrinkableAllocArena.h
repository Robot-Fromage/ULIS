// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ShrinkableAllocArena.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for ShrinkableAllocArena.
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
//          [client alloc][size]: stores the adress of the [data] and the size.
//                  e.g:
//                  [0xf4f568654][45][data]
//                  |->[points do data]
class ULIS_API FShrinkableAllocArena {
    friend class FShrinkableAllocMemoryPool;
public:
    typedef uint8* tAlloc;
    typedef uint8* tMetaBase;
    typedef uint8** tClient;

public:
    ~FShrinkableAllocArena();
    FShrinkableAllocArena(
          uint64 iArenaSize
        , uint32 iMaxAllocSize
    );
    FShrinkableAllocArena( const FShrinkableAllocArena& ) = delete;
    FShrinkableAllocArena& operator=( const FShrinkableAllocArena& ) = delete;

public:
    bool IsEmpty() const;
    bool IsInRange( const uint8* iPtr ) const;

    uint64 ArenaSize() const;
    uint32 MaxAllocSize() const;

    uint64 TotalMemory() const;
    uint64 AvailableMemory() const;
    uint64 UsedMemory() const;

    tClient Malloc( uint32 iSizeBytes = ULIS_UINT32_MAX ); // default max clamped to MaxAllocSize.
    void Free( tClient iClient );
    float LocalFragmentation() const;

    uint64 LowBlockAdress() const;
    uint64 HighBlockAdress() const;

    void Print( bool iShort = true, int iCol = 100 ) const;
    void DefragSelf();

    static bool IsFree( const uint8* iAlloc );
    static bool IsFree( const uint8** iClient );
    bool Shrink( uint8* iAlloc, uint32 iNewSizeBytes );
private:
    static bool IsMetaBaseAvailable( const uint8* iChunk );
    tMetaBase FirstEmptyMetaBaseMinAlloc( uint32 iMinimumSizeBytes = ULIS_UINT32_MAX, tMetaBase iFrom = nullptr ); // default max clamped to MaxAllocSize, default from to mBlock ( LowAdress )
    static tMetaBase AdvanceMetaBase( const tMetaBase iMetaBase );
    tMetaBase PreviousMetaBase( const tMetaBase iMetaBase );
    static uint32 MetaBaseSize( const tMetaBase iMetaBase );

private:
    const uint64 mArenaSize;
    const uint32 mMaxAllocSize;
    uint64 mUsedMemory;
    uint8* const mBlock;

    static constexpr const uint8 smMetaClientPadSize = sizeof( uint8** );
    static constexpr const uint8 smMetaSizePadSize = sizeof( uint32 );
    static constexpr const uint8 smMetaTotalPadSize = smMetaClientPadSize + smMetaSizePadSize;
};

ULIS_NAMESPACE_END

