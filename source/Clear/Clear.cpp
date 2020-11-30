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
void
BuildClearJob_Scanlines(
      const FClearCommandArgs* iCargs
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
    , const int64 iIndex
    , FClearJobArgs& iJargs
)
{
    const FFormatMetrics& fmt       = iCargs->block.FormatMetrics();
    uint8* const ULIS_RESTRICT dst  = iCargs->block.Bits() + iCargs->rect.x * fmt.BPP;
    const int64 bps                 = static_cast< int64 >( iCargs->block.BytesPerScanLine() );
    const int64 size                = iCargs->rect.w * fmt.BPP;
    iJargs.dst = dst + ( iCargs->rect.y + iIndex ) * bps;
    iJargs.size = size;
}

void
BuildClearJob_Chunks(
      const FClearCommandArgs* iCargs
    , const int64 iSize
    , const int64 iCount
    , const int64 iOffset
    , const int64 iIndex
    , FClearJobArgs& iJargs
)
{
    uint8* const ULIS_RESTRICT dst  = iCargs->block.Bits();
    const int64 btt                 = static_cast< int64 >( iCargs->block.BytesTotal() );
    iJargs.dst = dst + iIndex;
    iJargs.size = FMath::Min( iOffset + iSize, btt ) - iOffset;
}

template< void (*TDelegateInvoke)( const FClearJobArgs*, const FClearCommandArgs* ) >
void
ScheduleClearJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    const FClearCommandArgs* cargs  = dynamic_cast< const FClearCommandArgs* >( iCommand->Args() );
    RangeBasedSchedulingBuildJobs<
          FClearJobArgs
        , FClearCommandArgs
        , TDelegateInvoke
        , BuildClearJob_Scanlines
        , BuildClearJob_Chunks
    >
    (
          iCommand
        , iPolicy
        , static_cast< int64 >( cargs->block.BytesTotal() )
        , cargs->rect.h
        , iContiguous
    );
}

/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
void
InvokeClearMT_AX2(
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
InvokeClearMT_SSE4_2(
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
InvokeClearMT_MEM(
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
    ScheduleClearJobs< &InvokeClearMT_AX2 >( iCommand, iPolicy, iContiguous );
}

void
ScheduleClearMT_SSE4_2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    ScheduleClearJobs< &InvokeClearMT_SSE4_2 >( iCommand, iPolicy, iContiguous );
}

void
ScheduleClearMT_MEM(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    ScheduleClearJobs< &InvokeClearMT_MEM >( iCommand, iPolicy, iContiguous );
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedClearInvocationSchedulerSelector )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedClearInvocationSchedulerSelector )

ULIS_NAMESPACE_END

