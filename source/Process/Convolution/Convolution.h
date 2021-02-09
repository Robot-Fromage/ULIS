// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Convolution.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Convolution API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Image/Color.h"
#include "Image/Block.h"
#include "Image/Kernel.h"
#include "Math/Geometry/Rectangle.h"
#include "Process/Conv/srgb2linear.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Args
class FConvolutionCommandArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FConvolutionCommandArgs() override {}
    FConvolutionCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , const FKernel& iKernel
    )
        : FDualBufferCommandArgs( iSrc, iDst, iSrcRect, iDstRect )
        , kernel( iKernel )
        {}

    const FKernel& kernel;
};

/////////////////////////////////////////////////////
// Invocations
template< typename T >
void
InvokeConvolutionMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FConvolutionCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->src.FormatMetrics();
    uint8*       ULIS_RESTRICT  dst = jargs->dst;
    float* sum = new float[fmt.SPP];

    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        for( int i = 0; i < cargs->kernel.Width(); ++i ) {
            for( int j = 0; j < cargs->kernel.Width(); ++j ) {
                int src_x = x + i - cargs->kernel.Pivot().x;
                int src_y = jargs->line + j - cargs->kernel.Pivot().y;
                FColor color = cargs->src.Sample( src_x, src_y );
                float value = cargs->kernel.At( i, j );
                for( int k = 0; k < fmt.SPP; ++k ) {
                    sum[k] += ( *( (T*)( color.Bits() + k ) ) ) * value;
                }
            }
        }
        for( int k = 0; k < fmt.SPP; ++k ) {
            ( *( (T*)( dst + k ) ) ) = FMath::Clamp( static_cast< T >( sum[k] ), MinType< T >(), MaxType< T >() );
        }
        dst += fmt.BPP;
    }

    delete [] sum;
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleConvolutionMT_MEM_Generic, FDualBufferJobArgs, FConvolutionCommandArgs, &InvokeConvolutionMT_MEM_Generic< T > )
ULIS_DECLARE_DISPATCHER( FDispatchedConvolutionInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedConvolutionInvocationSchedulerSelector, &ScheduleConvolutionMT_MEM_Generic< T > )

ULIS_NAMESPACE_END

