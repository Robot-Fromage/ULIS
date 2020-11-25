// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Copy.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Copy entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Copy/Copy.h"
#include "Image/Block.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Job Building
template< void (*TDelegateInvoke)( const FCopyJobArgs*, const FCopyCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildCopyJobs_Scanlines(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
)
{
    const FCopyCommandArgs* cargs           = dynamic_cast< const FCopyCommandArgs* >( iCommand->Args() );
    const FFormatMetrics& fmt               = cargs->src.FormatMetrics();
    const uint8* const ULIS_RESTRICT src    = cargs->src.Bits() + cargs->srcRect.x * fmt.BPP;
    uint8* const ULIS_RESTRICT dst          = cargs->dst.Bits() + cargs->dstRect.x * fmt.BPP;
    const int64 src_bps                     = static_cast< int64 >( cargs->src.BytesPerScanLine() );
    const int64 dst_bps                     = static_cast< int64 >( cargs->dst.BytesPerScanLine() );
    const int64 size                        = cargs->dstRect.w * fmt.BPP;

    for( int i = 0; i < iNumJobs; ++i )
    {
        uint8* buf = new uint8[ iNumTasksPerJob * sizeof( FCopyJobArgs ) ];
        FCopyJobArgs* jargs = reinterpret_cast< FCopyJobArgs* >( buf );
        for( int i = 0; i < iNumTasksPerJob; ++i )
            new ( buf ) FCopyJobArgs(
              src + ( cargs->srcRect.y + i ) * src_bps
            , dst + ( cargs->dstRect.y + i ) * dst_bps
            , size
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FCopyJobArgs, FCopyCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
    }
}

template< void (*TDelegateInvoke)( const FCopyJobArgs*, const FCopyCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildCopyJobs_Chunks(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iSize
    , const int64 iCount
)
{
    const FCopyCommandArgs* cargs = dynamic_cast< const FCopyCommandArgs* >( iCommand->Args() );
    ULIS_ASSERT( cargs->contiguous, "Bad scheduling, illegal policy if contiguous flag is not set." );
    ULIS_ASSERT( cargs->src.BytesTotal() == cargs->dst.BytesTotal(), "Bad scheduling, chunk policy isn't possible if both blocks have different bytes total" );

    const uint8* const ULIS_RESTRICT src    = cargs->src.Bits();
    uint8* const ULIS_RESTRICT dst          = cargs->dst.Bits();
    const int64 btt                         = static_cast< int64 >( cargs->src.BytesTotal() );

    int64 index = 0;
    for( int i = 0; i < iCount; ++i )
    {
        uint8* buf = new uint8[ sizeof( FCopyJobArgs ) ];
        FCopyJobArgs* jargs = reinterpret_cast< FCopyJobArgs* >( buf );
        new ( buf ) FCopyJobArgs(
              src + index
            , dst + index
            , FMath::Min( index + iSize, btt ) - index
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FCopyJobArgs, FCopyCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
        index += iSize;
    }
    return;
}

template< void (*TDelegateInvoke)( const FCopyJobArgs*, const FCopyCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildCopyJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    const FCopyCommandArgs* cargs   = dynamic_cast< const FCopyCommandArgs* >( iCommand->Args() );
    const int64 btt                 = static_cast< int64 >( cargs->src.BytesTotal() );
    RangeBasedPolicyScheduleJobs< &BuildCopyJobs_Scanlines< TDelegateInvoke >, &BuildCopyJobs_Chunks< TDelegateInvoke > >( iCommand, iPolicy, btt, cargs->dstRect.h, cargs->contiguous );
}

/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
void InvokeCopyMTProcessScanline_AX2(
      const FCopyJobArgs* jargs
    , const FCopyCommandArgs* cargs
)
{
    const __m256i* ULIS_RESTRICT src    = reinterpret_cast< const __m256i* >( jargs->src );
    __m256i* ULIS_RESTRICT dst          = reinterpret_cast< __m256i* >( jargs->dst );
    int64 index = 0;
    for( index = 0; index < jargs->size - 32; index += 32 )
    {
        _mm256_storeu_si256( dst, _mm256_loadu_si256( src ) );
        ++src;
        ++dst;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memcpy( dst, src, jargs->size - index );
}
#endif // ULIS_COMPILETIME_AVX2_SUPPORT

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- SSE
#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
void InvokeCopyMTProcessScanline_SSE4_2(
      const FCopyJobArgs* jargs
    , const FCopyCommandArgs* cargs
)
{
    const __m128i* ULIS_RESTRICT src    = reinterpret_cast< const __m128i* >( jargs->src );
    __m128i* ULIS_RESTRICT dst          = reinterpret_cast< __m128i* >( jargs->dst );
    int64 index = 0;
    for( index = 0; index < jargs->size - 16; index += 16 )
    {
        _mm_storeu_si128( dst, _mm_loadu_si128( src ) );
        ++src;
        ++dst;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memcpy( dst, src, jargs->size - index );
}
#endif // __SE4_2__

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void InvokeCopyMTProcessScanline_MEM(
      const FCopyJobArgs* jargs
    , const FCopyCommandArgs* cargs
)
{
    memcpy( jargs->dst, jargs->src, jargs->size );
}

/////////////////////////////////////////////////////
// Schedulers
void
ScheduleCopyMT_AX2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildCopyJobs< &InvokeCopyMTProcessScanline_AX2 >( iCommand, iPolicy );
}

void
ScheduleCopyMT_SSE4_2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildCopyJobs< &InvokeCopyMTProcessScanline_SSE4_2 >( iCommand, iPolicy );
}

void
ScheduleCopyMT_MEM(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildCopyJobs< &InvokeCopyMTProcessScanline_MEM >( iCommand, iPolicy );
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedCopyInvocationSchedulerSelector )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedCopyInvocationSchedulerSelector )

ULIS_NAMESPACE_END

