// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FixedAllocMemoryPool.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for FFixedAllocMemoryPool.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/ForwardList.h"
#include "Memory/FixedAllocArena.h"

ULIS_NAMESPACE_BEGIN
// Export
template class ULIS_API TForwardList< FFixedAllocArena* >;

class ULIS_API FFixedAllocMemoryPool {
public:
    ~FFixedAllocMemoryPool();
    FFixedAllocMemoryPool(
          uint64 iArenaSize
        , uint32 iAllocSize
        , uint64 iTargetMemoryUsage = 1
    );
    FFixedAllocMemoryPool( const FFixedAllocMemoryPool& ) = delete;
    FFixedAllocMemoryPool& operator=( const FFixedAllocMemoryPool& ) = delete;

public:
    uint64 ArenaSize() const;
    uint32 AllocSize() const;
    uint8* Malloc();
    void Free( const uint8* iAlloc );
    uint64 TotalMemory() const;
    uint64 TotalCells() const;
    uint64 AvailableMemory() const;
    uint64 UsedMemory() const;
    uint32 NumAvailableCells() const;
    uint64 TargetMemoryUsage() const;
    void SetTargetMemoryUsage( uint64 iValue );
    bool AllocArenaIfNecessary();
    bool FreeArenaIfNecessary();

private:
    const uint64 mArenaSize;
    const uint32 mAllocSize;
    uint64 mTargetMemoryUsage;
    TForwardList< FFixedAllocArena* > mArenaPool;
};
ULIS_NAMESPACE_END

