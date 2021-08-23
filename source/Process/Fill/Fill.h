// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Fill.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Fill API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Image/Color.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FFillCommandArgs
class FFillCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FFillCommandArgs() override
    {
        delete [] buffer;
    }

    FFillCommandArgs(
          FBlock& iBlock
        , const uint8* iBuffer
        , const FRectI& iRect
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , buffer( iBuffer )
    {}

    const uint8* buffer;
};

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
// Invocations FillPreserveAlpha
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
template< typename T >
void
InvokeFillPreserveAlphaMT_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FFillPreserveAlphaCommandArgs* cargs
)
{
    const uint8* src = cargs->color.Bits();
    const FFormatMetrics& fmt = cargs->dst.Format();
    T* ULIS_RESTRICT dst = reinterpret_cast< T* >( jargs->dst );
    for( uint32 i = 0; i < jargs->size; ++i ) {
        const T alpha = dst[ fmt.AID ];
        memcpy( dst, src, fmt.BPP );
        dst[ fmt.AID ] = alpha;
        dst += fmt.SPP;
    }
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleFillPreserveAlphaMT_MEM_Generic, FSimpleBufferJobArgs, FFillPreserveAlphaCommandArgs, &InvokeFillPreserveAlphaMT_MEM_Generic< T > )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleFillMT_AVX );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleFillMT_SSE );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleFillMT_MEM );
ULIS_DECLARE_DISPATCHER( FDispatchedFillInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP(
      FDispatchedFillInvocationSchedulerSelector
    , &ScheduleFillMT_AVX
    , &ScheduleFillMT_SSE
    , &ScheduleFillMT_MEM
)
ULIS_DECLARE_DISPATCHER( FDispatchedFillPreserveAlphaInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedFillPreserveAlphaInvocationSchedulerSelector
    , &ScheduleFillPreserveAlphaMT_MEM_Generic< T >
)

ULIS_NAMESPACE_END

