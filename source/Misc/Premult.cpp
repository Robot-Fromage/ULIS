// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Extract.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Premult entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Premult.h"
#include "Base/HostDeviceInfo.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Thread/ThreadPool.h"

ULIS2_NAMESPACE_BEGIN
template< typename T >
void InvokesPremult( size_t iW, tByte* iDst, const FFormatInfo* iFmt ) {
    T* dst = reinterpret_cast< T* >( iDst );
    for( int i = 0; i < iW; ++i ) {
        T alpha = iFmt->HEA ? *( dst + iFmt->AID ) : MaxType< T >();
        for( int j = 0; j < iFmt->NCC; ++j ) {
            uint8 r = iFmt->IDT[j];
            *( dst + r ) = ( *( dst + r ) * alpha ) / MaxType< T >();
        }
        dst += iFmt->SPP;
    }
}

template< typename T >
void InvokesUnpremult( size_t iW, tByte* iDst, const FFormatInfo* iFmt ) {
    T* dst = reinterpret_cast< T* >( iDst );
    for( int i = 0; i < iW; ++i ) {
        T alpha = iFmt->HEA ? *( dst + iFmt->AID ) : MaxType< T >();
        for( int j = 0; j < iFmt->NCC; ++j ) {
            uint8 r = iFmt->IDT[j];
            *( dst + r ) = ( *( dst + r ) * MaxType< T >() ) / alpha;
        }
        dst += iFmt->SPP;
    }
}

typedef void (*fpDispatchedAlphamulInvoke)( size_t iW, tByte* iDst, const FFormatInfo* iFmt );
fpDispatchedAlphamulInvoke QueryDispatchedsPremultInvokeForParameters( eType iType ) {
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
Premultiply( FThreadPool*           iThreadPool
           , bool                   iBlocking
           , uint32                 iPerfIntent
           , const FHostDeviceInfo& iHostDeviceInfo
           , bool                   iCallCB
           , FBlock*                iDestination )
{
    // Assertions
    ULIS2_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS2_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS2_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    fpDispatchedAlphamulInvoke fptr = QueryDispatchedsPremultInvokeForParameters( iDestination->Type() );
    ULIS2_ASSERT( fptr, "No invocation found." );

    // Bake Params
    tByte*          dst = iDestination->DataPtr();
    size_t          bps = iDestination->BytesPerScanLine();
    const int       max = iDestination->Height();
    const size_t    len = iDestination->Width();
    ULIS2_ASSERT( fptr, "No dispatch invocation found." );
    ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , max
                                   , fptr, len, dst + ( pLINE * bps ), &iDestination->FormatInfo() )
    iDestination->Invalidate( iCallCB );
}


void
Unpremultiply( FThreadPool*             iThreadPool
             , bool                     iBlocking
             , uint32                   iPerfIntent
             , const FHostDeviceInfo&   iHostDeviceInfo
             , bool                     iCallCB
             , FBlock*                  iDestination )
{
    // Assertions
    ULIS2_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS2_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS2_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    fpDispatchedAlphamulInvoke fptr = QueryDispatchedUnpremultInvokeForParameters( iDestination->Type() );
    ULIS2_ASSERT( fptr, "No invocation found." );

    // Bake Params
    tByte*          dst = iDestination->DataPtr();
    size_t          bps = iDestination->BytesPerScanLine();
    const int       max = iDestination->Height();
    const size_t    len = iDestination->Width();
    ULIS2_ASSERT( fptr, "No dispatch invocation found." );
    ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , max
                                   , fptr, len, dst + ( pLINE * bps ), &iDestination->FormatInfo() )
    iDestination->Invalidate( iCallCB );
}


ULIS2_NAMESPACE_END

