// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         AlphaBlendMT_MEM_Generic.h
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
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeAlphaBlendMTProcessScanline_Separable_MEM_Generic_Subpixel( FBlendJobArgs iJobArgs, const FCommand* iCommand ) {
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
    m11 = ( notLastLine && onLeftBorder && hasLeftData )    ? TYPE2FLOAT( src - fmt.BPP,            fmt.AID ) : 0.f;
    m10 = ( hasLeftData && ( notFirstLine || hasTopData ) ) ? TYPE2FLOAT( src - iSrcBps - fmt.BPP,  fmt.AID ) : 0.f;
    vv1 = m10 * info.subpixelComponent.y + m11 * info.buspixelComponent.y;

    for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
        const bool notLastCol = x < info.backdropCoverage.x;
        m00 = m10;
        m01 = m11;
        vv0 = vv1;
        SampleSubpixelAlpha( res );
        const float alpha_bdp   = TYPE2FLOAT( bdp, fmt.AID );
        const float alpha_src   = res * info.opacityValue;
        const float alpha_comp  = AlphaNormalF( alpha_src, alpha_bdp );
        const float var         = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            float s11, s01, s10, s00, v1, v2, srcvf;
            SampleSubpixelChannel( srcvf, r );
            float bdpvf = TYPE2FLOAT( bdp, r );
            FLOAT2TYPE( bdp, r, SeparableCompOpF< Blend_Normal >( srcvf, bdpvf, alpha_bdp, var ) );
        }

        FLOAT2TYPE( bdp, fmt.AID, alpha_comp );
        src += fmt.BPP;
        bdp += fmt.BPP;
    }
}

template< typename T >
void
ScheduleAlphaBlendMT_Separable_MEM_Generic_Subpixel( FCommand* iCommand, const FSchedulePolicy& iPolicy ) {
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
                                   , InvokeAlphaBlendMTProcessScanline_Separable_MEM_Generic_Subpixel< T >
                                   , src + ( ( src_decal_y + pLINE )                * src_bps ) + src_decal_x
                                   , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                   , pLINE , src_bps, iInfo );
}

template< typename T >
void
InvokeAlphaBlendMTProcessScanline_Separable_MEM_Generic( const FBlendJobArgs* iJobArgs, const FCommand* iCommand ) {
    const FBlendCommandArgs&   info    = *iInfo;
    const FFormatMetrics&  fmt     = info.source->FormatMetrics();
    const uint8*        src     = iSrc;
    uint8*              bdp     = iBdp;

    for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
        const float alpha_src   = TYPE2FLOAT( src, fmt.AID ) * info.opacityValue;
        const float alpha_bdp   = TYPE2FLOAT( bdp, fmt.AID );
        const float alpha_comp  = AlphaNormalF( alpha_src, alpha_bdp );
        const float var         = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            float srcvf = TYPE2FLOAT( src, r );
            float bdpvf = TYPE2FLOAT( bdp, r );
            FLOAT2TYPE( bdp, r, SeparableCompOpF< Blend_Normal >( srcvf, bdpvf, alpha_bdp, var ) );
        }

        FLOAT2TYPE( bdp, fmt.AID, alpha_comp );
        src += fmt.BPP;
        bdp += fmt.BPP;
    }
}

template< typename T >
void
ScheduleAlphaBlendMT_Separable_MEM_Generic( FCommand* iCommand, const FSchedulePolicy& iPolicy, FThreadPool& iPool ) {
    FBlendCommandArgs* args     = dynamic_cast< FBlendCommandArgs* >( iCommand->Args() );
    const uint8* src            = args->source->Bits();
    uint8* bdp                  = args->backdrop->Bits();
    const uint32 src_bps        = args->source->BytesPerScanLine();
    const uint32 bdp_bps        = args->backdrop->BytesPerScanLine();
    const uint32 src_decal_y    = args->shift.y + args->sourceRect.y;
    const uint32 src_decal_x    = ( args->shift.x + args->sourceRect.x ) * args->source->BytesPerPixel();
    const uint32 bdp_decal_x    = ( args->backdropWorkingRect.x ) * args->source->BytesPerPixel();
    if( iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Mono )
    {
        // Mono: Single Job - Multi Tasks
        // Same for both policies: Blend doesn't support chunk based scheduling
        // as of now
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines
        FBlendJobArgs* job_args = new FBlendJobArgs[ args->backdropWorkingRect.h ];
        for( int i = 0; i < args->backdropWorkingRect.h; ++i )
            job_args[i] = FBlendJobArgs(
                  i
                , src_bps
                , src + ( ( src_decal_y + i ) * src_bps ) + src_decal_x
                , bdp + ( ( args->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
            );
        FJob* job = new FJob(
              args->backdropWorkingRect.h
            , InvokeAlphaBlendMTProcessScanline_Separable_MEM_Generic< T >
            , job_args );
        iCommand->AddJob( job );
    }
    else // iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Multi
    {
        // Multi: Multi Jobs - Single Task
        // Same for both policies: Blend doesn't support chunk based scheduling
        // as of now
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines
        for( int i = 0; i < args->backdropWorkingRect.h; ++i ) {
            FBlendJobArgs* job_args = new FBlendJobArgs[ 1 ];
            job_args[0] = FBlendJobArgs(
                  i
                , src_bps
                , src + ( ( src_decal_y + i ) * src_bps ) + src_decal_x
                , bdp + ( ( args->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
            );
            FJob* job = new FJob(
                  1
                , InvokeAlphaBlendMTProcessScanline_Separable_MEM_Generic< T >
                , job_args );
            iCommand->AddJob( job );
        }
    }
}

ULIS_NAMESPACE_END

