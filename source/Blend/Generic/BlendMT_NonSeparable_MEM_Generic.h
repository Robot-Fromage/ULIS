// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         BlendMT_NonSeparable_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation for Blend composition
*               operations, that involve non-speparable computation ( channels
*               not independent, calculation involves a change of model )
*               for generic formats, without optimisations.
*               This versions should work with any color model and any depth
*               or layout.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/BlendArgs.h"
#include "Blend/BlendHelpers.h"
#include "Blend/Func/AlphaFuncF.h"
#include "Blend/Func/NonSeparableBlendFuncF.h"
#include "Conv/ConvertFormatInvocations.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic_Subpixel(
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

    FColor src_sample( fmt.FMT );
    FRGBF src_conv;
    FRGBF bdp_conv;
    FRGBF res_conv;
    uint8* result = new uint8[ fmt.SPP ];

    // Query dispatched method
    FFormatMetrics rgbfFormatMetrics( eFormat::Format_RGBF );
    fpConversionInvocation conv_forward_fptr  = jargs->fwd;
    fpConversionInvocation conv_backward_fptr = jargs->bkd;
    ULIS_ASSERT( conv_forward_fptr, "No Conversion invocation found" );
    ULIS_ASSERT( conv_backward_fptr, "No Conversion invocation found" );


    ufloat m11, m01, m10, m00, vv0, vv1, res;
    m11 = ( notLastLine && onLeftBorder && hasLeftData )    ? TYPE2FLOAT( src - fmt.BPP,              fmt.AID ) : 0.f;
    m10 = ( hasLeftData && ( notFirstLine || hasTopData ) ) ? TYPE2FLOAT( src - jargs->src_bps - fmt.BPP,    fmt.AID ) : 0.f;
    vv1 = m10 * cargs->subpixelComponent.y + m11 * cargs->buspixelComponent.y;

    for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
        const bool notLastCol = x < cargs->backdropCoverage.x;
        m00 = m10;
        m01 = m11;
        vv0 = vv1;
        SampleSubpixelAlpha( res );
        const ufloat alpha_bdp  = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
        const ufloat alpha_src  = res * cargs->opacity;
        const ufloat alpha_comp = AlphaNormalF( alpha_src, alpha_bdp );
        const ufloat var        = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        ufloat alpha_result;
        ULIS_ASSIGN_ALPHAF( cargs->alphaMode, alpha_result, alpha_src, alpha_bdp );

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            const uint8 r = fmt.IDT[j];
            ufloat s11, s01, s10, s00, v1, v2, srcvf;
            SampleSubpixelChannel( srcvf, r );
            FLOAT2TYPE( src_sample.Bits(), r, srcvf );
        }

        conv_forward_fptr( fmt, src_sample.Bits(), rgbfFormatMetrics, reinterpret_cast< uint8* >( &src_conv.m[0] ), 1 );
        conv_forward_fptr( fmt, bdp, rgbfFormatMetrics, reinterpret_cast< uint8* >( &bdp_conv.m[0] ), 1 );
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_conv = NonSeparableOpF< _BM >( src_conv, bdp_conv );
        ULIS_SWITCH_FOR_ALL_DO( cargs->blendingMode, ULIS_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN
        conv_backward_fptr( rgbfFormatMetrics, reinterpret_cast< const uint8* >( &res_conv.m[0] ), fmt, result, 1 );

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            const uint8 r = fmt.IDT[j];
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
ScheduleBlendMT_NonSeparable_MEM_Generic_Subpixel(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildBlendJobs< &InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic_Subpixel< T > >( iCommand, iPolicy );
}

template< typename T >
void
InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->source.FormatMetrics();
    const uint8* ULIS_RESTRICT  src = jargs->src;
    uint8*       ULIS_RESTRICT  bdp = jargs->bdp;

    FRGBF src_conv;
    FRGBF bdp_conv;
    FRGBF res_conv;
    uint8* result = new uint8[ fmt.SPP ];

    // Query dispatched method
    FFormatMetrics rgbfFormatMetrics( eFormat::Format_RGBF );
    fpConversionInvocation conv_forward_fptr  = jargs->fwd;
    fpConversionInvocation conv_backward_fptr = jargs->bkd;
    ULIS_ASSERT( conv_forward_fptr,    "No Conversion invocation found" );
    ULIS_ASSERT( conv_backward_fptr,   "No Conversion invocation found" );

    for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
        const ufloat alpha_src  = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * cargs->opacity : cargs->opacity;
        const ufloat alpha_bdp  = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
        const ufloat alpha_comp = AlphaNormalF( alpha_src, alpha_bdp );
        const ufloat var        = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        ufloat alpha_result;
        ULIS_ASSIGN_ALPHAF( cargs->alphaMode, alpha_result, alpha_src, alpha_bdp );

        conv_forward_fptr( fmt, src, rgbfFormatMetrics, reinterpret_cast< uint8* >( &src_conv.m[0] ), 1 );
        conv_forward_fptr( fmt, bdp, rgbfFormatMetrics, reinterpret_cast< uint8* >( &bdp_conv.m[0] ), 1 );
        #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) res_conv = NonSeparableOpF< _BM >( src_conv, bdp_conv );
        ULIS_SWITCH_FOR_ALL_DO( cargs->blendingMode, ULIS_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
        #undef TMP_ASSIGN
        conv_backward_fptr( rgbfFormatMetrics, reinterpret_cast< const uint8* >( &res_conv.m[0] ), fmt, result, 1 );

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            const uint8 r = fmt.IDT[j];
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
ScheduleBlendMT_NonSeparable_MEM_Generic(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildBlendJobs< &InvokeBlendMTProcessScanline_NonSeparable_MEM_Generic< T > >( iCommand, iPolicy );
}

ULIS_NAMESPACE_END

