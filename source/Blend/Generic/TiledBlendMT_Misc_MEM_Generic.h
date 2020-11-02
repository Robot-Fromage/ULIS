// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         TiledBlendMT_Misc_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/BlendArgs.h"
#include "Blend/BlendHelpers.h"
#include "Blend/Modes.h"
#include "Blend/Func/AlphaFuncF.h"
#include "Blend/PRNG.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeTiledBlendMTProcessScanline_Misc_MEM_Generic(
      const FBlendJobArgs_Separable_MEM_Generic* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->source.FormatMetrics();
    const uint8* ULIS_RESTRICT  src = jargs->src;
    uint8*       ULIS_RESTRICT  bdp = jargs->bdp;

    switch( cargs->blendingMode ) {
        case Blend_Dissolve: {
            int32 seedy = cargs->backdropWorkingRect.y + jargs->line + 1;
            uint32 localPRNGSeed = ( 8253729 % seedy ) * GetBlendPRNGSeed() + ( 2396403 % ( seedy + 64578 ) * seedy );

            for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
                const ufloat alpha_bdp = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const ufloat alpha_src = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * cargs->opacity : cargs->opacity;
                localPRNGSeed = 8253729 * localPRNGSeed + 2396403;
                const ufloat toss = ( localPRNGSeed % 65537 ) / 65537.f;
                if( toss < alpha_src ) {
                    ufloat alpha_result;
                    ULIS_ASSIGN_ALPHAF( cargs->alphaMode, alpha_result, 1.f, alpha_bdp );
                    memcpy( bdp, src, fmt.BPP );
                    if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
                }
                src += fmt.BPP;
                bdp += fmt.BPP;
                if( ( x + cargs->shift.x ) % cargs->sourceRect.w == 0 )
                    src = iSrc;
            }
            break;
        }

        case Blend_BayerDither8x8: {
            for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
                const ufloat alpha_bdp  = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const ufloat alpha_src  = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * cargs->opacity : cargs->opacity;
                const ufloat bayerEl    = gBayer8x8Matrix[ ( cargs->backdropWorkingRect.y + jargs->line ) % 8 ][ ( cargs->backdropWorkingRect.x + x ) % 8 ];
                if( alpha_src >= bayerEl ) {
                    ufloat alpha_result;
                    ULIS_ASSIGN_ALPHAF( cargs->alphaMode, alpha_result, 1.f, alpha_bdp );
                    memcpy( bdp, src, fmt.BPP );
                    if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
                }
                src += fmt.BPP;
                bdp += fmt.BPP;
                if( ( x + cargs->shift.x ) % cargs->sourceRect.w == 0 )
                    src = iSrc;
            }
            break;
        }
    }
}

template< typename T >
void
ScheduleTiledBlendMT_Misc_MEM_Generic(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildTiledBlendJobs_Separable_MEM_Generic< &InvokeTiledBlendMTProcessScanline_Misc_MEM_Generic< T > >( iCommand, iPolicy );
}

ULIS_NAMESPACE_END

