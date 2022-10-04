// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CommandQueue.cpp
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides the definition for the FCommandQueue class.
* @license      Please refer to LICENSE.md
*/
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"

ULIS_NAMESPACE_BEGIN
FCommandQueue::~FCommandQueue()
{
    delete  d;
}

FCommandQueue::FCommandQueue( FThreadPool& iPool )
    : d( new  FCommandQueue_Private( iPool ) )
{
}

void
FCommandQueue::Flush()
{
    d->Flush();
}

void
FCommandQueue::Finish()
{
    d->Finish();
}

void
FCommandQueue::Fence()
{
    d->Fence();
}

ULIS_NAMESPACE_END

