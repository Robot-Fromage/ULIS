// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Raster.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementation of the Raster API entry
*               points in the FContext class.
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
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawLine
            , new FDrawLineCommandArgs(
                  iBlock
                , src_roi
                , iP0
                , iP1
                , color
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
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawLineAA
            , new FDrawLineCommandArgs(
                  iBlock
                , src_roi
                , iP0
                , iP1
                , color
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
FContext::DrawLineSP(
      FBlock& iBlock
    , const FVec2F& iP0
    , const FVec2F& iP1
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
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawLineSP
            , new FDrawLineSPCommandArgs(
                  iBlock
                , src_roi
                , iP0
                , iP1
                , color
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
FContext::DrawCircle(
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
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawCircle
            , new FDrawCircleCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iRadius
                , color
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
FContext::DrawCircleAA(
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
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawCircleAA
            , new FDrawCircleCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iRadius
                , color
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
FContext::DrawCircleSP(
      FBlock& iBlock
    , const FVec2F&            iCenter
    , const float              iRadius
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
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawCircleSP
            , new FDrawCircleSPCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iRadius
                , color
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
FContext::DrawArc(
      FBlock& iBlock
    , const FVec2I&            iCenter
    , const int                iRadius
    , const int                iStartDegree
    , const int                iEndDegree
    , const FColor&            iColor
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
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawArc
            , new FDrawArcCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iRadius
                , iStartDegree
                , iEndDegree
                , color
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
FContext::DrawArcAA(
      FBlock& iBlock
    , const FVec2I&            iCenter
    , const int                iRadius
    , const int                iStartDegree
    , const int                iEndDegree
    , const FColor&            iColor
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
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawArcAA
            , new FDrawArcCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iRadius
                , iStartDegree
                , iEndDegree
                , color
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
FContext::DrawArcSP(
      FBlock& iBlock
    , const FVec2F&            iCenter
    , const float              iRadius
    , const int                iStartDegree
    , const int                iEndDegree
    , const FColor&            iColor
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
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawArcSP
            , new FDrawArcSPCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iRadius
                , iStartDegree
                , iEndDegree
                , color
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
FContext::DrawEllipse(
      FBlock& iBlock
    , const FVec2I&            iCenter
    , const int                iA
    , const int                iB
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawEllipse
            , new FDrawEllipseCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iA
                , iB
                , color
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
FContext::DrawEllipseAA(
      FBlock& iBlock
    , const FVec2I&            iCenter
    , const int                iA
    , const int                iB
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawEllipseAA
            , new FDrawEllipseCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iA
                , iB
                , color
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
FContext::DrawEllipseSP(
      FBlock& iBlock
    , const FVec2F&            iCenter
    , const float              iA
    , const float              iB
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;

    // Check no-op
    if (src_roi.Area() <= 0)
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Convert color to right format
    FColor color = iColor.ToFormat( iBlock.Format() );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
            mContextualDispatchTable->mScheduleDrawEllipseSP
            , new FDrawEllipseSPCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iA
                , iB
                , color
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
FContext::DrawRotatedEllipse(
      FBlock&                  iBlock
    , const FVec2I&            iCenter
    , const int                iA
    , const int                iB
    , const int                iRotationDegrees
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawRotatedEllipse
            , new FDrawRotatedEllipseCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iA
                , iB
                , iRotationDegrees
                , color
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
FContext::DrawRotatedEllipseAA(
      FBlock&                  iBlock
    , const FVec2I&            iCenter
    , const int                iA
    , const int                iB
    , const int                iRotationDegrees
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawRotatedEllipseAA
            , new FDrawRotatedEllipseCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iA
                , iB
                , iRotationDegrees
                , color
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
FContext::DrawRotatedEllipseSP(
      FBlock&                  iBlock
    , const FVec2F&            iCenter
    , const float              iA
    , const float              iB
    , const int                iRotationDegrees
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawRotatedEllipseSP
            , new FDrawRotatedEllipseSPCommandArgs(
                  iBlock
                , src_roi
                , iCenter
                , iA
                , iB
                , iRotationDegrees
                , color
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
FContext::DrawRectangle(
      FBlock&                  iBlock
    , const FVec2I&            iTopLeft
    , const FVec2I&            iBottomRight
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    if( iFilled )
    {
        const int xmin = FMath::Min3(iTopLeft.x,iBottomRight.x,src_roi.x + src_roi.w);
        const int ymin = FMath::Min3(iTopLeft.y,iBottomRight.y,src_roi.y + src_roi.h);
        const int xmax = FMath::Max3(iTopLeft.x,iBottomRight.x,src_roi.x);
        const int ymax = FMath::Max3(iTopLeft.y,iBottomRight.y,src_roi.y);
        const int width = FMath::Max(xmax - xmin,0);
        const int height = FMath::Max(ymax - ymin,0);
        FRectI rect = FRectI( xmin, ymin, width, height );
        this->Fill( iBlock, color, rect, FSchedulePolicy::CacheEfficient, iNumWait, iWaitList, iEvent );
    }
    else
        // Bake and push command
        mCommandQueue.d->Push(
            new FCommand(
                  mContextualDispatchTable->mScheduleDrawRectangle
                , new FDrawRectangleCommandArgs(
                      iBlock
                    , src_roi
                    , iTopLeft
                    , iBottomRight
                    , color
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
FContext::DrawPolygon(
      FBlock&                  iBlock
    , const std::vector< FVec2I >&   iPoints
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawPolygon
            , new FDrawPolygonCommandArgs(
                  iBlock
                , src_roi
                , iPoints
                , color
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
FContext::DrawPolygonAA(
      FBlock&                  iBlock
    , const std::vector< FVec2I >&   iPoints
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawPolygonAA
            , new FDrawPolygonCommandArgs(
                  iBlock
                , src_roi
                , iPoints
                , color
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
FContext::DrawPolygonSP(
      FBlock&                  iBlock
    , const std::vector< FVec2F >&   iPoints
    , const FColor&            iColor
    , const bool               iFilled
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawPolygonSP
            , new FDrawPolygonSPCommandArgs(
                  iBlock
                , src_roi
                , iPoints
                , color
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
FContext::DrawQuadraticBezier(
      FBlock&                  iBlock
    , const FVec2I&            iCtrlPt0
    , const FVec2I&            iCtrlPt1
    , const FVec2I&            iCtrlPt2
    , const float              iWeight
    , const FColor&            iColor
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawQuadraticBezier
            , new FDrawQuadraticBezierCommandArgs(
                  iBlock
                , src_roi
                , iCtrlPt0
                , iCtrlPt1
                , iCtrlPt2
                , iWeight
                , color
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
FContext::DrawQuadraticBezierAA(
      FBlock&                  iBlock
    , const FVec2I&            iCtrlPt0
    , const FVec2I&            iCtrlPt1
    , const FVec2I&            iCtrlPt2
    , const float              iWeight
    , const FColor&            iColor
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawQuadraticBezierAA
            , new FDrawQuadraticBezierCommandArgs(
                  iBlock
                , src_roi
                , iCtrlPt0
                , iCtrlPt1
                , iCtrlPt2
                , iWeight
                , color
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
FContext::DrawQuadraticBezierSP(
      FBlock&                  iBlock
    , const FVec2F&            iCtrlPt0
    , const FVec2F&            iCtrlPt1
    , const FVec2F&            iCtrlPt2
    , const float              iWeight
    , const FColor&            iColor
    , const FRectI&            iClippingRect
    , const FSchedulePolicy&   iPolicy
    , uint32                   iNumWait
    , const FEvent*            iWaitList
    , FEvent*                  iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iClippingRect.Sanitized() & src_rect;
    
    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );
    
    // Convert color to right format
    FColor color = iColor.ToFormat(iBlock.Format());

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleDrawQuadraticBezierSP
            , new FDrawQuadraticBezierSPCommandArgs(
                  iBlock
                , src_roi
                , iCtrlPt0
                , iCtrlPt1
                , iCtrlPt2
                , iWeight
                , color
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

