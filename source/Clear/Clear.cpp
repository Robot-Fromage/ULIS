// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Clear.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Clear functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Clear/Clear.h"
#include "Clear/ClearHelpers.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
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
)
{
    BuildClearJobs< &InvokeClearMTProcessScanline_AX2 >( iCommand, iPolicy );
}

void
ScheduleClearMT_SSE4_2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildClearJobs< &InvokeClearMTProcessScanline_SSE4_2 >( iCommand, iPolicy );
}

void
ScheduleClearMT_MEM(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildClearJobs< &InvokeClearMTProcessScanline_MEM >( iCommand, iPolicy );
}

ULIS_NAMESPACE_END

