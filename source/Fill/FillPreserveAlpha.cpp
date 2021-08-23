// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         FillPreserveAlpha.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the FillPreserveAlpha entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Fill/FillPreserveAlpha.h"
#include "Base/HostDeviceInfo.h"
#include "Conv/Conv.h"
#include "Data/Block.h"
#include "Data/Pixel.h"
#include "Maths/Geometry.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocation Implementation
template< typename T >
void InvokeFillPreserveAlpha( size_t iW, tByte* iDst, const FFormatInfo* iFmt, std::shared_ptr< FPixelValue > iColor ) {
    const tByte* src = iColor->Ptr();
    T* dst = reinterpret_cast< T* >( iDst );
    for( int i = 0; i < iW; ++i ) {
        const T alpha = dst[ iFmt->AID ];
        memcpy( dst, src, iFmt->BPP );
        dst[ iFmt->AID ] = alpha;
        dst += iFmt->SPP;
    }
}

/////////////////////////////////////////////////////
// Dispatch
typedef void (*fpDispatchedFillPreserveAlphaInvoke)( size_t iW, tByte* iDst, const FFormatInfo* iFmt, std::shared_ptr< FPixelValue > iColor );
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
FillPreserveAlpha( FThreadPool*             iThreadPool
                 , bool                     iBlocking
                 , uint32                   iPerfIntent
                 , const FHostDeviceInfo&   iHostDeviceInfo
                 , bool                     iCallCB
                 , FBlock*                  iDestination
                 , const IPixel&            iColor
                 , const FRect&             iArea )
{
    // Assertions
    ULIS3_ASSERT( iDestination,             "Bad source."                                           );
    ULIS3_ASSERT( iThreadPool,              "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );

    if( !( iDestination->HasAlpha() ) )
        return;

    // Fit region of interest
    FRect roi = iArea & iDestination->Rect();

    // Check no-op
    if( roi.Area() <= 0 )
        return;

    // Query
    fpDispatchedFillPreserveAlphaInvoke fptr = QueryDispatchedFillPreserveAlphaInvokeForParameters( iDestination->Type() );
    ULIS3_ASSERT( fptr, "No invocation found." );

    // Bake color param, shared Ptr for thread safety and scope life time extension in non blocking multithreaded processing
    std::shared_ptr< FPixelValue > color = std::make_shared< FPixelValue >( iDestination->Format() );
    Conv( iColor, *color );
    tByte*          dst = iDestination->DataPtr();
    size_t          bps = iDestination->BytesPerScanLine();
    const int       max = roi.h;
    const size_t    len = roi.w;

    // Call
    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , max
                                   , fptr, len, dst + ( ( roi.y + pLINE ) * bps ) + roi.x, &iDestination->FormatInfo(), color )

    // Invalid
    iDestination->Invalidate( roi, iCallCB );
}

ULIS3_NAMESPACE_END

