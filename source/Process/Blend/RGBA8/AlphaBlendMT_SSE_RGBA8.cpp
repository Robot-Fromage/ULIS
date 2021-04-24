// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AlphaBlendMT_SSE_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Core/Core.h"
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
#include "Process/Blend/RGBA8/AlphaBlendMT_SSE_RGBA8.h"
#include "Process/Blend/Func/AlphaFuncF.h"
#include "Process/Blend/Func/AlphaFuncSSEF.h"
#include "Process/Blend/Func/SeparableBlendFuncSSEF.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN

ULIS_FORCEINLINE
__m128i
Downscale( __m128i iVal ) {
    return  _mm_srli_epi16( _mm_adds_epu16( _mm_adds_epu16( iVal, _mm_set1_epi16( 1 ) ), _mm_srli_epi16( iVal, 8 ) ), 8 );
}

void
InvokeAlphaBlendMT_Separable_SSE_RGBA8_Subpixel(
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
    //const bool hasTopData   = cargs->srcRect.y + cargs->shift.y > 0;

    Vec4f   TX( cargs->subpixelComponent.x );
    Vec4f   TY( cargs->subpixelComponent.y );
    Vec4f   UX( cargs->buspixelComponent.x );
    Vec4f   UY( cargs->buspixelComponent.y );

    Vec4f alpha_m11, alpha_m01, alpha_m10, alpha_m00, alpha_vv0, alpha_vv1, alpha_smp;
    Vec4f smpch_m11, smpch_m01, smpch_m10, smpch_m00, smpch_vv0, smpch_vv1, smpch_smp;
    alpha_m11 = ( notLastLine && onLeftBorder && hasLeftData )  ? *( src - fmt.BPP + fmt.AID                    ) / 255.f : 0.f;
    alpha_m10 = ( notFirstLine && onLeftBorder && hasLeftData ) ? *( src - fmt.BPP + fmt.AID - cargs->src_bps   ) / 255.f : 0.f;
    alpha_vv1 = alpha_m10 * TY + alpha_m11 * UY;
    smpch_m11 = ( notLastLine && onLeftBorder && hasLeftData )  ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( src - fmt.BPP                   ) ) ) ) ) / 255.f : 0.f;
    smpch_m10 = ( notFirstLine && onLeftBorder && hasLeftData ) ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( src - fmt.BPP - cargs->src_bps  ) ) ) ) ) / 255.f : 0.f;
    smpch_vv1 = ( smpch_m10 * alpha_m10 ) * TY + ( smpch_m11 * alpha_m11 )  * UY;

    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        const bool notLastCol = x < cargs->backdropCoverage.x;
        alpha_m00 = alpha_m10;
        alpha_m01 = alpha_m11;
        alpha_vv0 = alpha_vv1;
        smpch_m00 = smpch_m10;
        smpch_m01 = smpch_m11;
        smpch_vv0 = smpch_vv1;
        alpha_m11 = ( notLastCol && notLastLine )                     ? *( src + fmt.AID             ) / 255.f : 0.f;
        alpha_m10 = ( notLastCol && notFirstLine )  ? *( src + fmt.AID - cargs->src_bps   ) / 255.f : 0.f;
        alpha_vv1 = alpha_m10 * TY + alpha_m11 * UY;
        alpha_smp = alpha_vv0 * TX + alpha_vv1 * UX;
        smpch_m11 = ( notLastCol && notLastLine )                     ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( src                   ) ) ) ) ) / 255.f : 0.f;
        smpch_m10 = ( notLastCol && notFirstLine )  ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( src - cargs->src_bps  ) ) ) ) ) / 255.f : 0.f;
        smpch_vv1 = ( smpch_m10 * alpha_m10 ) * TY + ( smpch_m11 * alpha_m11 )  * UY;
        smpch_smp = select( alpha_smp == 0.f, 0.f, ( smpch_vv0 * TX + smpch_vv1 * UX ) / alpha_smp );

        Vec4f alpha_bdp     = *( bdp + fmt.AID ) / 255.f;
        Vec4f alpha_src     = alpha_smp * cargs->opacity;
        Vec4f alpha_comp    = AlphaNormalSSEF( alpha_src, alpha_bdp );
        Vec4f var           = select( alpha_comp == 0.f, 0.f, alpha_src / alpha_comp );

        Vec4f bdp_chan = Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( bdp ) ) ) ) ) / 255.f;
        Vec4f res_chan;
        res_chan = SeparableCompOpSSEF< Blend_Normal >( smpch_smp, bdp_chan, alpha_bdp, var ) * 255.f;

        auto _pack = _mm_cvtps_epi32( res_chan );
        _pack = _mm_packus_epi32( _pack, _pack );
        _pack = _mm_packus_epi16( _pack, _pack );
        *( uint32* )bdp = static_cast< uint32 >( _mm_cvtsi128_si32( _pack ) );
        *( bdp + fmt.AID ) = uint8( alpha_comp[0] * 0xFF );
        src += 4;
        bdp += 4;
    }
}

void
InvokeAlphaBlendMT_Separable_SSE_RGBA8(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->src.FormatMetrics();
    const uint8* ULIS_RESTRICT  src = jargs->src;
    uint8*       ULIS_RESTRICT  bdp = jargs->bdp;

    const __m128i FF = _mm_set1_epi16( 0xFF );
    for( int x = 0; x < cargs->dstRect.w; x+=2 ) {
        const uint8 alpha_bdp0 = bdp[fmt.AID];
        const uint8 alpha_bdp1 = bdp[fmt.AID + 4];
        const uint8 alpha_src0 = static_cast< uint8 >( src[fmt.AID] * cargs->opacity );
        const uint8 alpha_src1 = static_cast< uint8 >( src[fmt.AID + 4] * cargs->opacity );
        const uint8 alpha_result0 = static_cast< uint8 >( ( alpha_src0 + alpha_bdp0 ) - ConvType< uint16, uint8 >( alpha_src0 * alpha_bdp0 ) );
        const uint8 alpha_result1 = static_cast< uint8 >( ( alpha_src1 + alpha_bdp1 ) - ConvType< uint16, uint8 >( alpha_src1 * alpha_bdp1 ) );
        const uint8 var0 = alpha_result0 == 0 ? 0 : ( alpha_src0 * 0xFF ) / alpha_result0;
        const uint8 var1 = alpha_result1 == 0 ? 0 : ( alpha_src1 * 0xFF ) / alpha_result1;
        const __m128i var = _mm_set_epi16( var0, var0, var0, var0, var1, var1, var1, var1 );
        const __m128i alpha_bdp = _mm_set_epi16( alpha_bdp0, alpha_bdp0, alpha_bdp0, alpha_bdp0, alpha_bdp1, alpha_bdp1, alpha_bdp1, alpha_bdp1 );
        const __m128i src_chan = _mm_cvtepu8_epi16( _mm_loadu_si128( reinterpret_cast< const __m128i* >( src ) ) );
        const __m128i bdp_chan = _mm_cvtepu8_epi16( _mm_loadu_si128( reinterpret_cast< const __m128i* >( bdp ) ) );
        __m128i termA = Downscale( _mm_mullo_epi16( _mm_sub_epi16( FF, var ), bdp_chan ) );
        __m128i termB = Downscale( _mm_mullo_epi16( alpha_bdp, src_chan ) );
        __m128i termC = Downscale( _mm_mullo_epi16( _mm_sub_epi16( FF, alpha_bdp ), src_chan ) );
        __m128i termD = _mm_add_epi16( termB, termC );
        __m128i termE = Downscale( _mm_mullo_epi16( var, termD ) );
        __m128i termF = _mm_add_epi16( termA, termE );
        __m128i pack = _mm_packus_epi16( termF, termF );
        *( reinterpret_cast< int64* >( bdp ) )= _mm_cvtsi128_si64( pack );
        bdp[fmt.AID] = static_cast< uint8 >( alpha_result0 );
        bdp[fmt.AID + 4] = static_cast< uint8 >( alpha_result1 );
        src += 8;
        bdp += 8;
    }
}

ULIS_DEFINE_BLEND_COMMAND_SPECIALIZATION( AlphaBlendMT_Separable_SSE_RGBA8_Subpixel )
ULIS_DEFINE_BLEND_COMMAND_SPECIALIZATION( AlphaBlendMT_Separable_SSE_RGBA8 )

ULIS_NAMESPACE_END
#endif // ULIS_COMPILETIME_SSE_SUPPORT

