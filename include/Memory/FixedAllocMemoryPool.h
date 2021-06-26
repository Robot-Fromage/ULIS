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
#include "Memory/FixedAllocArena.h"
#include <list>

ULIS_NAMESPACE_BEGIN
#pragma warning(push)
#pragma warning(disable : 4251) // Shut warning C4251 dll export of stl classes
/////////////////////////////////////////////////////
/// @class      FFixedAllocMemoryPool
/// @brief      The FFixedAllocMemoryPool class is a class that provides a
///             configurable pool of fixed alloc arenas buffer to manages
///             allocations of fixed size data efficiently and optimize memory
///             consumption by evaluating sparsity, occupation and fragmentation
///             of the various arena pages and concatenate them if neccessary.
///             It is meant to be used for tiles or any objects of fixed
///             size, and can be driven asynchronously by a larger system if
///             necessary.
/// @details    FFixedAllocMemoryPool allows to obtain fixed allocations inside that
///             are resident of one arena page underlying block. It returns  "clients"
///             of the allocation, that is, a pointer to an allocation.
///             The API is mostly similar to FFixedAllocArena, but the pool aggregates
///             them in a list and manages fragmentation and higher level systems and
///             algorithms to drive them efficiently.
class ULIS_API FFixedAllocMemoryPool {
public:
    ~FFixedAllocMemoryPool();
    FFixedAllocMemoryPool(
          uint64 iArenaSize
        , uint32 iAllocSize
        , uint64 iTargetMemoryUsage = 0
        , float iDefragThreshold = 1/3.f
    );
    FFixedAllocMemoryPool( const FFixedAllocMemoryPool& ) = delete;
    FFixedAllocMemoryPool& operator=( const FFixedAllocMemoryPool& ) = delete;

public:
    // Size API
    /*! Obtain the arena size in bytes */
    byte_t ArenaSize() const;

    /*! Obtain the alloc size in bytes */
    byte_t AllocSize() const;

    /*! Obtain the number of total possible cells in the pool. */
    uint64 NumCells() const;

    /*! Obtain the number of currently free cells in the pool. */
    uint64 NumFree() const;

    /*! Obtain the number of currently used cells in the pool. */
    uint64 NumUsedCells() const;



    // Memory API
    /*! Obtain the total theoretical memory for this pool as bytes, given the current number or arenas. */
    byte_t TotalMemory() const;

    /*! Obtain current free memory as bytes. */
    byte_t FreeMemory() const;

    /*! Obtain current used memory as bytes. */
    byte_t UsedMemory() const;

    /*! Obtain the target memory usage as bytes. */
    byte_t TargetMemoryUsage() const;

    /*! Set the target memory usage as bytes. */
    void SetTargetMemoryUsage( byte_t iValue );



    // Fragmentation API
    /*! Get the defrag theshold. */
    float DefragThreshold() const;

    /*! Set the defrag theshold. */
    void SetDefragThreshold( float iValue );

    /*! Arbitrary heuristic to estimate fragmentation of the pool. */
    float Fragmentation() const;

    /*! Trigger a defrag only if the threshold is reached. */
    void DefragIfNecessary();

    /*! Force trigger a defrag. */
    void DefragForce();



    // Alloc API
    tClient Malloc();
    void Free( tClient iClient );
    bool AllocOneArenaIfNecessary();
    bool FreeOneArenaIfNecessary();



    // Debug API
    void DebugPrint() const;

private:
    // Private Data Members
    const uint64 mArenaSize; ///< Arena Size in bytes, without extra meta pad for each cell
    const uint64 mAllocSize; ///< Allocation Size in bytes, without extra meta pad for cell
    uint64 mTargetMemoryUsage; ///< approximate target memory to reach with arena pages, actual behaviour depends on policy.
    float mDefragThreshold; ///< A threshold that, if reached, allows to trigger a defrag.
    std::list< FFixedAllocArena* > mArenaPool; ///< The arenas pages, in a list.
};
#pragma warning(pop)

ULIS_NAMESPACE_END

