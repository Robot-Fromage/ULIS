// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Raster.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementation of the Raster API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Raster/RasterInvocations.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN

ulError
FContext::DrawLine(
      FBlock& iBlock
    , const FVec2I& iP0
    , const FVec2I& iP1
    , const FColor& iColor
    , const FRectI& iClippingRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawLine
            , new FDrawLineCommandArgs(
                  iBlock
                , src_roi
                , iP0
                , iP1
                , iColor
            )
            , iPolicy
            , false
            , true
            , iNumWait
            , iWaitList
            , iEvent
            , src_roi
        )
    );
    
    return  ULIS_NO_ERROR;
}

ulError
FContext::DrawLineAA(
      FBlock& iBlock
    , const FVec2I& iP0
    , const FVec2I& iP1
    , const FColor& iColor
    , const FRectI& iClippingRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawLineAA
            , new FDrawLineCommandArgs(
                  iBlock
                , src_roi
                , iP0
                , iP1
                , iColor
            )
            , iPolicy
            , false
            , true
            , iNumWait
            , iWaitList
            , iEvent
            , src_roi
        )
    );
    
    return  ULIS_NO_ERROR;
}

ulError
FContext::DrawCircleAndres(
      FBlock& iBlock
    , const FVec2I&            iCenter
    , const int                iRadius
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawCircleAndres
            , new FDrawCircleCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iRadius
                , iColor
                , iFilled
            )
            , iPolicy
            , false
            , true
            , iNumWait
            , iWaitList
            , iEvent
            , src_roi
        )
    );
    
    return  ULIS_NO_ERROR;
}

ulError
FContext::DrawCircleAndresAA(
      FBlock& iBlock
    , const FVec2I&            iCenter
    , const int                iRadius
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawCircleAndresAA
            , new FDrawCircleCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iRadius
                , iColor
                , iFilled
            )
            , iPolicy
            , false
            , true
            , iNumWait
            , iWaitList
            , iEvent
            , src_roi
        )
    );
    
    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

