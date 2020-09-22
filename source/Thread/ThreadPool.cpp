// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         ThreadPool.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FThreadPool class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Thread/ThreadPool.h"
#include <thread>

#ifdef ULIS_COMPILED_WITH_THREAD_SUPPORT
#include "Thread/ThreadPool_Private_Generic.h"
#else
#include "Thread/ThreadPool_Private_Mono.h"
#endif

ULIS_NAMESPACE_BEGIN
FThreadPool::~FThreadPool()
{
    delete  d;
}

FThreadPool::FThreadPool( uint32 iNumWorkers )
    : d( new FThreadPool_Private( iNumWorkers ) )
{
}

void
FThreadPool::ScheduleJob( FCommand* iCommand )
{
    d->ScheduleJob( iCommand );
}

void
FThreadPool::WaitForCompletion()
{
    d->WaitForCompletion();
}

void
FThreadPool::SetNumWorkers( uint32 iNumWorkers )
{
    d->SetNumWorkers( iNumWorkers );
}

uint32
FThreadPool::GetNumWorkers() const
{
    return  d->GetNumWorkers();
}

//static
uint32
FThreadPool::MaxWorkers()
{
    return  FThreadPool_Private::MaxWorkers();
}

ULIS_NAMESPACE_END

