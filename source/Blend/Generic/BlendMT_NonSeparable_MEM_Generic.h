// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         BlendMT_NonSeparable_MEM_Generic.h
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
#include "Blend/Func/NonSeparableBlendFuncF.h"
#include "Conv/ConvBuffer.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic_Subpixel( const uint8* iSrc, uint8* iBdp, int32 iLine, const uint32 iSrcBps, const FCommand* iCommand ) {
    const FBlendCommandArgs&   info    = *iInfo;
    const FFormatMetrics&  fmt     = info.source->FormatMetrics();
    const uint8*        src     = iSrc;
    uint8*              bdp     = iBdp;

    const bool notLastLine  = iLine < info.backdropCoverage.y;
    const bool notFirstLine = iLine > 0;
    const bool onLeftBorder = info.backdropWorkingRect.x == 0;
    const bool hasLeftData  = info.sourceRect.x + info.shift.x > 0;
    const bool hasTopData   = info.sourceRect.y + info.shift.y > 0;

    FColor src_sample( fmt.FMT );
    FRGBF src_conv;
    FRGBF bdp_conv;
    FRGBF res_conv;
    uint8* result = new uint8[ fmt.SPP ];

    // Query dispatched method
    FFormatMetrics rgbfFormatMetrics( eFormat::Format_RGBF );
    fpConversionInvocation conv_forward_fptr = QueryDispatchedConversionInvocation( fmt.FMT, eFormat::Format_RGBF );
    fpConversionInvocation conv_backward_fptr = QueryDispatchedConversionInvocation( eFormat::Format_RGBF, fmt.FMT );
    ULIS_ASSERT( conv_forward_fptr, "No Conversion invocation found" );
    ULIS_ASSERT( conv_backward_fptr, "No Conversion invocation found" );


    float m11, m01, m10, m00, vv0, vv1, res;
    m11 = ( notLastLine && onLeftBorder && hasLeftData )    ? TYPE2FLOAT( src - fmt.BPP,              fmt.AID ) : 0.f;
    m10 = ( hasLeftData && ( notFirstLine || hasTopData ) ) ? TYPE2FLOAT( src - iSrcBps - fmt.BPP,    fmt.AID ) : 0.f;
    vv1 = m10 * info.subpixelComponent.y + m11 * info.buspixelComponent.y;

    for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
        const bool notLastCol = x < info.backdropCoverage.x;
        m00 = m10;
        m01 = m11;
        vv0 = vv1;
        SampleSubpixelAlpha( res );
        const float alpha_bdp   = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
        const float alpha_src   = res * info.opacityValue;
        const float alpha_comp  = AlphaNormalF( alpha_src, alpha_bdp );
        const float var         = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        float alpha_result;
        ULIS_ASSIGN_ALPHAF( info.alphaMode, alpha_result, alpha_src, alpha_bdp );

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            float s11, s01, s10, s00, v1, v2, srcvf;
            SampleSubpixelChannel( srcvf, r );
            FLOAT2TYPE( src_sample.Bits(), r, srcvf );
        }

        conv_forward_fptr( fmt, src_sample.Bits(), rgbfFormatMetrics, reinterpret_cast< uint8* >( &src_conv.m[0] ), 1 );
        conv_forward_fptr( fmt, bdp, rgbfFormatMetrics, reinterpret_cast< uint8* >( &bdp_conv.m[0] ), 1 );
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_conv = NonSeparableOpF< _BM >( src_conv, bdp_conv );
        ULIS_SWITCH_FOR_ALL_DO( info.blendingMode, ULIS_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN
        conv_backward_fptr( rgbfFormatMetrics, reinterpret_cast< const uint8* >( &res_conv.m[0] ), fmt, result, 1 );

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            FLOAT2TYPE( bdp, r, ComposeF( TYPE2FLOAT( src, r ), TYPE2FLOAT( bdp, r ), alpha_bdp, var, TYPE2FLOAT( result, r ) ) );
        }

        if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
        src += fmt.BPP;
        bdp += fmt.BPP;
    }

    delete [] result;
}

template< typename T >
void
ScheduleBlendMT_NonSeparable_MEM_Generic_Subpixel( FCommand* iCommand, const FSchedulePolicy& iPolicy, FThreadPool& iPool ) {
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
                                   , InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic_Subpixel< T >
                                   , src + ( ( src_decal_y + pLINE )                * src_bps ) + src_decal_x
                                   , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                   , pLINE , src_bps, iInfo );
}

template< typename T >
void
InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic( const uint8* iSrc, uint8* iBdp, int32 iLine, const FCommand* iCommand ) {
    const FBlendCommandArgs&   info    = *iInfo;
    const FFormatMetrics&  fmt     = info.source->FormatMetrics();
    const uint8*        src     = iSrc;
    uint8*              bdp     = iBdp;

    FRGBF src_conv;
    FRGBF bdp_conv;
    FRGBF res_conv;
    uint8* result = new uint8[ fmt.SPP ];

    // Query dispatched method
    FFormatMetrics rgbfFormatMetrics( eFormat::Format_RGBF );
    fpConversionInvocation conv_forward_fptr = QueryDispatchedConversionInvocation( fmt.FMT, eFormat::Format_RGBF );
    fpConversionInvocation conv_backward_fptr = QueryDispatchedConversionInvocation( eFormat::Format_RGBF, fmt.FMT );
    ULIS_ASSERT( conv_forward_fptr,    "No Conversion invocation found" );
    ULIS_ASSERT( conv_backward_fptr,   "No Conversion invocation found" );

    for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
        const float alpha_src   = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * info.opacityValue : info.opacityValue;
        const float alpha_bdp   = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
        const float alpha_comp  = AlphaNormalF( alpha_src, alpha_bdp );
        const float var         = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        float alpha_result;
        ULIS_ASSIGN_ALPHAF( info.alphaMode, alpha_result, alpha_src, alpha_bdp );

        conv_forward_fptr( fmt, src, rgbfFormatMetrics, reinterpret_cast< uint8* >( &src_conv.m[0] ), 1 );
        conv_forward_fptr( fmt, bdp, rgbfFormatMetrics, reinterpret_cast< uint8* >( &bdp_conv.m[0] ), 1 );
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_conv = NonSeparableOpF< _BM >( src_conv, bdp_conv );
        ULIS_SWITCH_FOR_ALL_DO( info.blendingMode, ULIS_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN
        conv_backward_fptr( rgbfFormatMetrics, reinterpret_cast< const uint8* >( &res_conv.m[0] ), fmt, result, 1 );

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            FLOAT2TYPE( bdp, r, ComposeF( TYPE2FLOAT( src, r ), TYPE2FLOAT( bdp, r ), alpha_bdp, var, TYPE2FLOAT( result, r ) ) );
        }

        if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
        src += fmt.BPP;
        bdp += fmt.BPP;
    }

    delete [] result;
}

template< typename T >
void
ScheduleBlendMT_NonSeparable_MEM_Generic( FCommand* iCommand, const FSchedulePolicy& iPolicy, FThreadPool& iPool ) {
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
                                   , InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic< T >
                                   , src + ( ( src_decal_y + pLINE )                * src_bps ) + src_decal_x
                                   , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                   , pLINE , iInfo );
}

ULIS_NAMESPACE_END

