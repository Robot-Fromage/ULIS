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
    const FFormatMetrics& fmt = cargs->src.FormatMetrics();
    T* ULIS_RESTRICT dst = reinterpret_cast< T* >( jargs->dst );
    float* sum = new float[fmt.SPP];
    const int maxx = cargs->kernel.Width();
    const int maxy = cargs->kernel.Height();
    const int dx = cargs->kernel.Pivot().x - maxx;
    const int dy = cargs->kernel.Pivot().y - maxy;
    FColor transparent( fmt.FMT );
    const T* src = nullptr;

    // Gather x y
    const int y = jargs->line + cargs->srcRect.y;
    const int x1 = cargs->srcRect.x;
    const int x2 = cargs->dstRect.w + x1;
    for( int x = x1; x < x2; ++x ) {
        memset( sum, 0, fmt.SPP * sizeof( float ) );
        for( int i = 0; i < maxx; ++i ) {
            for( int j = 0; j < maxy; ++j ) {
                int src_x = x + i + dx;
                int src_y = y + j + dy;
                if( cargs->srcRect.HitTest( FVec2I( src_x, src_y ) ) )
                    src = reinterpret_cast< const T* >( cargs->src.PixelBits( src_x, src_y ) );
                else
                    src = reinterpret_cast< const T* >( transparent.Bits() );

                float value = cargs->kernel.At( i, j );
                for( int k = 0; k < fmt.SPP; ++k ) {
                    sum[k] += *( src + k ) * value;
                }
            }
        }
        for( int k = 0; k < fmt.SPP; ++k ) {
            *( dst + k ) = FMath::Clamp( static_cast< T >( sum[k] ), MinType< T >(), MaxType< T >() );
        }
        dst += fmt.SPP;
    }

    delete [] sum;
}

template< typename T >
void
InvokeConvolutionMaxMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FConvolutionCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->src.FormatMetrics();
    T* ULIS_RESTRICT dst = reinterpret_cast< T* >( jargs->dst );
    float* sum = new float[fmt.SPP];
    const int maxx = cargs->kernel.Width();
    const int maxy = cargs->kernel.Height();
    const int dx = cargs->kernel.Pivot().x - maxx;
    const int dy = cargs->kernel.Pivot().y - maxy;
    FColor transparent( fmt.FMT );
    const T* src = nullptr;

    // Gather x y
    const int y = jargs->line + cargs->srcRect.y;
    const int x1 = cargs->srcRect.x;
    const int x2 = cargs->dstRect.w + x1;
    for( int x = x1; x < x2; ++x ) {
        memset( sum, 0, fmt.SPP * sizeof( float ) );
        for( int i = 0; i < maxx; ++i ) {
            for( int j = 0; j < maxy; ++j ) {
                int src_x = x + i + dx;
                int src_y = y + j + dy;
                if( cargs->srcRect.HitTest( FVec2I( src_x, src_y ) ) )
                    src = reinterpret_cast< const T* >( cargs->src.PixelBits( src_x, src_y ) );
                else
                    src = reinterpret_cast< const T* >( transparent.Bits() );

                float value = cargs->kernel.At( i, j );
                for( int k = 0; k < fmt.SPP; ++k ) {
                    sum[k] = FMath::Max( sum[k], *( src + k ) * value );
                }
            }
        }
        for( int k = 0; k < fmt.SPP; ++k ) {
            *( dst + k ) = FMath::Clamp( static_cast< T >( sum[k] ), MinType< T >(), MaxType< T >() );
        }
        dst += fmt.SPP;
    }

    delete [] sum;
}

template< typename T >
void
InvokeConvolutionMinMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FConvolutionCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->src.FormatMetrics();
    T* ULIS_RESTRICT dst = reinterpret_cast< T* >( jargs->dst );
    float* sum = new float[fmt.SPP];
    const int maxx = cargs->kernel.Width();
    const int maxy = cargs->kernel.Height();
    const int dx = cargs->kernel.Pivot().x - maxx;
    const int dy = cargs->kernel.Pivot().y - maxy;
    FColor transparent( fmt.FMT );
    const T* src = nullptr;

    // Gather x y
    const int y = jargs->line + cargs->srcRect.y;
    const int x1 = cargs->srcRect.x;
    const int x2 = cargs->dstRect.w + x1;
    for( int x = x1; x < x2; ++x ) {
        for( int i = 0; i < fmt.SPP; ++i )
            sum[i] = ULIS_FLOAT_MAX;

        for( int i = 0; i < maxx; ++i ) {
            for( int j = 0; j < maxy; ++j ) {
                int src_x = x + i + dx;
                int src_y = y + j + dy;
                if( cargs->srcRect.HitTest( FVec2I( src_x, src_y ) ) )
                    src = reinterpret_cast< const T* >( cargs->src.PixelBits( src_x, src_y ) );
                else
                    src = reinterpret_cast< const T* >( transparent.Bits() );

                float value = cargs->kernel.At( i, j );
                for( int k = 0; k < fmt.SPP; ++k ) {
                    sum[k] = FMath::Min( sum[k], *( src + k ) * value );
                }
            }
        }
        for( int k = 0; k < fmt.SPP; ++k ) {
            *( dst + k ) = FMath::Clamp( static_cast< T >( sum[k] ), MinType< T >(), MaxType< T >() );
        }
        dst += fmt.SPP;
    }

    delete [] sum;
}

template< typename T >
void
InvokeConvolutionPremultMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FConvolutionCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->src.FormatMetrics();
    T* ULIS_RESTRICT dst = reinterpret_cast< T* >( jargs->dst );
    float* sum = new float[fmt.SPP];
    const int maxx = cargs->kernel.Width();
    const int maxy = cargs->kernel.Height();
    const int dx = cargs->kernel.Pivot().x - maxx;
    const int dy = cargs->kernel.Pivot().y - maxy;
    FColor transparent( fmt.FMT );
    const T* src = nullptr;
    const int cmin = fmt.AID ? 0 : 1;
    const int cmax = fmt.NCC + cmin;
    const float maxT = static_cast< float >( MaxType< T >() );

    // Gather x y
    const int y = jargs->line + cargs->srcRect.y;
    const int x1 = cargs->srcRect.x;
    const int x2 = cargs->dstRect.w + x1;
    for( int x = x1; x < x2; ++x ) {
        memset( sum, 0, fmt.SPP * sizeof( float ) );
        for( int i = 0; i < maxx; ++i ) {
            for( int j = 0; j < maxy; ++j ) {
                int src_x = x + i + dx;
                int src_y = y + j + dy;
                src = cargs->srcRect.HitTest( FVec2I( src_x, src_y ) ) ? reinterpret_cast< const T* >( cargs->src.PixelBits( src_x, src_y ) ) : reinterpret_cast< const T* >( transparent.Bits() );
                float alpha = maxT;
                float value = cargs->kernel.At( i, j );

                if( fmt.HEA ) {
                    alpha = static_cast< float >( src[ fmt.AID ] );
                    sum[ fmt.AID ] += alpha * value;
                }
                for( int k = cmin; k < cmax; ++k ) {
                    sum[k] += src[k] * alpha / maxT * value;
                }
            }
        }

        dst[fmt.AID] = FMath::Clamp( static_cast< T >( sum[fmt.AID] ), MinType< T >(), MaxType< T >() );
        for( int k = cmin; k < cmax; ++k ) {
            dst[k] = static_cast< T >( FMath::Clamp( static_cast< T >( sum[k] ), MinType< T >(), MaxType< T >() ) * maxT / sum[fmt.AID] );
        }
        dst += fmt.SPP;
    }

    delete [] sum;
}

template< typename T >
void
InvokeConvolutionPremultMaxMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FConvolutionCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->src.FormatMetrics();
    T* ULIS_RESTRICT dst = reinterpret_cast< T* >( jargs->dst );
    float* sum = new float[fmt.SPP];
    const int maxx = cargs->kernel.Width();
    const int maxy = cargs->kernel.Height();
    const int dx = cargs->kernel.Pivot().x - maxx;
    const int dy = cargs->kernel.Pivot().y - maxy;
    FColor transparent( fmt.FMT );
    const T* src = nullptr;
    const int cmin = fmt.AID ? 0 : 1;
    const int cmax = fmt.NCC + cmin;
    const float maxT = static_cast< float >( MaxType< T >() );

    // Gather x y
    const int y = jargs->line + cargs->srcRect.y;
    const int x1 = cargs->srcRect.x;
    const int x2 = cargs->dstRect.w + x1;
    for( int x = x1; x < x2; ++x ) {
        memset( sum, 0, fmt.SPP * sizeof( float ) );
        for( int i = 0; i < maxx; ++i ) {
            for( int j = 0; j < maxy; ++j ) {
                int src_x = x + i + dx;
                int src_y = y + j + dy;
                src = cargs->srcRect.HitTest( FVec2I( src_x, src_y ) ) ? reinterpret_cast< const T* >( cargs->src.PixelBits( src_x, src_y ) ) : reinterpret_cast< const T* >( transparent.Bits() );
                float alpha = maxT;
                float value = cargs->kernel.At( i, j );

                if( fmt.HEA ) {
                    alpha = static_cast< float >( src[ fmt.AID ] );
                    sum[ fmt.AID ] = FMath::Max( sum[ fmt.AID ], alpha * value );
                }
                for( int k = cmin; k < cmax; ++k ) {
                    sum[k] = FMath::Max( sum[k], src[k] * alpha / maxT * value );
                }
            }
        }

        dst[fmt.AID] = FMath::Clamp( static_cast< T >( sum[fmt.AID] ), MinType< T >(), MaxType< T >() );
        for( int k = cmin; k < cmax; ++k ) {
            dst[k] = static_cast< T >( FMath::Clamp( static_cast< T >( sum[k] ), MinType< T >(), MaxType< T >() ) * maxT / sum[fmt.AID] );
        }
        dst += fmt.SPP;
    }

    delete [] sum;
}

template< typename T >
void
InvokeConvolutionPremultMinMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FConvolutionCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->src.FormatMetrics();
    T* ULIS_RESTRICT dst = reinterpret_cast< T* >( jargs->dst );
    float* sum = new float[fmt.SPP];
    const int maxx = cargs->kernel.Width();
    const int maxy = cargs->kernel.Height();
    const int dx = cargs->kernel.Pivot().x - maxx;
    const int dy = cargs->kernel.Pivot().y - maxy;
    FColor transparent( fmt.FMT );
    const T* src = nullptr;
    const int cmin = fmt.AID ? 0 : 1;
    const int cmax = fmt.NCC + cmin;
    const float maxT = static_cast< float >( MaxType< T >() );

    // Gather x y
    const int y = jargs->line + cargs->srcRect.y;
    const int x1 = cargs->srcRect.x;
    const int x2 = cargs->dstRect.w + x1;
    for( int x = x1; x < x2; ++x ) {
        for( int i = 0; i < fmt.SPP; ++i )
            sum[i] = ULIS_FLOAT_MAX;

        for( int i = 0; i < maxx; ++i ) {
            for( int j = 0; j < maxy; ++j ) {
                int src_x = x + i + dx;
                int src_y = y + j + dy;
                src = cargs->srcRect.HitTest( FVec2I( src_x, src_y ) ) ? reinterpret_cast< const T* >( cargs->src.PixelBits( src_x, src_y ) ) : reinterpret_cast< const T* >( transparent.Bits() );
                float alpha = maxT;
                float value = cargs->kernel.At( i, j );

                if( fmt.HEA ) {
                    alpha = static_cast< float >( src[ fmt.AID ] );
                    sum[ fmt.AID ] = FMath::Min( sum[ fmt.AID ], alpha * value );
                }
                for( int k = cmin; k < cmax; ++k ) {
                    sum[k] = FMath::Min( sum[k], src[k] * alpha / maxT * value );
                }
            }
        }

        dst[fmt.AID] = FMath::Clamp( static_cast< T >( sum[fmt.AID] ), MinType< T >(), MaxType< T >() );
        for( int k = cmin; k < cmax; ++k ) {
            dst[k] = static_cast< T >( FMath::Clamp( static_cast< T >( sum[k] ), MinType< T >(), MaxType< T >() ) * maxT / sum[fmt.AID] );
        }
        dst += fmt.SPP;
    }

    delete [] sum;
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleConvolutionMT_MEM_Generic, FDualBufferJobArgs, FConvolutionCommandArgs, &InvokeConvolutionMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleConvolutionMaxMT_MEM_Generic, FDualBufferJobArgs, FConvolutionCommandArgs, &InvokeConvolutionMaxMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleConvolutionMinMT_MEM_Generic, FDualBufferJobArgs, FConvolutionCommandArgs, &InvokeConvolutionMinMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleConvolutionPremultMT_MEM_Generic, FDualBufferJobArgs, FConvolutionCommandArgs, &InvokeConvolutionPremultMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleConvolutionPremultMaxMT_MEM_Generic, FDualBufferJobArgs, FConvolutionCommandArgs, &InvokeConvolutionPremultMaxMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleConvolutionPremultMinMT_MEM_Generic, FDualBufferJobArgs, FConvolutionCommandArgs, &InvokeConvolutionPremultMinMT_MEM_Generic< T > )
ULIS_DECLARE_DISPATCHER( FDispatchedConvolutionInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedConvolutionMaxInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedConvolutionMinInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedConvolutionPremultInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedConvolutionPremultMaxInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedConvolutionPremultMinInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedConvolutionInvocationSchedulerSelector, &ScheduleConvolutionMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedConvolutionMaxInvocationSchedulerSelector, &ScheduleConvolutionMaxMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedConvolutionMinInvocationSchedulerSelector, &ScheduleConvolutionMinMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedConvolutionPremultInvocationSchedulerSelector, &ScheduleConvolutionPremultMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedConvolutionPremultMaxInvocationSchedulerSelector, &ScheduleConvolutionPremultMaxMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedConvolutionPremultMinInvocationSchedulerSelector, &ScheduleConvolutionPremultMinMT_MEM_Generic< T > )

ULIS_NAMESPACE_END

