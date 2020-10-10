// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         AlphaBlendMT_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation for Alpha Blend
*               composition, for generic formats, without optimisations.
*               This versions should work with any color model and any depth
*               or layout. Alpha Blend should be faster than the regular Blend
*               counterpart.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/BlendArgs.h"
#include "Blend/BlendHelpers.h"
#include "Blend/Func/AlphaFuncF.h"
#include "Blend/Func/SeparableBlendFuncF.h"
#include "Blend/Modes.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeAlphaBlendMTProcessScanline_Separable_MEM_Generic_Subpixel(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->source.FormatMetrics();
    const uint8 ULIS_RESTRICT * src = jargs->src;
    uint8       ULIS_RESTRICT * bdp = jargs->bdp;

    const bool notLastLine  = jargs->line < info.backdropCoverage.y;
    const bool notFirstLine = jargs->line > 0;
    const bool onLeftBorder = cargs->backdropWorkingRect.x == 0;
    const bool hasLeftData  = cargs->sourceRect.x + cargs->shift.x > 0;
    const bool hasTopData   = cargs->sourceRect.y + cargs->shift.y > 0;

    float m11, m01, m10, m00, vv0, vv1, res;
    m11 = ( notLastLine && onLeftBorder && hasLeftData )    ? TYPE2FLOAT( src - fmt.BPP,            fmt.AID ) : 0.f;
    m10 = ( hasLeftData && ( notFirstLine || hasTopData ) ) ? TYPE2FLOAT( src - iSrcBps - fmt.BPP,  fmt.AID ) : 0.f;
    vv1 = m10 * info.subpixelComponent.y + m11 * info.buspixelComponent.y;

    for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
        const bool notLastCol = x < cargs->backdropCoverage.x;
        m00 = m10;
        m01 = m11;
        vv0 = vv1;
        SampleSubpixelAlpha( res );
        const float alpha_bdp   = TYPE2FLOAT( bdp, fmt.AID );
        const float alpha_src   = res * info.opacityValue;
        const float alpha_comp  = AlphaNormalF( alpha_src, alpha_bdp );
        const float var         = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            const uint8 r = fmt.IDT[j];
            float s11, s01, s10, s00, v1, v2, srcvf;
            SampleSubpixelChannel( srcvf, r );
            const float bdpvf = TYPE2FLOAT( bdp, r );
            FLOAT2TYPE( bdp, r, SeparableCompOpF< Blend_Normal >( srcvf, bdpvf, alpha_bdp, var ) );
        }

        FLOAT2TYPE( bdp, fmt.AID, alpha_comp );
        src += fmt.BPP;
        bdp += fmt.BPP;
    }
}

template< typename T >
void
ScheduleAlphaBlendMT_Separable_MEM_Generic_Subpixel(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildBlendJobs< &InvokeAlphaBlendMTProcessScanline_Separable_MEM_Generic_Subpixel< T > >( iCommand, iPolicy );
}

template< typename T >
void
InvokeAlphaBlendMTProcessScanline_Separable_MEM_Generic(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
)
{
    const FFormatMetrics&       fmt = cargs->source.FormatMetrics();
    const uint8 ULIS_RESTRICT * src = jargs->src;
    uint8       ULIS_RESTRICT * bdp = jargs->bdp;

    for( int x = 0; x < cargs->backdropWorkingRect.w; ++x ) {
        const ufloat alpha_src  = TYPE2FLOAT( src, fmt.AID ) * info.opacityValue;
        const ufloat alpha_bdp  = TYPE2FLOAT( bdp, fmt.AID );
        const ufloat alpha_comp = AlphaNormalF( alpha_src, alpha_bdp );
        const ufloat var        = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            const uint8 r = fmt.IDT[j];
            const ufloat srcvf = TYPE2FLOAT( src, r );
            const ufloat bdpvf = TYPE2FLOAT( bdp, r );
            FLOAT2TYPE( bdp, r, SeparableCompOpF< Blend_Normal >( srcvf, bdpvf, alpha_bdp, var ) );
        }

        FLOAT2TYPE( bdp, fmt.AID, alpha_comp );
        src += fmt.BPP;
        bdp += fmt.BPP;
    }
}

template< typename T >
void
ScheduleAlphaBlendMT_Separable_MEM_Generic(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildBlendJobs< &InvokeAlphaBlendMTProcessScanline_Separable_MEM_Generic< T > >( iCommand, iPolicy );
}

ULIS_NAMESPACE_END

