// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Job.h
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides the declaration for the FJob class.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Command.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SchedulePolicy.h"
#include "Scheduling/Event.h"

ULIS_NAMESPACE_BEGIN
//class FJob;
typedef void (*fpTask)( const IJobArgs*, const ICommandArgs* );

/////////////////////////////////////////////////////
// ResolveScheduledJobInvocation
template< typename T, typename U, void (*TDelegateInvoke)( const T*, const U* ) >
static ULIS_FORCEINLINE void ResolveScheduledJobInvocation( const IJobArgs* iJobArgs, const ICommandArgs* iCommandArgs )
{
    const T* job_args = dynamic_cast< const T* >( iJobArgs );
    const U* cmd_args = dynamic_cast< const U* >( iCommandArgs );
    ULIS_ASSERT( job_args, "Bad cast" );
    ULIS_ASSERT( cmd_args, "Bad cast" );
    TDelegateInvoke( job_args, cmd_args );
}

#ifdef NEW_JOBSYSTEM

/////////////////////////////////////////////////////
/// @class      FJob
/// @brief      The IJob class provides an interface to execute a command's underlying tasks.
///             It can be accessed and execute its tasks concurrently.
/// @details    The IJob is used by FCommand when the command is being executed by the ThreadPool workers
///
///             \sa FCommand
///             \sa FThreadPool

class IJob {
public:
    /*! Destructor */
    virtual ~IJob() {}

    /*! explicitly deleted default constructor. */
    IJob() {}

    /*! explicitly deleted copy constructor. */
    IJob(const IJob&) = delete;

    /*! explicitly deleted move constructor. */
    IJob(IJob&&) = delete;

    /*! explicitly deleted copy assignment operator. */
    IJob& operator=(const IJob&) = delete;

    /*! explicitly deleted move assignment operator. */
    IJob& operator=(IJob&&) = delete;

public:
    /*! Start exec job tasks. */
    virtual void ExecuteConcurrently() = 0;

    /*! Returns the maximum amount of tasks that can be executed in concurrency. */
    virtual const uint64 GetMaxConcurrency() = 0;
};

/////////////////////////////////////////////////////
/// @class      FJob
/// @brief      The FJob class provides an way to execute a command's underlying tasks.
///             It can be accessed and execute its tasks concurrently.
/// @details    The FJob is used by FCommand when the command is being executed by the ThreadPool workers
///
///             \sa FCommand
///             \sa FThreadPool

template<
      typename TCommandArgs
    , typename TJobArgs
>
class FJob 
    : public IJob
{
public:
    /*! Destructor */
    virtual ~FJob()
    {
    }

    /*! Constructor */
    FJob(
        const uint64 iNumJobs
        , const uint64 iNumTaskPerJob
        , const uint64 iNumTasks
        , fpTask iTask
        , const TCommandArgs* iCommandArgs
    )
        : mNumJobs (iNumJobs)
        , mNumTaskPerJob( iNumTaskPerJob )
        , mNumTasks( iNumTasks )
        , mTask(iTask)
        , mCommandArgs(iCommandArgs)
        , mJobIndex(0)
    {
    }

    /*! explicitly deleted default constructor. */
    FJob() = delete;

    /*! explicitly deleted copy constructor. */
    FJob(const FJob&) = delete;

    /*! explicitly deleted move constructor. */
    FJob(FJob&&) = delete;

    /*! explicitly deleted copy assignment operator. */
    FJob& operator=(const FJob&) = delete;

    /*! explicitly deleted move assignment operator. */
    FJob& operator=(FJob&&) = delete;

public:
    /*! Start exec job tasks. */
    virtual void ExecuteConcurrently() override
    {
        while(true)
        {
            TJobArgs args;

            //relaxed, because we're just getting an index for us, not making any new memory available for other threads
            uint64 jobIndex = mJobIndex.fetch_add(1, ::std::memory_order_relaxed);
            if (jobIndex >= mNumJobs)
                break;

            //Compute all the tasks to do for the retrieved job index
            //Fill the args for each task and execute it
            uint64 firstTask = jobIndex * mNumTaskPerJob;
            uint64 endTask = FMath::Min(mNumTasks, (jobIndex + 1) * mNumTaskPerJob);
            for (uint64 i = firstTask; i < endTask; i++)
            {
                FillJobArgs(mCommandArgs, args, i);
                mTask( &args, mCommandArgs );
            }
        }
    }

    /*! Returns the maximum amount of tasks that can be executed in concurrency. */
    virtual const uint64 GetMaxConcurrency() override
    {
        return mNumJobs;
    }

protected:
    virtual void FillJobArgs(const TCommandArgs* iCommandArgs, TJobArgs& oJobArgs, uint64 iTaskIndex) = 0;

private:
    const uint64 mNumJobs;
    const uint64 mNumTaskPerJob;
    const uint64 mNumTasks;
    fpTask mTask;
    const TCommandArgs* mCommandArgs;
    ::std::atomic_uint64_t mJobIndex;
};

#else
/////////////////////////////////////////////////////
/// @class      FJob
/// @brief      The FJob class provides a way to store awaiting scheduled Jobs,
///             which are the actual workers scheduled from commands in
///             a command queue and in a pool, with all informations needed for
///             processecing the task.
/// @details    The FJob is used by FCommandQueue and FThreadPool and stores
///             information about a job and its parent command which status is
///
///             \sa FCommandQueue
///             \sa FThreadPool
class FJob {
public:
    /*! Destructor */
    ~FJob();

    /*! Constructor */
    FJob(
          uint32 iNumTasks
        , fpTask iTask
        , IJobArgs** iArgs
        , const FCommand* iParent
    );

    /*! explicitly deleted default constructor. */
    FJob() = delete;

    /*! explicitly deleted copy constructor. */
    FJob( const FJob& ) = delete;

    /*! explicitly deleted move constructor. */
    FJob( FJob&& ) = delete;

    /*! explicitly deleted copy assignment operator. */
    FJob& operator=( const FJob& ) = delete;

    /*! explicitly deleted move assignment operator. */
    FJob& operator=( FJob&& ) = delete;

    /*! Start exec job tasks. */
    void Execute() const;

    /*! Return the parent command */
    const FCommand* Parent() const;

private:
    uint32 mNumTasks;
    fpTask mTask;
    IJobArgs** mArgs;
    const FCommand* mParent;
};
#endif

ULIS_NAMESPACE_END

