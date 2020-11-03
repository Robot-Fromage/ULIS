// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Dispatch_SAT.ipp.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the SummedAreaTable entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/SummedAreaTable.h"
#include "Base/HostDeviceInfo.h"
#include "Misc/Filter.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN

template< typename T >
void 
InvokeComputeSummedAreaTable_XPass_MEM_Generic( const uint32 iLen, FBlock* iSAT, const tByte* iSrc, tByte* iDst ) {
    const FFormatInfo& fmt = iSAT->FormatInfo();
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
InvokeComputeSummedAreaTable_YPass_MEM_Generic( const uint32 iLen, FBlock* iSAT, tByte* iDst ) {
    const FFormatInfo& fmt = iSAT->FormatInfo();
    const tSize stride = iSAT->Width() * fmt.SPP;
    float* dst = reinterpret_cast< float* >( iDst ) + stride;

    for( uint32 y = 1; y < iLen; ++y ) {
        for( uint8 j = 0; j < fmt.SPP; ++j )
            dst[j] = static_cast< float >( dst[j] + *( dst - stride + j ) );
        dst += stride;
    }
}

template< typename T >
void ComputeSummedAreaTable_MEM_Generic( FThreadPool*           iThreadPool
                                   , bool                       iBlocking
                                   , uint32                     iPerfIntent
                                   , const FHostDeviceInfo&     iHostDeviceInfo
                                   , const FBlock*              iSource
                                   , FBlock*                    iSAT )
{
    const tByte*    src     = iSource->DataPtr();
    tByte*          bdp     = iSAT->DataPtr();
    const tSize     src_bps = iSource->BytesPerScanLine();
    const tSize     bdp_bps = iSAT->BytesPerScanLine();
    const tSize     src_bpp = iSource->BytesPerPixel();
    const tSize     bdp_bpp = iSAT->BytesPerPixel();
    const int       w       = iSource->Width();
    const int       h       = iSource->Height();

    {
        const FFormatInfo& fmt = iSAT->FormatInfo();
        const T* wsrc = reinterpret_cast< const T* >( src );
        float*   wdst = reinterpret_cast< float* >( bdp );
        const tSize stride = iSAT->Width()   * fmt.SPP;
        for( int y = 0; y < h; ++y ) {
            for( uint8 j = 0; j < iSource->SamplesPerPixel(); ++j )
                wdst[j] = static_cast< float >( wsrc[j] );
            wsrc += stride;
            wdst += stride;
        }
    }

    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , h
                                   , InvokeComputeSummedAreaTable_XPass_MEM_Generic< T >
                                   , w, iSAT
                                   , src + pLINE * src_bps
                                   , bdp + pLINE * bdp_bps );
    iThreadPool->WaitForCompletion();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , w
                                   , InvokeComputeSummedAreaTable_YPass_MEM_Generic< T >
                                   , h, iSAT
                                   , bdp + pLINE * bdp_bpp );
}

#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
void 
InvokeComputeSummedAreaTable_XPass_SSE42_RGBA8( const uint32 iLen, const tByte* iSrc, tByte* iDst ) {
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
InvokeComputeSummedAreaTable_YPass_SSE42_RGBA8( const uint32 iLen, const FBlock* iSource, FBlock* iSAT, const tByte* iSrc, tByte* iDst ) {
    const tSize stride = iSAT->Width() * 4;
    float* dst = reinterpret_cast< float* >( iDst ) + stride;
    for( uint32 y = 1; y < iLen; ++y ) {
        __m128 n = _mm_loadu_ps( dst );
        __m128 m = _mm_loadu_ps( dst - stride );
        __m128 r = _mm_add_ps( n, m );
        _mm_storeu_ps( dst, r );
        dst += stride;
    }
}

void ComputeSummedAreaTable_SSE42_RGBA8( FThreadPool*             iThreadPool
                                       , bool                     iBlocking
                                       , uint32                   iPerfIntent
                                       , const FHostDeviceInfo&   iHostDeviceInfo
                                       , const FBlock*            iSource
                                       , FBlock*                  iSAT )
{
    const tByte*    src     = iSource->DataPtr();
    tByte*          bdp     = iSAT->DataPtr();
    const tSize     src_bps = iSource->BytesPerScanLine();
    const tSize     bdp_bps = iSAT->BytesPerScanLine();
    const tSize     src_bpp = iSource->BytesPerPixel();
    const tSize     bdp_bpp = iSAT->BytesPerPixel();
    const int       w       = iSource->Width();
    const int       h       = iSource->Height();

    {
        const uint8* wsrc = reinterpret_cast< const uint8* >( src );
        float*   wdst = reinterpret_cast< float* >( bdp );
        const tSize src_stride = iSource->Width()   * 4;
        const tSize dst_stride = iSAT->Width()      * 4;
        for( int y = 0; y < h; ++y ) {
            _mm_storeu_ps( wdst, _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( reinterpret_cast< const __m128i* >( wsrc ) ) ) ) );
            wsrc += src_stride;
            wdst += dst_stride;
        }
    }

    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , h
                                   , InvokeComputeSummedAreaTable_XPass_SSE42_RGBA8
                                   , w
                                   , src + pLINE * src_bps
                                   , bdp + pLINE * bdp_bps );
    iThreadPool->WaitForCompletion();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , w
                                   , InvokeComputeSummedAreaTable_YPass_SSE42_RGBA8
                                   , h, iSource, iSAT
                                   , src + pLINE * src_bpp
                                   , bdp + pLINE * bdp_bpp );
}
#endif // ULIS3_COMPILETIME_SSE42_SUPPORT


typedef void (*fpDispatchedSATFunc)( FThreadPool*             iThreadPool
                                   , bool                     iBlocking
                                   , uint32                   iPerfIntent
                                   , const FHostDeviceInfo&   iHostDeviceInfo
                                   , const FBlock*            iSource
                                   , FBlock*                  iSAT );

template< typename T >
fpDispatchedSATFunc
QueryDispatchedSATFunctionForParameters_Generic( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatInfo& iFormatInfo ) {
    return  ComputeSummedAreaTable_MEM_Generic< T >;
}

fpDispatchedSATFunc
QueryDispatchedSATFunctionForParameters_RGBA8( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatInfo& iFormatInfo ) {
    #ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
        if( iHostDeviceInfo.HW_SSE42 )
            return  ComputeSummedAreaTable_SSE42_RGBA8;
        else
    #endif
            return  ComputeSummedAreaTable_MEM_Generic< uint8 >;
}


template< typename T >
fpDispatchedSATFunc
QueryDispatchedSATFunctionForParameters_imp( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatInfo& iFormatInfo ) {
    return  QueryDispatchedSATFunctionForParameters_Generic< T >( iPerfIntent, iHostDeviceInfo, iFormatInfo );
}

template<>
fpDispatchedSATFunc
QueryDispatchedSATFunctionForParameters_imp< uint8 >( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatInfo& iFormatInfo ) {
    // RGBA8 Signature, any layout
    if( iFormatInfo.HEA
     && iFormatInfo.NCC == 3
     && iFormatInfo.CM  == CM_RGB
     && ( iPerfIntent & ULIS3_PERF_SSE42 || iPerfIntent & ULIS3_PERF_AVX2 )
     && ( iHostDeviceInfo.HW_SSE42 || iHostDeviceInfo.HW_AVX2 ) ) {
        return  QueryDispatchedSATFunctionForParameters_RGBA8( iPerfIntent, iHostDeviceInfo, iFormatInfo );
    }

    return  QueryDispatchedSATFunctionForParameters_Generic< uint8 >( iPerfIntent, iHostDeviceInfo, iFormatInfo );
}

fpDispatchedSATFunc
QueryDispatchedSATFunctionForParameters( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatInfo& iFormatInfo ) {
    switch( iFormatInfo.TP ) {
        case TYPE_UINT8     : return  QueryDispatchedSATFunctionForParameters_imp< uint8   >( iPerfIntent, iHostDeviceInfo, iFormatInfo );
        case TYPE_UINT16    : return  QueryDispatchedSATFunctionForParameters_imp< uint16  >( iPerfIntent, iHostDeviceInfo, iFormatInfo );
        case TYPE_UINT32    : return  QueryDispatchedSATFunctionForParameters_imp< uint32  >( iPerfIntent, iHostDeviceInfo, iFormatInfo );
        case TYPE_UFLOAT    : return  QueryDispatchedSATFunctionForParameters_imp< ufloat  >( iPerfIntent, iHostDeviceInfo, iFormatInfo );
        case TYPE_UDOUBLE   : return  QueryDispatchedSATFunctionForParameters_imp< udouble >( iPerfIntent, iHostDeviceInfo, iFormatInfo );
    }
    return  nullptr;
}

ULIS3_NAMESPACE_END

