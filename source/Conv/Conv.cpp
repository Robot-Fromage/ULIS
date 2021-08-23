// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Conv.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Conv entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Conv/Conv.h"
#include "Conv/ConvBuffer.h"
#include "Copy/Copy.h"
#include "Data/Pixel.h"
#include "Data/Block.h"
#include "Maths/Maths.h"
#include "Color/ModelStructs.h"
#include "Color/srgb2linear.h"
#include "Thread/ThreadPool.h"
#include "lcms2.h"

ULIS2_NAMESPACE_BEGIN
void Conv( const IPixel& iSrc, IPixel& iDst ) {
    fpDispatchedConvInvoke fptr = QueryDispatchedConvInvokeForParameters( iSrc.Format(), iDst.Format() );
    fptr( &iSrc.FormatInfo(), iSrc.Ptr(), &iDst.FormatInfo(), iDst.Ptr(), 1 );
}

FPixelValue Conv( const IPixel& iSrc, tFormat iDst ) {
    FPixelValue dst( iDst );
    fpDispatchedConvInvoke fptr = QueryDispatchedConvInvokeForParameters( iSrc.Format(), iDst );
    fptr( &iSrc.FormatInfo(), iSrc.Ptr(), &dst.FormatInfo(), dst.Ptr(), 1 );
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
    ULIS2_ASSERT( iSource,                                      "Bad source."                                          );
    ULIS2_ASSERT( iDestination,                                 "Bad destination."                                     );
    ULIS2_ASSERT( iThreadPool,                                  "Bad pool"                                              );
    ULIS2_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS2_ASSERT( iSource->Width()  == iDestination->Width(),   "Blocks sizes don't match"                              );
    ULIS2_ASSERT( iSource->Height() == iDestination->Height(),  "Blocks sizes don't match"                              );

    // Check no-op
    if( iSource == iDestination )
        return;

    // Check same format perform copy ( faster ).
    if( iSource->Format() == iDestination->Format() ) {
        Copy( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, ULIS2_NOCB, iSource, iDestination, iSource->Rect(), FVec2I() );
        return;
    }

    // Query dispatched method
    fpDispatchedConvInvoke fptr = QueryDispatchedConvInvokeForParameters( iSource->Format(), iDestination->Format() );
    ULIS2_ASSERT( fptr, "No Conversion invocation found" );

    // Bake Params and call
    const tByte*    src = iSource->DataPtr();
    tByte*          dst = iDestination->DataPtr();
    tSize           src_bps = iSource->BytesPerScanLine();
    tSize           dst_bps = iDestination->BytesPerScanLine();
    const int   max = iSource->Height();
    const tSize len = iSource->Width();
    const FFormatInfo* srcnfo = &iSource->FormatInfo();
    const FFormatInfo* dstnfo = &iDestination->FormatInfo();
    ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
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
    ULIS2_ASSERT( iSource,                                       "Bad source."                                          );
    ULIS2_ASSERT( iThreadPool,                                  "Bad pool"                                              );
    ULIS2_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Alloc return buffer in desired format use the same size, then perform conversion
    FBlock* ret = new FBlock( iSource->Width(), iSource->Height(), iDestinationFormat );
    Conv( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, ret );
    return  ret;
}

ULIS2_NAMESPACE_END

