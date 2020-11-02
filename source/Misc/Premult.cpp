// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Premult.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Premult entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Premult.h"
#include "System/HostDeviceInfo.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Math.h"
#include "Thread/OldThreadPool.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void InvokesPremult( size_t iW, uint8* iDst, const FFormatMetrics& iFmt ) {
    T* dst = reinterpret_cast< T* >( iDst );
    for( int i = 0; i < iW; ++i ) {
        T alpha = iFmt.HEA ? *( dst + iFmt.AID ) : MaxType< T >();
        for( int j = 0; j < iFmt.NCC; ++j ) {
            uint8 r = iFmt.IDT[j];
            *( dst + r ) = ( *( dst + r ) * alpha ) / MaxType< T >();
        }
        dst += iFmt.SPP;
    }
}

template< typename T >
void InvokesUnpremult( size_t iW, uint8* iDst, const FFormatMetrics& iFmt ) {
    T* dst = reinterpret_cast< T* >( iDst );
    for( int i = 0; i < iW; ++i ) {
        T alpha = iFmt.HEA ? *( dst + iFmt.AID ) : MaxType< T >();
        for( int j = 0; j < iFmt.NCC; ++j ) {
            uint8 r = iFmt.IDT[j];
            *( dst + r ) = ( *( dst + r ) * MaxType< T >() ) / alpha;
        }
        dst += iFmt.SPP;
    }
}

typedef void (*fpDispatchedAlphamulInvoke)( size_t iW, uint8* iDst, const FFormatMetrics& iFmt );
fpDispatchedAlphamulInvoke QueryDispatchedPremultInvokeForParameters( eType iType ) {
    switch( iType ) {
        case TYPE_UINT8     : return  InvokesPremult< uint8 >;
        case TYPE_UINT16    : return  InvokesPremult< uint16 >;
        case TYPE_UINT32    : return  InvokesPremult< uint32 >;
        case TYPE_UFLOAT    : return  InvokesPremult< ufloat >;
        case TYPE_UDOUBLE   : return  InvokesPremult< udouble >;
    }
    return  nullptr;
}

fpDispatchedAlphamulInvoke QueryDispatchedUnpremultInvokeForParameters( eType iType ) {
    switch( iType ) {
        case TYPE_UINT8     : return  InvokesUnpremult< uint8 >;
        case TYPE_UINT16    : return  InvokesUnpremult< uint16 >;
        case TYPE_UINT32    : return  InvokesUnpremult< uint32 >;
        case TYPE_UFLOAT    : return  InvokesUnpremult< ufloat >;
        case TYPE_UDOUBLE   : return  InvokesUnpremult< udouble >;
    }
    return  nullptr;
}


void
Premultiply( FOldThreadPool*           iOldThreadPool
           , bool                   iBlocking
           , uint32                 iPerfIntent
           , const FHostDeviceInfo& iHostDeviceInfo
           , bool                   iCallCB
           , FBlock*                iDestination )
{
    // Assertions
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    fpDispatchedAlphamulInvoke fptr = QueryDispatchedPremultInvokeForParameters( iDestination->Type() );
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


void
Unpremultiply( FOldThreadPool*             iOldThreadPool
             , bool                     iBlocking
             , uint32                   iPerfIntent
             , const FHostDeviceInfo&   iHostDeviceInfo
             , bool                     iCallCB
             , FBlock*                  iDestination )
{
    // Assertions
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    fpDispatchedAlphamulInvoke fptr = QueryDispatchedUnpremultInvokeForParameters( iDestination->Type() );
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

