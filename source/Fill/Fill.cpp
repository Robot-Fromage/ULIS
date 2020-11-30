// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Fill.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Fill entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Fill/Fill.h"
#include "Image/Block.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Job Building
template< void (*TDelegateInvoke)( const FFillJobArgs*, const FFillCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildFillJobs_Scanlines(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
)
{
    const FFillCommandArgs* cargs  = dynamic_cast< const FFillCommandArgs* >( iCommand->Args() );
    const FFormatMetrics& fmt       = cargs->block.FormatMetrics();
    uint8* const ULIS_RESTRICT dst  = cargs->block.Bits() + cargs->rect.x * fmt.BPP;
    const int64 bps                 = static_cast< int64 >( cargs->block.BytesPerScanLine() );
    const int64 size                = cargs->rect.w * fmt.BPP;

    for( int i = 0; i < iNumJobs; ++i )
    {
        uint8* buf = new uint8[ iNumTasksPerJob * sizeof( FFillJobArgs ) ];
        FFillJobArgs* jargs = reinterpret_cast< FFillJobArgs* >( buf );
        for( int i = 0; i < iNumTasksPerJob; ++i )
            new ( buf ) FFillJobArgs(
              dst + ( cargs->rect.y + i ) * bps
            , size
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FFillJobArgs, FFillCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
    }
}

template< void (*TDelegateInvoke)( const FFillJobArgs*, const FFillCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildFillJobs_Chunks(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iSize
    , const int64 iCount
)
{
    const FFillCommandArgs* cargs  = dynamic_cast< const FFillCommandArgs* >( iCommand->Args() );
    uint8* const ULIS_RESTRICT dst  = cargs->block.Bits();
    const int64 btt                 = static_cast< int64 >( cargs->block.BytesTotal() );

    int64 index = 0;
    for( int i = 0; i < iCount; ++i )
    {
        uint8* buf = new uint8[ sizeof( FFillJobArgs ) ];
        FFillJobArgs* jargs = reinterpret_cast< FFillJobArgs* >( buf );
        new ( buf ) FFillJobArgs(
              dst + index
            , FMath::Min( index + iSize, btt ) - index
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FFillJobArgs, FFillCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
        index += iSize;
    }
    return;
}

template< void (*TDelegateInvoke)( const FFillJobArgs*, const FFillCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildFillJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    const FFillCommandArgs* cargs   = dynamic_cast< const FFillCommandArgs* >( iCommand->Args() );
    const int64 btt                 = static_cast< int64 >( cargs->block.BytesTotal() );
    RangeBasedPolicyScheduleJobs< &BuildFillJobs_Scanlines< TDelegateInvoke >, &BuildFillJobs_Chunks< TDelegateInvoke > >( iCommand, iPolicy, btt, cargs->rect.h, cargs->contiguous );
}

/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
void
InvokeFillMTProcessScanline_AX2(
      const FFillJobArgs* jargs
    , const FFillCommandArgs* cargs
)
{
    __m256i src = _mm256_lddqu_si256( reinterpret_cast< const __m256i* >( cargs->buffer ) );
    __m256i* ULIS_RESTRICT dst = reinterpret_cast< __m256i* >( jargs->dst );
    int64 index = 0;
    for( index = 0; index < jargs->size - 32; index += 32 ) {
        _mm256_storeu_si256( dst, src );
        ++dst;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memcpy( dst, cargs->buffer, jargs->size - index );
}
#endif // ULIS_COMPILETIME_AVX2_SUPPORT

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- SSE
#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
void
InvokeFillMTProcessScanline_SSE4_2(
      const FFillJobArgs* jargs
    , const FFillCommandArgs* cargs
)
{
    __m128i src = _mm_lddqu_si128( reinterpret_cast< const __m128i* >( cargs->buffer ) );
    __m128i* ULIS_RESTRICT dst = reinterpret_cast< __m128i* >( jargs->dst );
    int64 index = 0;
    for( index = 0; index < jargs->size - 16; index += 16 ) {
        _mm_storeu_si128( dst, _mm_setzero_si128() );
        ++dst;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 128 bit with SSE and perform a memset instead
    memcpy( dst, cargs->buffer, jargs->size - index );
}
#endif // __SE4_2__

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeFillMTProcessScanline_MEM(
      const FFillJobArgs* jargs
    , const FFillCommandArgs* cargs
)
{
    const uint8* src = cargs->buffer;
    const uint8 stride = cargs->block.BytesPerPixel();
    uint8* ULIS_RESTRICT dst = jargs->dst;
    for( uint32 i = 0; i < jargs->size; ++i ) {
        memcpy( dst, src, stride );
        dst += stride;
    }
}

/////////////////////////////////////////////////////
// Schedulers
void
ScheduleFillMT_AX2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    const FFillCommandArgs* cargs = dynamic_cast< const FFillCommandArgs* >( iCommand->Args() );
    const uint8 bpp = cargs->block.BytesPerPixel();
    const uint32 bps = cargs->block.BytesPerScanLine();

    if( bpp <= 32 && bps >= 32 ) {
        BuildFillJobs< &InvokeFillMTProcessScanline_AX2 >( iCommand, iPolicy );
    } else if( bpp <= 16 && bps >= 16 ) {
        BuildFillJobs< &InvokeFillMTProcessScanline_SSE4_2 >( iCommand, iPolicy );
    } else {
        BuildFillJobs< &InvokeFillMTProcessScanline_MEM >( iCommand, iPolicy );
    }
}

void
ScheduleFillMT_SSE4_2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    const FFillCommandArgs* cargs = dynamic_cast< const FFillCommandArgs* >( iCommand->Args() );
    const uint8 bpp = cargs->block.BytesPerPixel();
    const uint32 bps = cargs->block.BytesPerScanLine();
    if( bpp <= 16 && bps >= 16 ) {
        BuildFillJobs< &InvokeFillMTProcessScanline_SSE4_2 >( iCommand, iPolicy );
    } else {
        BuildFillJobs< &InvokeFillMTProcessScanline_MEM >( iCommand, iPolicy );
    }
}

void
ScheduleFillMT_MEM(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    const FFillCommandArgs* cargs = dynamic_cast< const FFillCommandArgs* >( iCommand->Args() );
    const uint8 bpp = cargs->block.BytesPerPixel();
    const uint32 bps = cargs->block.BytesPerScanLine();
    BuildFillJobs< &InvokeFillMTProcessScanline_MEM >( iCommand, iPolicy );
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedFillInvocationSchedulerSelector )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedFillInvocationSchedulerSelector )

ULIS_NAMESPACE_END

