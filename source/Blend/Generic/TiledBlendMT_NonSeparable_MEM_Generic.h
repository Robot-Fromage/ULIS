// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         TiledBlendMT_NonSeparable_MEM_Generic.h
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
#include "Image/Block.h"
#include "Conv/ConvBuffer.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeTiledBlendMTProcessScanline_NonSeparable_MEM_Generic( const uint8* iSrc, uint8* iBdp, int32 iLine, const FBlendCommandArgs* iArgs ) {
    const FBlendCommandArgs&   info    = *iInfo;
    const FFormat&  fmt     = info.source->FormatInfo();
    const uint8*        src     = iSrc + info.shift.x * fmt.BPP;
    uint8*              bdp     = iBdp;

    FRGBF src_conv;
    FRGBF bdp_conv;
    FRGBF res_conv;
    uint8* result = new uint8[ fmt.SPP ];

    // Query dispatched method
    FFormat rgbfFormatInfo( eFormat::Format_RGBF );
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

        conv_forward_fptr( fmt, src, rgbfFormatInfo, reinterpret_cast< uint8* >( &src_conv.m[0] ), 1 );
        conv_forward_fptr( fmt, bdp, rgbfFormatInfo, reinterpret_cast< uint8* >( &bdp_conv.m[0] ), 1 );
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_conv = NonSeparableOpF< _BM >( src_conv, bdp_conv );
        ULIS_SWITCH_FOR_ALL_DO( info.blendingMode, ULIS_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN
        conv_backward_fptr( rgbfFormatInfo, reinterpret_cast< const uint8* >( &res_conv.m[0] ), fmt, result, 1 );

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            FLOAT2TYPE( bdp, r, ComposeF( TYPE2FLOAT( src, r ), TYPE2FLOAT( bdp, r ), alpha_bdp, var, TYPE2FLOAT( result, r ) ) );
        }

        if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
        src += fmt.BPP;
        bdp += fmt.BPP;

        if( ( x + info.shift.x ) % info.sourceRect.w == 0 )
            src = iSrc;
    }

    delete [] result;
}

template< typename T >
void
ScheduleTiledBlendMT_NonSeparable_MEM_Generic( const FBlendCommandArgs* iArgs, const FSchedulePolicy& iPolicy, FThreadPool& iPool ) {
    const FBlendCommandArgs&   info        = *iInfo;
    const uint8*        src         = info.source->Bits();
    uint8*              bdp         = info.backdrop->Bits();
    const uint32         src_bps     = info.source->BytesPerScanLine();
    const uint32         bdp_bps     = info.backdrop->BytesPerScanLine();
    const uint32         src_decal_y = info.shift.y + info.sourceRect.y;
    const uint32         src_decal_x = ( info.sourceRect.x )  * info.source->BytesPerPixel();
    const uint32         bdp_decal_x = ( info.backdropWorkingRect.x )        * info.source->BytesPerPixel();
    ULIS_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.backdropWorkingRect.h
                                   , InvokeTiledBlendMTProcessScanline_NonSeparable_MEM_Generic< T >
                                   , src + ( ( info.sourceRect.y + ( ( info.shift.y + pLINE ) % info.sourceRect.h ) ) * src_bps ) + src_decal_x
                                   , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                   , pLINE , iInfo );
}

ULIS_NAMESPACE_END

