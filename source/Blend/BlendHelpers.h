// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BlendHelpers.h
* @author       Clement Berthaud
* @brief        This file provides some helper macros and functions for blending.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/BlendArgs.h"
#include "Scheduling/SchedulePolicy.h"
#include "Scheduling/Command.h"
#include "Scheduling/Job.h"

/////////////////////////////////////////////////////
// Macro Helpers for Redundant Compositing Operations
// TODO: this is quite clumsy at the moment, clean this by defining better
// macros for ASSIGN and small helper functions, possibly inline.
#define SampleSubpixelAlpha( _DST )                                                                                     \
    if( fmt.HEA ) {                                                                                                     \
        m11 = ( notLastCol && notLastLine )                     ? TYPE2FLOAT( src,                  fmt.AID ) : 0.f;    \
        m10 = ( notLastCol && ( notFirstLine || hasTopData ) )  ? TYPE2FLOAT( src - jargs->src_bps, fmt.AID ) : 0.f;    \
    } else {                                                                                                            \
        m11 = ( notLastCol && notLastLine )     ? 1.f : 0.f;                                                            \
        m10 = ( notLastCol && notFirstLine )    ? 1.f : 0.f;                                                            \
    }                                                                                                                   \
    vv1     = m10 * cargs->subpixelComponent.y + m11 * cargs->buspixelComponent.y;                                      \
    _DST    = vv0 * cargs->subpixelComponent.x + vv1 * cargs->buspixelComponent.x;

#define SampleSubpixelChannel( _DST, _CHAN )                                                                                                \
    s11 = ( notLastCol  && notLastLine )                                    ?   TYPE2FLOAT( src,                            _CHAN ) : 0.f;  \
    s01 = ( notLastLine && ( x > 0 || hasLeftData ) )                       ?   TYPE2FLOAT( src - fmt.BPP,                  _CHAN ) : 0.f;  \
    s10 = ( notLastCol && ( notFirstLine || hasTopData ) )                  ?   TYPE2FLOAT( src - jargs->src_bps,           _CHAN ) : 0.f;  \
    s00 = ( ( x > 0 || hasLeftData ) && ( notFirstLine || hasTopData ) )    ?   TYPE2FLOAT( src - jargs->src_bps - fmt.BPP, _CHAN ) : 0.f;  \
    v1 = ( s00 * m00 ) * cargs->subpixelComponent.y + ( s01 * m01 ) * cargs->buspixelComponent.y;                                           \
    v2 = ( s10 * m10 ) * cargs->subpixelComponent.y + ( s11 * m11 ) * cargs->buspixelComponent.y;                                           \
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
ULIS_FORCEINLINE
static
Vec4i
BuildRGBA8IndexTable( uint8 iRS )
{
    Vec4i result;
    switch( iRS ) {
        case 1:  for( int i = 0; i < 4; ++i ) result.insert( i, ( 3 - i )                             ); return  result;
        case 2:  for( int i = 0; i < 4; ++i ) result.insert( i, ( i + 1 ) > 3 ? 0 : i + 1             ); return  result;
        case 3:  for( int i = 0; i < 4; ++i ) result.insert( i, ( 3 - i ) - 1 < 0 ? 3 : ( 3 - i ) - 1 ); return  result;
        default: for( int i = 0; i < 4; ++i ) result.insert( i, i                                     ); return  result;
    }
}

ULIS_FORCEINLINE
static
const uint8*
ComputeBufferPosition(
      const uint8* iBase
    , const int iSourceRectY
    , const int iShiftY
    , const int iSourceRectH
    , const uint32 iSrcBps
    , const uint32 iSrcDecalX
    , const uint32 iSrcDecalY
    , const int iLine
    , const bool iTiled
)
{
    if( iTiled ) {
        return  iBase + ( ( iSourceRectY + ( ( iShiftY + iLine ) % iSourceRectH ) ) * iSrcBps ) + iSrcDecalX;
    } else {
        return  iBase + ( ( iSrcDecalY + iLine ) * iSrcBps ) + iSrcDecalX;
    }
}

template< void (*IMP)( const FBlendJobArgs*, const FBlendCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildBlendJobs( FCommand* iCommand, const FSchedulePolicy& iPolicy, const bool iTiled = false ) {
    const FBlendCommandArgs* cargs              = dynamic_cast< const FBlendCommandArgs* >( iCommand->Args() );
    const uint8* src                            = cargs->source.Bits();
    uint8* bdp                                  = cargs->backdrop.Bits();
    const uint32 src_bps                        = cargs->source.BytesPerScanLine();
    const uint32 bdp_bps                        = cargs->backdrop.BytesPerScanLine();
    const uint32 src_decal_y                    = cargs->shift.y + cargs->sourceRect.y;
    const uint32 src_decal_x                    = ( cargs->shift.x + cargs->sourceRect.x ) * cargs->source.BytesPerPixel();
    const uint32 bdp_decal_x                    = ( cargs->backdropWorkingRect.x ) * cargs->source.BytesPerPixel();
    const FFormatMetrics& fmt                   = cargs->source.FormatMetrics();
    fpConvertFormat conv_forward_fptr    = QueryDispatchedConvertFormatInvocation( fmt.FMT, eFormat::Format_RGBF );
    fpConvertFormat conv_backward_fptr   = QueryDispatchedConvertFormatInvocation( eFormat::Format_RGBF, fmt.FMT );
    Vec4i idt                                   = BuildRGBA8IndexTable( cargs->source.FormatMetrics().RSC );
    if( iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Mono )
    {
        uint8* buf = new uint8[ cargs->backdropWorkingRect.h * sizeof( FBlendJobArgs ) ];
        FBlendJobArgs* jargs = reinterpret_cast< FBlendJobArgs* >( buf );
        for( int i = 0; i < cargs->backdropWorkingRect.h; ++i )
            new ( buf + sizeof( FBlendJobArgs ) * i ) FBlendJobArgs(
                  i
                , src_bps
                , ComputeBufferPosition( src, cargs->sourceRect.y, cargs->shift.y, cargs->sourceRect.h, src_bps, src_decal_x, src_decal_y, i, iTiled )
                , bdp + ( ( cargs->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
                , conv_forward_fptr
                , conv_backward_fptr
                , idt
            );
        FJob* job = new FJob(
              cargs->backdropWorkingRect.h
            , &ResolveScheduledJobCall< FBlendJobArgs, FBlendCommandArgs, IMP >
            , jargs );
        iCommand->AddJob( job );
    }
    else // iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Multi
    {
        for( int i = 0; i < cargs->backdropWorkingRect.h; ++i ) {
            uint8* buf = new uint8[ sizeof( FBlendJobArgs ) ];
            FBlendJobArgs* jargs = reinterpret_cast< FBlendJobArgs* >( buf );
            new ( buf ) FBlendJobArgs(
                  i
                , src_bps
                , ComputeBufferPosition( src, cargs->sourceRect.y, cargs->shift.y, cargs->sourceRect.h, src_bps, src_decal_x, src_decal_y, i, iTiled )
                , bdp + ( ( cargs->backdropWorkingRect.y + i ) * bdp_bps ) + bdp_decal_x
                , conv_forward_fptr
                , conv_backward_fptr
                , idt
            );
            FJob* job = new FJob(
                  1
                , &ResolveScheduledJobCall< FBlendJobArgs, FBlendCommandArgs, IMP >
                , jargs );
            iCommand->AddJob( job );
        }
    }
}

ULIS_NAMESPACE_END

