// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Morpho.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Morpho API.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Image/Color.h"
#include "Image/Block.h"
#include "Image/StructuringElement.h"
#include "Math/Geometry/Rectangle.h"
#include "Process/Conv/srgb2linear.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Args
class FMorphoCommandArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FMorphoCommandArgs() override {}
    FMorphoCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , const FStructuringElement& iKernel
    )
        : FDualBufferCommandArgs( iSrc, iDst, iSrcRect, iDstRect )
        , kernel( iKernel )
        {}

    const FStructuringElement& kernel;
};

/////////////////////////////////////////////////////
// Invocations
template< typename T >
void
InvokeMorphoMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FMorphoCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->src.FormatMetrics();
    uint8*       ULIS_RESTRICT  dst = jargs->dst;
    bool* match = new bool[fmt.SPP];

    // StructuringElement ( blank = any )
    //   ___________
    //  |   |   |   |
    //  |   | 0 | 0 |
    //  |___|___|___|
    //  |   |   |   |
    //  | 1 | 1 | 0 |
    //  |___|___|___|
    //  |   |   |   |
    //  |   | 1 |   |
    //  |___|___|___|
    // match structure for channel ? -> OnSame/OnDiff -> Keep / SetOne / SetZero / Compute?
    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        for( int i = 0; i < cargs->kernel.Width(); ++i ) {
            for( int j = 0; j < cargs->kernel.Width(); ++j ) {
                memset( match, 1, fmt.BPP );
                int src_x = x + i - cargs->kernel.Pivot().x;
                int src_y = jargs->line + j - cargs->kernel.Pivot().y;
                FColor color = cargs->src.Sample( src_x, src_y );
                eMorphologicalElementValue mpel = cargs->kernel.At( i, j );
                if( mpel == MpE_Any )
                    continue;
                for( int k = 0; k < fmt.SPP; ++k ) {
                    T binary = ( *( (T*)( color.Bits() + k ) ) ) > MinType< T >() ? MaxType< T >() : MinType< T >();
                    // see De Morgan's laws
                    bool A = binary == MinType< T >();
                    bool B = mpel == MpE_Zero;
                    bool C = binary == MaxType< T >();
                    bool D = mpel == MpE_One;
                    if( ( ( !A || !B ) && ( !C || !D ) ) ) {
                        match[k] = false;
                    }
                }
            }
        }
        for( int k = 0; k < fmt.SPP; ++k ) {
            if( match[k] ) {
                // Default On Match Set One
                ( *( (T*)( dst + k ) ) ) = MaxType< T >();
            }
        }
        dst += fmt.BPP;
    }

    delete [] match;
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleMorphoMT_MEM_Generic, FDualBufferJobArgs, FMorphoCommandArgs, &InvokeMorphoMT_MEM_Generic< T > )
ULIS_DECLARE_DISPATCHER( FDispatchedMorphoInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedMorphoInvocationSchedulerSelector, &ScheduleMorphoMT_MEM_Generic< T > )

ULIS_NAMESPACE_END

