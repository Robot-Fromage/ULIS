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
    : mNumScheduledCommands(0)
    , mStopWorkers(false)
    , mWorkers()
    , mWorkersSemaphore(nullptr)
    , mCommandsToExecuteQueues()
    , mNumCommandsToExecute(nullptr)
    , mCurrentCommand(nullptr)
    , mOnEventReady(::std::bind(&FThreadPool_Private::OnEventReady, this, ::std::placeholders::_1))
{
    StartWorkers(iNumWorkers);
}

//--- Command Scheduling On Flush



void
FThreadPool_Private::StopWorkers()
{
    //Wait for all already scheduld commands to be processed
    WaitForCompletion();

    // Notify stop condition
    mStopWorkers = true;

    // Wake up all workers
    for (int i = 0; i < mWorkers.size(); i++)
        mWorkersSemaphore[i].signal(1);

    // Wait for all workers to finish
    for (auto& t : mWorkers)
        t.join();

    //Clear everything related with workers
    // mWorkersReady list, Workers list, command queues, tokens, semaphores, num commands

    mWorkers.clear();
    mWorkersReady.clear();

    mCommandsToExecuteQueues.clear();
    delete[] mNumCommandsToExecute;
    mNumCommandsToExecute = nullptr;
    
    delete[] mWorkersSemaphore;
    mWorkersSemaphore = nullptr;
    
    sgCommandsToExecuteProducerTokens.clear();
}

void
FThreadPool_Private::StartWorkers(uint32 iNumWorkers)
{
    //Make sure all workers will not stop immediately
    mStopWorkers = false;

    //Compute amount of workers
    uint32 max = FMath::Min(iNumWorkers, MaxWorkers());
    
    //Create one semaphore for each worker
    mWorkersSemaphore = new ::moodycamel::LightweightSemaphore[max];

    //Create each workers command queue and initialize it at 0 commands to execute
    //Also create the producer tokens so that each worker can produce commands to execute in every queues
    mCommandsToExecuteQueues.reserve(max);
    mNumCommandsToExecute = new std::atomic_uint64_t[max];
    for (uint32 i = 0; i < max; ++i)
    {
        mCommandsToExecuteQueues.emplace_back();
        mNumCommandsToExecute[i] = 0;
        sgCommandsToExecuteProducerTokens.push_back(::moodycamel::ProducerToken(mCommandsToExecuteQueues[i]));
    }

    // Create Workers Ready list
    mWorkersReady.reserve(max);
    for (uint32 i = 0; i < max; ++i)
    {
        mWorkersReady.push_back(i);
    }

    //Create and launch workers
    mWorkers.reserve(max);
    for (uint32 i = 0; i < max; ++i)
    {
        mWorkers.emplace_back(std::bind(&FThreadPool_Private::WorkProcess, this, i));
    }
}

void
FThreadPool_Private::SetNumWorkers(uint32 iNumWorkers)
{
    //Restart Workers with the given amount of workers
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

    //Add given queue size to the number of command scheduled, so WaitOnCompletion can wait
    //relaxed : because this does not ensure any memory to be shared between threads, it is just the increase of a counter
    mNumScheduledCommands.fetch_add(ioCommands.Size(), std::memory_order_relaxed);

    while( !ioCommands.IsEmpty() )
    {
        FCommand* command = const_cast<FCommand*>(ioCommands.Front());
        ioCommands.Pop();

        //Execute scheduling now if it is marked as Async        
        if( command->SchedulePolicy().TimePolicy() == eScheduleTimePolicy::ScheduleTime_Async )
            command->ProcessAsyncScheduling();

        command->Event()->SetOnInternalEventReady(mOnEventReady);
    }
}

void
FThreadPool_Private::FinishCommand(FCommand* iCommand)
{
    //When a command needs to be finished, we inform the event that all jobs where finished
    //this will then inform all children commands to one of there parent is finished
    //if all parents of a command are finished, it triggers OnEventReady() for the command
    
    iCommand->Event()->NotifyAllJobsFinished();
    delete iCommand;

    //mNumScheduledCommands--
    //release : because we ensure to other threads reading mNumScheduledCommands that iCommand is correctly destroyed
    uint64 num = mNumScheduledCommands.fetch_sub(1, ::std::memory_order_release);
    if (num == 1) // if num was 1, then we just finished the last scheduled commands and we can notify WaitOnCompletion() 
    {
        std::unique_lock<std::mutex> lock(mWaitMutex); //lock and unlock immediately just to assure synchronisation with WaitForCompletion wait()
        lock.unlock();
        mWaitCV.notify_one();
    }
}

void
FThreadPool_Private::OnEventReady(const FInternalEvent* iEvent)
{
    FCommand* command = const_cast<FCommand*>(iEvent->Command());
    if (!command->GetJob())
    {
        //If command has no job, then we assume the command is immediately finished
        FinishCommand(command);
        return;
    }

    //Initialize the command with an amount of workers working on it
    //this helps us to know when all expected workers has done working on the command and thus finish the command
    uint64 numWorkers = command->GetMaxConcurrency() < mWorkers.size() ? command->GetMaxConcurrency() : mWorkers.size();
    //relaxed : because we just increase a counter without ensuring any new memory to be shared
    command->WorkingThreads().fetch_add(numWorkers, std::memory_order_relaxed);

    //Lock the rest of the function
    //here we add the command to the most active and/or ready workers
    //as the 
    std::unique_lock<std::mutex> lock(mWorkersReadyMutex);
    uint32 index = sgThreadIndex;
    for (int i = 0; i < numWorkers; i++ )
    {
        uint32  index = mWorkersReady.front();
        mWorkersReady.erase(mWorkersReady.begin());
        mWorkersReady.push_back(index);

        //enqueue in a concurrent queue, but as we do this in a lock, we could maybe use a classic queue
        mCommandsToExecuteQueues[index].enqueue(sgCommandsToExecuteProducerTokens[index], command);

        //Increment the worker number of commands to execute
        //if we use a concurrent queue : relaxed because we can try reading concurrentqueue over and over again
        //if we use a classic queue : release because we need to ensure the command has been added to the queue
        //But if we use a classic queue, we will probably need a mutex lock here to avoid collisions anyway
        mNumCommandsToExecute[index].fetch_add(1, ::std::memory_order_relaxed /* ::std::memory_order_release */);

        //Wake up the corresponding worker
        mWorkersSemaphore[index].signal(1);
    }
}

void
FThreadPool_Private::WaitForCompletion()
{
#ifdef ULIS_ASSERT_ENABLED
    static std::thread::id mainThreadId = std::this_thread::get_id();
    ULIS_ASSERT( mainThreadId == std::this_thread::get_id(), "ScheduleCommands called on several threads." );
#endif // ULIS_ASSERT_ENABLED

    //acquire : because we need to be sure that, if mNumScheduledCommands == 0, all commands are done, and all threads are synchronized
    std::unique_lock<std::mutex> lock(mWaitMutex);
    mWaitCV.wait(lock, [this](){return  mNumScheduledCommands.load(std::memory_order_acquire) == 0;});
}

void
FThreadPool_Private::WorkProcess(uint32 iThreadIndex)
{
    //init the thread index in a thread local global variable
    sgThreadIndex = iThreadIndex;
    
    //init the worker producer tokens
    for(int i = 0; i < mCommandsToExecuteQueues.size(); i++)
    {
        sgCommandsToExecuteProducerTokens.push_back(::moodycamel::ProducerToken(mCommandsToExecuteQueues[i]));
    }

    // Loop until workers needs to stop
    while(!mStopWorkers)
    {
        //wait until there is some work to do
        //that ensures there is a command to execute, but does not ensure that there is actually jobs to do when the thread wakes up
        //the actual jobs could already have been done by another thread at the time
        //but we also ensure to wake up threads only when we suppose it is needed, so it is fine
        //wait() does not actually wait all the time but spins for a while before waiting to ensure great performances most of the time
        mWorkersSemaphore[sgThreadIndex].wait(); 

        //if we wake up and we need to stop, then stop immediately   
        if (mStopWorkers)
            return;

        //Execute a command (if available)
        ExecuteCommand();
    }
}

void
FThreadPool_Private::ReleaseCommandToExecute(FCommand* iCommand)
{
    //Before releasing the command, we inform that the worker is ready to accept a new job
    //this ensures that if we only need 1 thread all the time, we won't wake up other threads because we are not ready yet
    std::unique_lock<std::mutex> lock(mWorkersReadyMutex);
    for (auto it = mWorkersReady.begin(); it != mWorkersReady.end(); ++it)
    {
        if (*it != sgThreadIndex)
            continue;

        mWorkersReady.erase(it);
        mWorkersReady.insert(mWorkersReady.begin(), sgThreadIndex);
        break;
    }

    //Unlock because we don't need to be locked for the rest of the method
    lock.unlock();

    //if the command is nullptr, then we don't need to do anything more
    if (!iCommand)
        return;

    //Decrease the command working thread count, as the current worker has done its work on it
    //acq_rel : because, honestly I'm not sure if release or acquire alone are enough here
    //FinishCommand will delete the command and inform the children commands
    //So I guess we need to ensure any access to the command and any job has been done
    //We need to release to inform other thread, and to acquire to be informed from other threads
    //Thus, we use acq_rel to do both
    if (iCommand->WorkingThreads().fetch_sub(1, ::std::memory_order_acq_rel) == 1)
    {
        FinishCommand(iCommand);
    }
}

FCommand*
FThreadPool_Private::GetCommandToExecute()
{
    //Have a thread_local consumerToken to consume the worker command to execute queue
    static thread_local ::moodycamel::ConsumerToken consumerToken(mCommandsToExecuteQueues[sgThreadIndex]);

    //try to get a command until there is no more command to get
    //if we use a concurrent queue : relaxed because we can try reading concurrentqueue over and over again
    //if we use a classic queue : release because we need to be sure the command has been added to the queue
    //But if we use a classic queue, we will probably need a mutex lock here to avoid collisions anyway
    FCommand* command = nullptr;
    while (mNumCommandsToExecute[sgThreadIndex].load(::std::memory_order_relaxed) != 0)
    {
        if (!mCommandsToExecuteQueues[sgThreadIndex].try_dequeue(consumerToken, command))
            continue;

        //Decrement num commands to execute and break to return the retrieved command
        //relaxed : because it does not ensure anything for other threads
        mNumCommandsToExecute[sgThreadIndex].fetch_sub(1, ::std::memory_order_relaxed);
        break;
    }
    return command;
}

void
FThreadPool_Private::ExecuteCommand()
{
    //Simply get a command, execute it, and release it
    FCommand* command = GetCommandToExecute();
    if (command)
    {
        command->ExecuteConcurrently();
    }
    ReleaseCommandToExecute(command);
}

ULIS_NAMESPACE_END
