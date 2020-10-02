// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Event.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the FEvent class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FEvent
/// @brief      The FEvent class provides a way to get asynchronous status
///             information about how a task is being processed, once it has been
///             dispatched and scheduled on a multithreaded system, in
///             coordination with a FOldThreadPool and a FCommandQueue.
/// @details    The FEvent allows to get a handle of the chosen policy for
///             a given task, and wether it has been completed or not. It is used
///             in conjunction with FOldThreadPool, FSchedulePolicy, FCommandQueue
///             and FRasterContext.
///
///             \sa FRasterContext
///             \sa FSchedulePolicy
///             \sa FOldThreadPool
///             \sa FHardwareMetrics
///             \sa FCommandQueue
class ULIS_API FEvent
{
    friend class FRasterContext;
    friend class FCommandQueue;
    friend class FThreadPool;
    friend class FThreadPool_Private;
    friend class FInternalEvent;

public:
    /*! Destructor */
    ~FEvent();

    /*! Constructor */
    FEvent();

    /*! Getter for status */
    bool Finished() const;

    /*! Check wether the Event is already hooked or not */
    bool Hooked() const;

private:
    /*! Set status to finished */
    void SetFinished();

    /*! Hook internal event */
    void Hook( FInternalEvent* iHandle );

    /*! Getter for internal event */
    FInternalEvent* Handle() const;

private:
    bool bFinished;
    FInternalEvent* mHandle;
};

ULIS_NAMESPACE_END

