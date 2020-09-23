// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         AlphaBlendMT_AVX_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Blend/RGBA8/AlphaBlendMT_AVX_RGBA8.h"
#include "Blend/BlendHelpers.h"
#include "Blend/Modes.h"
#include "Blend/Func/AlphaFuncAVX.h"
#include "Blend/Func/SeparableBlendFuncAVXF.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Thread/ThreadPool.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeAlphaBlendMTProcessScanline_Separable_AVX_RGBA8_Subpixel(
      const uint8* iSrc
    , uint8* iBdp
    , int32 iLine
    , const uint32 iSrcBps
    , const FBlendCommandArgs* iArgs
)
{
    const FBlendCommandArgs&   info    = *iInfo;
    const FFormat&  fmt     = info.source->FormatInfo();
    const uint8*        src     = iSrc;
    uint8*              bdp     = iBdp;

    const bool notLastLine  = iLine < info.backdropCoverage.y;
    const bool notFirstLine = iLine > 0;
    const bool onLeftBorder = info.backdropWorkingRect.x == 0;
    const bool hasLeftData  = info.sourceRect.x + info.shift.x > 0;
    const bool hasTopData   = info.sourceRect.y + info.shift.y > 0;

    Vec8f TX( info.subpixelComponent.x );
    Vec8f TY( info.subpixelComponent.y );
    Vec8f UX( info.buspixelComponent.x );
    Vec8f UY( info.buspixelComponent.y );

    //   The gain is not huge compared to SSE
    //   _X_ | _X_ | _X_ | _X_ | _X_ | ...
    //  _____|_____|_____|_____|_____|_
    //   _X_ | m00 | m10 | m20 | m30 | ...
    //  _____|_____|_____|_____|_____|_
    //   _X_ | m01 | m11 | m21 | m31 | ...
    //  _____|_____|_____|_____|_____|_
    //        \     \   /     /
    //         \     \ /     /
    //          \     X     /
    //           \   / \   /
    //            \ /   \ /
    //             |     |
    //          vv0vv1 vv1vv2 -> res
    const uint8* p00 = iSrc - iSrcBps;
    const uint8* p10 = iSrc - iSrcBps + 4;
    const uint8* p01 = iSrc;
    const uint8* p11 = iSrc + 4;
    Vec8f alpha_m00m10, alpha_m10m20, alpha_m01m11, alpha_m11m21, alpha_vv0, alpha_vv1, alpha_smp;
    Vec8f smpch_m00m10, smpch_m10m20, smpch_m01m11, smpch_m11m21, smpch_vv0, smpch_vv1, smpch_smp;
    Vec4f alpha_m10 = ( hasLeftData && ( notFirstLine || hasTopData ) )   ? *( iSrc - 4 + fmt.AID - iSrcBps   ) / 255.f : 0.f;
    Vec4f alpha_m11 = ( notLastLine && onLeftBorder && hasLeftData )      ? *( iSrc - 4 + fmt.AID             ) / 255.f : 0.f;
    alpha_m10m20 = Vec8f( 0.f, alpha_m10 );
    alpha_m11m21 = Vec8f( 0.f, alpha_m11 );
    Vec4f fc10 = ( hasLeftData && ( notFirstLine || hasTopData ) )      ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si32( iSrc - 4 - iSrcBps     ) ) ) ) / 255.f : 0.f;
    Vec4f fc11 = ( notLastLine && onLeftBorder && hasLeftData )         ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si32( iSrc - 4               ) ) ) ) / 255.f : 0.f;
    smpch_m10m20 = Vec8f( 0.f, fc10 );
    smpch_m11m21 = Vec8f( 0.f, fc11 );

    for( int32 x = 0; x < info.backdropWorkingRect.w; x+=2 ) {
        const bool notLastCol           = x     < info.backdropCoverage.x;
        const bool notPenultimateCol    = x + 1 < info.backdropCoverage.x;
        const bool cond00 = notLastCol          && ( notFirstLine || hasTopData );
        const bool cond10 = notPenultimateCol   && ( notFirstLine || hasTopData );
        const bool cond01 = notLastCol          && notLastLine;
        const bool cond11 = notPenultimateCol   && notLastLine;

        // Load Alpha
        Vec4f la00 = cond00 ? p00[fmt.AID] / 255.f : 0.f;
        Vec4f la10 = cond10 ? p10[fmt.AID] / 255.f : 0.f;
        Vec4f la01 = cond01 ? p01[fmt.AID] / 255.f : 0.f;
        Vec4f la11 = cond11 ? p11[fmt.AID] / 255.f : 0.f;
        alpha_m00m10 = Vec8f( alpha_m10m20.get_high(), la00 );
        alpha_m01m11 = Vec8f( alpha_m11m21.get_high(), la01 );
        alpha_m10m20 = Vec8f( la00, la10 );
        alpha_m11m21 = Vec8f( la01, la11 );
        alpha_vv0 = alpha_m00m10 * TY + alpha_m01m11 * UY;
        alpha_vv1 = alpha_m10m20 * TY + alpha_m11m21 * UY;
        alpha_smp = alpha_vv0 * TX + alpha_vv1 * UX;

        // Load Channels
        Vec4f fc00 = cond00 ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si32( p00 ) ) ) ) / 255.f : 0.f;
        Vec4f fc10 = cond10 ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si32( p10 ) ) ) ) / 255.f : 0.f;
        Vec4f fc01 = cond01 ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si32( p01 ) ) ) ) / 255.f : 0.f;
        Vec4f fc11 = cond11 ? Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si32( p11 ) ) ) ) / 255.f : 0.f;

        smpch_m00m10 = Vec8f( smpch_m10m20.get_high(), fc00 );
        smpch_m01m11 = Vec8f( smpch_m11m21.get_high(), fc01 );
        smpch_m10m20 = Vec8f( fc00, fc10 );
        smpch_m11m21 = Vec8f( fc01, fc11 );
        smpch_vv0 = ( smpch_m00m10 * alpha_m00m10 ) * TY + ( smpch_m01m11 * alpha_m01m11 )  * UY;
        smpch_vv1 = ( smpch_m10m20 * alpha_m10m20 ) * TY + ( smpch_m11m21 * alpha_m11m21 )  * UY;
        smpch_smp = select( alpha_smp == 0.f, 0.f, ( smpch_vv0 * TX + smpch_vv1 * UX ) / alpha_smp );

        // Comp Alpha
        Vec8f alpha_bdp     = *( iBdp + fmt.AID ) / 255.f;
        Vec8f alpha_src     = alpha_smp * info.opacityValue;
        Vec8f alpha_comp    = AlphaNormalAVXF( alpha_src, alpha_bdp );
        Vec8f var           = select( alpha_comp == 0.f, 0.f, alpha_src / alpha_comp );
        Vec8f alpha_result = alpha_comp * 255.f;

        // Comp Channels
        __m128i bdp128 = _mm_setzero_si128();
        memcpy( &bdp128, iBdp, 8 );
        Vec8f   bdp_chan = Vec8f( _mm256_cvtepi32_ps( _mm256_cvtepu8_epi32( bdp128 ) ) ) / 255.f;
        Vec8f   res_chan;
        res_chan = SeparableCompOpAVXF< Blend_Normal >( smpch_smp, bdp_chan, alpha_bdp, var ) * 255.f;

        Vec8ui _pack0 = _mm256_cvtps_epi32( res_chan );
        Vec8us _pack1 = compress( _pack0 );
        auto _pack = _mm_packus_epi16( _pack1, _pack1 );
        _mm_storeu_si64( iBdp, _pack );
        iBdp[fmt.AID]      = static_cast< uint8 >( alpha_result[0] );
        iBdp[fmt.AID+4]    = static_cast< uint8 >( alpha_result[4] );

        iBdp += 8;
        p00 += 8;
        p10 += 8;
        p01 += 8;
        p11 += 8;
    }
}

void
ScheduleAlphaBlendMT_Separable_AVX_RGBA8_Subpixel(
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
    ULIS_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.backdropWorkingRect.h
                                   , InvokeAlphaBlendMTProcessScanline_Separable_AVX_RGBA8_Subpixel
                                   , src + ( ( src_decal_y + pLINE )                * src_bps ) + src_decal_x
                                   , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                   , pLINE , src_bps, iInfo );
}

void
InvokeAlphaBlendMTProcessScanline_Separable_AVX_RGBA8(
      const uint8* iSrc
    , uint8* iBdp
    , int32 iLine
    , const FBlendCommandArgs* iArgs
)
{
    const FBlendCommandArgs&   info    = *iInfo;
    const FFormat&  fmt     = info.source->FormatInfo();
    const uint8*        src     = iSrc;
    uint8*              bdp     = iBdp;

    const uint32 len = info.backdropWorkingRect.w / 2;
    for( uint32 i = 0; i < len; ++i ) {
        Vec8f   alpha_bdp   = Vec8f( iBdp[fmt.AID], iBdp[fmt.AID + 4] ) / 255.f;
        Vec8f   alpha_src   = Vec8f( iSrc[fmt.AID], iSrc[fmt.AID + 4] ) / 255.f * info.opacityValue;
        Vec8f   alpha_comp  = AlphaNormalAVXF( alpha_src, alpha_bdp );
        Vec8f   var         = select( alpha_comp == 0.f, 0.f, ( alpha_src / alpha_comp ) );
        Vec8f   alpha_result = alpha_comp * 255.f;

        Vec8f   src_chan = Vec8f( _mm256_cvtepi32_ps( _mm256_cvtepu8_epi32( _mm_loadu_si64( iSrc ) ) ) ) / 255.f;
        Vec8f   bdp_chan = Vec8f( _mm256_cvtepi32_ps( _mm256_cvtepu8_epi32( _mm_loadu_si64( iBdp ) ) ) ) / 255.f;
        Vec8f   res_chan;
        res_chan = SeparableCompOpAVXF< Blend_Normal >( src_chan, bdp_chan, alpha_bdp, var ) * 255.f;

        Vec8ui _pack0 = _mm256_cvtps_epi32( res_chan );
        Vec8us _pack1 = compress( _pack0 );
        auto _pack = _mm_packus_epi16( _pack1, _pack1 );
        _mm_storeu_si64( iBdp, _pack );
        iBdp[fmt.AID]      = static_cast< uint8 >( alpha_result[0] );
        iBdp[fmt.AID+4]    = static_cast< uint8 >( alpha_result[4] );

        iSrc += 8;
        iBdp += 8;
    }

    // In case W is odd, process one last pixel.
    if( info.backdropWorkingRect.w % 2 ) {
        Vec8f   alpha_bdp   = Vec8f( iBdp[fmt.AID], 0 ) / 255.f;
        Vec8f   alpha_src   = Vec8f( iSrc[fmt.AID], 0 ) / 255.f * info.opacityValue;
        Vec8f   alpha_comp  = AlphaNormalAVXF( alpha_src, alpha_bdp );
        Vec8f   var         = select( alpha_comp == 0.f, 0.f, ( alpha_src / alpha_comp ) );
        Vec8f   alpha_result = alpha_comp * 255.f;

        Vec8f   src_chan = Vec8f( _mm256_cvtepi32_ps( _mm256_cvtepu8_epi32( _mm_loadu_si32( iSrc ) ) ) ) / 255.f;
        Vec8f   bdp_chan = Vec8f( _mm256_cvtepi32_ps( _mm256_cvtepu8_epi32( _mm_loadu_si32( iBdp ) ) ) ) / 255.f;
        Vec8f   res_chan;
        res_chan = SeparableCompOpAVXF< Blend_Normal >( src_chan, bdp_chan, alpha_bdp, var ) * 255.f;

        Vec8ui _pack0 = _mm256_cvtps_epi32( res_chan );
        Vec8us _pack1 = compress( _pack0 );
        auto _pack = _mm_packus_epi16( _pack1, _pack1 );
        _mm_storeu_si32( iBdp, _pack );
        iBdp[fmt.AID] = static_cast< uint8 >( alpha_result[0] );
    }
}

void
ScheduleAlphaBlendMT_Separable_AVX_RGBA8(
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
    ULIS_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                , info.backdropWorkingRect.h
                                , InvokeAlphaBlendMTProcessScanline_Separable_AVX_RGBA8
                                , src + ( ( src_decal_y + pLINE )                * src_bps ) + src_decal_x
                                , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                , pLINE , iInfo );
}

ULIS_NAMESPACE_END

