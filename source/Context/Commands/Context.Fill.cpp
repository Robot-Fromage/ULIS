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
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Fill/Fill.h"
#include "Process/Conv/Conv.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
void
FContext::Fill(
          FBlock& iBlock
        , const FRectI& iRect
        , const ISample& iColor
        , const FSchedulePolicy& iPolicy
        , uint32 iNumWait
        , const FEvent* iWaitList
        , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI rect = iBlock.Rect();
    const FRectI roi = iRect.Sanitized() & rect;

    // Check no-op
    if( roi.Area() <= 0 )
        return  FinishEventNoOP( iEvent );

    // Forward arguments baking
    // This one is a bit tricky so here is a breakdown of the steps:
    FColor color    = iColor.ToFormat( iBlock.Format() );       // iColor can be in any format, so first we convert it to the block format.
    uint8* srcb     = color.Bits();
    uint8 bpp       = iBlock.BytesPerPixel();                   // We gather the Bytes Per Pixel for the format
    uint32 size     = FMath::Max( uint32(32), uint32( bpp ) );  // We define a size that is max of 32 ( avx2 ) and BPP ( bytes )
    uint32 stride   = size - ( size % bpp );
    uint8* buf      = new uint8[ size ];                        // We allocate a buffer that is length size, it will be deleted in ~FFillCommandArgs()
    for( uint32 i = 0; i < stride; i+= bpp )                    // We repeat the color N times in the buffer ( as many can fit )
        memcpy( (void*)( ( buf ) + i ), srcb, bpp );


    // Bake and push command
    mCommandQueue.Push(
        new FCommand(
              mContextualDispatchTable->mScheduleFill
            , new FFillCommandArgs(
                  iBlock
                , buf
                , roi
            )
            , iPolicy
            , roi == rect
            , iNumWait
            , iWaitList
            , iEvent
        )
    );
}

void
FContext::FillPreserveAlpha(
          FBlock& iBlock
        , const FRectI& iRect
        , const ISample& iColor
        , const FSchedulePolicy& iPolicy
        , uint32 iNumWait
        , const FEvent* iWaitList
        , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI rect = iBlock.Rect();
    const FRectI roi = iRect.Sanitized() & rect;

    // Check no-op
    if( roi.Area() <= 0 )
        return  FinishEventNoOP( iEvent );

    // Bake and push command
    mCommandQueue.Push(
        new FCommand(
              mContextualDispatchTable->mScheduleFillPreserveAlpha
            , new FFillPreserveAlphaCommandArgs(
                  iBlock
                , iColor.ToFormat( iBlock.Format() )
                , roi
            )
            , iPolicy
            , roi == rect
            , iNumWait
            , iWaitList
            , iEvent
        )
    );
}

ULIS_NAMESPACE_END

