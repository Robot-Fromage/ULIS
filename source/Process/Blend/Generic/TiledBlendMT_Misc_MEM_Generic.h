// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TiledBlendMT_Misc_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Blend specialization as described in the title.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Core/Constants.h"
#include "Process/Blend/BlendArgs.h"
#include "Process/Blend/Func/AlphaFuncF.h"
#include "Process/Blend/PRNG.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeTiledBlendMT_Misc_MEM_Generic(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->src.FormatMetrics();
    const uint8* ULIS_RESTRICT  base = jargs->src;
    const uint8* ULIS_RESTRICT  src = jargs->src;
    uint8*       ULIS_RESTRICT  bdp = jargs->bdp;

    switch( cargs->blendingMode ) {
        case Blend_Dissolve: {
            int32 seedy = cargs->dstRect.y + jargs->line + 1;
            uint32 localPRNGSeed = ( 8253729 % seedy ) * GetBlendPRNGSeed() + ( 2396403 % ( seedy + 64578 ) * seedy );

            for( int x = 1; x < cargs->dstRect.w + 1; ++x ) {
                const ufloat alpha_bdp = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const ufloat alpha_src = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * cargs->opacity : cargs->opacity;
                localPRNGSeed = 8253729 * localPRNGSeed + 2396403;
                const ufloat toss = ( localPRNGSeed % 65537 ) / 65537.f;
                if( toss < alpha_src ) {
                    ufloat alpha_result = 0.f;
                    #define ACTION( _AM, iTarget, iSrc, iBdp ) iTarget = AlphaF< _AM >( iSrc, iBdp );
                    ULIS_SWITCH_FOR_ALL_DO( cargs->alphaMode, ULIS_FOR_ALL_AM_DO, ACTION, alpha_result, 1.f, alpha_bdp )
                    #undef ACTION
                    memcpy( bdp, src, fmt.BPP );
                    if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
                }
                src += fmt.BPP;
                bdp += fmt.BPP;
                if( ( ( x + cargs->shift.x ) % (cargs->srcRect.w ) == 0 ) )
                    src = base;
            }
            break;
        }

        case Blend_BayerDither8x8: {
            for( int x = 1; x < cargs->dstRect.w + 1; ++x ) {
                const ufloat alpha_bdp  = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const ufloat alpha_src  = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * cargs->opacity : cargs->opacity;
                const ufloat bayerEl    = gBayer8x8Matrix[ ( cargs->dstRect.y + jargs->line ) % 8 ][ ( cargs->dstRect.x + x ) % 8 ];
                if( alpha_src >= bayerEl ) {
                    ufloat alpha_result = 0.f;
                    #define ACTION( _AM, iTarget, iSrc, iBdp ) iTarget = AlphaF< _AM >( iSrc, iBdp );
                    ULIS_SWITCH_FOR_ALL_DO( cargs->alphaMode, ULIS_FOR_ALL_AM_DO, ACTION, alpha_result, 1.f, alpha_bdp )
                    #undef ACTION
                    memcpy( bdp, src, fmt.BPP );
                    if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
                }
                src += fmt.BPP;
                bdp += fmt.BPP;
                if( ( ( x + cargs->shift.x ) % (cargs->srcRect.w ) == 0 ) )
                    src = base;
            }
            break;
        }
    }
}

ULIS_DEFINE_BLEND_COMMAND_GENERIC( TiledBlendMT_Misc_MEM_Generic                )

ULIS_NAMESPACE_END

