// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Sanitize.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Sanitize entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Sanitize.h"
#include "System/Device.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Math.h"
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void InvokeSanitize( size_t iW, uint8* iDst, const FFormatMetrics& iFmt ) {
    T* dst = reinterpret_cast< T* >( iDst );
    const T zero = MinType< T >();
    for( int i = 0; i < iW; ++i ) {
        const T alpha = iFmt.HEA ? *( dst + iFmt.AID ) : MaxType< T >();
        for( int j = 0; j < iFmt.NCC; ++j ) {
            uint8 r = iFmt.IDT[j];
            *( dst + r ) = alpha == zero ? zero : *( dst + r );
        }
        dst += iFmt.SPP;
    }
}

typedef void (*fpDispatchedAlphamulInvoke)( size_t iW, uint8* iDst, const FFormatMetrics& iFmt );
fpDispatchedAlphamulInvoke QueryDispatchedSanitizeForParameters( eType iType ) {
        switch( iType ) {
        case TYPE_UINT8     : return  InvokeSanitize< uint8 >;
        case TYPE_UINT16    : return  InvokeSanitize< uint16 >;
        case TYPE_UINT32    : return  InvokeSanitize< uint32 >;
        case TYPE_UFLOAT    : return  InvokeSanitize< ufloat >;
        case TYPE_UDOUBLE   : return  InvokeSanitize< udouble >;
    }
    return  nullptr;
}

void
SanitizeZeroAlpha( FOldThreadPool*           iOldThreadPool
                 , bool                   iBlocking
                 , uint32                 iPerfIntent
                 , const FHardwareMetrics& iHostDeviceInfo
                 , bool                   iCallCB
                 , FBlock*                iDestination )
{
    // Assertions
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    fpDispatchedAlphamulInvoke fptr = QueryDispatchedSanitizeForParameters( iDestination->Type() );
    ULIS_ASSERT( fptr, "No invocation found." );

    // Bake Params
    uint8*          dst = iDestination->Bits();
    size_t          bps = iDestination->BytesPerScanLine();
    const int       max = iDestination->Height();
    const size_t    len = iDestination->Width();
    ULIS_ASSERT( fptr, "No dispatch invocation found." );
    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , max
                                   , fptr, len, dst + ( pLINE * bps ), iDestination->FormatMetrics() )
    iDestination->Dirty( iCallCB );
}

ULIS_NAMESPACE_END

