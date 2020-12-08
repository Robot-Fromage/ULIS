// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TiledBlendMT_NonSeparable_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Blend.h"
#include "Blend/BlendHelpers.h"
#include "Blend/Func/AlphaFuncF.h"
#include "Blend/Func/NonSeparableBlendFuncF.h"
#include "Image/Block.h"
#include "Conv/Conv.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeTiledBlendMTProcessScanline_NonSeparable_MEM_Generic(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->source.FormatMetrics();
    const uint8* ULIS_RESTRICT  base = jargs->src;
    const uint8* ULIS_RESTRICT  src = jargs->src;
    uint8*       ULIS_RESTRICT  bdp = jargs->bdp;

    FRGBF src_conv;
    FRGBF bdp_conv;
    FRGBF res_conv;
    uint8* result = new uint8[ fmt.SPP ];

    // Query dispatched method
    FFormatMetrics rgbfFormatMetrics( eFormat::Format_RGBF );
    fpConvertFormat conv_forward_fptr  = jargs->fwd;
    fpConvertFormat conv_backward_fptr = jargs->bkd;
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

        if( ( x + cargs->shift.x ) % cargs->sourceRect.w == 0 )
            src = base;
    }

    delete [] result;
}

template< typename T >
void
ScheduleTiledBlendMT_NonSeparable_MEM_Generic(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildBlendJobs< &InvokeTiledBlendMTProcessScanline_NonSeparable_MEM_Generic< T > >( iCommand, iPolicy, true );
}

ULIS_NAMESPACE_END

