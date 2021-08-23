// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Sanitize.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Sanitize entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Sanitize.h"
#include "Base/HostDeviceInfo.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN
template< typename T >
void InvokeSanitize( size_t iW, tByte* iDst, const FFormatInfo* iFmt ) {
    T* dst = reinterpret_cast< T* >( iDst );
    const T zero = MinType< T >();
    for( int i = 0; i < iW; ++i ) {
        const T alpha = iFmt->HEA ? *( dst + iFmt->AID ) : MaxType< T >();
        for( int j = 0; j < iFmt->NCC; ++j ) {
            uint8 r = iFmt->IDT[j];
            *( dst + r ) = alpha == zero ? zero : *( dst + r );
        }
        dst += iFmt->SPP;
    }
}

typedef void (*fpDispatchedAlphamulInvoke)( size_t iW, tByte* iDst, const FFormatInfo* iFmt );
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
SanitizeZeroAlpha( FThreadPool*           iThreadPool
                 , bool                   iBlocking
                 , uint32                 iPerfIntent
                 , const FHostDeviceInfo& iHostDeviceInfo
                 , bool                   iCallCB
                 , FBlock*                iDestination )
{
    // Assertions
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    fpDispatchedAlphamulInvoke fptr = QueryDispatchedSanitizeForParameters( iDestination->Type() );
    ULIS3_ASSERT( fptr, "No invocation found." );

    // Bake Params
    tByte*          dst = iDestination->DataPtr();
    size_t          bps = iDestination->BytesPerScanLine();
    const int       max = iDestination->Height();
    const size_t    len = iDestination->Width();
    ULIS3_ASSERT( fptr, "No dispatch invocation found." );
    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , max
                                   , fptr, len, dst + ( pLINE * bps ), &iDestination->FormatInfo() )
    iDestination->Invalidate( iCallCB );
}

ULIS3_NAMESPACE_END

