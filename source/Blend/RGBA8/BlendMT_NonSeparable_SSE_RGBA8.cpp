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
#include "Thread/ThreadPool.h"
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
      const uint8* iSrc
    , uint8* iBdp
    , int32 iLine
    , const uint32 iSrcBps
    , const FBlendCommandArgs* iArgs
    , const Vec4i iIDT
)
{
    const FBlendCommandArgs&   info    = *iInfo;
    const FFormat&  fmt     = info.source->FormatInfo();

    const bool notLastLine  = iLine < info.backdropCoverage.y;
    const bool notFirstLine = iLine > 0;
    const bool onLeftBorder = info.backdropWorkingRect.x == 0;
    const bool hasLeftData  = info.sourceRect.x + info.shift.x > 0;
    const bool hasTopData   = info.sourceRect.y + info.shift.y > 0;

    Vec4f   TX( info.subpixelComponent.x );
    Vec4f   TY( info.subpixelComponent.y );
    Vec4f   UX( info.buspixelComponent.x );
    Vec4f   UY( info.buspixelComponent.y );

    Vec4f alpha_m11, alpha_m01, alpha_m10, alpha_m00, alpha_vv0, alpha_vv1, alpha_smp;
    Vec4f smpch_m11, smpch_m01, smpch_m10, smpch_m00, smpch_vv0, smpch_vv1, smpch_smp;
    alpha_m11 = ( notLastLine && onLeftBorder && hasLeftData )      ? *( iSrc - fmt.BPP + fmt.AID           ) / 255.f : 0.f;
    alpha_m10 = ( hasLeftData && ( notFirstLine || hasTopData ) )   ? *( iSrc - fmt.BPP + fmt.AID - iSrcBps ) / 255.f : 0.f;
    alpha_vv1 = alpha_m10 * TY + alpha_m11 * UY;
    smpch_m11 = ( notLastLine && onLeftBorder && hasLeftData )      ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iSrc - fmt.BPP            ) ) ) ) ) / 255.f : 0.f;
    smpch_m10 = ( hasLeftData && ( notFirstLine || hasTopData ) )   ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iSrc - fmt.BPP - iSrcBps  ) ) ) ) ) / 255.f : 0.f;
    smpch_vv1 = ( smpch_m10 * alpha_m10 ) * TY + ( smpch_m11 * alpha_m11 )  * UY;

    for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
        const bool notLastCol = x < info.backdropCoverage.x;
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
        Vec4f alpha_src     = alpha_smp * info.opacityValue;
        Vec4f alpha_comp    = AlphaNormalSSEF( alpha_src, alpha_bdp );
        Vec4f var           = select( alpha_comp == 0.f, 0.f, alpha_src / alpha_comp );
        Vec4f alpha_result;
        ULIS_ASSIGN_ALPHASSEF( info.alphaMode, alpha_result, alpha_src, alpha_bdp );

        Vec4f bdp_chan = lookup4( iIDT, Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iBdp ) ) ) ) ) / 255.f );
        smpch_smp.insert( 3, 0.f );
        bdp_chan.insert( 3, 0.f );
        Vec4f res_chan;
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_chan = NonSeparableCompOpSSEF< _BM >( smpch_smp, bdp_chan, alpha_bdp, var ) * 255.f;
        ULIS_SWITCH_FOR_ALL_DO( info.blendingMode, ULIS_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
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
      const FBlendCommandArgs* iArgs
    , const FSchedulePolicy& iPolicy
    , FThreadPool& iPool
)
{
    const FBlendCommandArgs&   info        = *iInfo;
    const uint8*        src         = info.source->Bits();
    uint8*              bdp         = info.backdrop->Bits();
    const uint32         src_bps     = info.source->BytesPerScanLine();
    const uint32         bdp_bps     = info.backdrop->BytesPerScanLine();
    const uint32         src_decal_y = info.shift.y + info.sourceRect.y;
    const uint32         src_decal_x = ( info.shift.x + info.sourceRect.x )  * info.source->BytesPerPixel();
    const uint32         bdp_decal_x = ( info.backdropWorkingRect.x )        * info.source->BytesPerPixel();
    Vec4i idt;
    BuildRGBA8IndexTable( info.source->FormatInfo().RSC, &idt );
    ULIS_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.backdropWorkingRect.h
                                   , InvokeBlendMTProcessScanline_NonSeparable_SSE_RGBA8_Subpixel
                                   , src + ( ( src_decal_y + pLINE )                * src_bps ) + src_decal_x
                                   , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                   , pLINE , src_bps, iInfo, idt );
}

void
InvokeBlendMTProcessScanline_NonSeparable_SSE_RGBA8(
      const uint8* iSrc
    , uint8* iBdp
    , int32 iLine
    , const FBlendCommandArgs* iArgs
    , const Vec4i iIDT
)
{
    const FBlendCommandArgs&   info    = *iInfo;
    const FFormat&  fmt     = info.source->FormatInfo();

    for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
        ufloat alpha_bdp    = iBdp[fmt.AID] / 255.f;
        ufloat alpha_src    = ( iSrc[fmt.AID] / 255.f ) * info.opacityValue;
        ufloat alpha_comp   = AlphaNormalF( alpha_src, alpha_bdp );
        ufloat var          = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        ufloat alpha_result;
        ULIS_ASSIGN_ALPHAF( info.alphaMode, alpha_result, alpha_src, alpha_bdp );

        Vec4f src_chan = lookup4( iIDT, Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iSrc ) ) ) ) ) / 255.f );
        Vec4f bdp_chan = lookup4( iIDT, Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( (const __m128i*)( iBdp ) ) ) ) ) / 255.f );
        src_chan.insert( 3, 0.f );
        bdp_chan.insert( 3, 0.f );
        Vec4f res_chan;
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_chan = NonSeparableCompOpSSEF< _BM >( src_chan, bdp_chan, alpha_bdp, var ) * 255.f;
        ULIS_SWITCH_FOR_ALL_DO( info.blendingMode, ULIS_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
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
      const FBlendCommandArgs* iArgs
    , const FSchedulePolicy& iPolicy
    , FThreadPool& iPool
)
{
    const FBlendCommandArgs&   info        = *iInfo;
    const uint8*        src         = info.source->Bits();
    uint8*              bdp         = info.backdrop->Bits();
    const uint32         src_bps     = info.source->BytesPerScanLine();
    const uint32         bdp_bps     = info.backdrop->BytesPerScanLine();
    const uint32         src_decal_y = info.shift.y + info.sourceRect.y;
    const uint32         src_decal_x = ( info.shift.x + info.sourceRect.x )  * info.source->BytesPerPixel();
    const uint32         bdp_decal_x = ( info.backdropWorkingRect.x )        * info.source->BytesPerPixel();
    Vec4i idt;
    BuildRGBA8IndexTable( info.source->FormatInfo().RSC, &idt );
    ULIS_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                , info.backdropWorkingRect.h
                                , InvokeBlendMTProcessScanline_NonSeparable_SSE_RGBA8
                                , src + ( ( src_decal_y + pLINE )                * src_bps ) + src_decal_x
                                , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                , pLINE , iInfo, idt );
}

ULIS_NAMESPACE_END

