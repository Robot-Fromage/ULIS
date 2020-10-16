// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         BlendMT_NonSeparable_SSE_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Blend/RGBA8/BlendMT_NonSeparable_SSE_RGBA8.h"
#include "Blend/BlendHelpers.h"
#include "Blend/Modes.h"
#include "Blend/Func/AlphaFuncF.h"
#include "Blend/Func/AlphaFuncSSEF.h"
#include "Blend/Func/NonSeparableBlendFuncSSEF.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
ULIS_FORCEINLINE
void
BuildRGBA8IndexTable(
      uint8 iRS
    , Vec4i* oIDT
)
{
    switch( iRS ) {
        case 1:  for( int i = 0; i < 4; ++i ) oIDT->insert( i, ( 3 - i )                             ); break;
        case 2:  for( int i = 0; i < 4; ++i ) oIDT->insert( i, ( i + 1 ) > 3 ? 0 : i + 1             ); break;
        case 3:  for( int i = 0; i < 4; ++i ) oIDT->insert( i, ( 3 - i ) - 1 < 0 ? 3 : ( 3 - i ) - 1 ); break;
        default: for( int i = 0; i < 4; ++i ) oIDT->insert( i, i                                     ); break;
    }
}

void
InvokeBlendMTProcessScanline_NonSeparable_SSE_RGBA8_Subpixel(
      const FBlendJobArgs_Separable_MEM_Generic* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->source.FormatMetrics();
    const uint8* ULIS_RESTRICT  src = jargs->src;
    uint8*       ULIS_RESTRICT  bdp = jargs->bdp;

    const bool notLastLine  = jargs->line < cargs->backdropCoverage.y;
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
    alpha_m11 = ( notLastLine && onLeftBorder && hasLeftData )      ? *( iSrc - fmt.BPP + fmt.AID           ) / 255.f : 0.f;
    alpha_m10 = ( hasLeftData && ( notFirstLine || hasTopData ) )   ? *( iSrc - fmt.BPP + fmt.AID - iSrcBps ) / 255.f : 0.f;
    alpha_vv1 = alpha_m10 * TY + alpha_m11 * UY;
    smpch_m11 = ( notLastLine && onLeftBorder && hasLeftData )      ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iSrc - fmt.BPP            ) ) ) ) ) / 255.f : 0.f;
    smpch_m10 = ( hasLeftData && ( notFirstLine || hasTopData ) )   ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iSrc - fmt.BPP - iSrcBps  ) ) ) ) ) / 255.f : 0.f;
    smpch_vv1 = ( smpch_m10 * alpha_m10 ) * TY + ( smpch_m11 * alpha_m11 )  * UY;

    for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
        const bool notLastCol = x < cargs->backdropCoverage.x;
        alpha_m00 = alpha_m10;
        alpha_m01 = alpha_m11;
        alpha_vv0 = alpha_vv1;
        smpch_m00 = smpch_m10;
        smpch_m01 = smpch_m11;
        smpch_vv0 = smpch_vv1;
        alpha_m11 = ( notLastCol && notLastLine )                     ? *( iSrc + fmt.AID             ) / 255.f : 0.f;
        alpha_m10 = ( notLastCol && ( notFirstLine || hasTopData ) )  ? *( iSrc + fmt.AID - iSrcBps   ) / 255.f : 0.f;
        alpha_vv1 = alpha_m10 * TY + alpha_m11 * UY;
        alpha_smp = alpha_vv0 * TX + alpha_vv1 * UX;
        smpch_m11 = ( notLastCol && notLastLine )                     ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iSrc              ) ) ) ) ) / 255.f : 0.f;
        smpch_m10 = ( notLastCol && ( notFirstLine || hasTopData ) )  ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iSrc - iSrcBps    ) ) ) ) ) / 255.f : 0.f;
        smpch_vv1 = ( smpch_m10 * alpha_m10 ) * TY + ( smpch_m11 * alpha_m11 )  * UY;
        smpch_smp = lookup4( iIDT, select( alpha_smp == 0.f, 0.f, ( smpch_vv0 * TX + smpch_vv1 * UX ) / alpha_smp ) );

        Vec4f alpha_bdp     = *( iBdp + fmt.AID ) / 255.f;
        Vec4f alpha_src     = alpha_smp * cargs->opacity;
        Vec4f alpha_comp    = AlphaNormalSSEF( alpha_src, alpha_bdp );
        Vec4f var           = select( alpha_comp == 0.f, 0.f, alpha_src / alpha_comp );
        Vec4f alpha_result;
        ULIS_ASSIGN_ALPHASSEF( cargs->alphaMode, alpha_result, alpha_src, alpha_bdp );

        Vec4f bdp_chan = lookup4( iIDT, Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iBdp ) ) ) ) ) / 255.f );
        smpch_smp.insert( 3, 0.f );
        bdp_chan.insert( 3, 0.f );
        Vec4f res_chan;
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_chan = NonSeparableCompOpSSEF< _BM >( smpch_smp, bdp_chan, alpha_bdp, var ) * 255.f;
        ULIS_SWITCH_FOR_ALL_DO( cargs->blendingMode, ULIS_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN

        res_chan = lookup4( iIDT, res_chan );
        auto _pack = _mm_cvtps_epi32( res_chan );
        _pack = _mm_packus_epi32( _pack, _pack );
        _pack = _mm_packus_epi16( _pack, _pack );
        *( uint32* )iBdp = static_cast< uint32 >( _mm_cvtsi128_si32( _pack ) );
        *( iBdp + fmt.AID ) = uint8( alpha_result[0] * 0xFF );
        iSrc += 4;
        iBdp += 4;
    }
}

void
ScheduleBlendMT_NonSeparable_SSE_RGBA8_Subpixel(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    const FBlendCommandArgs&   info        = *iInfo;
    const uint8*        src         = cargs->source->Bits();
    uint8*              bdp         = cargs->backdrop->Bits();
    const uint32         src_bps     = cargs->source->BytesPerScanLine();
    const uint32         bdp_bps     = cargs->backdrop->BytesPerScanLine();
    const uint32         src_decal_y = cargs->shift.y + cargs->sourceRect.y;
    const uint32         src_decal_x = ( cargs->shift.x + cargs->sourceRect.x )  * cargs->source->BytesPerPixel();
    const uint32         bdp_decal_x = ( cargs->backdropWorkingRect.x )        * cargs->source->BytesPerPixel();
    Vec4i idt;
    BuildRGBA8IndexTable( cargs->source->FormatMetrics().RSC, &idt );
    ULIS_MACRO_INLINE_PARALLEL_FOR( cargs->perfIntent, cargs->pool, cargs->blocking
                                   , cargs->backdropWorkingRect.h
                                   , InvokeBlendMTProcessScanline_NonSeparable_SSE_RGBA8_Subpixel
                                   , src + ( ( src_decal_y + pLINE )                * src_bps ) + src_decal_x
                                   , bdp + ( ( cargs->backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                   , pLINE , src_bps, iInfo, idt );
}

void
InvokeBlendMTProcessScanline_NonSeparable_SSE_RGBA8(
      const FBlendJobArgs_Separable_MEM_Generic* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->source.FormatMetrics();
    const uint8* ULIS_RESTRICT  src = jargs->src;
    uint8*       ULIS_RESTRICT  bdp = jargs->bdp;

    for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
        ufloat alpha_bdp    = iBdp[fmt.AID] / 255.f;
        ufloat alpha_src    = ( iSrc[fmt.AID] / 255.f ) * cargs->opacity;
        ufloat alpha_comp   = AlphaNormalF( alpha_src, alpha_bdp );
        ufloat var          = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        ufloat alpha_result;
        ULIS_ASSIGN_ALPHAF( cargs->alphaMode, alpha_result, alpha_src, alpha_bdp );

        Vec4f src_chan = lookup4( iIDT, Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iSrc ) ) ) ) ) / 255.f );
        Vec4f bdp_chan = lookup4( iIDT, Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iBdp ) ) ) ) ) / 255.f );
        src_chan.insert( 3, 0.f );
        bdp_chan.insert( 3, 0.f );
        Vec4f res_chan;
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_chan = NonSeparableCompOpSSEF< _BM >( src_chan, bdp_chan, alpha_bdp, var ) * 255.f;
        ULIS_SWITCH_FOR_ALL_DO( cargs->blendingMode, ULIS_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN

        res_chan = lookup4( iIDT, res_chan );
        auto _pack = _mm_cvtps_epi32( res_chan );
        _pack = _mm_packus_epi32( _pack, _pack );
        _pack = _mm_packus_epi16( _pack, _pack );
        *( uint32* )iBdp = static_cast< uint32 >( _mm_cvtsi128_si32( _pack ) );
        *( iBdp + fmt.AID ) = uint8( alpha_result * 0xFF );
        iSrc += 4;
        iBdp += 4;
    }
}

void
ScheduleBlendMT_NonSeparable_SSE_RGBA8(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    const FBlendCommandArgs&   info        = *iInfo;
    const uint8*        src         = cargs->source->Bits();
    uint8*              bdp         = cargs->backdrop->Bits();
    const uint32         src_bps     = cargs->source->BytesPerScanLine();
    const uint32         bdp_bps     = cargs->backdrop->BytesPerScanLine();
    const uint32         src_decal_y = cargs->shift.y + cargs->sourceRect.y;
    const uint32         src_decal_x = ( cargs->shift.x + cargs->sourceRect.x )  * cargs->source->BytesPerPixel();
    const uint32         bdp_decal_x = ( cargs->backdropWorkingRect.x )        * cargs->source->BytesPerPixel();
    Vec4i idt;
    BuildRGBA8IndexTable( cargs->source->FormatMetrics().RSC, &idt );
    ULIS_MACRO_INLINE_PARALLEL_FOR( cargs->perfIntent, cargs->pool, cargs->blocking
                                , cargs->backdropWorkingRect.h
                                , InvokeBlendMTProcessScanline_NonSeparable_SSE_RGBA8
                                , src + ( ( src_decal_y + pLINE )                * src_bps ) + src_decal_x
                                , bdp + ( ( cargs->backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                , pLINE , iInfo, idt );
}

ULIS_NAMESPACE_END

