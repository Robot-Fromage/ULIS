// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Dispatch_PremultipliedSAT.ipp.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the SummedAreaTable entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/SummedAreaTable.h"
#include "System/HostDeviceInfo.h"
#include "Misc/Filter.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Math.h"
#include "Thread/OldThreadPool.h"

ULIS_NAMESPACE_BEGIN

template< typename T >
void 
InvokeComputePremultipliedSummedAreaTable_XPass_MEM_Generic( const uint32 iLen, FBlock* iSAT, const uint8* iSrc, uint8* iDst ) {
    const FFormatMetrics& fmt = iSAT->FormatMetrics();
    const T* src = reinterpret_cast< const T* >( iSrc )  + fmt.SPP;
    float*   dst = reinterpret_cast< float* >( iDst )    + fmt.SPP;

    float max = static_cast< float >( MaxType< T >() );
    for( uint32 x = 1; x < iLen; ++x ) {

        float alpha = max;
        if( fmt.HEA ) {
            alpha = static_cast< float >( src[fmt.AID] );
            dst[fmt.AID] = alpha + *( dst - fmt.SPP + fmt.AID );
        }

        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            dst[r] = static_cast< float >( src[r] * alpha / max + *( dst - fmt.SPP + r ) );
        }
        src += fmt.SPP;
        dst += fmt.SPP;
    }
}

template< typename T >
void 
InvokeComputePremultipliedSummedAreaTable_YPass_MEM_Generic( const uint32 iLen, FBlock* iSAT, uint8* iDst ) {
    const FFormatMetrics& fmt = iSAT->FormatMetrics();
    const uint32 stride = iSAT->Width() * fmt.SPP;
    float* dst = reinterpret_cast< float* >( iDst ) + stride;

    for( uint32 y = 1; y < iLen; ++y ) {
        for( uint8 j = 0; j < fmt.SPP; ++j ) {
            dst[j] = static_cast< float >( dst[j] + *( dst - stride + j ) );
        }
        dst += stride;
    }
}

template< typename T >
void ComputePremultipliedSummedAreaTable_MEM_Generic( FOldThreadPool*              iOldThreadPool
                                                    , bool                      iBlocking
                                                    , uint32                    iPerfIntent
                                                    , const FHostDeviceInfo&    iHostDeviceInfo
                                                    , const FBlock*             iSource
                                                    , FBlock*                   iSAT )
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
        float max = static_cast< float >( MaxType< T >() );
        for( int y = 0; y < h; ++y ) {
            float alpha = max;
            if( fmt.HEA ) {
                alpha = static_cast< float >( wsrc[fmt.AID] );
                wdst[fmt.AID] = alpha;
            }

            for( uint8 j = 0; j < fmt.NCC; ++j ) {
                uint8 r = fmt.IDT[j];
                wdst[r] = static_cast< float >( wsrc[r] * alpha / max );
            }
            wsrc += stride;
            wdst += stride;
        }
    }

    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , h
                                   , InvokeComputePremultipliedSummedAreaTable_XPass_MEM_Generic< T >
                                   , w, iSAT
                                   , src + pLINE * src_bps
                                   , bdp + pLINE * bdp_bps );
    iOldThreadPool->WaitForCompletion();
    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , w
                                   , InvokeComputePremultipliedSummedAreaTable_YPass_MEM_Generic< T >
                                   , h, iSAT
                                   , bdp + pLINE * bdp_bpp );
}

#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
void 
InvokeComputePremultipliedSummedAreaTable_XPass_SSE42_RGBA8( const uint32 iLen, const uint8* iSrc, uint8* iDst, uint8 iAID ) {
    const uint8* src = reinterpret_cast< const uint8* >( iSrc ) + 4;
    float*       dst = reinterpret_cast< float* >( iDst )       + 4;
    __m128 max = _mm_set_ps1( 255.f );
    for( uint32 x = 1; x < iLen; ++x ) {
        float alpha = static_cast< float >( src[iAID] );
        __m128 alphav = _mm_set_ps1( alpha );

        __m128 n = _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( reinterpret_cast< const __m128i* >( src ) ) ) );
        __m128 m = _mm_loadu_ps( dst - 4 );
        __m128 r = _mm_add_ps( _mm_div_ps( _mm_mul_ps( n, alphav ), max ), m );
        _mm_storeu_ps( dst, r );
        dst[iAID] = alpha + *( dst - 4 + iAID );
        src += 4;
        dst += 4;
    }
}

void 
InvokeComputePremultipliedSummedAreaTable_YPass_SSE42_RGBA8( const uint32 iLen, const FBlock* iSource, FBlock* iSAT, const uint8* iSrc, uint8* iDst ) {
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

void ComputePremultipliedSummedAreaTable_SSE42_RGBA8( FOldThreadPool*              iOldThreadPool
                                                    , bool                      iBlocking
                                                    , uint32                    iPerfIntent
                                                    , const FHostDeviceInfo&    iHostDeviceInfo
                                                    , const FBlock*             iSource
                                                    , FBlock*                   iSAT )
{
    const uint8*    src     = iSource->Bits();
    uint8*          bdp     = iSAT->Bits();
    const uint32     src_bps = iSource->BytesPerScanLine();
    const uint32     bdp_bps = iSAT->BytesPerScanLine();
    const uint32     src_bpp = iSource->BytesPerPixel();
    const uint32     bdp_bpp = iSAT->BytesPerPixel();
    const int       w       = iSource->Width();
    const int       h       = iSource->Height();
    const FFormatMetrics& fmt = iSAT->FormatMetrics();
    {
        const uint8* wsrc = reinterpret_cast< const uint8* >( src );
        float*   wdst = reinterpret_cast< float* >( bdp );
        const uint32 src_stride = iSource->Width()   * 4;
        const uint32 dst_stride = iSAT->Width()      * 4;
        __m128 max = _mm_set_ps1( 255.f );
        for( int y = 0; y < h; ++y ) {
            __m128 alpha = _mm_set_ps1( static_cast< float >( wsrc[fmt.AID] ) );
            _mm_storeu_ps( wdst, _mm_div_ps( _mm_mul_ps( _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( reinterpret_cast< const __m128i* >( wsrc ) ) ) ), alpha ), max ) );
            wsrc += src_stride;
            wdst += dst_stride;
        }
    }

    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , h
                                   , InvokeComputePremultipliedSummedAreaTable_XPass_SSE42_RGBA8
                                   , w
                                   , src + pLINE * src_bps
                                   , bdp + pLINE * bdp_bps, fmt.AID );
    iOldThreadPool->WaitForCompletion();
    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , w
                                   , InvokeComputePremultipliedSummedAreaTable_YPass_SSE42_RGBA8
                                   , h, iSource, iSAT
                                   , src + pLINE * src_bpp
                                   , bdp + pLINE * bdp_bpp );
}
#endif // ULIS_COMPILETIME_SSE42_SUPPORT


typedef void (*fpDispatchedSATFunc)( FOldThreadPool*             iOldThreadPool
                                   , bool                     iBlocking
                                   , uint32                   iPerfIntent
                                   , const FHostDeviceInfo&   iHostDeviceInfo
                                   , const FBlock*            iSource
                                   , FBlock*                  iSAT );

template< typename T >
fpDispatchedSATFunc
QueryDispatchedPremultipliedSATFunctionForParameters_Generic( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics ) {
    return  ComputePremultipliedSummedAreaTable_MEM_Generic< T >;
}

fpDispatchedSATFunc
QueryDispatchedPremultipliedSATFunctionForParameters_RGBA8( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics ) {
    #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
        if( iHostDeviceInfo.HW_SSE42 )
            return  ComputePremultipliedSummedAreaTable_SSE42_RGBA8;
        else
    #endif
            return  ComputePremultipliedSummedAreaTable_MEM_Generic< uint8 >;
}


template< typename T >
fpDispatchedSATFunc
QueryDispatchedPremultipliedSATFunctionForParameters_imp( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics ) {
    return  QueryDispatchedPremultipliedSATFunctionForParameters_Generic< T >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
}

template<>
fpDispatchedSATFunc
QueryDispatchedPremultipliedSATFunctionForParameters_imp< uint8 >( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics ) {
    // RGBA8 Signature, any layout
    if( iFormatMetrics.HEA
     && iFormatMetrics.NCC == 3
     && iFormatMetrics.CM  == CM_RGB
     && ( iPerfIntent & ULIS_PERF_SSE42 || iPerfIntent & ULIS_PERF_AVX2 )
     && ( iHostDeviceInfo.HW_SSE42 || iHostDeviceInfo.HW_AVX2 ) ) {
        return  QueryDispatchedPremultipliedSATFunctionForParameters_RGBA8( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
    }

    return  QueryDispatchedPremultipliedSATFunctionForParameters_Generic< uint8 >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
}

fpDispatchedSATFunc
QueryDispatchedPremultipliedSATFunctionForParameters( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics ) {
    switch( iFormatMetrics.TP ) {
        case TYPE_UINT8     : return  QueryDispatchedPremultipliedSATFunctionForParameters_imp< uint8   >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
        case TYPE_UINT16    : return  QueryDispatchedPremultipliedSATFunctionForParameters_imp< uint16  >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
        case TYPE_UINT32    : return  QueryDispatchedPremultipliedSATFunctionForParameters_imp< uint32  >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
        case TYPE_UFLOAT    : return  QueryDispatchedPremultipliedSATFunctionForParameters_imp< ufloat  >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
        case TYPE_UDOUBLE   : return  QueryDispatchedPremultipliedSATFunctionForParameters_imp< udouble >( iPerfIntent, iHostDeviceInfo, iFormatMetrics );
    }
    return  nullptr;
}

ULIS_NAMESPACE_END

