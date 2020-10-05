// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         CommandQueue.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FCommandQueue class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
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

void
FCommandQueue::Push( FCommand* iCommand )
{
    d->Push( iCommand );
}

ULIS_NAMESPACE_END

