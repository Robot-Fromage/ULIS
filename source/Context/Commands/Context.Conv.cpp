// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Conv.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the conv API entry
*               points in the FContext class.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Conv/Conv.h"
#include "Process/Conv/Conv.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
ulError
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
    // In case of same format, we can optimize by using the faster Copy version,
    // since no conversion is actually involved.
    // Warning though: this works only if it is also the same format as the context.
    // If source and destination have the same format, but are not in the context
    // format, in order to keep the copy optimization and allow conv to work on
    // any format as part of the conv exception, the conv fall backs again to the
    // proper conv invocation that will perform a low perf format aware "copy conv".
    if( ( iSource.Format() == iDestination.Format() ) && ( iSource.Format() == Format() ) ) {
        return  FContext::Copy( iSource, iDestination, iSourceRect, iPosition, iPolicy, iNumWait, iWaitList, iEvent );
    }

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_roi = FRectI::FromPositionAndSize( iPosition, src_roi.Size() ) & dst_rect;

    // Check no-op
    if( dst_roi.Sanitized().Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
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
            , false // No chunk allowed, incompatible. ( ( src_roi == src_rect ) && ( dst_roi == dst_rect ) && ( src_rect == dst_rect ) )
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

