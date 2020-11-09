// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Context.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the clear API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Clear/ClearArgs.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
void
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
        return  FinishEventNoOP( iEvent );

    // Forward arguments baking
    // Check wether the whole image buffer is to be cleaned.
    // If so, chunk based scheduling policy are made available.
    const bool whole = src_roi == src_rect;

    // Bake and push command
    mCommandQueue.Push(
        new FCommand(
              mContextualDispatchTable->mScheduleClear
            , new FClearCommandArgs(
                  iBlock
                , src_roi
                , whole
            )
            , iPolicy
            , iNumWait
            , iWaitList
            , iEvent
        )
    );
}

ULIS_NAMESPACE_END

