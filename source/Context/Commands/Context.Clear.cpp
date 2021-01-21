// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Clear.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the clear API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Clear/Clear.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Scheduling/SimpleBufferArgs.h"

ULIS_NAMESPACE_BEGIN
ulError
FContext::Clear(
          FBlock& iBlock
        , const FRectI& iRect
        , const FSchedulePolicy& iPolicy
        , uint32 iNumWait
        , const FEvent* iWaitList
        , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iRect.Sanitized() & src_rect;

    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleClear
            , new FSimpleBufferCommandArgs(
                  iBlock
                , src_roi
            )
            , iPolicy
            , src_roi == src_rect
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , src_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

