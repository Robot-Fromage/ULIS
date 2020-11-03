// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Extract.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Extract entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Trim.h"
#include "Base/HostDeviceInfo.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Thread/ThreadPool.h"
#include <atomic>

ULIS3_NAMESPACE_BEGIN

template< typename T >
void InvokeDetectTrimAlphaEdge( int32 iLine, size_t iW, const tByte* iSrc, const FFormatInfo* iFmt, std::atomic_int* iLeft, std::atomic_int* iTop, std::atomic_int* iRight, std::atomic_int* iBot ) {
    const T* src = reinterpret_cast< const T* >( iSrc );
    for( int i = 0; i < iW; ++i ) {
        if( *( src + iFmt->AID ) > MinType< T >() ) {
            if( iLine < iTop->load() )    iTop->store( iLine );
            if( i < iLeft->load() )       iLeft->store( i );
            if( iLine > iBot->load() )    iBot->store( iLine );
            if( i > iRight->load() )      iRight->store( i );
        }
        src += iFmt->SPP;
    }
}

typedef void (*fpDispatchedDetectTrimAlphaEdgeInvoke)( int32 iLine, size_t iW, const tByte* iSrc, const FFormatInfo* iFmt, std::atomic_int* iLeft, std::atomic_int* iTop, std::atomic_int* iRight, std::atomic_int* iBot );
fpDispatchedDetectTrimAlphaEdgeInvoke QueryDispatchedDetectTrimAlphaEdgeInvokeForParameters( eType iType ) {
        switch( iType ) {
        case TYPE_UINT8     : return  InvokeDetectTrimAlphaEdge< uint8 >;
        case TYPE_UINT16    : return  InvokeDetectTrimAlphaEdge< uint16>;
        case TYPE_UINT32    : return  InvokeDetectTrimAlphaEdge< uint32 >;
        case TYPE_UFLOAT    : return  InvokeDetectTrimAlphaEdge< ufloat >;
        case TYPE_UDOUBLE   : return  InvokeDetectTrimAlphaEdge< udouble >;
    }
    return  nullptr;
}

FRect GetTrimmedTransparencyRect( FThreadPool*            iThreadPool
                                , bool                    iBlocking
                                , uint32                  iPerfIntent
                                , const FHostDeviceInfo&  iHostDeviceInfo
                                , bool                    iCallCB
                                , const FBlock*           iSource )
{
    // Assertions
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Format info
    const FFormatInfo& fmt( iSource->FormatInfo() );

    if( !fmt.HEA )
        return  iSource->Rect();

    const int len = iSource->Width();
    const int max = iSource->Height();
    const tByte* src = iSource->DataPtr();
    std::atomic_int left( INT_MAX );
    std::atomic_int top( INT_MAX );
    std::atomic_int right( 0 );
    std::atomic_int bot( 0 );

    fpDispatchedDetectTrimAlphaEdgeInvoke fptr = QueryDispatchedDetectTrimAlphaEdgeInvokeForParameters( iSource->Type() );
    ULIS3_ASSERT( fptr, "No invocation found." );
    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, ULIS3_BLOCKING
                                   , max
                                   , fptr, pLINE, len, src, &fmt, &left, &top, &right, &bot );
    iSource->Invalidate( iCallCB );
    return  FRect( left, top, ( right - left ) + 1, ( bot - top ) + 1 );
}

ULIS3_NAMESPACE_END

