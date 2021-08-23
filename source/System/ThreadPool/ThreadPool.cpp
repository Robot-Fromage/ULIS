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

