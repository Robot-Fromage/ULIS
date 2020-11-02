// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Copy.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Copy entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Copy/Copy.h"
#include "System/Device.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Thread/ThreadPool.h"


ULIS_NAMESPACE_BEGIN

#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
void InvokeCopyMTProcessScanline_AX2( uint8* iDst, const uint8* iSrc, const uint32 iCount, const uint32 iStride )
{
    uint32 index;
    for( index = 0; index < ( iCount - 32 ); index += iStride )
    {
        _mm256_storeu_si256( (__m256i*)iDst, _mm256_loadu_si256( (const __m256i*)iSrc ) );
        iSrc += iStride;
        iDst += iStride;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memcpy( iDst, iSrc, iCount - index );
}
#endif // ULIS_COMPILETIME_AVX2_SUPPORT

#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
void InvokeCopyMTProcessScanline_SSE( uint8* iDst, const uint8* iSrc, const uint32 iCount, const uint32 iStride )
{
    uint32 index;
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

void InvokeCopyMTProcessScanline_MEM( uint8* iDst, const uint8* iSrc, uint32 iCount )
{
    memcpy( iDst, iSrc, iCount );
}


void
Copy_imp( FOldThreadPool*              iOldThreadPool
        , bool                      iBlocking
        , uint32                    iPerfIntent
        , const FHardwareMetrics&    iHostDeviceInfo
        , bool                      iCallCB
        , const FBlock*             iSource
        , FBlock*                   iDestination
        , const FRectI&              iSrcROI
        , const FRectI&              iDstROI
        , const FVec2I&             iShift )
{
    const uint32 bpp = iDestination->BytesPerPixel();
    const uint32 src_bps = iSource->BytesPerScanLine();
    const uint32 dst_bps = iDestination->BytesPerScanLine();
    const uint32 srh = ( iShift.x + iSrcROI.x ) * bpp;
    const uint32 dsh = iDstROI.x * bpp;
    const uint8*src = iSource->Bits() + srh;
    uint8*      dst = iDestination->Bits() + dsh;
    const auto basesrcy = iShift.y + iSrcROI.y;
    const auto basedsty = iDstROI.y;
    #define SRC src + ( ( basesrcy + pLINE ) * src_bps )
    #define DST dst + ( ( basedsty + pLINE ) * dst_bps )
    const uint32 count = iDstROI.w * bpp;
    #ifdef ULIS_COMPILETIME_AVX2_SUPPORT
    if( ( iPerfIntent & ULIS_PERF_AVX2 ) && iHostDeviceInfo.HW_AVX2 && ( src_bps + dst_bps ) >= 64 ) {
        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeCopyMTProcessScanline_AX2, DST, SRC, count, 32 )
    } else
    #endif
    #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
    if( ( iPerfIntent & ULIS_PERF_SSE42 ) && iHostDeviceInfo.HW_SSE42 && ( src_bps + dst_bps ) >= 32 ) {
        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeCopyMTProcessScanline_SSE, DST, SRC, count, 16 )
    } else
    #endif
    {
        ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                       , iDstROI.h
                                       , InvokeCopyMTProcessScanline_MEM, DST, SRC, count )
    }
}


void Copy( FOldThreadPool*             iOldThreadPool
         , bool                     iBlocking
         , uint32                   iPerfIntent
         , const FHardwareMetrics&   iHostDeviceInfo
         , bool                     iCallCB
         , const FBlock*            iSource
         , FBlock*                  iDestination
         , const FRectI&             iArea
         , const FVec2I&            iPos )
{
    // Assertions
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match"                                  );

    // Compute coordinates of target rect in destination, with source rect dimension
    // Ensure the selected target actually fits in destination
    FRectI src_roi = iArea & iSource->Rect();
    int target_xmin = iPos.x;
    int target_ymin = iPos.y;
    int target_xmax = target_xmin + src_roi.w;
    int target_ymax = target_ymin + src_roi.h;
    FRectI dst_target = FRectI::FromMinMax( target_xmin, target_ymin, target_xmax, target_ymax );

    // Ensure the selected target actually fits in destination
    FRectI dst_fit = dst_target & iDestination->Rect();
    if( dst_fit.Area() <= 0 ) return;

    // Forward arguments baking
    const int   translationX        = dst_fit.x - dst_target.x;
    const int   translationY        = dst_fit.y - dst_target.y;
    const FVec2I shift( translationX, translationY );

    // Call
    Copy_imp( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, iDestination, src_roi, dst_fit, shift );

    // Invalidate
    iDestination->Dirty( dst_fit, iCallCB );
}

FBlock* XCopy( FOldThreadPool*           iOldThreadPool
             , bool                   iBlocking
             , uint32                 iPerfIntent
             , const FHardwareMetrics& iHostDeviceInfo
             , bool                   iCallCB
             , const FBlock*          iSource
             , const FRectI&           iArea )
{
    ULIS_ASSERT( iSource, "Bad source." );

    FRectI src_roi = iArea & iSource->Rect();

    if( src_roi.Area() <= 0 )
        return  nullptr;

    FBlock* ret = new FBlock( src_roi.w, src_roi.h, iSource->Format() );
    Copy( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, ret, src_roi, FVec2I( 0, 0 ) );
    return  ret;
}

void
CopyRaw( const FBlock* iSrc, FBlock* iDst, bool iCallCB ) {
    // Assertions
    ULIS_ASSERT( iSrc,                             "Bad source"                                );
    ULIS_ASSERT( iDst,                             "Bad destination"                           );
    ULIS_ASSERT( iSrc != iDst,                     "Destination and source cannot be the same" );
    ULIS_ASSERT( iSrc->Format() == iDst->Format(), "Formats do not matchs"                     );

    // One call, supposedly more efficient for small block.
    memcpy( iDst->Bits(), iSrc->Bits(), iSrc->BytesTotal() );

    // Invalid
    iDst->Dirty( iCallCB );
}

FBlock*
XCopyRaw( const FBlock* iSrc, bool iCallCB ) {
    // Assertions
    ULIS_ASSERT( iSrc, "Bad source" );
    FBlock* ret = new FBlock( iSrc->Width(), iSrc->Height(), iSrc->Format() );
    memcpy( ret->Bits(), iSrc->Bits(), iSrc->BytesTotal() );
    return  ret;
}


ULIS_NAMESPACE_END

