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
typedef void (*fpCommandScheduler)( FCommand*, const FSchedulePolicy&, bool );

/////////////////////////////////////////////////////
// ResolveScheduleCommandCall
template< typename ARGS, void (*IMP)( const ARGS*, const FSchedulePolicy&, bool ) >
static ULIS_FORCEINLINE void ResolveScheduleCommandCall( const ICommandArgs* iArgs, const FSchedulePolicy& iPolicy, bool iContiguous )
{
    const ARGS* args = dynamic_cast< const ARGS* >( iArgs );
    ULIS_ASSERT( args, "Bad cast" );
    IMP( args, iPolicy, iContiguous );
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

    /*! Get the args */
    const ICommandArgs* Args() const;

    /*! Reserve some space for jobs */
    void ReserveJobs( uint64 iNum );

    /*! Add a job */
    void AddJob( FJob* iJob );

    bool IsReady() const;

    FSharedInternalEvent Event() const;

    void Execute( FThreadPool& iPool ) const;

private:
    const ICommandArgs* mArgs;
    FSharedInternalEvent mEvent;
    TArray< FJob* > mJobs;
};

ULIS_NAMESPACE_END

