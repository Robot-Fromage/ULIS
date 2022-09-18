// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Vector.cpp
* @author       Gary GABRIEL
* @brief        This file provides the implementation of the Vector API entry
*               points in the FContext class.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Vector/VectorInvocations.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN

ulError
FContext::DrawVectorObject ( 
      FBlock& iBlock
    , FVectorObject& iVecObj
    , BLContext& iBLCtx
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

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawVectorObject
            , new FDrawVectorObjectCommandArgs(
                  iBlock
                , src_roi
                , iVecObj
                , iBLCtx
            )
            , iPolicy
            , true
            , true // force mono because we use Blend2D.
            , iNumWait
            , iWaitList
            , iEvent
            , src_roi
        )
    );

    return  ULIS_NO_ERROR;
}
ULIS_NAMESPACE_END
