// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Command.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FCommand class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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
class FJob;
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

    /*! Reserve some space for jobs */
    void ReserveJobs( uint64 iNum );

    /*! Add a job */
    void AddJob( const FJob* iJob );

    /*! Query num jobs */
    uint64 NumJobs() const;

    /*!
    Check if the command is ready for processing.
    That is, all events in wait list are finished.
    */
    bool ReadyForProcessing() const;

    /*!
    Check if the command is ready for scheduling.
    That is, all events in wait list are scheduled too.
    If an event in wait list is still idle, the command will block forever.
    */
    bool ReadyForScheduling() const;

    void ProcessAsyncScheduling();

    FSharedInternalEvent Event() const;

    const TArray< const FJob* >& Jobs() const;

private:
    const ICommandArgs* mArgs;
    FSharedInternalEvent mEvent;
    TArray< const FJob* > mJobs;
    fpCommandScheduler mSched;
    FSchedulePolicy mPolicy;
    bool mContiguous;
    bool mForceMonoChunk;
    bool mScheduled;
};

ULIS_NAMESPACE_END

