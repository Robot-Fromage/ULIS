// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Command.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FCommand class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/CommandArgs.h"
#include "Scheduling/SchedulePolicy.h"
#include "Scheduling/TaskEvent.h"

ULIS_NAMESPACE_BEGIN
class FCommand;
typedef void (*fpCommandScheduler)( const ICommandArgs* );

/////////////////////////////////////////////////////
/// @class      FCommand
/// @brief      The FCommand class provides a way to store awaiting commands in
///             a command queue, with all informations needed for scheduling and
///             arguments about the operation.
/// @details    The FCommand is used by FCommandQueue and stores information about
///             a command which status is read through a FTaskEvent, which
///             scheduling information is stored in a FSchedulePolicy, and
///             which operation arguments are stored in a ICommandArgs child class.
///
///             \sa FCommandQueue
class ULIS_API FCommand {
public:
    /*! Destructor */
    ~FCommand();

    /*! Constructor */
    FCommand(
          fpCommandScheduler iSched
        , const ICommandArgs* iArgs
        , const FSchedulePolicy& iPolicy
        , uint32 iNumWait
        , const FTaskEvent* iWaitList
        , FTaskEvent* iEvent
    );

    FCommand() = delete;
    FCommand( const FCommand& ) = delete;
    FCommand( FCommand&& ) = delete;
    FCommand& operator=( const FCommand& ) = delete;
    FCommand& operator=( FCommand&& ) = delete;

    bool IsReady() const;
    void Execute();

private:
    fpCommandScheduler  mSched;
    const ICommandArgs* mArgs;
    FSchedulePolicy     mPolicy;
    uint32              mNumWait;
    const FTaskEvent*   mWaitList;
    FTaskEvent*         mEvent;
};

ULIS_NAMESPACE_END

