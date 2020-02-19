// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Copy.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Copy entry point functions.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Copy/Copy.h"
#include "Base/CPU.h"
#include "Base/Perf.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Thread/ParallelFor.h"
#include "Thread/ThreadPool.h"
#include <immintrin.h>

ULIS2_NAMESPACE_BEGIN
#ifdef __AVX2__
void InvokeCopyMTProcessScanline_AX2( tByte* iDst, const tByte* iSrc, const tSize iCount, const tSize iStride )
{
    tSize index;
    for( index = 0; index < ( iCount - 32 ); index += iStride )
    {
        _mm256_storeu_si256( (__m256i*)iDst, _mm256_loadu_si256( (const __m256i*)iSrc ) );
        iDst += iStride;
    }
    // Remaining unaligned scanline end:
    // avoid concurrent write on 256 bit with AVS and perform a memcpy instead
    memcpy( iDst, &iSrc, iCount - index );
}
#endif // __AVX2__

#ifdef __SSE4_2__
void InvokeCopyMTProcessScanline_SSE( tByte* iDst, const tByte* iSrc, const tSize iCount, const tSize iStride )
{
    tSize index;
    for( index = 0; index < ( iCount - 16 ); index += iStride )
    {
        _mm_storeu_si128( (__m128i*)iDst, _mm_loadu_si128( (const __m128i*)iSrc ) );
        iDst += iStride;
    }
    // Remaining unaligned scanline end:
    // avoid concurrent write on 128 bit with SSE and perform a memcpy instead
    memcpy( iDst, &iSrc, iCount - index );
}
#endif // __SE4_2__

void InvokeCopyMTProcessScanline_MEM( tByte* iDst, const tByte* iSrc, tSize iCount )
{
    memcpy( iDst, iSrc, iCount );
}


void
Copy_imp( FThreadPool*  iPool
        , bool          iBlocking
        , const FPerf&  iPerf
        , const FCPU&   iCPU
        , const FBlock* iSrc
        , FBlock*       iDst
        , const FRect&  iSrcRoi
        , const FRect&  iDstRoi )
{
    const tSize bpc = iDst->BytesPerSample();
    const tSize spp = iDst->SamplesPerPixel();
    const tSize bpp = iDst->BytesPerPixel();
    const tSize w   = iDst->Width();
    const tSize bps = iDst->BytesPerScanLine();
    const tSize srh = iSrcRoi.x * bpp;
    const tSize dsh = iDstRoi.x * bpp;
    const tByte*srb = iSrc->DataPtr() + srh;
    tByte*      dsb = iDst->DataPtr() + dsh;
    #define SRC srb + ( ( iSrcRoi.y + pLINE ) * bps )
    #define DST dsb + ( ( iDstRoi.y + pLINE ) * bps )
    #ifdef __AVX2__
    if( iPerf.UseAVX2() && iCPU.info.HW_AVX2 && bpp <= 32 && bps >= 32 )
    {
        const tSize stride = 32;
        const tSize count = iSrcRoi.w * bpp;
        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerf, iPool, iBlocking, iSrcRoi.h, InvokeCopyMTProcessScanline_AX2, DST, SRC, count, stride )
    }
    else
    #endif // __AVX2__
    #ifdef __SSE4_2__
    if( iPerf.UseSSE4_2() && iCPU.info.HW_SSE42 && bpp <= 16 && bps >= 16 )
    {
        const tSize stride = 16;
        const tSize count = iSrcRoi.w * bpp;
        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerf, iPool, iBlocking, iSrcRoi.h, InvokeCopyMTProcessScanline_SSE, DST, SRC, count, stride )
    }
    else
    #endif // __SSE4_2__
    {
        const tSize count = iSrcRoi.w * bpp;
        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerf, iPool, iBlocking, iSrcRoi.h, InvokeCopyMTProcessScanline_MEM, DST, SRC, iSrcRoi.w )
    }
}


void
Copy( FThreadPool*      iPool
    , bool              iBlocking
    , const FPerf&      iPerf
    , const FCPU&       iCPU
    , const FBlock*     iSrc
    , FBlock*           iDst
    , int               iDstX
    , int               iDstY
    , bool              iCallInvalidCB )
{
    CopyRect( iPool, iBlocking, iPerf, iCPU, iSrc, iDst, iSrc->Rect(), iDstX, iDstY, iCallInvalidCB );
}


void
CopyRect( FThreadPool*      iPool
        , bool              iBlocking
        , const FPerf&      iPerf
        , const FCPU&       iCPU
        , const FBlock*     iSrc
        , FBlock*           iDst
        , const FRect&      iSrcRect
        , int               iDstX
        , int               iDstY
        , bool              iCallInvalidCB )
{
    ULIS2_ASSERT( iPool,                                    "Bad pool" );
    ULIS2_ASSERT( iSrc,                                     "Bad source" );
    ULIS2_ASSERT( iDst,                                     "Bad destination" );
    ULIS2_ASSERT( iSrc->Format() == iDst->Format(),         "Formats do not match" );
    ULIS2_ASSERT( !( (!iBlocking) && (iCallInvalidCB ) ),   "Calling invalid CB on non-blocking operation may induce race condition and undefined behaviours." );
    ULIS2_WARNING( iSrc != iDst,                            "Copying a block on itself may trigger data race, use at your own risk or ensure written areas do not overlap." );
    // Ensure the selected source rect actually fits in source dimensions.
    FRect src_roi = iSrcRect & iSrc->Rect();

    // Compute coordinates of target rect in destination, with source rect dimension
    int target_xmin = iDstX;
    int target_ymin = iDstY;
    int target_xmax = iDstX + src_roi.w;
    int target_ymax = iDstY + src_roi.h;
    FRect dst_target = FRect::FromMinMax( target_xmin, target_ymin, target_xmax, target_ymax );

    // Ensure the selected target actually fits in destination
    FRect dst_fit = dst_target & iDst->Rect();
    if( dst_fit.Area() <= 0 ) return;

    Copy_imp( iPool, iBlocking, iPerf, iCPU, iSrc, iDst, src_roi, dst_fit );
    iDst->Invalidate( dst_fit, iCallInvalidCB );
}


void
CopyRaw( const FBlock* iSrc
       , FBlock*       iDst
       , bool          iCallInvalidCB )
{
    ULIS2_ASSERT( iSrc,                             "Bad source" );
    ULIS2_ASSERT( iDst,                             "Bad destination" );
    ULIS2_ASSERT( iSrc != iDst,                     "Destination and source cannot be the same" );
    ULIS2_ASSERT( iSrc->Format() == iDst->Format(), "Formats do not matchs" );
    // One call, supposedly more efficient for small block.
    memcpy( iDst->DataPtr(), iSrc->DataPtr(), iSrc->BytesTotal() );
}


ULIS2_NAMESPACE_END

