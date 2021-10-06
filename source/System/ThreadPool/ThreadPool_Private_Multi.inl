// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool_Private_Multi.inl
* @author       Clement Berthaud
* @brief        This file provides the definition for the FThreadPool_Private
*               class for generic systems.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/ThreadPool/ThreadPool_Private_Multi.h"

ULIS_NAMESPACE_BEGIN

//Index of the current thread
static thread_local uint32 sgThreadIndex = 0;
static thread_local std::vector<::moodycamel::ProducerToken> sgCommandsToExecuteProducerTokens;


FThreadPool_Private::~FThreadPool_Private()
{
    StopWorkers();
}

FThreadPool_Private::FThreadPool_Private( uint32 iNumWorkers )
    : mStopWorkers(false)
    , mWorkers()
    , mCommandsToPrepare()
    , mCommandsToExecuteQueues()
    , mCurrentCommand(nullptr)
    , mCommandToExecuteMutex()
    , mWorkersSemaphore(nullptr)
    , mNumScheduledCommands(0)
    , mOnEventReady(::std::bind(&FThreadPool_Private::OnEventReady, this, ::std::placeholders::_1))
{
    StartWorkers(iNumWorkers);
}

//--- Command Scheduling On Flush



void
FThreadPool_Private::StopWorkers()
{
    WaitForCompletion();

    // Notify stop condition
    mStopWorkers = true;

    // Join all threads
    for (int i = 0; i < mWorkers.size(); i++)
    {
        mWorkersSemaphore[i].signal(1);
    }

    for (auto& t : mWorkers)
    {
        t.join();
    }

    mWorkers.clear();
    mCommandsToExecuteQueues.clear();
    delete[] mWorkersSemaphore;
    mWorkersSemaphore = nullptr;
}

void
FThreadPool_Private::StartWorkers(uint32 iNumWorkers)
{
    mStopWorkers = false;

    uint32 max = FMath::Min(iNumWorkers, MaxWorkers());
    mWorkers.reserve(max);
    mCommandsToExecuteQueues.reserve(max);
    mWorkersSemaphore = new ::moodycamel::LightweightSemaphore[max];
    for (uint32 i = 0; i < max; ++i)
    {
        mCommandsToExecuteQueues.emplace_back();
    }

    for (uint32 i = 0; i < max; ++i)
    {
        mWorkers.emplace_back(std::bind(&FThreadPool_Private::WorkProcess, this, i));
    }
}

void
FThreadPool_Private::SetNumWorkers(uint32 iNumWorkers)
{
    StopWorkers();
    StartWorkers(iNumWorkers);
}

uint32
FThreadPool_Private::GetNumWorkers() const
{
    return  static_cast<uint32>(mWorkers.size());
}

//static
uint32
FThreadPool_Private::MaxWorkers()
{
    return  std::thread::hardware_concurrency();
}

void
FThreadPool_Private::ScheduleCommands( TQueue< const FCommand* >& ioCommands )
{
#ifdef ULIS_ASSERT_ENABLED
    static std::thread::id mainThreadId = std::this_thread::get_id();
    ULIS_ASSERT( mainThreadId == std::this_thread::get_id(), "ScheduleCommands called on several threads." );
#endif // ULIS_ASSERT_ENABLED

    mNumScheduledCommands.fetch_add( ioCommands.Size(), std::memory_order_relaxed);

    static ::moodycamel::ProducerToken producerToken(mCommandsToPrepare);
    
    std::vector<FCommand*> commands;
    commands.reserve(ioCommands.Size());
    while( !ioCommands.IsEmpty() )
    {
        FCommand* cmd = const_cast<FCommand*>(ioCommands.Front());
        ioCommands.Pop();
        commands.push_back(cmd);
    }

    mCommandsToPrepare.enqueue_bulk(producerToken, commands.data(), commands.size());
    for (int i = 0; i < mWorkers.size(); i++)
    {
        mWorkersSemaphore[i].signal(1);
    }
}


void
FThreadPool_Private::OnEventReady(const FInternalEvent* iEvent)
{
    FCommand* command = const_cast<FCommand*>(iEvent->Command());

#ifdef NEW_JOBSYSTEM
    if (!command->GetJob())
    {
        command->Event()->NotifyAllJobsFinished();
        delete command;
        mNumScheduledCommands.fetch_sub(1, ::std::memory_order_release);
        return;
    }
#else
    if (command->Jobs().Size() == 0)
    {
        command->Event()->NotifyAllJobsFinished();
        delete command;
        mNumScheduledCommands.fetch_sub(1, ::std::memory_order_release);
        return;
    }
#endif

    command->WorkingThreads().fetch_add(mWorkers.size(), std::memory_order_relaxed);
    
    //add x working threads
    for (int i = 0; i < mWorkers.size(); i++)
    {
        mCommandsToExecuteQueues[i].enqueue(sgCommandsToExecuteProducerTokens[i], command);
        mWorkersSemaphore[i].signal();
    }
}

void
FThreadPool_Private::WaitForCompletion()
{
#ifdef ULIS_ASSERT_ENABLED
    static std::thread::id mainThreadId = std::this_thread::get_id();
    ULIS_ASSERT( mainThreadId == std::this_thread::get_id(), "ScheduleCommands called on several threads." );
#endif // ULIS_ASSERT_ENABLED

    while (mNumScheduledCommands.load(std::memory_order_acquire) != 0)
    {
    }
}

void
FThreadPool_Private::WorkProcess(uint32 iThreadIndex)
{
    sgThreadIndex = iThreadIndex;
    
    for(int i = 0; i < mCommandsToExecuteQueues.size(); i++)
    {
        sgCommandsToExecuteProducerTokens.push_back(::moodycamel::ProducerToken(mCommandsToExecuteQueues[i]));
    }

    while(!mStopWorkers)
    {
        mWorkersSemaphore[sgThreadIndex].wait(); //does not actually wait all the time but spins for a while before waiting to ensure great performances most of the time
        Work();
    }
}

void
FThreadPool_Private::Work()
{
    PrepareCommands();
    ExecuteCommand();
}

void
FThreadPool_Private::PrepareCommands()
{
    static thread_local ::moodycamel::ConsumerToken consumerToken(mCommandsToPrepare);
    FCommand* command;

    while (mCommandsToPrepare.try_dequeue(consumerToken, command))
    {
        if( command->SchedulePolicy().TimePolicy() == eScheduleTimePolicy::ScheduleTime_Async )
            command->ProcessAsyncScheduling();

        command->Event()->SetOnInternalEventReady(mOnEventReady);
    }
}

void
FThreadPool_Private::ReleaseCommandToExecute(FCommand* iCommand)
{
    if (!iCommand)
        return;

    if (iCommand->WorkingThreads().fetch_sub(1, ::std::memory_order_acq_rel) == 1) //aqc_rel to be sure that any other threads work is visible before destruction and any of our work is visible to other threads at this point
    {
        iCommand->Event()->NotifyAllJobsFinished();
        mNumScheduledCommands.fetch_sub(1, ::std::memory_order_release);
        delete iCommand;
    }
}

FCommand*
FThreadPool_Private::GetCommandToExecute()
{
    static thread_local ::moodycamel::ConsumerToken consumerToken(mCommandsToExecuteQueues[sgThreadIndex]);
    FCommand* command = nullptr;
    if (!mCommandsToExecuteQueues[sgThreadIndex].try_dequeue(consumerToken, command))
        return nullptr;
    return command;
}

void
FThreadPool_Private::ExecuteCommand()
{
    FCommand* command = GetCommandToExecute();
    while(command)
    {
        
#ifdef NEW_JOBSYSTEM
        command->ExecuteConcurrently();
#else
        FJob* job = command->GetJobForExecution();
        while(job)
        {
            job->Execute();
            job = command->GetJobForExecution();
        }
#endif
    
        ReleaseCommandToExecute(command);
        command = GetCommandToExecute();
    }
}

ULIS_NAMESPACE_END

