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
#include "System/HostDeviceInfo.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Thread/OldThreadPool.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
void InvokeFillMTProcessScanline_AX2( uint8* iDst, const uint32 iCount, const uint32 iStride ) {
    int64 index;
    for( index = 0; index < int64( iCount ) - 32; index += iStride ) {
        _mm256_storeu_si256( (__m256i*)iDst, _mm256_setzero_si256() );
        iDst += iStride;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memset( iDst, 0, iCount - index );
}
#endif // ULIS_COMPILETIME_AVX2_SUPPORT

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- SSE
#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
void InvokeFillMTProcessScanline_SSE4_2( uint8* iDst, const uint32 iCount, const uint32 iStride ) {
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
void InvokeFillMTProcessScanline_MEM( uint8* iDst, uint32 iCount, const uint32 iStride ) {
    // Full scanline width instead of many BPP clears
    memset( iDst, 0, iCount );
}

/////////////////////////////////////////////////////
// Implementation
void Clear_imp( FOldThreadPool*            iOldThreadPool
              , bool                    iBlocking
              , uint32                  iPerfIntent
              , const FHostDeviceInfo&  iHostDeviceInfo
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

/////////////////////////////////////////////////////
// Clear
 void Clear( FOldThreadPool*              iOldThreadPool
           , bool                      iBlocking
           , uint32                    iPerfIntent
           , const FHostDeviceInfo&    iHostDeviceInfo
           , bool                      iCallCB
           , FBlock*                   iDestination
           , const FRectI&              iArea )
{
    // Assertions
    ULIS_ASSERT( iDestination,             "Bad source."                                           );
    ULIS_ASSERT( iOldThreadPool,              "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );
    // Fit region of interest
    FRectI roi = iArea & iDestination->Rect();

    // Check no-op
    if( roi.Area() <= 0 )
        return;

    // Call
    Clear_imp( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iDestination, iArea );

    // Invalid
    iDestination->Dirty( roi, iCallCB );
}

/////////////////////////////////////////////////////
// ClearRaw
void ClearRaw( FBlock* iDst, bool iCallCB ) {
    ULIS_ASSERT( iDst, "Bad destination" );
    memset( iDst->Bits(), 0, iDst->BytesTotal() );
    iDst->Dirty( iCallCB );
}

ULIS_NAMESPACE_END

