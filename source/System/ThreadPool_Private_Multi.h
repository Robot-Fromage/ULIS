// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool_Private_Multi.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the FThreadPool_Private
*               class for generic systems.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/Array.h"
#include "Memory/Queue.h"
#include "Scheduling/Job.h"
#include "Scheduling/Command.h"

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <random>
#include <thread>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FThreadPool_Private
/// @brief      The FThreadPool_Private class provides the private implementation
///             for FThreadPool using the pimpl idiom.
/// @details    This version of the private implementation is for generic
///             systems with multithreading support.
///
///             \sa FThreadPool
class FThreadPool_Private
{
public:
    ~FThreadPool_Private();
    FThreadPool_Private( uint32 iNumWorkers = MaxWorkers() );
    FThreadPool_Private( const FThreadPool_Private& ) = delete;
    FThreadPool_Private& operator=( const FThreadPool_Private& ) = delete;
    void ScheduleCommands( TQueue< const FCommand* >& ioCommands );
    void WaitForCompletion();
    void SetNumWorkers( uint32 iNumWorkers );
    uint32 GetNumWorkers() const;
    static uint32 MaxWorkers();

private:
    void ScheduleJob( const FJob* iJob );
    void WorkProcess();
    void ScheduleProcess();

private:
    // Private Data
    uint32                              mNumBusy;
    bool                                bStop;
    std::vector< std::thread >          mWorkers;
    std::deque< const FJob* >           mJobs;
    std::deque< const FCommand* >       mCommands;
    std::mutex                          mJobsQueueMutex;
    std::mutex                          mCommandsQueueMutex;
    std::condition_variable             cvJob;
    std::condition_variable             cvFinished;
    std::thread                         mScheduler;
};

ULIS_NAMESPACE_END

