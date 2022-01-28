// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Command.h
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides the declaration for the FCommand class.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/Array.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SchedulePolicy.h"
#include "Scheduling/Event.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
class FCommand;
#ifdef NEW_JOBSYSTEM
class IJob;
#else
class FJob;
#endif
typedef void (*fpCommandScheduler)( FCommand*, const FSchedulePolicy&, bool, bool );

/////////////////////////////////////////////////////
/// @class      FCommand
/// @brief      The FCommand class provides a way to store awaiting commands in
///             a command queue, with all informations needed for scheduling and
///             arguments about the operation.
/// @details    The FCommand is used by FCommandQueue and stores information about
///             a command which status is read through a FEvent, which
///             scheduling information is stored in a FSchedulePolicy, and
///             which operation arguments are stored in a ICommandArgs child class.
///
///             \sa FCommandQueue
class FCommand {
public:
    /*! Destructor */
    ~FCommand();

    /*! Constructor */
    FCommand(
          fpCommandScheduler iSched
        , const ICommandArgs* iArgs
        , const FSchedulePolicy& iPolicy
        , bool iContiguous
        , bool iForceMonoChunk
        , uint32 iNumWait
        , const FEvent* iWaitList
        , FEvent* iEvent
        , const FRectI& iEventGeometry
    );

    /*! explicitly deleted default constructor. */
    FCommand() = delete;

    /*! explicitly deleted copy constructor. */
    FCommand( const FCommand& ) = delete;

    /*! explicitly deleted move constructor. */
    FCommand( FCommand&& ) = delete;

    /*! explicitly deleted copy assignment operator. */
    FCommand& operator=( const FCommand& ) = delete;

    /*! explicitly deleted move assignment operator. */
    FCommand& operator=( FCommand&& ) = delete;

    /*! Get the args */
    const ICommandArgs* Args() const;

#ifdef NEW_JOBSYSTEM
    /* Executes the command concurrently */
    void ExecuteConcurrently();

    /* Sets the job */
    void SetJob(IJob* iJob);

    /* Get the job */
    IJob* GetJob();

    /* Returns the maximum amount of tasks that can be executed in concurrency */
    uint64 GetMaxConcurrency() const;
#else
    /*! Reserve some space for jobs */
    void ReserveJobs( uint64 iNum );

    /*! Add a job */
    void AddJob( const FJob* iJob );

    /*! Gets a FJob for execution, or nullptr if none is available */
    FJob* GetJobForExecution();

    const TArray< const FJob* >& Jobs() const;

    /*! Query num jobs */
    uint64 NumJobs() const;
#endif

    /* Returns the amount of threads currently working on this command*/
    ::std::atomic_uint32_t& WorkingThreads();

    void ProcessAsyncScheduling();

    FSharedInternalEvent Event() const;

    const FSchedulePolicy& SchedulePolicy() const;

private:
    const ICommandArgs* mArgs;
    FSharedInternalEvent mEvent;

#ifdef NEW_JOBSYSTEM
    IJob* mJob;
#else
    TArray< const FJob* > mJobs;
    ::std::atomic_uint64_t mJobIndex; //index to the next job to execute
#endif
    ::std::atomic_uint32_t mWorkingThreads;
    fpCommandScheduler mSched;
    FSchedulePolicy mPolicy;
    bool mContiguous;
    bool mForceMonoChunk;
};

ULIS_NAMESPACE_END

