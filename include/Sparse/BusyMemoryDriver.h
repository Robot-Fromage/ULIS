// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         UncompressedMemoryDriver.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the BusyMemoryDriver class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

#include <atomic>
#include <list>
#include <mutex>
#include <thread>
#include <unordered_map>

ULIS_NAMESPACE_BEGIN
#pragma warning(push)
#pragma warning(disable : 4251) // Shut warning C4251 dll export of stl classes
/////////////////////////////////////////////////////
/// @class      FBusyMemoryDriver
/// @brief      The is a subcomponent of FTilePool that manages busy tile
///             memory.
class ULIS_API FBusyMemoryDriver
{
public:
    // Construction / Destruction
    /*! Destructor. */
    ~FBusyMemoryDriver();

    /*! Constructor. */
    FBusyMemoryDriver();

    /*! Explicitely deleted copy constructor */
    FBusyMemoryDriver( const FBusyMemoryDriver& ) = delete;

    /*! Explicitely deleted copy assignment operator */
    FBusyMemoryDriver& operator=( const FBusyMemoryDriver& ) = delete;

public:
    // Core API
    void Submit( FTile* iTile );

private:
    // Private Workers API
    // Worker sub-processes
    void SanitizeDirtyHashedBatch();
    void SanitizeCorrectlyHashedBatch();
    // Main worker
    void WorkProcess();

private:
    // Private Data Members
    std::list< FTile* > mDirtyHashedTiles;
    std::unordered_map< uint32, FTile* > mCorrectlyHashedTiles;
    std::mutex mMutexDirtyHashedTilesLock;
    std::mutex mMutexCorrectlyHashedTilesLock;
    std::atomic< bool > bStopWorker;
    std::thread* const mWorker;
    double mWorkerRelaxTime_ms;
    uint32 mDirtyHashedBatchSize;
    uint32 mCorrectlyHashedBatchSize;
};
#pragma warning(pop)

ULIS_NAMESPACE_END

