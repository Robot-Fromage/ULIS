// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         ThreadPool_Private_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the FThreadPool_Private
*               class for generic systems.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Thread/ThreadPool.h"
#include <atomic>
#include <condition_variable>
#include <deque>
#include "Memory/Array.h"
#include "Memory/Queue.h"
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
class ULIS_API FThreadPool::FThreadPool_Private
{
public:
    ~FThreadPool_Private();
    FThreadPool_Private( uint32 iNumWorkers = MaxWorkers() );
    void ScheduleJob( FCommand* iCommand );
    void WaitForCompletion();
    void SetNumWorkers( uint32 iNumWorkers );
    uint32 GetNumWorkers() const;
    static uint32 MaxWorkers();

private:
    // Private Data
    unsigned int                        busy;
    bool                                stop;
    std::vector< std::thread >          workers;
    std::deque< std::function<void()> > tasks;
    std::mutex                          queue_mutex;
    std::condition_variable             cv_task;
    std::condition_variable             cv_finished;
};

FThreadPool::FThreadPool_Private::~FThreadPool_Private()
{
}

FThreadPool::FThreadPool_Private::FThreadPool_Private( uint32 iNumWorkers )
{
}

void
FThreadPool::FThreadPool_Private::ScheduleJob( FCommand* iCommand )
{
}

void
FThreadPool::FThreadPool_Private::WaitForCompletion()
{
}

void
FThreadPool::FThreadPool_Private::SetNumWorkers( uint32 iNumWorkers )
{
}

uint32
FThreadPool::FThreadPool_Private::GetNumWorkers() const
{
    return  1;
}

//static
uint32
FThreadPool::FThreadPool_Private::MaxWorkers()
{
    return  std::thread::hardware_concurrency();
}

ULIS_NAMESPACE_END

