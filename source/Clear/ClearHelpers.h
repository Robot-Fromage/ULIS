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
BuildClearJobs( FCommand* iCommand, const FSchedulePolicy& iPolicy ) {
    const FClearCommandArgs* cargs  = dynamic_cast< const FClearCommandArgs* >( iCommand->Args() );
    uint8* dst                      = cargs->block.Bits();
    const uint32 bps                = cargs->block.BytesPerScanLine();
    const uint32 src_decal_x        = cargs->rect.x * cargs->block.BytesPerPixel();

    if( iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Mono )
    {
        if( iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines )
        {
        }
        else // iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        {
            if( iPolicy.ParameterPolicy() == eScheduleParameterPolicy::ScheduleParameter_Count )
            {
            }
            else // iPolicy.ParameterPolicy() == eScheduleParameterPolicy::ScheduleParameter_Length
            {
            }
        }
    }
    else // iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Multi
    {
        if( iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines )
        {
        }
        else // iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Chunks
        {
            if( iPolicy.ParameterPolicy() == eScheduleParameterPolicy::ScheduleParameter_Count )
            {
            }
            else // iPolicy.ParameterPolicy() == eScheduleParameterPolicy::ScheduleParameter_Length
            {
            }
        }
    }

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

/////////////////////////////////////////////////////
// Implementation
void Clear_imp( FOldThreadPool*            iOldThreadPool
              , bool                    iBlocking
              , uint32                  iPerfIntent
              , const FHardwareMetrics&  iHostDeviceInfo
              , bool                    iCallCB
              , FBlock*                 iDestination
              , const FRectI&            iArea )
{
    const FFormatMetrics&  fmt     = iDestination->FormatMetrics();
    const uint32         bpp     = fmt.BPP;
    const uint32         w       = iDestination->Width();
    const uint32         bps     = iDestination->BytesPerScanLine();
    const uint32         dsh     = iArea.x * bpp;
    uint8*              dsb     = iDestination->Bits() + dsh;
    const uint32         count   = iArea.w * bpp;
    #define DST dsb + ( ( iArea.y + static_cast< int64 >( pLINE ) ) * static_cast< int64 >( bps ) )

    #ifdef ULIS_COMPILETIME_AVX2_SUPPORT
    if( ( iPerfIntent & ULIS_PERF_AVX2 ) && iHostDeviceInfo.HW_AVX2 && bps >= 32 ) {
        const uint32 stride = 32;
        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_AX2, DST, count, stride )
    } else
    #endif
    #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
    if( ( iPerfIntent & ULIS_PERF_SSE42 ) && iHostDeviceInfo.HW_SSE42 && bps >= 16 ) {
        const uint32 stride = 16;
        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_SSE4_2, DST, count, stride )
    } else
    #endif
    {
        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_MEM, DST, count, bpp )
    }
}

ULIS_NAMESPACE_END

