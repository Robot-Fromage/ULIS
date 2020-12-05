// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Context.Conv.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the conv API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Conv/Conv.h"
#include "Conv/ConvertFormatInvocations.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
void
FContext::ConvertFormat(
          const FBlock& iSource
        , FBlock& iDestination
        , const FRectI& iSourceRect
        , const FVec2I& iPosition
        , const FSchedulePolicy& iPolicy
        , uint32 iNumWait
        , const FEvent* iWaitList
        , FEvent* iEvent
)
{
    ULIS_ASSERT( &iSource != &iDestination, "Source and Backdrop are the same block." );

    // In case of same format, we can optimize by using the faster Copy version,
    // since no conversion is actually involved.
    if( iSource.Format() == iDestination.Format(), "Formats mismatch." ) {
        FContext::Copy( iSource, iDestination, iSourceRect, iPosition, iPolicy, iNumWait, iWaitList, iEvent );
        return;
    }

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_roi = FRectI::FromPositionAndSize( iPosition, src_roi.Size() ) & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNoOP( iEvent );

    // Bake and push command
    mCommandQueue.Push(
        new FCommand(
              mContextualDispatchTable->mScheduleConvertFormat
            , new FConvCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
                , QueryDispatchedConvertFormatInvocation( iSource.Format(), iDestination.Format() )
            )
            , iPolicy
            , ( ( src_roi == src_rect ) && ( dst_roi == dst_rect ) && ( src_rect == dst_rect ) )
            , iNumWait
            , iWaitList
            , iEvent
        )
    );
}

ULIS_NAMESPACE_END

