// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Extract.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Extract entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Trim.h"
#include "System/Device.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Math.h"
#include "Thread/ThreadPool.h"
#include <atomic>

ULIS_NAMESPACE_BEGIN

template< typename T >
void InvokeDetectTrimAlphaEdge( int32 iLine, size_t iW, const uint8* iSrc, const FFormatMetrics& iFmt, std::atomic_int* iLeft, std::atomic_int* iTop, std::atomic_int* iRight, std::atomic_int* iBot ) {
    const T* src = reinterpret_cast< const T* >( iSrc );
    for( int i = 0; i < iW; ++i ) {
        if( *( src + iFmt.AID ) > MinType< T >() ) {
            if( iLine < iTop->load() )    iTop->store( iLine );
            if( i < iLeft->load() )       iLeft->store( i );
            if( iLine > iBot->load() )    iBot->store( iLine );
            if( i > iRight->load() )      iRight->store( i );
        }
        src += iFmt.SPP;
    }
}

typedef void (*fpDispatchedDetectTrimAlphaEdgeInvoke)( int32 iLine, size_t iW, const uint8* iSrc, const FFormatMetrics& iFmt, std::atomic_int* iLeft, std::atomic_int* iTop, std::atomic_int* iRight, std::atomic_int* iBot );
fpDispatchedDetectTrimAlphaEdgeInvoke QueryDispatchedDetectTrimAlphaEdgeInvokeForParameters( eType iType ) {
        switch( iType ) {
        case Type_uint8     : return  InvokeDetectTrimAlphaEdge< uint8 >;
        case Type_uint16    : return  InvokeDetectTrimAlphaEdge< uint16>;
        case Type_uint32    : return  InvokeDetectTrimAlphaEdge< uint32 >;
        case Type_ufloat    : return  InvokeDetectTrimAlphaEdge< ufloat >;
        case TYPE_UDOUBLE   : return  InvokeDetectTrimAlphaEdge< udouble >;
    }
    return  nullptr;
}

FRectI GetTrimmedTransparencyRect( FOldThreadPool*            iOldThreadPool
                                , bool                    iBlocking
                                , uint32                  iPerfIntent
                                , const FHardwareMetrics&  iHostDeviceInfo
                                , bool                    iCallCB
                                , const FBlock*           iSource )
{
    // Assertions
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Format info
    const FFormatMetrics& fmt( iSource->FormatMetrics() );

    if( !fmt.HEA )
        return  iSource->Rect();

    const int len = iSource->Width();
    const int max = iSource->Height();
    const uint8* src = iSource->Bits();
    std::atomic_int left( INT_MAX );
    std::atomic_int top( INT_MAX );
    std::atomic_int right( 0 );
    std::atomic_int bot( 0 );

    fpDispatchedDetectTrimAlphaEdgeInvoke fptr = QueryDispatchedDetectTrimAlphaEdgeInvokeForParameters( iSource->Type() );
    ULIS_ASSERT( fptr, "No invocation found." );
    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, ULIS_BLOCKING
                                   , max
                                   , fptr, pLINE, len, src, fmt, &left, &top, &right, &bot );
    iSource->Dirty( iCallCB );
    return  FRectI( left, top, ( right - left ) + 1, ( bot - top ) + 1 );
}

ULIS_NAMESPACE_END

