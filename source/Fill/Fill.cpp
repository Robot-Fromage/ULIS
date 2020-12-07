// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Fill.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Fill API.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Fill/Fill.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS_COMPILETIME_AVX_SUPPORT
void
InvokeFillMT_AX2(
      const FSimpleBufferJobArgs* jargs
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
#endif // ULIS_COMPILETIME_AVX_SUPPORT

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- SSE
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
void
InvokeFillMT_SSE(
      const FSimpleBufferJobArgs* jargs
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
InvokeFillMT_MEM(
      const FSimpleBufferJobArgs* jargs
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
    , bool iContiguous
)
{
    const FFillCommandArgs* cargs = dynamic_cast< const FFillCommandArgs* >( iCommand->Args() );
    const uint8 bpp = cargs->block.BytesPerPixel();
    const uint32 bps = cargs->block.BytesPerScanLine();

    if( bpp <= 32 && bps >= 32 ) {
        ScheduleSimpleBufferJobs< FSimpleBufferJobArgs, FFillCommandArgs, &InvokeFillMT_AX2 >( iCommand, iPolicy, iContiguous );
    } else if( bpp <= 16 && bps >= 16 ) {
        ScheduleSimpleBufferJobs< FSimpleBufferJobArgs, FFillCommandArgs, &InvokeFillMT_SSE >( iCommand, iPolicy, iContiguous );
    } else {
        ScheduleSimpleBufferJobs< FSimpleBufferJobArgs, FFillCommandArgs, &InvokeFillMT_MEM >( iCommand, iPolicy, iContiguous );
    }
}

void
ScheduleFillMT_SSE4_2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    const FFillCommandArgs* cargs = dynamic_cast< const FFillCommandArgs* >( iCommand->Args() );
    const uint8 bpp = cargs->block.BytesPerPixel();
    const uint32 bps = cargs->block.BytesPerScanLine();
    if( bpp <= 16 && bps >= 16 ) {
        ScheduleSimpleBufferJobs< FSimpleBufferJobArgs, FFillCommandArgs, &InvokeFillMT_SSE >( iCommand, iPolicy, iContiguous );
    } else {
        ScheduleSimpleBufferJobs< FSimpleBufferJobArgs, FFillCommandArgs, &InvokeFillMT_MEM >( iCommand, iPolicy, iContiguous );
    }
}

void
ScheduleFillMT_MEM(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    ScheduleSimpleBufferJobs< FSimpleBufferJobArgs, FFillCommandArgs, &InvokeFillMT_MEM >( iCommand, iPolicy, iContiguous );
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedFillInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedFillPreserveAlphaInvocationSchedulerSelector )

ULIS_NAMESPACE_END

