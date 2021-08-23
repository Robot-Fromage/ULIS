// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         TiledBlendMT_Separable_SSE_RGBA8.ipp
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
#include "Blend/Func/AlphaFuncF.ipp"
#include "Blend/Func/AlphaFuncSSEF.ipp"
#include "Blend/Func/SeparableBlendFuncSSEF.ipp"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Thread/ThreadPool.h"
#include <vectorclass.h>

ULIS3_NAMESPACE_BEGIN
void
InvokeTiledBlendMTProcessScanline_Separable_SSE_RGBA8( const tByte* iSrc, tByte* iBdp, int32 iLine, std::shared_ptr< const FBlendArgs > iInfo ) {
    const FBlendArgs&   info    = *iInfo;
    const FFormatInfo&  fmt     = info.source->FormatInfo();
    const tByte*        src     = iSrc + info.shift.x * fmt.BPP;
    tByte*              bdp     = iBdp;

    for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
        ufloat alpha_bdp    = bdp[fmt.AID] / 255.f;
        ufloat alpha_src    = ( src[fmt.AID] / 255.f ) * info.opacityValue;
        ufloat alpha_comp   = AlphaNormalF( alpha_src, alpha_bdp );
        ufloat var          = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        ufloat alpha_result;
        ULIS3_ASSIGN_ALPHAF( info.alphaMode, alpha_result, alpha_src, alpha_bdp );
        Vec4f src_chan = Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( reinterpret_cast< const __m128i* >( src ) ) ) ) ) / 255.f;
        Vec4f bdp_chan = Vec4f( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( reinterpret_cast< const __m128i* >( bdp ) ) ) ) ) / 255.f;
        Vec4f res_chan;

        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_chan = SeparableCompOpSSEF< _BM >( src_chan, bdp_chan, alpha_bdp, var ) * 255.f;
        ULIS3_SWITCH_FOR_ALL_DO( info.blendingMode, ULIS3_FOR_ALL_SEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN

        auto _pack = _mm_cvtps_epi32( res_chan );
        _pack = _mm_packus_epi32( _pack, _pack );
        _pack = _mm_packus_epi16( _pack, _pack );
        *( uint32* )bdp = static_cast< uint32 >( _mm_cvtsi128_si32( _pack ) );
        bdp[fmt.AID] = static_cast< uint8 >( alpha_result * 0xFF );

        src += 4;
        bdp += 4;
        if( ( x + info.shift.x ) % info.sourceRect.w == 0 )
            src = iSrc;
    }
}

void
TiledBlendMT_Separable_SSE_RGBA8( std::shared_ptr< const FBlendArgs > iInfo ) {
    const FBlendArgs&   info        = *iInfo;
    const tByte*        src         = info.source->DataPtr();
    tByte*              bdp         = info.backdrop->DataPtr();
    const tSize         src_bps     = info.source->BytesPerScanLine();
    const tSize         bdp_bps     = info.backdrop->BytesPerScanLine();
    const tSize         src_decal_y = info.shift.y + info.sourceRect.y;
    const tSize         src_decal_x = ( info.sourceRect.x )  * info.source->BytesPerPixel();
    const tSize         bdp_decal_x = ( info.backdropWorkingRect.x )        * info.source->BytesPerPixel();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                , info.backdropWorkingRect.h
                                , InvokeTiledBlendMTProcessScanline_Separable_SSE_RGBA8
                                , src + ( ( info.sourceRect.y + ( ( info.shift.y + pLINE ) % info.sourceRect.h ) ) * src_bps ) + src_decal_x
                                , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                , pLINE , iInfo );
}

ULIS3_NAMESPACE_END

