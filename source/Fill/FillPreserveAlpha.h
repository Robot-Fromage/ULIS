// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         FillPreserveAlpha.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for FillPreserveAlpha API.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Dispatch/Dispatcher.h"
#include "Image/Color.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBuffer.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FFillPreserveAlphaCommandArgs
class FFillPreserveAlphaCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FFillPreserveAlphaCommandArgs() override {}
    FFillPreserveAlphaCommandArgs(
          FBlock& iBlock
        , const FColor& iColor
        , const FRectI& iRect
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , color( iColor )
    {}

    const FColor color;
};

/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
template< typename T >
void
InvokeFillPreserveAlphaMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FFillPreserveAlphaCommandArgs* cargs
)
{
    const uint8* src = cargs->color.Bits();
    const FFormatMetrics& fmt = cargs->block.Format();
    const uint8 stride = cargs->block.BytesPerPixel();
    uint8* ULIS_RESTRICT dst = jargs->dst;
    for( uint32 i = 0; i < jargs->size; ++i ) {
        const T alpha = dst[ iFmt.AID ];
        memcpy( dst, src, stride );
        dst[ fmt.AID ] = alpha;
        dst += stride;
    }
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_DECLARE_DISPATCHER( FDispatchedFillPreserveAlphaInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedFillPreserveAlphaInvocationSchedulerSelector
    , &ScheduleSimpleBufferJobs<
          FSimpleBufferJobArgs
        , FFillPreserveAlphaCommandArgs
        , &InvokeFillPreserveAlphaMT_MEM< T >
    >
)
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedFillPreserveAlphaInvocationSchedulerSelector )

ULIS_NAMESPACE_END

