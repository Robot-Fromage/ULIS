// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         BlendMT_Separable_MEM_Generic.h
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
#include "Blend/Func/SeparableBlendFuncF.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeBlendMTProcessScanline_Separable_MEM_Generic_Subpixel(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FBlendCommandArgs&   info    = *iInfo;
    const FFormatMetrics&  fmt     = info.source->FormatMetrics();
    const uint8*        src     = iSrc;
    uint8*              bdp     = iBdp;
    const bool notLastLine  = iLine < info.backdropCoverage.y;
    const bool notFirstLine = iLine > 0;
    const bool onLeftBorder = info.backdropWorkingRect.x == 0;
    const bool hasLeftData  = info.sourceRect.x + info.shift.x > 0;
    const bool hasTopData   = info.sourceRect.y + info.shift.y > 0;

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
            float bdpvf = TYPE2FLOAT( bdp, r );
            #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) FLOAT2TYPE( bdp, r, SeparableCompOpF< _BM >( srcvf, bdpvf, alpha_bdp, var ) );
            ULIS_SWITCH_FOR_ALL_DO( info.blendingMode, ULIS_FOR_ALL_SEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
            #undef TMP_ASSIGN
        }

        if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
        src += fmt.BPP;
        bdp += fmt.BPP;
    }
}

template< typename T >
void
ScheduleBlendMT_Separable_MEM_Generic_Subpixel(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildBlendJobs< &InvokeBlendMTProcessScanline_Separable_MEM_Generic_Subpixel< T > >( iCommand, iPolicy );
}

template< typename T >
void
InvokeBlendMTProcessScanline_Separable_MEM_Generic(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FBlendCommandArgs&   info    = *iInfo;
    const FFormatMetrics&  fmt     = info.source->FormatMetrics();
    const uint8*        src     = iSrc;
    uint8*              bdp     = iBdp;

    for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
        const float alpha_src  = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * info.opacityValue : info.opacityValue;
        const float alpha_bdp  = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
        const float alpha_comp = AlphaNormalF( alpha_src, alpha_bdp );
        const float var        = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        float alpha_result;
        ULIS_ASSIGN_ALPHAF( info.alphaMode, alpha_result, alpha_src, alpha_bdp );
        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            float srcvf = TYPE2FLOAT( src, r );
            float bdpvf = TYPE2FLOAT( bdp, r );
            #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) FLOAT2TYPE( bdp, r, SeparableCompOpF< _BM >( srcvf, bdpvf, alpha_bdp, var ) );
            ULIS_SWITCH_FOR_ALL_DO( info.blendingMode, ULIS_FOR_ALL_SEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
            #undef TMP_ASSIGN
        }
        if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
        src += fmt.BPP;
        bdp += fmt.BPP;
    }
}

template< typename T >
void
ScheduleBlendMT_Separable_MEM_Generic(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildBlendJobs< &InvokeBlendMTProcessScanline_Separable_MEM_Generic< T > >( iCommand, iPolicy );
}

ULIS_NAMESPACE_END

