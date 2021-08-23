// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Fill.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Fill entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Fill/Fill.h"
#include "Base/HostDeviceInfo.h"
#include "Conv/Conv.h"
#include "Data/Block.h"
#include "Data/Pixel.h"
#include "Maths/Geometry.h"
#include "Thread/ThreadPool.h"
#include <vectorclass.h>

ULIS2_NAMESPACE_BEGIN
#ifdef __AVX2__
void ULIS2_VECTORCALL
InvokeFillMTProcessScanline_AX2( tByte* iDst, __m256i iSrc, const tSize iCount, const tSize iStride ) {
    tSize index = 0;
    for( index = 0; index < ( iCount - 32 ); index += iStride ) {
        _mm256_storeu_si256( (__m256i*)iDst, iSrc );
        iDst += iStride;
    }

    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memcpy instead
    memcpy( iDst, &iSrc, iCount - index );
}
#endif // __AVX2__

#ifdef __SSE4_2__
void ULIS2_VECTORCALL
InvokeFillMTProcessScanline_SSE( tByte* iDst, __m128i iSrc, const tSize iCount, const tSize iStride ) {
    tSize index;
    for( index = 0; index < ( iCount - 16 ); index += iStride ) {
        _mm_storeu_si128( (__m128i*)iDst, iSrc );
        iDst += iStride;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 128 bit with SSE and perform a memcpy instead
    memcpy( iDst, &iSrc, iCount - index );
}
#endif // __SE4_2__

void
InvokeFillMTProcessScanline_MEM( tByte* iDst, std::shared_ptr< const FPixelValue > iSrc, tSize iCount, tSize iStride ) {
    const tByte* src = iSrc->Ptr();
    for( uint32 i = 0; i < iCount; ++i ) {
        memcpy( iDst, src, iStride );
        iDst += iStride;
    }
}

void
Fill_imp( FThreadPool*                          iThreadPool
        , bool                                  iBlocking
        , uint32                                iPerfIntent
        , const FHostDeviceInfo&                iHostDeviceInfo
        , bool                                  iCallCB
        , FBlock*                               iDestination
        , std::shared_ptr< const FPixelValue >  iColor
        , const FRect&                          iDstROI )
{
    // Bake Params
    const tSize bpp     = iDestination->BytesPerPixel();
    const tSize bps     = iDestination->BytesPerScanLine();
    const tSize dsh     = iDstROI.x * bpp;
    tByte*      dsb     = iDestination->DataPtr() + dsh;
    #define DST dsb + ( ( iDstROI.y + pLINE ) * bps )

#ifdef __AVX2__
    if( ( iPerfIntent & ULIS2_PERF_AVX2 ) && iHostDeviceInfo.HW_AVX2 && bpp <= 32 && bps >= 32 ) {
        tSize   count   = iDstROI.w * bpp;
        tSize   stride  = 32 - ( 32 % bpp );
        tByte*  srcb    = new tByte[32];

        for( tSize i = 0; i < stride; i+= bpp )
            memcpy( (void*)( ( srcb ) + i ), iColor->Ptr(), bpp );

        __m256i src = _mm256_lddqu_si256( (const __m256i*)srcb );

        delete [] srcb;

        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeFillMTProcessScanline_AX2, DST, src, count, stride )
    } else
#endif
#ifdef __SSE4_2__
    if( ( iPerfIntent & ULIS2_PERF_SSE42 ) && iHostDeviceInfo.HW_SSE42 && bpp <= 16 && bps >= 16 ) {
        tSize   count   = iDstROI.w * bpp;
        tSize   stride  = 16 - ( 16 % bpp );
        tByte*  srcb    = new tByte[16];

        for( tSize i = 0; i < stride; i+= bpp )
            memcpy( (void*)( ( srcb ) + i ), iColor->Ptr(), bpp );

        __m128i src = _mm_lddqu_si128( (const __m128i*)srcb );

        delete [] srcb;

        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeFillMTProcessScanline_SSE, DST, src, count, stride )
    } else
#endif
    {
        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeFillMTProcessScanline_MEM, DST, iColor, iDstROI.w, bpp )
    }
}

void
Fill( FThreadPool*              iThreadPool
    , bool                      iBlocking
    , uint32                    iPerfIntent
    , const FHostDeviceInfo&    iHostDeviceInfo
    , bool                      iCallCB
    , FBlock*                   iDestination
    , const IPixel&             iColor
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

    // Bake color param, shared Ptr for thread safety and scope life time extension in non blocking multithreaded processing
    std::shared_ptr< FPixelValue > color = std::make_shared< FPixelValue >( iDestination->Format() );
    Conv( iColor, *color );

    // Call
    Fill_imp( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iDestination, color, roi );

    // Invalid
    iDestination->Invalidate( roi, iCallCB );
}

ULIS2_NAMESPACE_END

