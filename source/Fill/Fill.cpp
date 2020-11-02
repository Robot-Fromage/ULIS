// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Fill.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Fill entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Fill/Fill.h"
#include "System/Device.h"
#include "Conv/Conv.h"
#include "Image/Block.h"
#include "Image/Pixel.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
void ULIS_VECTORCALL
InvokeFillMTProcessScanline_AX2( uint8* iDst, std::shared_ptr< const FBlock > iBuf, const uint32 iCount, const uint32 iStride ) {
    __m256i src = _mm256_lddqu_si256( (const __m256i*)iBuf->Bits() );

    uint32 index = 0;
    for( index = 0; index < ( iCount - 32 ); index += iStride ) {
        _mm256_storeu_si256( (__m256i*)iDst, src );
        iDst += iStride;
    }

    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memcpy instead
    memcpy( iDst, iBuf->Bits(), iCount - index );
}
#endif // ULIS_COMPILETIME_AVX2_SUPPORT

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- SSE
#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
void ULIS_VECTORCALL
InvokeFillMTProcessScanline_SSE( uint8* iDst, std::shared_ptr< const FBlock > iBuf, const uint32 iCount, const uint32 iStride ) {
    __m128i src = _mm_lddqu_si128( (const __m128i*)iBuf->Bits() );

    uint32 index;
    for( index = 0; index < ( iCount - 16 ); index += iStride ) {
        _mm_storeu_si128( (__m128i*)iDst, src );
        iDst += iStride;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 128 bit with SSE and perform a memcpy instead
    memcpy( iDst, iBuf->Bits(), iCount - index );
}
#endif // __SE4_2__

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeFillMTProcessScanline_MEM( uint8* iDst, std::shared_ptr< const FColor > iSrc, uint32 iCount, uint32 iStride ) {
    const uint8* src = iSrc->Bits();
    for( uint32 i = 0; i < iCount; ++i ) {
        memcpy( iDst, src, iStride );
        iDst += iStride;
    }
}

/////////////////////////////////////////////////////
// Implementation
void
Fill_imp( FOldThreadPool*                          iOldThreadPool
        , bool                                  iBlocking
        , uint32                                iPerfIntent
        , const FHardwareMetrics&                iHostDeviceInfo
        , bool                                  iCallCB
        , FBlock*                               iDestination
        , std::shared_ptr< const FColor >  iColor
        , const FRectI&                          iDstROI )
{
    // Bake Params
    const uint32 bpp     = iDestination->BytesPerPixel();
    const uint32 bps     = iDestination->BytesPerScanLine();
    const uint32 dsh     = iDstROI.x * bpp;
    uint8*      dsb     = iDestination->Bits() + dsh;
    #define DST dsb + ( ( iDstROI.y + pLINE ) * bps )

#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
    if( ( iPerfIntent & ULIS_PERF_AVX2 ) && iHostDeviceInfo.HW_AVX2 && bpp <= 32 && bps >= 32 ) {
        uint32   count   = iDstROI.w * bpp;
        uint32   stride  = 32 - ( 32 % bpp );
        std::shared_ptr< FBlock > buf = std::make_shared< FBlock >( 32, 1, eFormat::Format_G8 );
        uint8* srcb = buf->Bits();

        for( uint32 i = 0; i < stride; i+= bpp )
            memcpy( (void*)( ( srcb ) + i ), iColor->Bits(), bpp );

        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeFillMTProcessScanline_AX2, DST, buf, count, stride )
    } else
#endif
#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
    if( ( iPerfIntent & ULIS_PERF_SSE42 ) && iHostDeviceInfo.HW_SSE42 && bpp <= 16 && bps >= 16 ) {
        uint32   count   = iDstROI.w * bpp;
        uint32   stride  = 16 - ( 16 % bpp );
        std::shared_ptr< FBlock > buf = std::make_shared< FBlock >( 16, 1, eFormat::Format_G8 );
        uint8* srcb = buf->Bits();

        for( uint32 i = 0; i < stride; i+= bpp )
            memcpy( (void*)( ( srcb ) + i ), iColor->Bits(), bpp );

        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeFillMTProcessScanline_SSE, DST, buf, count, stride )
    } else
#endif
    {
        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeFillMTProcessScanline_MEM, DST, iColor, iDstROI.w, bpp )
    }
}

/////////////////////////////////////////////////////
// Fill
void
Fill( FOldThreadPool*              iOldThreadPool
    , bool                      iBlocking
    , uint32                    iPerfIntent
    , const FHardwareMetrics&    iHostDeviceInfo
    , bool                      iCallCB
    , FBlock*                   iDestination
    , const ISample&             iColor
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

    // Bake color param, shared Ptr for thread safety and scope life time extension in non blocking multithreaded processing
    std::shared_ptr< FColor > color = std::make_shared< FColor >( iDestination->Format() );
    Conv( iColor, *color );

    // Call
    Fill_imp( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iDestination, color, roi );

    // Invalid
    iDestination->Dirty( roi, iCallCB );
}

ULIS_NAMESPACE_END

