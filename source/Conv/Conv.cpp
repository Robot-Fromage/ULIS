// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Conv.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Conv entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Conv/Conv.h"
#include "Conv/ConvBuffer.h"
#include "Conv/srgb2linear.h"
#include "Copy/Copy.h"
#include "Data/Block.h"
#include "Data/Pixel.h"
#include "Maths/Maths.h"
#include "Thread/ThreadPool.h"
#include "lcms2.h"

ULIS3_NAMESPACE_BEGIN
void Conv( const IPixel& iSrc, IPixel& iDst ) {
    if( iSrc.Format() == iDst.Format() ) {
        iDst.AssignMemoryUnsafe( iSrc );
    } else {
        fpConversionInvocation fptr = QueryDispatchedConversionInvocation( iSrc.Format(), iDst.Format() );
        fptr( &iSrc.FormatInfo(), iSrc.Ptr(), &iDst.FormatInfo(), iDst.Ptr(), 1 );
    }
}

FPixelValue Conv( const IPixel& iSrc, tFormat iDst ) {
    FPixelValue dst( iDst );
    if( iSrc.Format() == iDst ) {
        dst.AssignMemoryUnsafe( iSrc );
    } else {
        fpConversionInvocation fptr = QueryDispatchedConversionInvocation( iSrc.Format(), iDst );
        fptr( &iSrc.FormatInfo(), iSrc.Ptr(), &dst.FormatInfo(), dst.Ptr(), 1 );
    }
    return  dst;
}

void Conv( FThreadPool*           iThreadPool
         , bool                   iBlocking
         , uint32                 iPerfIntent
         , const FHostDeviceInfo& iHostDeviceInfo
         , bool                   iCallCB
         , const FBlock*          iSource
         , FBlock*                iDestination )
{
    // Assertions
    ULIS3_ASSERT( iSource,                                      "Bad source."                                          );
    ULIS3_ASSERT( iDestination,                                 "Bad destination."                                     );
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool"                                              );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS3_ASSERT( iSource->Width()  == iDestination->Width(),   "Blocks sizes don't match"                              );
    ULIS3_ASSERT( iSource->Height() == iDestination->Height(),  "Blocks sizes don't match"                              );

    // Check no-op
    if( iSource == iDestination )
        return;

    // Check same format perform copy ( faster ).
    if( iSource->Format() == iDestination->Format() ) {
        Copy( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, ULIS3_NOCB, iSource, iDestination, iSource->Rect(), FVec2I() );
        return;
    }

    // Query dispatched method
    fpConversionInvocation fptr = QueryDispatchedConversionInvocation( iSource->Format(), iDestination->Format() );
    ULIS3_ASSERT( fptr, "No Conversion invocation found" );

    // Bake Params and call
    const tByte*    src = iSource->DataPtr();
    tByte*          dst = iDestination->DataPtr();
    tSize           src_bps = iSource->BytesPerScanLine();
    tSize           dst_bps = iDestination->BytesPerScanLine();
    const int   max = iSource->Height();
    const tSize len = iSource->Width();
    const FFormatInfo* srcnfo = &iSource->FormatInfo();
    const FFormatInfo* dstnfo = &iDestination->FormatInfo();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , max
                                   , fptr
                                   , srcnfo
                                   , src + ( pLINE * src_bps )
                                   , dstnfo
                                   , dst + ( pLINE * dst_bps )
                                   , len );

    // Invalid
    iDestination->Invalidate( iDestination->Rect(), iCallCB );
}

FBlock* XConv( FThreadPool*           iThreadPool
             , bool                   iBlocking
             , uint32                 iPerfIntent
             , const FHostDeviceInfo& iHostDeviceInfo
             , bool                   iCallCB
             , const FBlock*          iSource
             , tFormat                iDestinationFormat )
{
    // Assertions
    ULIS3_ASSERT( iSource,                                       "Bad source."                                          );
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool"                                              );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Alloc return buffer in desired format use the same size, then perform conversion
    FBlock* ret = new FBlock( iSource->Width(), iSource->Height(), iDestinationFormat );
    Conv( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, ret );
    return  ret;
}

ULIS3_NAMESPACE_END

