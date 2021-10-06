// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Job.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FJob class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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
        const int64 iNumJobs
        , const int64 iNumTaskPerJob
        , fpTask iTask
        , const TCommandArgs* iCommandArgs
    )
        : mNumJobs (iNumJobs)
        , mNumTaskPerJob( iNumTaskPerJob )
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
    void ExecuteConcurrently()
    {
        while(true)
        {
            TJobArgs args;

            int64 jobIndex = mJobIndex.fetch_add(1, ::std::memory_order_relaxed);
            if (jobIndex >= mNumJobs)
                break;

            uint64 firstTask = jobIndex * mNumTaskPerJob;
            for (int i = 0; i < mNumTaskPerJob; i++)
            {
                FillJobArgs(mCommandArgs, args, firstTask + i);
                mTask( &args, mCommandArgs );
            }
        }
    }

protected:
    virtual void FillJobArgs(const TCommandArgs* iCommandArgs, TJobArgs& oJobArgs, uint64 iTaskIndex) = 0;

private:
    const int64 mNumJobs;
    const int64 mNumTaskPerJob;
    fpTask mTask;
    const TCommandArgs* mCommandArgs;
    ::std::atomic_int64_t mJobIndex;
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

