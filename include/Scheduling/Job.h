// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
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
#include "Scheduling/TaskEvent.h"

ULIS_NAMESPACE_BEGIN
class FJob;
typedef void (*fpScheduledJob)( const IJobArgs*, const ICommandArgs* );
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
class ULIS_API FJob {
public:
    /*! Destructor */
    ~FJob();

    /*! Constructor */
    FJob(
          uint32 iNumTasks
        , fpScheduledJob* iTasks
        , FCommand* iParent
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

private:
    uint32 mNumTasks;
    fpScheduledJob* mTasks;
    FCommand* mParent;
};

ULIS_NAMESPACE_END

