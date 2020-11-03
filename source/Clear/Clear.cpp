// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Clear.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Clear functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Clear/Clear.h"
#include "Data/Block.h"
#include "Base/HostDeviceInfo.h"
#include "Maths/Geometry.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS3_COMPILETIME_AVX2_SUPPORT
void InvokeFillMTProcessScanline_AX2( tByte* iDst, const tSize iCount, const tSize iStride ) {
    int64 index;
    for( index = 0; index < int64( iCount ) - 32; index += iStride ) {
        _mm256_storeu_si256( (__m256i*)iDst, _mm256_setzero_si256() );
        iDst += iStride;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memset( iDst, 0, iCount - index );
}
#endif // ULIS3_COMPILETIME_AVX2_SUPPORT

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- SSE
#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
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

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void InvokeFillMTProcessScanline_MEM( tByte* iDst, tSize iCount, const tSize iStride ) {
    // Full scanline width instead of many BPP clears
    memset( iDst, 0, iCount );
}

/////////////////////////////////////////////////////
// Implementation
void Clear_imp( FThreadPool*            iThreadPool
              , bool                    iBlocking
              , uint32                  iPerfIntent
              , const FHostDeviceInfo&  iHostDeviceInfo
              , bool                    iCallCB
              , FBlock*                 iDestination
              , const FRect&            iArea )
{
    const FFormatInfo&  fmt     = iDestination->FormatInfo();
    const tSize         bpp     = fmt.BPP;
    const tSize         w       = iDestination->Width();
    const tSize         bps     = iDestination->BytesPerScanLine();
    const tSize         dsh     = iArea.x * bpp;
    tByte*              dsb     = iDestination->DataPtr() + dsh;
    const tSize         count   = iArea.w * bpp;
    #define DST dsb + ( ( iArea.y + static_cast< int64 >( pLINE ) ) * static_cast< int64 >( bps ) )

    #ifdef ULIS3_COMPILETIME_AVX2_SUPPORT
    if( ( iPerfIntent & ULIS3_PERF_AVX2 ) && iHostDeviceInfo.HW_AVX2 && bps >= 32 ) {
        const tSize stride = 32;
        ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_AX2, DST, count, stride )
    } else
    #endif
    #ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
    if( ( iPerfIntent & ULIS3_PERF_SSE42 ) && iHostDeviceInfo.HW_SSE42 && bps >= 16 ) {
        const tSize stride = 16;
        ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_SSE4_2, DST, count, stride )
    } else
    #endif
    {
        ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iArea.h
                                       , InvokeFillMTProcessScanline_MEM, DST, count, bpp )
    }
}

/////////////////////////////////////////////////////
// Clear
 void Clear( FThreadPool*              iThreadPool
           , bool                      iBlocking
           , uint32                    iPerfIntent
           , const FHostDeviceInfo&    iHostDeviceInfo
           , bool                      iCallCB
           , FBlock*                   iDestination
           , const FRect&              iArea )
{
    // Assertions
    ULIS3_ASSERT( iDestination,             "Bad source."                                           );
    ULIS3_ASSERT( iThreadPool,              "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );
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

/////////////////////////////////////////////////////
// ClearRaw
void ClearRaw( FBlock* iDst, bool iCallCB ) {
    ULIS3_ASSERT( iDst, "Bad destination" );
    memset( iDst->DataPtr(), 0, iDst->BytesTotal() );
    iDst->Invalidate( iCallCB );
}

ULIS3_NAMESPACE_END

