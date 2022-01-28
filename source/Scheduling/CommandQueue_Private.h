// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CommandQueue_Private.h
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides the declaration for the FCommandQueue_Private class.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/Queue.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/Command.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FCommandQueue_Private
/// @brief      The FCommandQueue_Private class provides a way to enqueue tasks for being
///             processed asynchronously in coordination with a FThreadPool
/// @details    The FCommandQueue_Private stores a TQueue of FCommand and schedules the
///             commands on the FThreadPool.
///
///             \sa FCommand
///             \sa FThreadPool
class FCommandQueue_Private
{
    typedef TQueue< const FCommand* > tQueue;

public:
    /*! Destructor */
    ~FCommandQueue_Private();

    /*! Constructor */
    FCommandQueue_Private( FThreadPool& iPool );

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

    /*!
        Push, insert a new command at the end of the queue.
    */
    void Push( const FCommand* iCommand );

private:
    FThreadPool& mPool;
    tQueue mQueue;
};

ULIS_NAMESPACE_END

