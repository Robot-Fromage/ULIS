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
    for( index = 0; index < jargs->count - 32; index += 32 ) {
        _mm256_storeu_si256( dst, _mm256_setzero_si256() );
        ++dst;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memset( dst, 0, jargs->count - index );
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
    for( index = 0; index < jargs->count - 16; index += 16 ) {
        _mm_storeu_si128( dst, _mm_setzero_si128() );
        ++dst;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 128 bit with SSE and perform a memset instead
    memset( dst, 0, jargs->count - index );
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
    memset( jargs->dst, 0, jargs->count );
}

/////////////////////////////////////////////////////
// Implementation
void Clear_imp( FOldThreadPool*            iOldThreadPool
              , bool                    iBlocking
              , uint32                  iPerfIntent
              , const FHardwareMetrics&  iHostDeviceInfo
              , bool                    iCallCB
              , FBlock*                 iDestination
              , const FRectI&            iArea )
{
    const FFormatMetrics&  fmt     = iDestination->FormatMetrics();
    const uint32         bpp     = fmt.BPP;
    const uint32         w       = iDestination->Width();
    const uint32         bps     = iDestination->BytesPerScanLine();
    const uint32         dsh     = iArea.x * bpp;
    uint8*              dsb     = iDestination->Bits() + dsh;
    const uint32         count   = iArea.w * bpp;
    #define DST dsb + ( ( iArea.y + static_cast< int64 >( pLINE ) ) * static_cast< int64 >( bps ) )

    #ifdef ULIS_COMPILETIME_AVX2_SUPPORT
    if( ( iPerfIntent & ULIS_PERF_AVX2 ) && iHostDeviceInfo.HW_AVX2 && bps >= 32 ) {
        const uint32 stride = 32;
        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_AX2, DST, count, stride )
    } else
    #endif
    #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
    if( ( iPerfIntent & ULIS_PERF_SSE42 ) && iHostDeviceInfo.HW_SSE42 && bps >= 16 ) {
        const uint32 stride = 16;
        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_SSE4_2, DST, count, stride )
    } else
    #endif
    {
        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_MEM, DST, count, bpp )
    }
}

void
ScheduleClearMT_AX2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
}

void
ScheduleClearMT_SSE4_2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
}

void
ScheduleClearMT_MEM(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
}

ULIS_NAMESPACE_END

