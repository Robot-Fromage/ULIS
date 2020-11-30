// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Clear.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Clear API.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Clear/Clear.h"
#include "Image/Block.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Job Building
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

template< void (*TDelegateInvoke)( const FClearJobArgs*, const FClearCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildClearJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    const FClearCommandArgs* cargs  = dynamic_cast< const FClearCommandArgs* >( iCommand->Args() );
    const int64 btt                 = static_cast< int64 >( cargs->block.BytesTotal() );
    RangeBasedPolicyScheduleJobs< &BuildClearJobs_Scanlines< TDelegateInvoke >, &BuildClearJobs_Chunks< TDelegateInvoke > >( iCommand, iPolicy, btt, cargs->rect.h, iContiguous );
}

/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
void
InvokeClearMTProcessScanline_AX2(
      const FClearJobArgs* jargs
    , const FClearCommandArgs* cargs
)
{
    __m256i* ULIS_RESTRICT dst = reinterpret_cast< __m256i* >( jargs->dst );
    int64 index = 0;
    for( index = 0; index < jargs->size - 32; index += 32 ) {
        _mm256_storeu_si256( dst, _mm256_setzero_si256() );
        ++dst;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memset( dst, 0, jargs->size - index );
}
#endif // ULIS_COMPILETIME_AVX2_SUPPORT

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- SSE
#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
void
InvokeClearMTProcessScanline_SSE4_2(
      const FClearJobArgs* jargs
    , const FClearCommandArgs* cargs
)
{
    __m128i* ULIS_RESTRICT dst = reinterpret_cast< __m128i* >( jargs->dst );
    int64 index = 0;
    for( index = 0; index < jargs->size - 16; index += 16 ) {
        _mm_storeu_si128( dst, _mm_setzero_si128() );
        ++dst;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 128 bit with SSE and perform a memset instead
    memset( dst, 0, jargs->size - index );
}
#endif // __SE4_2__

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeClearMTProcessScanline_MEM(
      const FClearJobArgs* jargs
    , const FClearCommandArgs* cargs
)
{
    // Full scanline width instead of many BPP clears
    memset( jargs->dst, 0, jargs->size );
}

/////////////////////////////////////////////////////
// Schedulers
void
ScheduleClearMT_AX2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    BuildClearJobs< &InvokeClearMTProcessScanline_AX2 >( iCommand, iPolicy, iContiguous );
}

void
ScheduleClearMT_SSE4_2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    BuildClearJobs< &InvokeClearMTProcessScanline_SSE4_2 >( iCommand, iPolicy, iContiguous );
}

void
ScheduleClearMT_MEM(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    BuildClearJobs< &InvokeClearMTProcessScanline_MEM >( iCommand, iPolicy, iContiguous );
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedClearInvocationSchedulerSelector )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedClearInvocationSchedulerSelector )

ULIS_NAMESPACE_END

