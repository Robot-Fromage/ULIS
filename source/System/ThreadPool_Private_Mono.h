// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool_Private_Mono.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the FThreadPool_Private
*               class for systems without actual thread support.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/ThreadPool.h"
#include "Scheduling/Job.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FThreadPool_Private
/// @brief      The FThreadPool_Private class provides the private implementation
///             for FThreadPool using the pimpl idiom.
/// @details    This version of the private implementation is for systems
///             without actual multi threading support.
///
///             \sa FThreadPool
class FThreadPool::FThreadPool_Private
{
public:
    ~FThreadPool_Private();
    FThreadPool_Private( uint32 iNumWorkers );
    void ScheduleJob( FJob* iJob );
    void WaitForCompletion();
    void SetNumWorkers( uint32 iNumWorkers );
    uint32 GetNumWorkers() const;
    static uint32 MaxWorkers();
};

FThreadPool::FThreadPool_Private::~FThreadPool_Private()
{
}

FThreadPool::FThreadPool_Private::FThreadPool_Private( uint32 iNumWorkers )
{
}

void
FThreadPool::FThreadPool_Private::ScheduleJob( FJob* iJob )
{
    iJob->Execute();

    FEvent* evt = iJob->Parent()->Event();
    if( evt )
        evt->SetFinished();

    delete  iJob;
}

void
FThreadPool::FThreadPool_Private::WaitForCompletion()
{
}

void
FThreadPool::FThreadPool_Private::SetNumWorkers( uint32 iNumWorkers )
{
}

uint32
FThreadPool::FThreadPool_Private::GetNumWorkers() const
{
    return  1;
}

//static
uint32
FThreadPool::FThreadPool_Private::MaxWorkers()
{
    return  1;
}

ULIS_NAMESPACE_END

