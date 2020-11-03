// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Clear.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Clear entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Clear/Clear.h"
#include "Base/HostDeviceInfo.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Thread/ThreadPool.h"

ULIS2_NAMESPACE_BEGIN
#ifdef __AVX2__
void InvokeFillMTProcessScanline_AX2( tByte* iDst, const tSize iCount, const tSize iStride ) {
    int64 index;
    for( index = 0; index < int64( iCount ) - 32; index += iStride ) {
        _mm256_storeu_si256( (__m256i*)iDst, _mm256_setzero_si256() );
        iDst += iStride;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memset( iDst, 0, iCount - index );
}
#endif // __AVX2__

#ifdef __SSE4_2__
void InvokeFillMTProcessScanline_SSE4_2( tByte* iDst, const tSize iCount, const tSize iStride ) {
    int64 index;
    for( index = 0; index < int64( iCount ) - 16; index += iStride ) {
        _mm_storeu_si128( (__m128i*)iDst, _mm_setzero_si128() );
        iDst += iStride;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 128 bit with SSE and perform a memset instead
    memset( iDst, 0, iCount - index );
}
#endif // __SE4_2__

void InvokeFillMTProcessScanline_MEM( tByte* iDst, tSize iCount, tSize iStride ) {
    memset( iDst, 0, iCount );
}

void Clear_imp( FThreadPool*            iThreadPool
           , bool                       iBlocking
           , uint32                     iPerfIntent
           , const FHostDeviceInfo&     iHostDeviceInfo
           , bool                       iCallCB
           , FBlock*                    iDestination
           , const FRect&               iArea )
{
    const FFormatInfo& fmt = iDestination->FormatInfo();
    const tSize bpp = fmt.BPP;
    const tSize w   = iDestination->Width();
    const tSize bps = iDestination->BytesPerScanLine();
    const tSize dsh = iArea.x * bpp;
    tByte*      dsb = iDestination->DataPtr() + dsh;
    #define DST dsb + ( ( iArea.y + static_cast< int64 >( pLINE ) ) * static_cast< int64 >( bps ) )

    #ifdef __AVX2__
    if( ( iPerfIntent & ULIS2_PERF_AVX2 ) && iHostDeviceInfo.HW_AVX2 && bps >= 32 ) {
        const tSize stride = 32;
        const tSize count = iArea.w * bpp;
        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_AX2, DST, count, stride )
    } else
    #endif
    #ifdef __SSE4_2__
    if( ( iPerfIntent & ULIS2_PERF_SSE42 ) && iHostDeviceInfo.HW_SSE42 && bps >= 16 ) {
        const tSize stride = 16;
        const tSize count = iArea.w * bpp;
        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_SSE4_2, DST, count, stride )
    } else
    #endif
    {
        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_MEM, DST, iArea.w, bpp )
    }
}

 void Clear( FThreadPool*              iThreadPool
           , bool                      iBlocking
           , uint32                    iPerfIntent
           , const FHostDeviceInfo&    iHostDeviceInfo
           , bool                      iCallCB
           , FBlock*                   iDestination
           , const FRect&              iArea )
{
    // Assertions
    ULIS2_ASSERT( iDestination,             "Bad source."                                           );
    ULIS2_ASSERT( iThreadPool,              "Bad pool."                                             );
    ULIS2_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );
    // Fit region of interest
    FRect roi = iArea & iDestination->Rect();

    // Check no-op
    if( roi.Area() <= 0 )
        return;

    // Call
    Clear_imp( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iDestination, iArea );

    // Invalid
    iDestination->Invalidate( roi, iCallCB );
}

void ClearRaw( FBlock* iDst, bool iCallCB ) {
    ULIS2_ASSERT( iDst, "Bad destination" );
    memset( iDst->DataPtr(), 0, iDst->BytesTotal() );
    iDst->Invalidate( iCallCB );
}

ULIS2_NAMESPACE_END

