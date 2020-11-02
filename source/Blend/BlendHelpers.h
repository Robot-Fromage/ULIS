// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         BlendHelpers.h
* @author       Clement Berthaud
* @brief        This file provides some helper macros and functions for blending.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "BlendArgs.h"

/////////////////////////////////////////////////////
// Static Values Helpers for Misc Compositing Operations
static const float gBayer8x8Matrix[8][8] = {
    { 0.015625f, 0.765625f, 0.203125f, 0.953125f, 0.0625f, 0.8125f, 0.25f, 1.0f          },
    { 0.515625f, 0.265625f, 0.703125f, 0.453125f, 0.5625f, 0.3125f, 0.75f, 0.5f          },
    { 0.140625f, 0.890625f, 0.078125f, 0.828125f, 0.1875f, 0.9375f, 0.125f, 0.875f       },
    { 0.640625f, 0.390625f, 0.578125f, 0.328125f, 0.6875f, 0.4375f, 0.625f, 0.375f       },
    { 0.046875f, 0.796875f, 0.234375f, 0.984375f, 0.03125f, 0.78125f, 0.21875f, 0.96875f },
    { 0.546875f, 0.296875f, 0.734375f, 0.484375f, 0.53125f, 0.28125f, 0.71875f, 0.46875f },
    { 0.171875f, 0.921875f, 0.109375f, 0.859375f, 0.15625f, 0.90625f, 0.09375f, 0.84375f },
    { 0.671875f, 0.421875f, 0.609375f, 0.359375f, 0.65625f, 0.40625f, 0.59375f, 0.34375f } };

/////////////////////////////////////////////////////
// Macro Helpers for Redundant Compositing Operations
// TODO: this is quite clumsy at the moment, clean this by defining better
// macros for ASSIGN and small helper functions, possibly inline.
#define SampleSubpixelAlpha( _DST )                                                                                 \
    if( fmt.HEA ) {                                                                                                 \
        m11 = ( notLastCol && notLastLine )                     ? TYPE2FLOAT( src,              fmt.AID ) : 0.f;    \
        m10 = ( notLastCol && ( notFirstLine || hasTopData ) )  ? TYPE2FLOAT( src - iSrcBps,    fmt.AID ) : 0.f;    \
    } else {                                                                                                        \
        m11 = ( notLastCol && notLastLine )     ? 1.f : 0.f;                                                        \
        m10 = ( notLastCol && notFirstLine )    ? 1.f : 0.f;                                                        \
    }                                                                                                               \
    vv1     = m10 * cargs->subpixelComponent.y + m11 * cargs->buspixelComponent.y;                                  \
    _DST    = vv0 * cargs->subpixelComponent.x + vv1 * cargs->buspixelComponent.x;

#define SampleSubpixelChannel( _DST, _CHAN )                                                                                            \
    s11 = ( notLastCol  && notLastLine )                                    ?   TYPE2FLOAT( src,                        _CHAN ) : 0.f;  \
    s01 = ( notLastLine && ( x > 0 || hasLeftData ) )                       ?   TYPE2FLOAT( src - fmt.BPP,              _CHAN ) : 0.f;  \
    s10 = ( notLastCol && ( notFirstLine || hasTopData ) )                  ?   TYPE2FLOAT( src - iSrcBps,              _CHAN ) : 0.f;  \
    s00 = ( ( x > 0 || hasLeftData ) && ( notFirstLine || hasTopData ) )    ?   TYPE2FLOAT( src - iSrcBps - fmt.BPP,    _CHAN ) : 0.f;  \
    v1 = ( s00 * m00 ) * cargs->subpixelComponent.y + ( s01 * m01 ) * cargs->buspixelComponent.y;                                       \
    v2 = ( s10 * m10 ) * cargs->subpixelComponent.y + ( s11 * m11 ) * cargs->buspixelComponent.y;                                       \
    _DST = res == 0.f ? 0.f : ( ( v1 ) * cargs->subpixelComponent.x + ( v2 ) * cargs->buspixelComponent.x ) / res;

#define ULIS_ACTION_ASSIGN_ALPHAF( _AM, iTarget, iSrc, iBdp )      iTarget = AlphaF< _AM >( iSrc, iBdp );
#define ULIS_ACTION_ASSIGN_ALPHASSEF( _AM, iTarget, iSrc, iBdp )   iTarget = AlphaSSEF< _AM >( iSrc, iBdp );
#define ULIS_ACTION_ASSIGN_ALPHAAVXF( _AM, iTarget, iSrc, iBdp )   iTarget = AlphaAVXF< _AM >( iSrc, iBdp );
#define ULIS_ASSIGN_ALPHAF( iAlphaMode, iTarget, iSrc, iBdp )      ULIS_SWITCH_FOR_ALL_DO( iAlphaMode, ULIS_FOR_ALL_AM_DO, ULIS_ACTION_ASSIGN_ALPHAF, iTarget, iSrc, iBdp )
#define ULIS_ASSIGN_ALPHASSEF( iAlphaMode, iTarget, iSrc, iBdp )   ULIS_SWITCH_FOR_ALL_DO( iAlphaMode, ULIS_FOR_ALL_AM_DO, ULIS_ACTION_ASSIGN_ALPHASSEF, iTarget, iSrc, iBdp )
#define ULIS_ASSIGN_ALPHAAVXF( iAlphaMode, iTarget, iSrc, iBdp )   ULIS_SWITCH_FOR_ALL_DO( iAlphaMode, ULIS_FOR_ALL_AM_DO, ULIS_ACTION_ASSIGN_ALPHAAVXF, iTarget, iSrc, iBdp )

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Helper for Redundant Job building and scheduling operations
template< void (*IMP)( const FBlendJobArgs_Separable_MEM_Generic*, const FBlendCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildBlendJobs_Separable_MEM_Generic( FCommand* iCommand, const FSchedulePolicy& iPolicy ) {
    const FBlendCommandArgs* cargs = dynamic_cast< const FBlendCommandArgs* >( iCommand->Args() );
    const uint8* src            = cargs->source.Bits();
    uint8* bdp                  = cargs->backdrop.Bits();
    const uint32 src_bps        = cargs->source.BytesPerScanLine();
    const uint32 bdp_bps        = cargs->backdrop.BytesPerScanLine();
    const uint32 src_decal_y    = cargs->shift.y + cargs->sourceRect.y;
    const uint32 src_decal_x    = ( cargs->shift.x + cargs->sourceRect.x ) * cargs->source.BytesPerPixel();
    const uint32 bdp_decal_x    = ( cargs->backdropWorkingRect.x ) * cargs->source.BytesPerPixel();
    if( iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Mono )
    {
        // Mono: Single Job - Multi Tasks
        // Same for both policies: Blend doesn't support chunk based scheduling
        // as of now
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines
        FBlendJobArgs_Separable_MEM_Generic* jargs = new FBlendJobArgs_Separable_MEM_Generic[ cargs->backdropWorkingRect.h ];
        for( int i = 0; i < cargs->backdropWorkingRect.h; ++i )
            jargs[i] = FBlendJobArgs_Separable_MEM_Generic(
                  i
                , src_bps
                , src + ( ( src_decal_y + i ) * src_bps ) + src_decal_x
                , bdp + ( ( cargs->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
            );
        FJob* job = new FJob(
              cargs->backdropWorkingRect.h
            , &ResolveScheduledJobCall< FBlendJobArgs_Separable_MEM_Generic, FBlendCommandArgs, IMP >
            , jargs );
        iCommand->AddJob( job );
    }
    else // iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Multi
    {
        // Multi: Multi Jobs - Single Task
        // Same for both policies: Blend doesn't support chunk based scheduling
        // as of now
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines
        for( int i = 0; i < cargs->backdropWorkingRect.h; ++i ) {
            FBlendJobArgs_Separable_MEM_Generic* jargs = new FBlendJobArgs_Separable_MEM_Generic[ 1 ];
            jargs[0] = FBlendJobArgs_Separable_MEM_Generic(
                  i
                , src_bps
                , src + ( ( src_decal_y + i ) * src_bps ) + src_decal_x
                , bdp + ( ( cargs->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
            );
            FJob* job = new FJob(
                  1
                , &ResolveScheduledJobCall< FBlendJobArgs_Separable_MEM_Generic, FBlendCommandArgs, IMP >
                , jargs );
            iCommand->AddJob( job );
        }
    }
}

template< void (*IMP)( const FBlendJobArgs_NonSeparable_MEM_Generic*, const FBlendCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildBlendJobs_NonSeparable_MEM_Generic( FCommand* iCommand, const FSchedulePolicy& iPolicy ) {
    const FBlendCommandArgs* cargs = dynamic_cast< const FBlendCommandArgs* >( iCommand->Args() );
    const uint8* src            = cargs->source.Bits();
    uint8* bdp                  = cargs->backdrop.Bits();
    const uint32 src_bps        = cargs->source.BytesPerScanLine();
    const uint32 bdp_bps        = cargs->backdrop.BytesPerScanLine();
    const uint32 src_decal_y    = cargs->shift.y + cargs->sourceRect.y;
    const uint32 src_decal_x    = ( cargs->shift.x + cargs->sourceRect.x ) * cargs->source.BytesPerPixel();
    const uint32 bdp_decal_x    = ( cargs->backdropWorkingRect.x ) * cargs->source.BytesPerPixel();
    const FFormatMetrics& fmt   = cargs->source.FormatMetrics();
    fpConversionInvocation conv_forward_fptr  = QueryDispatchedConversionInvocation( fmt.FMT, eFormat::Format_RGBF );
    fpConversionInvocation conv_backward_fptr = QueryDispatchedConversionInvocation( eFormat::Format_RGBF, fmt.FMT );
    if( iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Mono )
    {
        // Mono: Single Job - Multi Tasks
        // Same for both policies: Blend doesn't support chunk based scheduling
        // as of now
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines
        FBlendJobArgs_NonSeparable_MEM_Generic* jargs = new FBlendJobArgs_NonSeparable_MEM_Generic[ cargs->backdropWorkingRect.h ];
        for( int i = 0; i < cargs->backdropWorkingRect.h; ++i )
            jargs[i] = FBlendJobArgs_NonSeparable_MEM_Generic(
                  i
                , src_bps
                , src + ( ( src_decal_y + i ) * src_bps ) + src_decal_x
                , bdp + ( ( cargs->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
                , conv_forward_fptr
                , conv_backward_fptr
            );
        FJob* job = new FJob(
              cargs->backdropWorkingRect.h
            , &ResolveScheduledJobCall< FBlendJobArgs_NonSeparable_MEM_Generic, FBlendCommandArgs, IMP >
            , jargs );
        iCommand->AddJob( job );
    }
    else // iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Multi
    {
        // Multi: Multi Jobs - Single Task
        // Same for both policies: Blend doesn't support chunk based scheduling
        // as of now
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines
        for( int i = 0; i < cargs->backdropWorkingRect.h; ++i ) {
            FBlendJobArgs_NonSeparable_MEM_Generic* jargs = new FBlendJobArgs_NonSeparable_MEM_Generic[ 1 ];
            jargs[0] = FBlendJobArgs_NonSeparable_MEM_Generic(
                  i
                , src_bps
                , src + ( ( src_decal_y + i ) * src_bps ) + src_decal_x
                , bdp + ( ( cargs->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
                , conv_forward_fptr
                , conv_backward_fptr
            );
            FJob* job = new FJob(
                  1
                , &ResolveScheduledJobCall< FBlendJobArgs_NonSeparable_MEM_Generic, FBlendCommandArgs, IMP >
                , jargs );
            iCommand->AddJob( job );
        }
    }
}

template< void (*IMP)( const FBlendJobArgs_Separable_MEM_Generic*, const FBlendCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildTiledBlendJobs_Separable_MEM_Generic( FCommand* iCommand, const FSchedulePolicy& iPolicy ) {
    const FBlendCommandArgs* cargs = dynamic_cast< const FBlendCommandArgs* >( iCommand->Args() );
    const uint8* src            = cargs->source.Bits();
    uint8* bdp                  = cargs->backdrop.Bits();
    const uint32 src_bps        = cargs->source.BytesPerScanLine();
    const uint32 bdp_bps        = cargs->backdrop.BytesPerScanLine();
    const uint32 src_decal_y    = cargs->shift.y + cargs->sourceRect.y;
    const uint32 src_decal_x    = ( cargs->shift.x + cargs->sourceRect.x ) * cargs->source.BytesPerPixel();
    const uint32 bdp_decal_x    = ( cargs->backdropWorkingRect.x ) * cargs->source.BytesPerPixel();
    if( iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Mono )
    {
        // Mono: Single Job - Multi Tasks
        // Same for both policies: Blend doesn't support chunk based scheduling
        // as of now
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines
        FBlendJobArgs_Separable_MEM_Generic* jargs = new FBlendJobArgs_Separable_MEM_Generic[ cargs->backdropWorkingRect.h ];
        for( int i = 0; i < cargs->backdropWorkingRect.h; ++i )
            jargs[i] = FBlendJobArgs_Separable_MEM_Generic(
                  i
                , src_bps
                , src + ( ( cargs->sourceRect.y + ( ( cargs->shift.y + i ) % cargs->sourceRect.h ) ) * src_bps ) + src_decal_x
                , bdp + ( ( cargs->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
            );
        FJob* job = new FJob(
              cargs->backdropWorkingRect.h
            , &ResolveScheduledJobCall< FBlendJobArgs_Separable_MEM_Generic, FBlendCommandArgs, IMP >
            , jargs );
        iCommand->AddJob( job );
    }
    else // iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Multi
    {
        // Multi: Multi Jobs - Single Task
        // Same for both policies: Blend doesn't support chunk based scheduling
        // as of now
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines
        for( int i = 0; i < cargs->backdropWorkingRect.h; ++i ) {
            FBlendJobArgs_Separable_MEM_Generic* jargs = new FBlendJobArgs_Separable_MEM_Generic[ 1 ];
            jargs[0] = FBlendJobArgs_Separable_MEM_Generic(
                  i
                , src_bps
                , src + ( ( cargs->sourceRect.y + ( ( cargs->shift.y + i ) % cargs->sourceRect.h ) ) * src_bps ) + src_decal_x
                , bdp + ( ( cargs->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
            );
            FJob* job = new FJob(
                  1
                , &ResolveScheduledJobCall< FBlendJobArgs_Separable_MEM_Generic, FBlendCommandArgs, IMP >
                , jargs );
            iCommand->AddJob( job );
        }
    }
}

template< void (*IMP)( const FBlendJobArgs_NonSeparable_MEM_Generic*, const FBlendCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildTiledBlendJobs_NonSeparable_MEM_Generic( FCommand* iCommand, const FSchedulePolicy& iPolicy ) {
    const FBlendCommandArgs* cargs = dynamic_cast< const FBlendCommandArgs* >( iCommand->Args() );
    const uint8* src            = cargs->source.Bits();
    uint8* bdp                  = cargs->backdrop.Bits();
    const uint32 src_bps        = cargs->source.BytesPerScanLine();
    const uint32 bdp_bps        = cargs->backdrop.BytesPerScanLine();
    const uint32 src_decal_y    = cargs->shift.y + cargs->sourceRect.y;
    const uint32 src_decal_x    = ( cargs->shift.x + cargs->sourceRect.x ) * cargs->source.BytesPerPixel();
    const uint32 bdp_decal_x    = ( cargs->backdropWorkingRect.x ) * cargs->source.BytesPerPixel();
    fpConversionInvocation conv_forward_fptr  = QueryDispatchedConversionInvocation( fmt.FMT, eFormat::Format_RGBF );
    fpConversionInvocation conv_backward_fptr = QueryDispatchedConversionInvocation( eFormat::Format_RGBF, fmt.FMT );
    if( iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Mono )
    {
        // Mono: Single Job - Multi Tasks
        // Same for both policies: Blend doesn't support chunk based scheduling
        // as of now
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines
        FBlendJobArgs_NonSeparable_MEM_Generic* jargs = new FBlendJobArgs_NonSeparable_MEM_Generic[ cargs->backdropWorkingRect.h ];
        for( int i = 0; i < cargs->backdropWorkingRect.h; ++i )
            jargs[i] = FBlendJobArgs_NonSeparable_MEM_Generic(
                  i
                , src_bps
                , src + ( ( cargs->sourceRect.y + ( ( cargs->shift.y + i ) % cargs->sourceRect.h ) ) * src_bps ) + src_decal_x
                , bdp + ( ( cargs->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
                , conv_forward_fptr
                , conv_backward_fptr
            );
        FJob* job = new FJob(
              cargs->backdropWorkingRect.h
            , &ResolveScheduledJobCall< FBlendJobArgs_NonSeparable_MEM_Generic, FBlendCommandArgs, IMP >
            , jargs );
        iCommand->AddJob( job );
    }
    else // iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Multi
    {
        // Multi: Multi Jobs - Single Task
        // Same for both policies: Blend doesn't support chunk based scheduling
        // as of now
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        //iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines
        for( int i = 0; i < cargs->backdropWorkingRect.h; ++i ) {
            FBlendJobArgs_NonSeparable_MEM_Generic* jargs = new FBlendJobArgs_NonSeparable_MEM_Generic[ 1 ];
            jargs[0] = FBlendJobArgs_NonSeparable_MEM_Generic(
                  i
                , src_bps
                , src + ( ( cargs->sourceRect.y + ( ( cargs->shift.y + i ) % cargs->sourceRect.h ) ) * src_bps ) + src_decal_x
                , bdp + ( ( cargs->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
                , conv_forward_fptr
                , conv_backward_fptr
            );
            FJob* job = new FJob(
                  1
                , &ResolveScheduledJobCall< FBlendJobArgs_NonSeparable_MEM_Generic, FBlendCommandArgs, IMP >
                , jargs );
            iCommand->AddJob( job );
        }
    }
}

ULIS_NAMESPACE_END

