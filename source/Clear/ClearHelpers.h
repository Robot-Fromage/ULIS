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
#include "Clear/ClearArgs.h"
#include "Scheduling/SchedulePolicy.h"
#include "Scheduling/Command.h"
#include "Scheduling/Job.h"

ULIS_NAMESPACE_BEGIN
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

template< void (*IMP)( const FClearJobArgs*, const FClearCommandArgs* ) >
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
    fpConversionInvocation conv_forward_fptr    = QueryDispatchedConversionInvocation( fmt.FMT, eFormat::Format_RGBF );
    fpConversionInvocation conv_backward_fptr   = QueryDispatchedConversionInvocation( eFormat::Format_RGBF, fmt.FMT );
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

