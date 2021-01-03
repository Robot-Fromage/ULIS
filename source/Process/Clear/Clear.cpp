// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Clear.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Clear API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/Clear/Clear.h"
#include "Image/Block.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"
#include "Scheduling/SimpleBufferArgs.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS_COMPILETIME_AVX_SUPPORT
void
InvokeClearMT_AVX(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
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
#endif // ULIS_COMPILETIME_AVX_SUPPORT

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- SSE
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
void
InvokeClearMT_SSE(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
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
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    // Full scanline width instead of many BPP clears
    memset( jargs->dst, 0, jargs->size );
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleClearMT_AVX, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeClearMT_AVX )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleClearMT_SSE, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeClearMT_SSE )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleClearMT_MEM, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeClearMT_MEM )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedClearInvocationSchedulerSelector )

ULIS_NAMESPACE_END

