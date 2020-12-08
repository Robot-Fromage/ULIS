// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BlendMT_Misc_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation for Blend composition
*               operations, that involve misc computation ( channels are
*               grouped altogether but doesn't involve a change of model )
*               for generic formats, without optimisations.
*               This versions should work with any color model and any depth
*               or layout.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Core/Constants.h"
#include "Blend/BlendHelpers.h"
#include "Blend/Func/AlphaFuncF.h"
#include "Blend/PRNG.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeBlendMTProcessScanline_Misc_MEM_Generic_Subpixel(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->source.FormatMetrics();
    const uint8* ULIS_RESTRICT  src = jargs->src;
    uint8*       ULIS_RESTRICT  bdp = jargs->bdp;

    const bool notLastLine  = jargs->line < uint32( cargs->backdropCoverage.y );
    const bool notFirstLine = jargs->line > 0;
    const bool onLeftBorder = cargs->backdropWorkingRect.x == 0;
    const bool hasLeftData  = cargs->sourceRect.x + cargs->shift.x > 0;
    const bool hasTopData   = cargs->sourceRect.y + cargs->shift.y > 0;

    switch( cargs->blendingMode ) {
        case Blend_Dissolve: {
            int32 seedy = cargs->backdropWorkingRect.y + jargs->line + 1;
            uint32 localPRNGSeed = ( 8253729 % seedy ) * GetBlendPRNGSeed() + ( 2396403 % ( seedy + 64578 ) * seedy );
            ufloat m11, m01, m10, m00, vv0, vv1, res;
            m11 = ( notLastLine && onLeftBorder && hasLeftData )    ? TYPE2FLOAT( src - fmt.BPP,                    fmt.AID ) : 0.f;
            m10 = ( hasLeftData && ( notFirstLine || hasTopData ) ) ? TYPE2FLOAT( src - jargs->src_bps - fmt.BPP,   fmt.AID ) : 0.f;
            vv1 = m10 * cargs->subpixelComponent.y + m11 * cargs->buspixelComponent.y;

            for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
                const bool notLastCol = x < cargs->backdropCoverage.x;
                m00 = m10;
                m01 = m11;
                vv0 = vv1;
                SampleSubpixelAlpha( res );
                const ufloat alpha_bdp = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const ufloat alpha_src = res * cargs->opacity;
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
            }
            break;
        }

        case Blend_BayerDither8x8: {
            ufloat m11, m01, m10, m00, vv0, vv1, res;
            m11 = ( notLastLine && onLeftBorder && hasLeftData )    ? TYPE2FLOAT( src - fmt.BPP,                    fmt.AID ) : 0.f;
            m10 = ( hasLeftData && ( notFirstLine || hasTopData ) ) ? TYPE2FLOAT( src - jargs->src_bps - fmt.BPP,   fmt.AID ) : 0.f;
            vv1 = m10 * cargs->subpixelComponent.y + m11 * cargs->buspixelComponent.y;

            for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
                const bool notLastCol = x < cargs->backdropCoverage.x;
                m00 = m10;
                m01 = m11;
                vv0 = vv1;
                SampleSubpixelAlpha( res );
                const ufloat alpha_bdp  = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const ufloat alpha_src  = res * cargs->opacity;
                const uint32 bayerX     = ( cargs->backdropWorkingRect.x + x )     % 8;
                const uint32 bayerY     = ( cargs->backdropWorkingRect.y + jargs->line ) % 8;
                const ufloat bayerEl = gBayer8x8Matrix[ bayerY ][ bayerX ];
                if( alpha_src >= bayerEl ) {
                    ufloat alpha_result;
                    ULIS_ASSIGN_ALPHAF( cargs->alphaMode, alpha_result, 1.f, alpha_bdp );
                    memcpy( bdp, src, fmt.BPP );
                    if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
                }
                src += fmt.BPP;
                bdp += fmt.BPP;
            }
            break;
        }
    }
}

template< typename T >
void
ScheduleBlendMT_Misc_MEM_Generic_Subpixel(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildBlendJobs< &InvokeBlendMTProcessScanline_Misc_MEM_Generic_Subpixel< T > >( iCommand, iPolicy );
}

template< typename T >
void
InvokeBlendMTProcessScanline_Misc_MEM_Generic(
      const FBlendJobArgs* jargs
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
            }
            break;
        }
    }
}

template< typename T >
void
ScheduleBlendMT_Misc_MEM_Generic(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildBlendJobs< &InvokeBlendMTProcessScanline_Misc_MEM_Generic< T > >( iCommand, iPolicy );
}

ULIS_NAMESPACE_END

