// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         FillPreserveAlpha.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the FillPreserveAlpha entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Fill/FillPreserveAlpha.h"
#include "System/Device.h"
#include "Conv/Conv.h"
#include "Image/Block.h"
#include "Image/Pixel.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocation Implementation
template< typename T >
void InvokeFillPreserveAlpha( size_t iW, uint8* iDst, const FFormatMetrics& iFmt, std::shared_ptr< FColor > iColor ) {
    const uint8* src = iColor->Bits();
    T* dst = reinterpret_cast< T* >( iDst );
    for( int i = 0; i < iW; ++i ) {
        const T alpha = dst[ iFmt.AID ];
        memcpy( dst, src, iFmt.BPP );
        dst[ iFmt.AID ] = alpha;
        dst += iFmt.SPP;
    }
}

/////////////////////////////////////////////////////
// Dispatch
typedef void (*fpDispatchedFillPreserveAlphaInvoke)( size_t iW, uint8* iDst, const FFormatMetrics& iFmt, std::shared_ptr< FColor > iColor );
fpDispatchedFillPreserveAlphaInvoke QueryDispatchedFillPreserveAlphaInvokeForParameters( eType iType ) {
    switch( iType ) {
        case TYPE_UINT8     : return  InvokeFillPreserveAlpha< uint8 >;
        case TYPE_UINT16    : return  InvokeFillPreserveAlpha< uint16 >;
        case TYPE_UINT32    : return  InvokeFillPreserveAlpha< uint32 >;
        case TYPE_UFLOAT    : return  InvokeFillPreserveAlpha< ufloat >;
        case TYPE_UDOUBLE   : return  InvokeFillPreserveAlpha< udouble >;
    }
    return  nullptr;
}

/////////////////////////////////////////////////////
// FillPreserveAlpha
void
FillPreserveAlpha( FOldThreadPool*             iOldThreadPool
                 , bool                     iBlocking
                 , uint32                   iPerfIntent
                 , const FHostDeviceInfo&   iHostDeviceInfo
                 , bool                     iCallCB
                 , FBlock*                  iDestination
                 , const ISample&            iColor
                 , const FRectI&             iArea )
{
    // Assertions
    ULIS_ASSERT( iDestination,             "Bad source."                                           );
    ULIS_ASSERT( iOldThreadPool,              "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );

    if( !( iDestination->HasAlpha() ) )
        return;

    // Fit region of interest
    FRectI roi = iArea & iDestination->Rect();

    // Check no-op
    if( roi.Area() <= 0 )
        return;

    // Query
    fpDispatchedFillPreserveAlphaInvoke fptr = QueryDispatchedFillPreserveAlphaInvokeForParameters( iDestination->Type() );
    ULIS_ASSERT( fptr, "No invocation found." );

    // Bake color param, shared Ptr for thread safety and scope life time extension in non blocking multithreaded processing
    std::shared_ptr< FColor > color = std::make_shared< FColor >( iDestination->Format() );
    Conv( iColor, *color );
    uint8*          dst = iDestination->Bits();
    size_t          bps = iDestination->BytesPerScanLine();
    const int       max = roi.h;
    const size_t    len = roi.w;

    // Call
    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , max
                                   , fptr, len, dst + ( ( roi.y + pLINE ) * bps ) + roi.x, iDestination->FormatMetrics(), color )

    // Invalid
    iDestination->Dirty( roi, iCallCB );
}

ULIS_NAMESPACE_END

