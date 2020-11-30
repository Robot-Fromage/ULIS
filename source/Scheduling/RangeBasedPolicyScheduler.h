// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         RangeBasedPolicyScheduler.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the RangeBasedPolicyScheduler tools.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Scanline Job Building
template< void (*TDelegateInvoke)( const FClearJobArgs*, const FClearCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildClearJobs_Scanlines(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
)
{
    const FClearCommandArgs* cargs  = dynamic_cast< const FClearCommandArgs* >( iCommand->Args() );
    const FFormatMetrics& fmt       = cargs->block.FormatMetrics();
    uint8* const ULIS_RESTRICT dst  = cargs->block.Bits() + cargs->rect.x * fmt.BPP;
    const int64 bps                 = static_cast< int64 >( cargs->block.BytesPerScanLine() );
    const int64 size                = cargs->rect.w * fmt.BPP;

    for( int i = 0; i < iNumJobs; ++i )
    {
        uint8* buf = new uint8[ iNumTasksPerJob * sizeof( FClearJobArgs ) ];
        FClearJobArgs* jargs = reinterpret_cast< FClearJobArgs* >( buf );
        for( int i = 0; i < iNumTasksPerJob; ++i )
            new ( buf ) FClearJobArgs(
              dst + ( cargs->rect.y + i ) * bps
            , size
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FClearJobArgs, FClearCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
    }
}

/////////////////////////////////////////////////////
// Chunk Job Building
template< void (*TDelegateInvoke)( const FClearJobArgs*, const FClearCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildClearJobs_Chunks(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iSize
    , const int64 iCount
)
{
    const FClearCommandArgs* cargs  = dynamic_cast< const FClearCommandArgs* >( iCommand->Args() );
    uint8* const ULIS_RESTRICT dst  = cargs->block.Bits();
    const int64 btt                 = static_cast< int64 >( cargs->block.BytesTotal() );

    int64 index = 0;
    for( int i = 0; i < iCount; ++i )
    {
        uint8* buf = new uint8[ sizeof( FClearJobArgs ) ];
        FClearJobArgs* jargs = reinterpret_cast< FClearJobArgs* >( buf );
        new ( buf ) FClearJobArgs(
              dst + index
            , FMath::Min( index + iSize, btt ) - index
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FClearJobArgs, FClearCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
        index += iSize;
    }
    return;
}

/////////////////////////////////////////////////////
// Master Job Building
template<
      void (*TDelegateScanlines)( FCommand*, const FSchedulePolicy&, const int64, const int64 )
    , void (*TDelegateChunks)( FCommand*, const FSchedulePolicy&, const int64, const int64 )
>
ULIS_FORCEINLINE
static
void
RangeBasedPolicyScheduleJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iBytesTotal
    , const int64 iNumScanlines
    , const bool iChunkAllowed
)
{
    if( iPolicy.RunPolicy() == eScheduleRunPolicy::ScheduleRun_Mono )
        if( iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines )
            goto mono_scanlines;
        else
            if( !( iChunkAllowed ) )
                goto mono_scanlines;
            else
                goto mono_chunks;
    else
        if( iPolicy.ModePolicy() == eScheduleModePolicy::ScheduleMode_Scanlines )
            goto multi_scanlines;
        else
            if( !( iChunkAllowed ) )
                goto multi_scanlines;
            else
                if( iPolicy.ParameterPolicy() == eScheduleParameterPolicy::ScheduleParameter_Count )
                    goto multi_chunks_count;
                else
                    goto multi_chunks_length;

mono_scanlines:
    {
        TDelegateScanlines( iCommand, iPolicy, 1, iNumScanlines );
        return;
    }

multi_scanlines:
    {
        TDelegateScanlines( iCommand, iPolicy, iNumScanlines, 1 );
        return;
    }

mono_chunks:
    {
        TDelegateChunks( iCommand, iPolicy, iBytesTotal, 1 );
        return;
    }

multi_chunks_count:
    {
        const int64 count = FMath::Max( iPolicy.Value(), int64(1) );
        const int64 size = int64( FMath::Ceil( iBytesTotal / float( count ) ) );
        TDelegateChunks( iCommand, iPolicy, size, count );
        return;
    }

multi_chunks_length:
    {
        const int64 size = FMath::Max( iPolicy.Value(), int64(1) );
        const int64 count = int64( FMath::Ceil( iBytesTotal / float( size ) ) );
        TDelegateChunks( iCommand, iPolicy, size, count );
        return;
    }
}

ULIS_NAMESPACE_END

