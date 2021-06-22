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
#include <vector>

ULIS_NAMESPACE_BEGIN
#pragma warning(push)
#pragma warning(disable : 4251) // Shut warning C4251 dll export of stl classes
class ULIS_API FFixedAllocArena {
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
    uint64 ArenaSize() const;
    uint32 AllocSize() const;
    uint32 TotalCells() const;
    uint32 NumAvailableCells() const;
    uint32 NumUsedCells() const;
    uint8* Malloc();
    void Free( const uint8* iAlloc );
    bool IsInRange( const uint8* iAlloc ) const;
    uint64 LowAdress() const;
    uint64 HighAdress() const;

private:
    const uint64 mArenaSize;
    const uint32 mAllocSize;
    const uint32 mTotalCells;
    const uint32 mNumAvailableCells;
    uint8* const mBlock;
    std::vector< bool > mUsed;
};
#pragma warning(pop)

ULIS_NAMESPACE_END

