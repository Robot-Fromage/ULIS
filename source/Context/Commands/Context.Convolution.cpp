// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Convolution.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the Convolution API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Image/Block.h"
#include "Image/Kernel.h"
#include "Image/StructuringElement.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
ulError
FContext::Convolve(
      const FBlock& iSource
    , FBlock iDestination
    , const FKernel& iKernel
    , const FRectI& iSourceRect
    , const FVec2I& iPosition
    , eResamplingMethod iResamplingMethod
    , eBorderMode iBorderMode
    , const ISample& iBorderValue
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iDestination
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iDestination.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_roi = FRectI::FromPositionAndSize( iPosition, src_roi.Size() ) & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleConvolve
            , new FDualBufferCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
            )
            , iPolicy
            , false // force scanlines
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::MorphologicalProcess(
      const FBlock& iSource
    , FBlock iDestination
    , const FStructuringElement& iKernel
    , const FRectI& iSourceRect
    , const FVec2I& iPosition
    , eResamplingMethod iResamplingMethod
    , eBorderMode iBorderMode
    , const ISample& iBorderValue
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iDestination
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iDestination.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_roi = FRectI::FromPositionAndSize( iPosition, src_roi.Size() ) & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleMorphologicalProcess
            , new FDualBufferCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
            )
            , iPolicy
            , false // force scanlines
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

