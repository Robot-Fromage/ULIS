// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Dispatch_SAT.ipp.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the SummedAreaTable entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/SummedAreaTable.h"
#include "System/Device.h"
#include "Misc/Filter.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Math.h"
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN

template< typename T >
void 
InvokeComputeSummedAreaTable_XPass_MEM_Generic( const uint32 iLen, FBlock* iSAT, const uint8* iSrc, uint8* iDst ) {
    const FFormatMetrics& fmt = iSAT->FormatMetrics();
    const T* src = reinterpret_cast< const T* >( iSrc )  + fmt.SPP;
    float*   dst = reinterpret_cast< float* >( iDst )    + fmt.SPP;

    for( uint32 x = 1; x < iLen; ++x ) {
        for( uint8 j = 0; j < fmt.SPP; ++j )
            dst[j] = static_cast< float >( src[j] + *( dst - fmt.SPP + j ) );
        src += fmt.SPP;
        dst += fmt.SPP;
    }
}

template< typename T >
void 
InvokeComputeSummedAreaTable_YPass_MEM_Generic( const uint32 iLen, FBlock* iSAT, uint8* iDst ) {
    const FFormatMetrics& fmt = iSAT->FormatMetrics();
    const uint32 stride = iSAT->Width() * fmt.SPP;
    float* dst = reinterpret_cast< float* >( iDst ) + stride;

    for( uint32 y = 1; y < iLen; ++y ) {
        for( uint8 j = 0; j < fmt.SPP; ++j )
            dst[j] = static_cast< float >( dst[j] + *( dst - stride + j ) );
        dst += stride;
    }
}

template< typename T >
void ComputeSummedAreaTable_MEM_Generic( FOldThreadPool*           iOldThreadPool
                                   , bool                       iBlocking
                                   , uint32                     iPerfIntent
                                   , const FHardwareMetrics&     iHostDeviceInfo
                                   , const FBlock*              iSource
                                   , FBlock*                    iSAT )
{
    const uint8*    src     = iSource->Bits();
    uint8*          bdp     = iSAT->Bits();
    const uint32     src_bps = iSource->BytesPerScanLine();
    const uint32     bdp_bps = iSAT->BytesPerScanLine();
    const uint32     src_bpp = iSource->BytesPerPixel();
    const uint32     bdp_bpp = iSAT->BytesPerPixel();
    const int       w       = iSource->Width();
    const int       h       = iSource->Height();

    {
        const FFormatMetrics& fmt = iSAT->FormatMetrics();
        const T* wsrc = reinterpret_cast< const T* >( src );
        float*   wdst = reinterpret_cast< float* >( bdp );
        const uint32 stride = iSAT->Width()   * fmt.SPP;
        for( int y = 0; y < h; ++y ) {
            for( uint8 j = 0; j < iSource->SamplesPerPixel(); ++j )
                wdst[j] = static_cast< float >( wsrc[j] );
            wsrc += stride;
            wdst += stride;
        }
    }

    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , h
                                   , InvokeComputeSummedAreaTable_XPass_MEM_Generic< T >
                                   , w, iSAT
                                   , src + pLINE * src_bps
                                   , bdp + pLINE * bdp_bps );
    iOldThreadPool->WaitForCompletion();
    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , w
                                   , InvokeComputeSummedAreaTable_YPass_MEM_Generic< T >
                                   , h, iSAT
                                   , bdp + pLINE * bdp_bpp );
}

#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
void 
InvokeComputeSummedAreaTable_XPass_SSE42_RGBA8( const uint32 iLen, const uint8* iSrc, uint8* iDst ) {
    const uint8* src = reinterpret_cast< const uint8* >( iSrc ) + 4;
    float*       dst = reinterpret_cast< float* >( iDst )       + 4;
    for( uint32 x = 1; x < iLen; ++x ) {
        __m128 n = _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( reinterpret_cast< const __m128i* >( src ) ) ) );
        __m128 m = _mm_loadu_ps( dst - 4 );
        __m128 r = _mm_add_ps( n, m );
        _mm_storeu_ps( dst, r );
        src += 4;
        dst += 4;
    }
}

void 
InvokeComputeSummedAreaTable_YPass_SSE42_RGBA8( const uint32 iLen, const FBlock* iSource, FBlock* iSAT, const uint8* iSrc, uint8* iDst ) {
    const uint32 stride = iSAT->Width() * 4;
    float* dst = reinterpret_cast< float* >( iDst ) + stride;
    for( uint32 y = 1; y < iLen; ++y ) {
        __m128 n = _mm_loadu_ps( dst );
        __m128 m = _mm_loadu_ps( dst - stride );
        __m128 r = _mm_add_ps( n, m );
        _mm_storeu_ps( dst, r );
        dst += stride;
    }
}

void ComputeSummedAreaTable_SSE42_RGBA8( FOldThreadPool*             iOldThreadPool
                                       , bool                     iBlocking
                                       , uint32                   iPerfIntent
                                       , const FHardwareMetrics&   iHostDeviceInfo
                                       , const FBlock*            iSource
                                       , FBlock*                  iSAT )
{
    const uint8*    src     = iSource->Bits();
    uint8*          bdp     = iSAT->Bits();
    const uint32     src_bps = iSource->BytesPerScanLine();
    const uint32     bdp_bps = iSAT->BytesPerScanLine();
    const uint32     src_bpp = iSource->BytesPerPixel();
    const uint32     bdp_bpp = iSAT->BytesPerPixel();
    const int       w       = iSource->Width();
    const int       h       = iSource->Height();

    {
        const uint8* wsrc = reinterpret_cast< const uint8* >( src );
        float*   wdst = reinterpret_cast< float* >( bdp );
        const uint32 src_stride = iSource->Width()   * 4;
        const uint32 dst_stride = iSAT->Width()      * 4;
        for( int y = 0; y < h; ++y ) {
            _mm_storeu_ps( wdst, _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( reinterpret_cast< const __m128i* >( wsrc ) ) ) ) );
            wsrc += src_stride;
            wdst += dst_stride;
        }
    }

    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , h
                                   , InvokeComputeSummedAreaTable_XPass_SSE42_RGBA8
                                   , w
                                   , src + pLINE * src_bps
                                   , bdp + pLINE * bdp_bps );
    iOldThreadPool->WaitForCompletion();
    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , w
                                   , InvokeComputeSummedAreaTable_YPass_SSE42_RGBA8
                                   , h, iSource, iSAT
                                   , src + pLINE * src_bpp
                                   , bdp + pLINE * bdp_bpp );
}
#endif // ULIS_COMPILETIME_SSE42_SUPPORT


typedef void (*fpDispatchedSATFunc)( FOldThreadPool*             iOldThreadPool
                                   , bool                     iBlocking
                                   , uint32                   iPerfIntent
                                   , const FHardwareMetrics&   iHostDeviceInfo
                                   , const FBlock*            iSource
                                   , FBlock*                  iSAT );

template< typename T >
fpDispatchedSATFunc
QueryDispatchedSATFunctionForParameters_Generic( uint32 iPerfIntent, const FHardwareMetrics& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics ) {
    return  ComputeSummedAreaTable_MEM_Generic< T >;
}

fpDispatchedSATFunc
QueryDispatchedSATFunctionForParameters_RGBA8( uint32 iPerfIntent, const FHardwareMetrics& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics ) {
    #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
        if( iHostDeviceInfo.HW_SSE42 )
            return  ComputeSummedAreaTable_SSE42_RGBA8;
        else
    #endif
            return  ComputeSummedAreaTable_MEM_Generic< uint8 >;
}


template< typename T >
fpDispatchedSATFunc
QueryDispatchedSATFunctionForParameters_imp( uint32 iPerfIntent, const FHardwareMetrics& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics ) {
    return  QueryDispatchedSATFunctionForParameters_Generic< T >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
}

template<>
fpDispatchedSATFunc
QueryDispatchedSATFunctionForParameters_imp< uint8 >( uint32 iPerfIntent, const FHardwareMetrics& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics ) {
    // RGBA8 Signature, any layout
    if( iFormatMetrics.HEA
     && iFormatMetrics.NCC == 3
     && iFormatMetrics.CM  == CM_RGB
     && ( iPerfIntent & ULIS_PERF_SSE42 || iPerfIntent & ULIS_PERF_AVX2 )
     && ( iHostDeviceInfo.HW_SSE42 || iHostDeviceInfo.HW_AVX2 ) ) {
        return  QueryDispatchedSATFunctionForParameters_RGBA8( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
    }

    return  QueryDispatchedSATFunctionForParameters_Generic< uint8 >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
}

fpDispatchedSATFunc
QueryDispatchedSATFunctionForParameters( uint32 iPerfIntent, const FHardwareMetrics& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics ) {
    switch( iFormatMetrics.TP ) {
        case Type_uint8     : return  QueryDispatchedSATFunctionForParameters_imp< uint8   >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
        case Type_uint16    : return  QueryDispatchedSATFunctionForParameters_imp< uint16  >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
        case Type_uint32    : return  QueryDispatchedSATFunctionForParameters_imp< uint32  >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
        case Type_ufloat    : return  QueryDispatchedSATFunctionForParameters_imp< ufloat  >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
        case TYPE_UDOUBLE   : return  QueryDispatchedSATFunctionForParameters_imp< udouble >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
    }
    return  nullptr;
}

ULIS_NAMESPACE_END

