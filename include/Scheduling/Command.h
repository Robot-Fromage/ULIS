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
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SchedulePolicy.h"
#include "Scheduling/Event.h"

ULIS_NAMESPACE_BEGIN
class FCommand;
typedef void (*fpCommandScheduler)( const ICommandArgs*, const FSchedulePolicy&, FThreadPool& );

/////////////////////////////////////////////////////
// ResolveScheduleCommandCall
template< typename ARGS, void (*IMP)( const ARGS*, const FSchedulePolicy&, FThreadPool& ) >
static ULIS_FORCEINLINE void ResolveScheduleCommandCall( const ICommandArgs* iArgs, const FSchedulePolicy& iPolicy, FThreadPool& iPool )
{
    const ARGS* args = dynamic_cast< const ARGS* >( iArgs );
    ULIS_ASSERT( args, "Bad cast" );
    IMP( args, iPolicy, iPool );
}

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
        , const FEvent* iWaitList
        , FEvent* iEvent
    );

    /*! Explicitely deleted default constructor. */
    FCommand() = delete;

    /*! Explicitely deleted copy constructor. */
    FCommand( const FCommand& ) = delete;

    /*! Explicitely deleted move constructor. */
    FCommand( FCommand&& ) = delete;

    /*! Explicitely deleted copy assignment operator. */
    FCommand& operator=( const FCommand& ) = delete;

    /*! Explicitely deleted move assignment operator. */
    FCommand& operator=( FCommand&& ) = delete;

    /*!
        Check wether the command is ready to execute ( according to the event wait list ).
    */
    bool IsReady() const;

    /*! Start scheduling command. */
    void Execute( FThreadPool& iPool );

    /*! Get the policy. */
    const FSchedulePolicy& Policy() const;

    /*! Get the event */
    FEvent* Event() const;

    /*! Get the args */
    const ICommandArgs* Args() const;

private:
    fpCommandScheduler  mSched;
    const ICommandArgs* mArgs;
    FSchedulePolicy     mPolicy;
    uint32              mNumWait;
    const FEvent*   mWaitList;
    FEvent*         mEvent;
};

ULIS_NAMESPACE_END

