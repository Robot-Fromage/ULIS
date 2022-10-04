// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool.cpp
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides the definition for the FThreadPool class.
* @license      Please refer to LICENSE.md
*/
#include "System/ThreadPool/ThreadPool.h"
#include "System/ThreadPool/ThreadPool_Private.h"

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

