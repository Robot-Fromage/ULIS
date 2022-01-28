// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AlphaBlendMT_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation for Alpha Blend
*               composition, for generic formats, without optimisations.
*               This versions should work with any color model and any depth
*               or layout, as long as the block has alpha.
*               Alpha Blend should be faster than the regular Blend
*               counterpart.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Blend/BlendArgs.h"
#include "Process/Blend/Func/AlphaFuncF.h"
#include "Process/Blend/Func/SeparableBlendFuncF.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeAlphaBlendMT_Separable_MEM_Generic_Subpixel(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->src.FormatMetrics();
    const uint8* ULIS_RESTRICT  src = jargs->src;
    uint8*       ULIS_RESTRICT  bdp = jargs->bdp;

    const bool notLastLine  = jargs->line < uint32( cargs->backdropCoverage.y );
    const bool notFirstLine = jargs->line > 0;
    const bool onLeftBorder = cargs->dstRect.x == 0;
    const bool hasLeftData  = cargs->srcRect.x + cargs->shift.x > 0;
    const bool hasTopData   = cargs->srcRect.y + cargs->shift.y > 0;

    ufloat m11, m01, m10, m00, vv0, vv1, res;
    m11 = ( notLastLine && onLeftBorder && hasLeftData )    ? TYPE2FLOAT( src - fmt.BPP,                    fmt.AID ) : 0.f;
    m10 = ( ( notFirstLine || hasTopData ) && onLeftBorder && hasLeftData )    ? TYPE2FLOAT( src - cargs->src_bps - fmt.BPP,   fmt.AID ) : 0.f;
    vv1 = m10 * cargs->subpixelComponent.y + m11 * cargs->buspixelComponent.y;

    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        const bool notLastCol = x < cargs->backdropCoverage.x;
        m00 = m10;
        m01 = m11;
        vv0 = vv1;

        { //SampleSubpixelAlpha( res );
            if( fmt.HEA ) {
                m11 = ( notLastCol && notLastLine )     ? TYPE2FLOAT( src,                  fmt.AID ) : 0.f;
                m10 = ( notLastCol && ( notFirstLine || hasTopData ) )    ? TYPE2FLOAT( src - cargs->src_bps, fmt.AID ) : 0.f;
            } else {
                m11 = ( notLastCol && notLastLine )     ? 1.f : 0.f;
                m10 = ( notLastCol && ( notFirstLine || hasTopData ) )    ? 1.f : 0.f;
            }
            vv1 = m10 * cargs->subpixelComponent.y + m11 * cargs->buspixelComponent.y;
            res = vv0 * cargs->subpixelComponent.x + vv1 * cargs->buspixelComponent.x;
        }

        const ufloat alpha_bdp   = TYPE2FLOAT( bdp, fmt.AID );
        const ufloat alpha_src   = res * cargs->opacity;
        const ufloat alpha_comp  = AlphaNormalF( alpha_src, alpha_bdp );
        const ufloat var         = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            const uint8 r = fmt.IDT[j];
            ufloat s11, s01, s10, s00, v1, v2, srcvf;
            SampleSubpixelChannel( srcvf, r );
            const ufloat bdpvf = TYPE2FLOAT( bdp, r );
            FLOAT2TYPE( bdp, r, SeparableCompOpF< Blend_Normal >( srcvf, bdpvf, alpha_bdp, var ) );
        }

        FLOAT2TYPE( bdp, fmt.AID, alpha_comp );
        src += fmt.BPP;
        bdp += fmt.BPP;
    }
}

template< typename T >
void
InvokeAlphaBlendMT_Separable_MEM_Generic(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->src.FormatMetrics();
    const uint8* ULIS_RESTRICT  src = jargs->src;
    uint8*       ULIS_RESTRICT  bdp = jargs->bdp;

    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        const ufloat alpha_src  = TYPE2FLOAT( src, fmt.AID ) * cargs->opacity;
        const ufloat alpha_bdp  = TYPE2FLOAT( bdp, fmt.AID );
        const ufloat alpha_comp = AlphaNormalF( alpha_src, alpha_bdp );
        const ufloat var        = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            const uint8 r = fmt.IDT[j];
            const ufloat srcvf = TYPE2FLOAT( src, r );
            const ufloat bdpvf = TYPE2FLOAT( bdp, r );
            FLOAT2TYPE( bdp, r, SeparableCompOpF< Blend_Normal >( srcvf, bdpvf, alpha_bdp, var ) );
        }

        FLOAT2TYPE( bdp, fmt.AID, alpha_comp );
        src += fmt.BPP;
        bdp += fmt.BPP;
    }
}

ULIS_DEFINE_BLEND_COMMAND_GENERIC( AlphaBlendMT_Separable_MEM_Generic_Subpixel  )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( AlphaBlendMT_Separable_MEM_Generic           )

ULIS_NAMESPACE_END

