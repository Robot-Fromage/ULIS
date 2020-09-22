// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         CommandQueue.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FCommandQueue class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/Queue.h"
#include "Scheduling/Command.h"

ULIS_NAMESPACE_BEGIN
// Export of TQueue< FCommand >
template class ULIS_API TQueue< FCommand* >;

/////////////////////////////////////////////////////
/// @class      FCommandQueue
/// @brief      The FCommandQueue class provides a way to enqueue tasks for being
///             processed asynchronously in coordination with a FOldThreadPool
/// @details    The FCommandQueue stores a TQueue of FCommand and schedules the
///             commands on the FOldThreadPool.
///
///             \sa FCommand
///             \sa FOldThreadPool
class ULIS_API FCommandQueue
{
    friend class FRasterContext;
    typedef TQueue< FCommand* > tQueue;

public:
    /*! Destructor */
    ~FCommandQueue();

    /*! Constructor */
    FCommandQueue( FThreadPool& iPool );

public:
    /*!
        Issue all commands and return immediately.
    */
    void Flush();

    /*!
        Issue all commands and wait for completion
    */
    void Finish();

    /*!
        Wait for completion of all already issued commands
    */
    void Fence();

private:
    /*!
        Push, insert a new command at the end of the queue.
    */
    void Push( FCommand* iCommand );

    /*!
        Cleanse the idle queue.
    */
    void CleanseIdle();

    /*!
        Cleanse the scheduled queue.
    */
    void CleanseScheduled();

private:
    tQueue mIdleQueue;
    tQueue mScheduledQueue;
    FThreadPool& mPool;
};

ULIS_NAMESPACE_END

