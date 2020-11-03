// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Extract.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the GammaCompress entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/GammaCompress.h"
#include "Base/HostDeviceInfo.h"
#include "Conv/srgb2linear.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN
template< typename T >
void InvokesRGB2Linear( size_t iW, tByte* iDst, const FFormatInfo* iFmt ) {
    T* dst = reinterpret_cast< T* >( iDst );
    for( int i = 0; i < iW; ++i ) {
        for( int j = 0; j < iFmt->NCC; ++j ) {
            uint8 r = iFmt->IDT[j];
            *( dst + r ) = sel_srgb2linearT< T >( *( dst + r ) );
        }
        dst += iFmt->SPP;
    }
}

template< typename T >
void InvokeLinear2sRGB( size_t iW, tByte* iDst, const FFormatInfo* iFmt ) {
    T* dst = reinterpret_cast< T* >( iDst );
    for( int i = 0; i < iW; ++i ) {
        for( int j = 0; j < iFmt->NCC; ++j ) {
            uint8 r = iFmt->IDT[j];
            *( dst + r ) = sel_linear2srgbT< T >( *( dst + r ) );
        }
        dst += iFmt->SPP;
    }
}

typedef void (*fpDispatchedGammaCompressInvoke)( size_t iW, tByte* iDst, const FFormatInfo* iFmt );
fpDispatchedGammaCompressInvoke QueryDispatchedsRGB2LinearInvokeForParameters( eType iType ) {
    switch( iType ) {
        case TYPE_UINT8     : return  InvokesRGB2Linear< uint8 >;
        case TYPE_UINT16    : return  InvokesRGB2Linear< uint16 >;
        case TYPE_UINT32    : return  InvokesRGB2Linear< uint32 >;
        case TYPE_UFLOAT    : return  InvokesRGB2Linear< ufloat >;
        case TYPE_UDOUBLE   : return  InvokesRGB2Linear< udouble >;
    }
    return  nullptr;
}

fpDispatchedGammaCompressInvoke QueryDispatchedLinear2sRGBInvokeForParameters( eType iType ) {
    switch( iType ) {
        case TYPE_UINT8     : return  InvokeLinear2sRGB< uint8 >;
        case TYPE_UINT16    : return  InvokeLinear2sRGB< uint16 >;
        case TYPE_UINT32    : return  InvokeLinear2sRGB< uint32 >;
        case TYPE_UFLOAT    : return  InvokeLinear2sRGB< ufloat >;
        case TYPE_UDOUBLE   : return  InvokeLinear2sRGB< udouble >;
    }
    return  nullptr;
}


void
ApplysRGB2Linear( FThreadPool*           iThreadPool
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

    fpDispatchedGammaCompressInvoke fptr = QueryDispatchedsRGB2LinearInvokeForParameters( iDestination->Type() );
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


void
ApplyLinear2sRGB( FThreadPool*           iThreadPool
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

    fpDispatchedGammaCompressInvoke fptr = QueryDispatchedLinear2sRGBInvokeForParameters( iDestination->Type() );
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

