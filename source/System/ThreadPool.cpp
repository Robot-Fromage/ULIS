// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FThreadPool class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/ThreadPool.h"
#include <thread>

#ifdef ULIS_COMPILED_WITH_THREAD_SUPPORT
#include "System/ThreadPool_Private_Generic.h"
#else
#include "System/ThreadPool_Private_Mono.h"
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
FThreadPool::ScheduleCommand( const FCommand* iCommand )
{
    d->ScheduleCommand( iCommand );
}

void
FThreadPool::ScheduleJob( const FJob* iJob )
{
    d->ScheduleJob( iJob );
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

