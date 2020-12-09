// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Copy.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Copy entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Copy/Copy.h"
#include "Image/Block.h"
#include "Scheduling/DualBufferArgs.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS_COMPILETIME_AVX_SUPPORT
void InvokeCopyMT_AVX(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
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
#endif // ULIS_COMPILETIME_AVX_SUPPORT

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- SSE
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
void InvokeCopyMT_SSE(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
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
void InvokeCopyMT_MEM(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
    memcpy( jargs->dst, jargs->src, jargs->size );
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleCopyMT_AVX, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeCopyMT_AVX )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleCopyMT_SSE, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeCopyMT_SSE )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleCopyMT_MEM, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeCopyMT_MEM )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedCopyInvocationSchedulerSelector )

ULIS_NAMESPACE_END

