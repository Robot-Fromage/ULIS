// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Dispatch.h
* @author       Clement Berthaud
* @brief        This file provides implementation for the Dispatch utilities.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Base/HostDeviceInfo.h"
#include "Data/FormatInfo.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Dispatch Tests
static ULIS3_FORCEINLINE bool DispatchTestIsUnorderedRGBA8( const FFormatInfo& iFormatInfo ) {
    return  ( iFormatInfo.FMT & ULIS3_FORMAT_MASK_LAYOUT ) == ULIS3_FORMAT_RGBA8;
}

static ULIS3_FORCEINLINE bool DispatchTestIsUnorderedRGBAF( const FFormatInfo& iFormatInfo ) {
    return  ( iFormatInfo.FMT & ULIS3_FORMAT_MASK_LAYOUT ) == ULIS3_FORMAT_RGBAF;
}

/////////////////////////////////////////////////////
// Dispatch typedefs
typedef bool (*fpCond)( const FFormatInfo& iFormatInfo );

/////////////////////////////////////////////////////
// Actual Dispatch Implementation
template< typename _DISPATCHER >
class TDispatcher {
public:
    static ULIS3_FORCEINLINE typename _DISPATCHER::fpQuery Query( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatInfo& iFormatInfo, const typename _DISPATCHER::tExtra& iExtra ) {
        for( int i = 0; i < _DISPATCHER::spec_size; ++i ) {
            if( _DISPATCHER::spec_table[i].select_cond( iFormatInfo ) ) {
                #ifdef ULIS3_COMPILETIME_AVX2_SUPPORT
                    if( iPerfIntent & ULIS3_PERF_AVX2 && iHostDeviceInfo.HW_AVX2 )
                        return  _DISPATCHER::spec_table[i].select_AVX( iExtra );
                    else
                #endif
                #ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
                    if( iPerfIntent & ULIS3_PERF_SSE42 && iHostDeviceInfo.HW_SSE42 )
                        return  _DISPATCHER::spec_table[i].select_SSE( iExtra );
                    else
                #endif
                        return  _DISPATCHER::spec_table[i].select_MEM( iExtra );
            }
        }

        #define TMP_CALL( _TYPE_ID, _E0, _E2, _E3 ) return  QueryGeneric< _E0 >( iPerfIntent, iHostDeviceInfo, iFormatInfo, iExtra );
        ULIS3_SWITCH_FOR_ALL_DO( iFormatInfo.TP, ULIS3_FOR_ALL_TYPES_ID_DO, TMP_CALL, 0, 0, 0 )
        #undef TMP_CALL

        ULIS3_ASSERT( false, "No Dispatch found." );
        return  nullptr;
    }

private:
    template< typename T >
    static ULIS3_FORCEINLINE typename _DISPATCHER::fpQuery QueryGeneric( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatInfo& iFormatInfo, const typename _DISPATCHER::tExtra& iExtra ) {
        #ifdef ULIS3_COMPILETIME_AVX2_SUPPORT
            if( iPerfIntent & ULIS3_PERF_AVX2 && iHostDeviceInfo.HW_AVX2 )
                return  _DISPATCHER:: template TGenericDispatchGroup< T >::select_AVX_Generic( iExtra );
            else
        #endif
        #ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
            if( iPerfIntent & ULIS3_PERF_SSE42 && iHostDeviceInfo.HW_SSE42 )
                return  _DISPATCHER:: template TGenericDispatchGroup< T >::select_SSE_Generic( iExtra );
            else
        #endif
                return  _DISPATCHER:: template TGenericDispatchGroup< T >::select_MEM_Generic( iExtra );
    }
};

/////////////////////////////////////////////////////
// Macro Helper for Dispatcher definition
#ifdef ULIS3_COMPILETIME_AVX2_SUPPORT
    #define ULIS3_DISPATCH_SELECT_GENAVX( TAG, AVX )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_AVX_Generic = AVX;
#else
    #define ULIS3_DISPATCH_SELECT_GENAVX( TAG, AVX )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_AVX_Generic = nullptr;
#endif

#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
    #define ULIS3_DISPATCH_SELECT_GENSSE( TAG, SSE )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_SSE_Generic = SSE;
#else
    #define ULIS3_DISPATCH_SELECT_GENSSE( TAG, AVX )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_SSE_Generic = nullptr;
#endif

#define ULIS3_DISPATCH_SELECT_GENMEM( TAG, MEM )                                                                        \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_MEM_Generic = MEM;

#define ULIS3_BEGIN_DISPATCHER( TAG, FPT, EXT, GENAVX, GENSSE, GENMEM ) \
struct TAG {                                                            \
    typedef FPT fpQuery;                                                \
    typedef EXT tExtra;                                                 \
    typedef fpQuery(*fpSelect)( const tExtra& );                        \
    struct FSpecDispatchGroup {                                         \
        const fpCond    select_cond;                                    \
        const fpSelect  select_AVX;                                     \
        const fpSelect  select_SSE;                                     \
        const fpSelect  select_MEM;                                     \
    };                                                                  \
    static const FSpecDispatchGroup spec_table[];                       \
    static const int spec_size;                                         \
    template< typename T >                                              \
    struct TGenericDispatchGroup {                                      \
        static const fpSelect select_AVX_Generic;                       \
        static const fpSelect select_SSE_Generic;                       \
        static const fpSelect select_MEM_Generic;                       \
    };                                                                  \
};                                                                      \
ULIS3_DISPATCH_SELECT_GENAVX( TAG, GENAVX );                            \
ULIS3_DISPATCH_SELECT_GENSSE( TAG, GENSSE );                            \
ULIS3_DISPATCH_SELECT_GENMEM( TAG, GENMEM );                            \
const typename TAG::FSpecDispatchGroup  TAG::spec_table[] = {

#define ULIS3_BEGIN_DISPATCHER_GENERIC( TAG, FPT, EXT, GENMEM )  ULIS3_BEGIN_DISPATCHER( TAG, FPT, EXT, GENMEM, GENMEM, GENMEM )

#ifdef ULIS3_COMPILETIME_AVX2_SUPPORT
    #ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
        #define ULIS3_DECL_DISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, _AVX, _SSE, _MEM },
    #else
        #define ULIS3_DECL_DISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, _SSE, _MEM },
    #endif
#else
    #ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
        #define ULIS3_DECL_DISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, _SSE, _MEM },
    #else
        #define ULIS3_DECL_DISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, nullptr, _MEM },
    #endif
#endif

#define ULIS3_END_DISPATCHER( TAG )                                                                             \
    { nullptr, nullptr, nullptr, nullptr }                                                                      \
};                                                                                                              \
const int TAG::spec_size = sizeof( TAG::spec_table ) / sizeof( TAG::FSpecDispatchGroup ) - 1;

ULIS3_NAMESPACE_END

