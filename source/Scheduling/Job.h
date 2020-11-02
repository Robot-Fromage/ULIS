// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Job.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FJob class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Command.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SchedulePolicy.h"
#include "Scheduling/Event.h"

ULIS_NAMESPACE_BEGIN
class FJob;
typedef void (*fpTask)( const IJobArgs*, const ICommandArgs* );

/////////////////////////////////////////////////////
// ResolveScheduledJobCall
template< typename T, typename U, void (*IMP)( const T*, const U* ) >
static ULIS_FORCEINLINE void ResolveScheduledJobCall( const IJobArgs* iJobArgs, const ICommandArgs* iCommandArgs )
{
    const T* job_args = dynamic_cast< const T* >( iJobArgs );
    const T* cmd_args = dynamic_cast< const U* >( iCommandArgs );
    ULIS_ASSERT( job_args, "Bad cast" );
    ULIS_ASSERT( cmd_args, "Bad cast" );
    IMP( job_args, cmd_args );
}

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
        , const IJobArgs* iArgs
    );

    /*! Explicitely deleted default constructor. */
    FJob() = delete;

    /*! Explicitely deleted copy constructor. */
    FJob( const FJob& ) = delete;

    /*! Explicitely deleted move constructor. */
    FJob( FJob&& ) = delete;

    /*! Explicitely deleted copy assignment operator. */
    FJob& operator=( const FJob& ) = delete;

    /*! Explicitely deleted move assignment operator. */
    FJob& operator=( FJob&& ) = delete;

    /*! Start exec job tasks. */
    void Execute();

    /*! Return the parent command */
    const FCommand* Parent() const;

private:
    uint32 mNumTasks;
    fpTask mTask;
    const IJobArgs* mArgs;
    const FCommand* mParent;
};

ULIS_NAMESPACE_END

