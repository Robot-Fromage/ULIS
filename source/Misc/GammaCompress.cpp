// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Extract.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the GammaCompress entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/GammaCompress.h"
#include "System/Device.h"
#include "Conv/srgb2linear.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Math.h"
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void InvokesRGB2Linear( size_t iW, uint8* iDst, const FFormatMetrics& iFmt ) {
    T* dst = reinterpret_cast< T* >( iDst );
    for( int i = 0; i < iW; ++i ) {
        for( int j = 0; j < iFmt.NCC; ++j ) {
            uint8 r = iFmt.IDT[j];
            *( dst + r ) = sel_srgb2linearT< T >( *( dst + r ) );
        }
        dst += iFmt.SPP;
    }
}

template< typename T >
void InvokeLinear2sRGB( size_t iW, uint8* iDst, const FFormatMetrics& iFmt ) {
    T* dst = reinterpret_cast< T* >( iDst );
    for( int i = 0; i < iW; ++i ) {
        for( int j = 0; j < iFmt.NCC; ++j ) {
            uint8 r = iFmt.IDT[j];
            *( dst + r ) = sel_linear2srgbT< T >( *( dst + r ) );
        }
        dst += iFmt.SPP;
    }
}

typedef void (*fpDispatchedGammaCompressInvoke)( size_t iW, uint8* iDst, const FFormatMetrics& iFmt );
fpDispatchedGammaCompressInvoke QueryDispatchedsRGB2LinearInvokeForParameters( eType iType ) {
    switch( iType ) {
        case Type_uint8     : return  InvokesRGB2Linear< uint8 >;
        case Type_uint16    : return  InvokesRGB2Linear< uint16 >;
        case Type_uint32    : return  InvokesRGB2Linear< uint32 >;
        case Type_ufloat    : return  InvokesRGB2Linear< ufloat >;
        case TYPE_UDOUBLE   : return  InvokesRGB2Linear< udouble >;
    }
    return  nullptr;
}

fpDispatchedGammaCompressInvoke QueryDispatchedLinear2sRGBInvokeForParameters( eType iType ) {
    switch( iType ) {
        case Type_uint8     : return  InvokeLinear2sRGB< uint8 >;
        case Type_uint16    : return  InvokeLinear2sRGB< uint16 >;
        case Type_uint32    : return  InvokeLinear2sRGB< uint32 >;
        case Type_ufloat    : return  InvokeLinear2sRGB< ufloat >;
        case TYPE_UDOUBLE   : return  InvokeLinear2sRGB< udouble >;
    }
    return  nullptr;
}


void
ApplysRGB2Linear( FOldThreadPool*           iOldThreadPool
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

    fpDispatchedGammaCompressInvoke fptr = QueryDispatchedsRGB2LinearInvokeForParameters( iDestination->Type() );
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
ApplyLinear2sRGB( FOldThreadPool*           iOldThreadPool
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

    fpDispatchedGammaCompressInvoke fptr = QueryDispatchedLinear2sRGBInvokeForParameters( iDestination->Type() );
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

