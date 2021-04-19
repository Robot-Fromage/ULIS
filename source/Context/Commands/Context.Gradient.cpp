// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Fill.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the fill API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Gradient/Gradient.h"
#include "Image/Block.h"
#include "Image/Color.h"
#include "Image/Gradient.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
ulError
FContext::RasterGradient(
      FBlock& iBlock
    , const FVec2F& iStart
    , const FVec2F& iEnd
    , const FSanitizedGradient& iGradient
    , float iDithering
    , eGradientType iType
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR( iGradient.Format()    == Format(), "Bad format", ULIS_ERROR_FORMATS_MISMATCH )
    ULIS_ASSERT_RETURN_ERROR( iBlock.Format()       == Format(), "Bad format", ULIS_ERROR_FORMATS_MISMATCH )

    // Sanitize geometry
    const FRectI rect = iBlock.Rect();
    const FRectI roi = iRect.Sanitized() & rect;

    // Check no-op
    if( roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleRasterGradient( iType )
            , new FGradientCommandArgs(
                  iBlock
                , roi
                , iStart
                , iEnd
                , iGradient
                , iDithering
                , iType
            )
            , iPolicy
            , false
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , roi
        )
    );

    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

