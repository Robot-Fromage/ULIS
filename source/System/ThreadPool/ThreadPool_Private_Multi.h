// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool_Private_Multi.h
* @author       Clement Berthaud && Eric Scholl
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
#include "ThirdParty/ConcurrentQueue/blockingconcurrentqueue.h"

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <random>
#include <thread>
#include <stack>

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
    void WorkProcess(uint32 iThreadIndex);
    // void Work();
    // bool PrepareCommands();
    void FinishCommand(FCommand* iCommand);
    void ExecuteCommand();
    //void ScheduleProcess(FCommand* iCmd);
    void OnEventReady(const FInternalEvent* iEvent);

    void StopWorkers();
    void StartWorkers(uint32 iNumWorkers);

    FCommand* GetCommandToExecute();
    void ReleaseCommandToExecute(FCommand* iCommand);

private:
    // Private Data
    std::atomic_uint64_t mNumScheduledCommands; //an atomic counter to know how many commands are queued
    ::std::mutex mWaitMutex; //a mutex to use with a condition variable to WaitForCompletion()
    ::std::condition_variable mWaitCV; //The condition variable to WaitForCompletion()

    bool                                mStopWorkers;
    std::vector< std::thread >          mWorkers;
    ::moodycamel::LightweightSemaphore* mWorkersSemaphore; //Semaphore to signal all workers to wake up when something needs to be done

    //::moodycamel::ConcurrentQueue<FCommand*> mCommandsToPrepare; //the queue of commands ready to be prepared (Preparing a command means creating its jobs if needed)
    std::vector<::moodycamel::ConcurrentQueue<FCommand*>> mCommandsToExecuteQueues; //The queue of commands ready to execute
    std::atomic_uint64_t* mNumCommandsToExecute;
    FCommand* mCurrentCommand;

    ::std::function<void(const FInternalEvent*)>  mOnEventReady;
    ::std::mutex mWorkersReadyMutex;
    ::std::vector<uint32> mWorkersReady;
};

ULIS_NAMESPACE_END

