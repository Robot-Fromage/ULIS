// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BlendMT_NonSeparable_SSE_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Blend/RGBA8/BlendMT_NonSeparable_SSE_RGBA8.h"
#include "Blend/BlendHelpers.h"
#include "Blend/Func/AlphaFuncF.h"
#include "Blend/Func/AlphaFuncSSEF.h"
#include "Blend/Func/NonSeparableBlendFuncSSEF.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeBlendMT_NonSeparable_SSE_RGBA8_Subpixel(
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

    Vec4f   TX( cargs->subpixelComponent.x );
    Vec4f   TY( cargs->subpixelComponent.y );
    Vec4f   UX( cargs->buspixelComponent.x );
    Vec4f   UY( cargs->buspixelComponent.y );

    Vec4f alpha_m11, alpha_m01, alpha_m10, alpha_m00, alpha_vv0, alpha_vv1, alpha_smp;
    Vec4f smpch_m11, smpch_m01, smpch_m10, smpch_m00, smpch_vv0, smpch_vv1, smpch_smp;
    alpha_m11 = ( notLastLine && onLeftBorder && hasLeftData )      ? *( src - fmt.BPP + fmt.AID                    ) / 255.f : 0.f;
    alpha_m10 = ( hasLeftData && ( notFirstLine || hasTopData ) )   ? *( src - fmt.BPP + fmt.AID - jargs->src_bps   ) / 255.f : 0.f;
    alpha_vv1 = alpha_m10 * TY + alpha_m11 * UY;
    smpch_m11 = ( notLastLine && onLeftBorder && hasLeftData )      ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( src - fmt.BPP                   ) ) ) ) ) / 255.f : 0.f;
    smpch_m10 = ( hasLeftData && ( notFirstLine || hasTopData ) )   ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( src - fmt.BPP - jargs->src_bps  ) ) ) ) ) / 255.f : 0.f;
    smpch_vv1 = ( smpch_m10 * alpha_m10 ) * TY + ( smpch_m11 * alpha_m11 )  * UY;

    for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
        const bool notLastCol = x < cargs->backdropCoverage.x;
        alpha_m00 = alpha_m10;
        alpha_m01 = alpha_m11;
        alpha_vv0 = alpha_vv1;
        smpch_m00 = smpch_m10;
        smpch_m01 = smpch_m11;
        smpch_vv0 = smpch_vv1;
        alpha_m11 = ( notLastCol && notLastLine )                     ? *( src + fmt.AID                    ) / 255.f : 0.f;
        alpha_m10 = ( notLastCol && ( notFirstLine || hasTopData ) )  ? *( src + fmt.AID - jargs->src_bps   ) / 255.f : 0.f;
        alpha_vv1 = alpha_m10 * TY + alpha_m11 * UY;
        alpha_smp = alpha_vv0 * TX + alpha_vv1 * UX;
        smpch_m11 = ( notLastCol && notLastLine )                     ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( src                   ) ) ) ) ) / 255.f : 0.f;
        smpch_m10 = ( notLastCol && ( notFirstLine || hasTopData ) )  ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( src - jargs->src_bps  ) ) ) ) ) / 255.f : 0.f;
        smpch_vv1 = ( smpch_m10 * alpha_m10 ) * TY + ( smpch_m11 * alpha_m11 )  * UY;
        smpch_smp = lookup4( jargs->idt, select( alpha_smp == 0.f, 0.f, ( smpch_vv0 * TX + smpch_vv1 * UX ) / alpha_smp ) );

        Vec4f alpha_bdp     = *( bdp + fmt.AID ) / 255.f;
        Vec4f alpha_src     = alpha_smp * cargs->opacity;
        Vec4f alpha_comp    = AlphaNormalSSEF( alpha_src, alpha_bdp );
        Vec4f var           = select( alpha_comp == 0.f, 0.f, alpha_src / alpha_comp );
        Vec4f alpha_result;
        ULIS_ASSIGN_ALPHASSEF( cargs->alphaMode, alpha_result, alpha_src, alpha_bdp );

        Vec4f bdp_chan = lookup4( jargs->idt, Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( bdp ) ) ) ) ) / 255.f );
        smpch_smp.insert( 3, 0.f );
        bdp_chan.insert( 3, 0.f );
        Vec4f res_chan;
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_chan = NonSeparableCompOpSSEF< _BM >( smpch_smp, bdp_chan, alpha_bdp, var ) * 255.f;
        ULIS_SWITCH_FOR_ALL_DO( cargs->blendingMode, ULIS_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN

        res_chan = lookup4( jargs->idt, res_chan );
        auto _pack = _mm_cvtps_epi32( res_chan );
        _pack = _mm_packus_epi32( _pack, _pack );
        _pack = _mm_packus_epi16( _pack, _pack );
        *( uint32* )bdp = static_cast< uint32 >( _mm_cvtsi128_si32( _pack ) );
        *( bdp + fmt.AID ) = uint8( alpha_result[0] * 0xFF );
        src += 4;
        bdp += 4;
    }
}

void
ScheduleBlendMT_NonSeparable_SSE_RGBA8_Subpixel(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildBlendJobs< &InvokeBlendMT_NonSeparable_SSE_RGBA8_Subpixel >( iCommand, iPolicy );
}

void
InvokeBlendMT_NonSeparable_SSE_RGBA8(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->source.FormatMetrics();
    const uint8* ULIS_RESTRICT  src = jargs->src;
    uint8*       ULIS_RESTRICT  bdp = jargs->bdp;

    for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
        ufloat alpha_bdp    = bdp[fmt.AID] / 255.f;
        ufloat alpha_src    = ( src[fmt.AID] / 255.f ) * cargs->opacity;
        ufloat alpha_comp   = AlphaNormalF( alpha_src, alpha_bdp );
        ufloat var          = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        ufloat alpha_result;
        ULIS_ASSIGN_ALPHAF( cargs->alphaMode, alpha_result, alpha_src, alpha_bdp );

        Vec4f src_chan = lookup4( jargs->idt, Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( src ) ) ) ) ) / 255.f );
        Vec4f bdp_chan = lookup4( jargs->idt, Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( bdp ) ) ) ) ) / 255.f );
        src_chan.insert( 3, 0.f );
        bdp_chan.insert( 3, 0.f );
        Vec4f res_chan;
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_chan = NonSeparableCompOpSSEF< _BM >( src_chan, bdp_chan, alpha_bdp, var ) * 255.f;
        ULIS_SWITCH_FOR_ALL_DO( cargs->blendingMode, ULIS_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN

        res_chan = lookup4( jargs->idt, res_chan );
        auto _pack = _mm_cvtps_epi32( res_chan );
        _pack = _mm_packus_epi32( _pack, _pack );
        _pack = _mm_packus_epi16( _pack, _pack );
        *( uint32* )bdp = static_cast< uint32 >( _mm_cvtsi128_si32( _pack ) );
        *( bdp + fmt.AID ) = uint8( alpha_result * 0xFF );
        src += 4;
        bdp += 4;
    }
}

void
ScheduleBlendMT_NonSeparable_SSE_RGBA8(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildBlendJobs< &InvokeBlendMT_NonSeparable_SSE_RGBA8 >( iCommand, iPolicy );
}

ULIS_NAMESPACE_END

