// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ShrinkableAllocMemoryPool.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for FShrinkableAllocMemoryPool.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/ShrinkableAllocArena.h"
#include <list>

ULIS_NAMESPACE_BEGIN
#pragma warning(push)
#pragma warning(disable : 4251) // Shut warning C4251 dll export of stl classes
class ULIS_API FShrinkableAllocMemoryPool {
public:
    ~FShrinkableAllocMemoryPool();
    FShrinkableAllocMemoryPool(
          uint64 iArenaSize
        , uint32 iAllocSize
        , uint64 iTargetMemoryUsage = 1
        , float iDefragThreshold = 0.333f
    );
    FShrinkableAllocMemoryPool( const FShrinkableAllocMemoryPool& ) = delete;
    FShrinkableAllocMemoryPool& operator=( const FShrinkableAllocMemoryPool& ) = delete;

public:
    uint64 ArenaSize() const;
    uint32 AllocSize() const;

    uint64 NumCells() const;
    uint64 NumAvailableCells() const;
    uint64 NumUsedCells() const;

    uint64 TotalMemory() const;
    uint64 AvailableMemory() const;
    uint64 UsedMemory() const;

    uint64 TargetMemoryUsage() const;
    void SetTargetMemoryUsage( uint64 iValue );

    float DefragThreshold() const;
    void SetDefragThreshold( float iValue );

    tClient Malloc();
    void Free( tClient iAlloc );
    float Fragmentation() const;
    void DefragIfNecessary();
    void DefragForce();

    bool AllocOneArenaIfNecessary();
    bool FreeOneArenaIfNecessary();

    void Print() const;

private:
    const uint64 mArenaSize;
    const uint32 mAllocSize;
    uint64 mTargetMemoryUsage;
    float mDefragThreshold;
    std::list< FShrinkableAllocArena* > mArenaPool;
};
#pragma warning(pop)
ULIS_NAMESPACE_END

