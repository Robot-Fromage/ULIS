// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
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
#include "Image/Block.h"
#include "Image/Pixel.h"
#include "Math/Math.h"
#include "Thread/OldThreadPool.h"
#include "lcms2.h"

ULIS_NAMESPACE_BEGIN
void Conv( const ISample& iSrc, ISample& iDst ) {
    if( iSrc.Format() == iDst.Format() ) {
        memcpy( iDst.Bits(), iSrc.Bits(), iDst.BytesPerPixel() );
    } else {
        fpConversionInvocation fptr = QueryDispatchedConversionInvocation( iSrc.Format(), iDst.Format() );
        fptr( iSrc.FormatMetrics(), iSrc.Bits(), iDst.FormatMetrics(), iDst.Bits(), 1 );
    }
}

FColor Conv( const ISample& iSrc, eFormat iDst ) {
    FColor dst( iDst );
    if( iSrc.Format() == iDst ) {
        memcpy( dst.Bits(), iSrc.Bits(), dst.BytesPerPixel() );
    } else {
        fpConversionInvocation fptr = QueryDispatchedConversionInvocation( iSrc.Format(), iDst );
        fptr( iSrc.FormatMetrics(), iSrc.Bits(), dst.FormatMetrics(), dst.Bits(), 1 );
    }
    return  dst;
}

void Conv(
      FOldThreadPool*           iOldThreadPool
    , bool                      iBlocking
    , uint32                    iPerfIntent
    , const FHostDeviceInfo&    iHostDeviceInfo
    , bool                      iCallCB
    , const FBlock*             iSource
    , FBlock*                   iDestination
)
{
    // Assertions
    ULIS_ASSERT( iSource,                                       "Bad source."                                           );
    ULIS_ASSERT( iDestination,                                  "Bad destination."                                      );
    ULIS_ASSERT( iOldThreadPool,                                "Bad pool"                                              );
    ULIS_ASSERT( !iCallCB || iBlocking,                         "Callback flag is specified on non-blocking operation." );
    ULIS_ASSERT( iSource->Width()  == iDestination->Width(),    "Blocks sizes don't match"                              );
    ULIS_ASSERT( iSource->Height() == iDestination->Height(),   "Blocks sizes don't match"                              );

    // Check no-op
    if( iSource == iDestination )
        return;

    // Check same format perform copy ( faster ).
    if( iSource->Format() == iDestination->Format() ) {
        Copy( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, ULIS_NOCB, iSource, iDestination, iSource->Rect(), FVec2I() );
        return;
    }

    // Query dispatched method
    fpConversionInvocation fptr = QueryDispatchedConversionInvocation( iSource->Format(), iDestination->Format() );
    ULIS_ASSERT( fptr, "No Conversion invocation found" );

    // Bake Params and call
    const uint8*    src     = iSource->Bits();
    uint8*          dst     = iDestination->Bits();
    uint32          src_bps = iSource->BytesPerScanLine();
    uint32          dst_bps = iDestination->BytesPerScanLine();
    const int       max     = iSource->Height();
    const uint32    len     = iSource->Width();
    const FFormatMetrics&  srcnfo  = iSource->FormatMetrics();
    const FFormatMetrics&  dstnfo  = iDestination->FormatMetrics();
    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , max
                                   , fptr
                                   , srcnfo
                                   , src + ( pLINE * src_bps )
                                   , dstnfo
                                   , dst + ( pLINE * dst_bps )
                                   , len );

    // Invalid
    iDestination->Dirty( iDestination->Rect(), iCallCB );
}

FBlock* XConv( FOldThreadPool*           iOldThreadPool
             , bool                   iBlocking
             , uint32                 iPerfIntent
             , const FHostDeviceInfo& iHostDeviceInfo
             , bool                   iCallCB
             , const FBlock*          iSource
             , eFormat                iDestinationFormat )
{
    // Assertions
    ULIS_ASSERT( iSource,                                       "Bad source."                                          );
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool"                                              );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Alloc return buffer in desired format use the same size, then perform conversion
    FBlock* ret = new FBlock( iSource->Width(), iSource->Height(), iDestinationFormat );
    Conv( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, ret );
    return  ret;
}

ULIS_NAMESPACE_END

