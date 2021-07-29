// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         UncompressedMemoryDriver.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the UncompressedMemoryDriver class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/FixedAllocMemoryPool.h"

#include <forward_list>
#include <mutex>
#include <atomic>

ULIS_NAMESPACE_BEGIN
#pragma warning(push)
#pragma warning(disable : 4251) // Shut warning C4251 dll export of stl classes
/////////////////////////////////////////////////////
/// @class      FUncompressedMemoryDriver
/// @brief      The is a subcomponent of FTilePool that manages uncompressed tile
///             memory.
class ULIS_API FUncompressedMemoryDriver
{
public:
    // Construction / Destruction
    /*! Destructor. */
    ~FUncompressedMemoryDriver();

    /*! Constructor. */
    FUncompressedMemoryDriver(
          const uint8* iBackground
        , byte_t iTileSize
        , uint64 iNumCellPerArena
        , byte_t iTargetMemoryUsage
        , ufloat iDefragThreshold = 1/3.f
        , double iRelaxTime = 0.0
        , uint32 iDeallocBatchSize = 1
        , uint32 iAllocBatchSize = 1
    );

    /*! Explicitely deleted copy constructor */
    FUncompressedMemoryDriver( const FUncompressedMemoryDriver& ) = delete;

    /*! Explicitely deleted copy assignment operator */
    FUncompressedMemoryDriver& operator=( const FUncompressedMemoryDriver& ) = delete;

public:
    // Core API
    void PurgeAllNow();
    void SanitizeNow();
    tClient QueryOne();
    void SetBackground( const uint8* iBackground );

private:
    // Private Workers API
    // Worker sub-processes
    void AllocBatch();
    void DeallocBatch();
    // Main worker
    void WorkProcess();

private:
    const uint8* mBackground;
    FFixedAllocMemoryPool mAllocPool;
    std::forward_list< tClient > mAvailableTiles;
    std::mutex mAvailableTilesMutex;
    const uint64 mBytesPerTile;
    std::atomic< bool > bStopWorker;
    std::thread* const mWorker;
    double mWorkerRelaxTime_ms;
    uint32 mDeallocBatchSize;
    uint32 mAllocBatchSize;
};
#pragma warning(pop)

ULIS_NAMESPACE_END

