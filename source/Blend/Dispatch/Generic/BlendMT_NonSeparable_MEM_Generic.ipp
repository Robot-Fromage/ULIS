// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         BlendMT_NonSeparable_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic Blend entry point functions.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Blend.h"
#include "Blend/Modes.h"
#include "Blend/Func/AlphaFuncF.ipp"
#include "Blend/Func/CompositingHelpers.ipp"
#include "Blend/Func/NonSeparableBlendFuncF.ipp"
#include "Color/ModelStructs.h"
#include "Conv/Conv.h"
#include "Conv/ConvBuffer.h"
#include "Maths/Geometry.h"

ULIS2_NAMESPACE_BEGIN
template< typename T >
void
InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic_Subpixel( const tByte* iSrc, tByte* iBdp, int32 iLine, const tSize iSrcBps, const FFormatInfo* iFmtInfo, std::shared_ptr< const FBlendInfo > iBlendParams ) {
    const FBlendInfo&   blendInfo       = *iBlendParams;
    const tByte*        src             = iSrc;
    tByte*              bdp             = iBdp;

    const bool notLastLine  = iLine < blendInfo._backdropCoverage.y;
    const bool notFirstLine = iLine > 0;
    const bool onLeftBorder = blendInfo._backdropWorkingRect.x == 0;
    const bool hasLeftData  = blendInfo.sourceRect.x + blendInfo._shift.x > 0;
    const bool hasTopData   = blendInfo.sourceRect.y + blendInfo._shift.y > 0;

    FPixelValue src_sample( iFmtInfo->FMT );
    FRGBF_arr src_conv;
    FRGBF_arr bdp_conv;
    FRGBF_arr res_conv;
    tByte* result = new tByte[ iFmtInfo->SPP ];

    // Query dispatched method
    FFormatInfo rgbfFormatInfo( ULIS2_FORMAT_RGBF );
    fpDispatchedConvInvoke conv_forward_fptr = QueryDispatchedConvInvokeForParameters( iFmtInfo->FMT, ULIS2_FORMAT_RGBF );
    fpDispatchedConvInvoke conv_backward_fptr = QueryDispatchedConvInvokeForParameters( ULIS2_FORMAT_RGBF, iFmtInfo->FMT );
    ULIS2_ASSERT( conv_forward_fptr, "No Conversion invocation found" );
    ULIS2_ASSERT( conv_backward_fptr, "No Conversion invocation found" );


    float m11, m01, m10, m00, vv0, vv1, res;
    m11 = ( notLastLine && onLeftBorder && hasLeftData )    ? TYPE2FLOAT( src - iFmtInfo->BPP,              iFmtInfo->AID ) : 0.f;
    m10 = ( hasLeftData && ( notFirstLine || hasTopData ) ) ? TYPE2FLOAT( src - iSrcBps - iFmtInfo->BPP,    iFmtInfo->AID ) : 0.f;
    vv1 = m10 * blendInfo.backdropPosition.y + m11 * blendInfo._buspixelComponent.y;

    for( int x = 0; x < blendInfo._backdropWorkingRect.w; ++x ) {
        const bool notLastCol = x < blendInfo._backdropCoverage.x;
        m00 = m10;
        m01 = m11;
        vv0 = vv1;
        SampleSubpixelAlpha( res );
        const float alpha_bdp   = iFmtInfo->HEA ? TYPE2FLOAT( bdp, iFmtInfo->AID ) : 1.f;
        const float alpha_src   = res * blendInfo.opacityValue;
        const float alpha_comp  = AlphaNormalF( alpha_src, alpha_bdp );
        const float var         = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        float alpha_result;
        ULIS2_ASSIGN_ALPHAF( blendInfo.alphaMode, alpha_result, alpha_src, alpha_bdp );

        for( uint8 j = 0; j < iFmtInfo->NCC; ++j ) {
            uint8 r = iFmtInfo->IDT[j];
            float s11, s01, s10, s00, v1, v2, srcvf;
            SampleSubpixelChannel( srcvf, r );
            FLOAT2TYPE( src_sample.Ptr(), r, srcvf );
        }

        conv_forward_fptr( *iFmtInfo, src_sample.Ptr(), rgbfFormatInfo, reinterpret_cast< tByte* >( &src_conv.m[0] ), 1 );
        conv_forward_fptr( *iFmtInfo, bdp, rgbfFormatInfo, reinterpret_cast< tByte* >( &bdp_conv.m[0] ), 1 );
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_conv = NonSeparableOpF< _BM >( src_conv, bdp_conv );
        ULIS2_SWITCH_FOR_ALL_DO( blendInfo.blendingMode, ULIS2_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN
        conv_backward_fptr( rgbfFormatInfo, reinterpret_cast< const tByte* >( &res_conv.m[0] ), *iFmtInfo, result, 1 );

        for( uint8 j = 0; j < iFmtInfo->NCC; ++j ) {
            uint8 r = iFmtInfo->IDT[j];
            FLOAT2TYPE( bdp, r, ComposeF( TYPE2FLOAT( src, r ), TYPE2FLOAT( bdp, r ), alpha_bdp, var, TYPE2FLOAT( result, r ) ) );
        }

        if( iFmtInfo->HEA ) FLOAT2TYPE( bdp, iFmtInfo->AID, alpha_result );
        src += iFmtInfo->BPP;
        bdp += iFmtInfo->BPP;
    }
}

template< typename T >
void
BlendMT_NonSeparable_MEM_Generic_Subpixel( const FFormatInfo& iFormatInfo, std::shared_ptr< const FBlendInfo > iBlendParams ) {
    const FBlendInfo&   blendInfo   = *iBlendParams;
    const tByte*        src         = iBlendParams->source->DataPtr();
    tByte*              bdp         = iBlendParams->backdrop->DataPtr();
    const tSize         src_bps     = iBlendParams->source->BytesPerScanLine();
    const tSize         bdp_bps     = iBlendParams->backdrop->BytesPerScanLine();
    const tSize         src_decal_y = blendInfo._shift.y + blendInfo.sourceRect.y;
    const tSize         src_decal_x = ( blendInfo._shift.x + blendInfo.sourceRect.x )   * iFormatInfo.BPP;
    const tSize         bdp_decal_x = ( blendInfo._backdropWorkingRect.x )              * iFormatInfo.BPP;
    ULIS2_MACRO_INLINE_PARALLEL_FOR( blendInfo.perfInfo.intent, blendInfo.perfInfo.pool, blendInfo.perfInfo.blocking
                                   , blendInfo._backdropWorkingRect.h
                                   , InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic_Subpixel< T >
                                   , src + ( ( src_decal_y + pLINE )                        * src_bps ) + src_decal_x
                                   , bdp + ( ( blendInfo._backdropWorkingRect.y + pLINE )   * bdp_bps ) + bdp_decal_x
                                   , pLINE , src_bps, &iFormatInfo, iBlendParams );
}

template< typename T >
void
InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic( const tByte* iSrc, tByte* iBdp, int32 iLine, const FFormatInfo* iFmtInfo, std::shared_ptr< const FBlendInfo > iBlendParams ) {
    const FBlendInfo&   blendInfo   = *iBlendParams;
    const tByte*        src         = iSrc;
    tByte*              bdp         = iBdp;

    FRGBF_arr src_conv;
    FRGBF_arr bdp_conv;
    FRGBF_arr res_conv;
    tByte* result = new tByte[ iFmtInfo->SPP ];

    // Query dispatched method
    FFormatInfo rgbfFormatInfo( ULIS2_FORMAT_RGBF );
    fpDispatchedConvInvoke conv_forward_fptr = QueryDispatchedConvInvokeForParameters( iFmtInfo->FMT, ULIS2_FORMAT_RGBF );
    fpDispatchedConvInvoke conv_backward_fptr = QueryDispatchedConvInvokeForParameters( ULIS2_FORMAT_RGBF, iFmtInfo->FMT );
    ULIS2_ASSERT( conv_forward_fptr, "No Conversion invocation found" );
    ULIS2_ASSERT( conv_backward_fptr, "No Conversion invocation found" );

    for( int x = 0; x < blendInfo._backdropWorkingRect.w; ++x ) {
        const float alpha_src       = iFmtInfo->HEA ? TYPE2FLOAT( src, iFmtInfo->AID ) * blendInfo.opacityValue : blendInfo.opacityValue;
        const float alpha_bdp       = iFmtInfo->HEA ? TYPE2FLOAT( bdp, iFmtInfo->AID ) : 1.f;
        const float alpha_comp      = AlphaNormalF( alpha_src, alpha_bdp );
        const float var             = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        float alpha_result;
        ULIS2_ASSIGN_ALPHAF( blendInfo.alphaMode, alpha_result, alpha_src, alpha_bdp );

        conv_forward_fptr( *iFmtInfo, src, rgbfFormatInfo, reinterpret_cast< tByte* >( &src_conv.m[0] ), 1 );
        conv_forward_fptr( *iFmtInfo, bdp, rgbfFormatInfo, reinterpret_cast< tByte* >( &bdp_conv.m[0] ), 1 );
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_conv = NonSeparableOpF< _BM >( src_conv, bdp_conv );
        ULIS2_SWITCH_FOR_ALL_DO( blendInfo.blendingMode, ULIS2_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN
        conv_backward_fptr( rgbfFormatInfo, reinterpret_cast< const tByte* >( &res_conv.m[0] ), *iFmtInfo, result, 1 );

        for( uint8 j = 0; j < iFmtInfo->NCC; ++j ) {
            uint8 r = iFmtInfo->IDT[j];
            FLOAT2TYPE( bdp, r, ComposeF( TYPE2FLOAT( src, r ), TYPE2FLOAT( bdp, r ), alpha_bdp, var, TYPE2FLOAT( result, r ) ) );
        }

        if( iFmtInfo->HEA ) FLOAT2TYPE( bdp, iFmtInfo->AID, alpha_result );
        src += iFmtInfo->BPP;
        bdp += iFmtInfo->BPP;
    }
}

template< typename T >
void
BlendMT_NonSeparable_MEM_Generic( const FFormatInfo& iFormatInfo, std::shared_ptr< const FBlendInfo > iBlendParams ) {
    const FBlendInfo&   blendInfo   = *iBlendParams;
    const tByte*        src         = iBlendParams->source->DataPtr();
    tByte*              bdp         = iBlendParams->backdrop->DataPtr();
    const tSize         src_bps     = iBlendParams->source->BytesPerScanLine();
    const tSize         bdp_bps     = iBlendParams->backdrop->BytesPerScanLine();
    const tSize         src_decal_y = blendInfo._shift.y + blendInfo.sourceRect.y;
    const tSize         src_decal_x = ( blendInfo._shift.x + blendInfo.sourceRect.x )   * iFormatInfo.BPP;
    const tSize         bdp_decal_x = ( blendInfo._backdropWorkingRect.x )              * iFormatInfo.BPP;
    ULIS2_MACRO_INLINE_PARALLEL_FOR( blendInfo.perfInfo.intent, blendInfo.perfInfo.pool, blendInfo.perfInfo.blocking
                                   , blendInfo._backdropWorkingRect.h
                                   , InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic< T >
                                   , src + ( ( src_decal_y + pLINE )                        * src_bps ) + src_decal_x
                                   , bdp + ( ( blendInfo._backdropWorkingRect.y + pLINE )   * bdp_bps ) + bdp_decal_x
                                   , pLINE , &iFormatInfo, iBlendParams );
}

ULIS2_NAMESPACE_END

