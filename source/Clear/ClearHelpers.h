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
template< void (*IMP)( const FClearJobArgs*, const FClearCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildClearJobs( FCommand* iCommand, const FSchedulePolicy& iPolicy ) {
    const FClearCommandArgs* cargs  = dynamic_cast< const FClearCommandArgs* >( iCommand->Args() );
    const FFormatMetrics& fmt       = cargs->block.FormatMetrics();
    uint8* base                     = cargs->block.Bits() + cargs->rect.x * fmt.BPP;
    const int64 bps                 = static_cast< int64 >( cargs->block.BytesPerScanLine() );
    const int64 btt                 = static_cast< int64 >( cargs->block.BytesTotal() );

    if( iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Mono )
        if( iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines )
            goto mono_scanlines;
        else
            if( !( cargs->whole ) )
                goto mono_scanlines;
            else
                goto mono_chunks;
    else
        if( iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines )
            goto multi_scanlines;
        else
            if( !( cargs->whole ) )
                goto multi_scanlines;
            else
                if( iPolicy.ParameterPolicy() == eScheduleParameterPolicy::ScheduleParameter_Count )
                    goto multi_chunks_count;
                else
                    goto multi_chunks_length;

mono_scanlines:
    {
        uint8* buf = new uint8[ cargs->rect.h * sizeof( FClearJobArgs ) ];
        FClearJobArgs* jargs = reinterpret_cast< FClearJobArgs* >( buf );
        for( int i = 0; i < cargs->rect.h; ++i )
            new ( buf + sizeof( FClearJobArgs ) * i ) FClearJobArgs(
                  base + ( cargs->rect.y + i ) * bps
                , bps
            );
        FJob* job = new FJob(
                cargs->rect.h
            , &ResolveScheduledJobCall< FClearJobArgs, FClearCommandArgs, IMP >
            , jargs );
        iCommand->AddJob( job );
        return;
    }

multi_scanlines:
    {
        for( int i = 0; i < cargs->rect.h; ++i ) {
            uint8* buf = new uint8[ sizeof( FClearJobArgs ) ];
            FClearJobArgs* jargs = reinterpret_cast< FClearJobArgs* >( buf );
            new ( buf ) FClearJobArgs(
                  base + ( cargs->rect.y + i ) * bps
                , bps
            );
            FJob* job = new FJob(
                  1
                , &ResolveScheduledJobCall< FClearJobArgs, FClearCommandArgs, IMP >
                , jargs );
            iCommand->AddJob( job );
        }
        return;
    }

mono_chunks:
    {
        uint8* buf = new uint8[ sizeof( FClearJobArgs ) ];
        FClearJobArgs* jargs = reinterpret_cast< FClearJobArgs* >( buf );
        new ( buf ) FClearJobArgs(
              base
            , btt
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FClearJobArgs, FClearCommandArgs, IMP >
            , jargs );
        iCommand->AddJob( job );
        return;
    }

multi_chunks_count:
    {
        const int64 count = FMath::Max( iPolicy.Value(), int64(1) );
        const int64 size = int64( FMath::Ceil( btt / float( count ) ) );
        int64 index = 0;
        for( int i = 0; i < count; ++i ) {
            uint8* buf = new uint8[ sizeof( FClearJobArgs ) ];
            FClearJobArgs* jargs = reinterpret_cast< FClearJobArgs* >( buf );
            new ( buf ) FClearJobArgs(
                  base + index
                , FMath::Min( index + size, btt ) - index
            );
            FJob* job = new FJob(
                  1
                , &ResolveScheduledJobCall< FClearJobArgs, FClearCommandArgs, IMP >
                , jargs );
            iCommand->AddJob( job );
            index += size;
        }
        return;
    }

multi_chunks_length:
    
    {
        const int64 size = FMath::Max( iPolicy.Value(), int64(1) );
        const int64 count = int64( FMath::Ceil( btt / float( size ) ) );
        int64 index = 0;
        for( int i = 0; i < count; ++i ) {
            uint8* buf = new uint8[ sizeof( FClearJobArgs ) ];
            FClearJobArgs* jargs = reinterpret_cast< FClearJobArgs* >( buf );
            new ( buf ) FClearJobArgs(
                  base + index
                , FMath::Min( index + size, btt ) - index
            );
            FJob* job = new FJob(
                  1
                , &ResolveScheduledJobCall< FClearJobArgs, FClearCommandArgs, IMP >
                , jargs );
            iCommand->AddJob( job );
            index += size;
        }
        return;
    }
}

ULIS_NAMESPACE_END

