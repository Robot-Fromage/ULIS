// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Context.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for the FContext core API.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Blend/BlendDispatch.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FContext: Construction / Destruction
FContext::~FContext()
{
    delete  mContextualDispatchTable;
}

FContext::FContext(
      FCommandQueue& iQueue
    , eFormat iFormat
)
    : mCommandQueue( iQueue )
    , mFormat( iFormat )
    , mHardwareMetrics( FHardwareMetrics() )
    , mContextualDispatchTable( new  FContextualDispatchTable( mHardwareMetrics, mFormat ) )
{
}

/////////////////////////////////////////////////////
// FContext: Control Flow
void
FContext::Flush()
{
    mCommandQueue.Flush();
}

void
FContext::Finish()
{
    mCommandQueue.Finish();
}

void
FContext::Fence()
{
    mCommandQueue.Fence();
}

eFormat
FContext::Format() const
{
    return  mFormat;
}

void
FContext::FinishEventNoOP( FEvent* iEvent )
{
    if( iEvent ) {
        iEvent->d->m->SetFinished();
    }
}

ULIS_NAMESPACE_END

