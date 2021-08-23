// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Copy.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Copy entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Copy/Copy.h"
#include "Base/HostDeviceInfo.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Thread/ThreadPool.h"


ULIS2_NAMESPACE_BEGIN

#ifdef __AVX2__
void InvokeCopyMTProcessScanline_AX2( tByte* iDst, const tByte* iSrc, const tSize iCount, const tSize iStride )
{
    tSize index;
    for( index = 0; index < ( iCount - 32 ); index += iStride )
    {
        _mm256_storeu_si256( (__m256i*)iDst, _mm256_loadu_si256( (const __m256i*)iSrc ) );
        iSrc += iStride;
        iDst += iStride;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memcpy( iDst, iSrc, iCount - index );
}
#endif // __AVX2__

#ifdef __SSE4_2__
void InvokeCopyMTProcessScanline_SSE( tByte* iDst, const tByte* iSrc, const tSize iCount, const tSize iStride )
{
    tSize index;
    for( index = 0; index < ( iCount - 16 ); index += iStride )
    {
        _mm_storeu_si128( (__m128i*)iDst, _mm_loadu_si128( (const __m128i*)iSrc ) );
        iSrc += iStride;
        iDst += iStride;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memcpy( iDst, iSrc, iCount - index );
}
#endif // __SE4_2__

void InvokeCopyMTProcessScanline_MEM( tByte* iDst, const tByte* iSrc, tSize iCount )
{
    memcpy( iDst, iSrc, iCount );
}


void
Copy_imp( FThreadPool*              iThreadPool
        , bool                      iBlocking
        , uint32                    iPerfIntent
        , const FHostDeviceInfo&    iHostDeviceInfo
        , bool                      iCallCB
        , const FBlock*             iSource
        , FBlock*                   iDestination
        , const FRect&              iSrcROI
        , const FRect&              iDstROI
        , const FVec2I&             iShift )
{
    const tSize bpp = iDestination->BytesPerPixel();
    const tSize src_bps = iSource->BytesPerScanLine();
    const tSize dst_bps = iDestination->BytesPerScanLine();
    const tSize srh = ( iShift.x + iSrcROI.x ) * bpp;
    const tSize dsh = iDstROI.x * bpp;
    const tByte*src = iSource->DataPtr() + srh;
    tByte*      dst = iDestination->DataPtr() + dsh;
    const auto basesrcy = iShift.y + iSrcROI.y;
    const auto basedsty = iDstROI.y;
    #define SRC src + ( ( basesrcy + pLINE ) * src_bps )
    #define DST dst + ( ( basedsty + pLINE ) * dst_bps )
    const tSize count = iDstROI.w * bpp;
    #ifdef __AVX2__
    if( ( iPerfIntent & ULIS2_PERF_AVX2 ) && iHostDeviceInfo.HW_AVX2 && ( src_bps + dst_bps ) >= 64 ) {
        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeCopyMTProcessScanline_AX2, DST, SRC, count, 32 )
    } else
    #endif
    #ifdef __SSE4_2__
    if( ( iPerfIntent & ULIS2_PERF_SSE42 ) && iHostDeviceInfo.HW_SSE42 && ( src_bps + dst_bps ) >= 32 ) {
        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeCopyMTProcessScanline_SSE, DST, SRC, count, 16 )
    } else
    #endif
    {
        ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeCopyMTProcessScanline_MEM, DST, SRC, count )
    }
}


void Copy( FThreadPool*             iThreadPool
         , bool                     iBlocking
         , uint32                   iPerfIntent
         , const FHostDeviceInfo&   iHostDeviceInfo
         , bool                     iCallCB
         , const FBlock*            iSource
         , FBlock*                  iDestination
         , const FRect&             iArea
         , const FVec2I&            iPos )
{
    // Assertions
    ULIS2_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS2_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS2_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS2_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match"                                  );

    // Compute coordinates of target rect in destination, with source rect dimension
    // Ensure the selected target actually fits in destination
    FRect src_roi = iArea & iSource->Rect();
    int target_xmin = iPos.x;
    int target_ymin = iPos.y;
    int target_xmax = target_xmin + src_roi.w;
    int target_ymax = target_ymin + src_roi.h;
    FRect dst_target = FRect::FromMinMax( target_xmin, target_ymin, target_xmax, target_ymax );

    // Ensure the selected target actually fits in destination
    FRect dst_fit = dst_target & iDestination->Rect();
    if( dst_fit.Area() <= 0 ) return;

    // Forward arguments baking
    const int   translationX        = dst_fit.x - dst_target.x;
    const int   translationY        = dst_fit.y - dst_target.y;
    const FVec2I shift( translationX, translationY );

    // Call
    Copy_imp( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, iDestination, src_roi, dst_fit, shift );

    // Invalidate
    iDestination->Invalidate( dst_fit, iCallCB );
}

FBlock* XCopy( FThreadPool*           iThreadPool
             , bool                   iBlocking
             , uint32                 iPerfIntent
             , const FHostDeviceInfo& iHostDeviceInfo
             , bool                   iCallCB
             , const FBlock*          iSource )
{
    ULIS2_ASSERT( iSource, "Bad source." );
    FBlock* ret = new FBlock( iSource->Width(), iSource->Height(), iSource->Format() );
    Copy( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, ret, iSource->Rect(), FVec2I( 0, 0 ) );
    return  ret;
}

void
CopyRaw( const FBlock* iSrc, FBlock* iDst, bool iCallCB ) {
    // Assertions
    ULIS2_ASSERT( iSrc,                             "Bad source"                                );
    ULIS2_ASSERT( iDst,                             "Bad destination"                           );
    ULIS2_ASSERT( iSrc != iDst,                     "Destination and source cannot be the same" );
    ULIS2_ASSERT( iSrc->Format() == iDst->Format(), "Formats do not matchs"                     );

    // One call, supposedly more efficient for small block.
    memcpy( iDst->DataPtr(), iSrc->DataPtr(), iSrc->BytesTotal() );

    // Invalid
    iDst->Invalidate( iCallCB );
}

FBlock*
XCopyRaw( const FBlock* iSrc, bool iCallCB ) {
    // Assertions
    ULIS2_ASSERT( iSrc, "Bad source" );
    FBlock* ret = new FBlock( iSrc->Width(), iSrc->Height(), iSrc->Format() );
    memcpy( ret->DataPtr(), iSrc->DataPtr(), iSrc->BytesTotal() );
    return  ret;
}


ULIS2_NAMESPACE_END

