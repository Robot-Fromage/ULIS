// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for the FContext core API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Process/Custom/No_OP.h"

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
    , ePerformanceIntent iPerfIntent
)
    : mCommandQueue( iQueue )
    , mContextualDispatchTable( new  FContextualDispatchTable( FHardwareMetrics(), iFormat, iPerfIntent ) )
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

void
FContext::Wait( const FEvent& iEvent )
{
    iEvent.Wait();
}

eFormat
FContext::Format() const
{
    return  mContextualDispatchTable->mFormat;
}

ulError
FContext::FinishEventNo_OP( FEvent* iEvent, ulError iError )
{
    if( iEvent )
        iEvent->d->m->NotifyAllJobsFinished();

    return  iError;
}

ulError
FContext::Dummy_OP(
      uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    mCommandQueue.d->Push(
        new FCommand(
              &ScheduleNo_OP
            , new FNo_OPCommandArgs()
            , FSchedulePolicy()
            , true
            , true
            , iNumWait
            , iWaitList
            , iEvent
            , FRectI()
        )
    );

    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

