// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         ThreadInterface.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the ThreadInterface entry points.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Thread/ThreadInterface.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN

FThreadPool*
XCreateThreadPool( uint32 iCount ) {
    return  new  FThreadPool( iCount );
}


void
XDeleteThreadPool( FThreadPool* iThreadPool ) {
    ULIS3_ASSERT( iThreadPool, "Invalid Input Arg" );
    iThreadPool->WaitForCompletion();
    delete  iThreadPool;
}


void
Fence( FThreadPool& iPool ) {
    iPool.WaitForCompletion();
}


void
SetNumWorkers( FThreadPool& iPool, uint32 iValue ) {
    iPool.SetNumWorkers( iValue );
}


uint32
GetProcessed( FThreadPool& iPool ) {
    return  iPool.GetProcessed();
}


uint32
GetNumWorkers( FThreadPool& iPool ) {
    return  iPool.GetNumWorkers();
}


uint32
MaxWorkers() {
    return  FThreadPool::MaxWorkers();
}


ULIS3_NAMESPACE_END

