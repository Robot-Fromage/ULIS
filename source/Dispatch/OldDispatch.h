// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
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
#include "System/Device.h"
#include "Image/Format.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Dispatch Tests
static ULIS_FORCEINLINE bool OldDispatchTestIsUnorderedRGBA8( const FFormatMetrics& iFormatMetrics ) {
    return  ( iFormatMetrics.FMT & ULIS_FORMAT_MASK_LAYOUT ) == eFormat::Format_RGBA8;
}

static ULIS_FORCEINLINE bool OldDispatchTestIsUnorderedRGBAF( const FFormatMetrics& iFormatMetrics ) {
    return  ( iFormatMetrics.FMT & ULIS_FORMAT_MASK_LAYOUT ) == eFormat::Format_RGBAF;
}

/////////////////////////////////////////////////////
// Dispatch typedefs
typedef bool (*fpOldCond)( const FFormatMetrics& iFormatMetrics );

/////////////////////////////////////////////////////
// Actual Dispatch Implementation
template< typename IMP >
class TOldDispatcher {
public:
    static ULIS_FORCEINLINE typename IMP::fpQuery Query( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics, const typename IMP::tExtra& iExtra ) {
        for( int i = 0; i < IMP::spec_size; ++i ) {
            if( IMP::spec_table[i].select_cond( iFormatMetrics ) ) {
                #ifdef ULIS_COMPILETIME_AVX2_SUPPORT
                    if( iPerfIntent & ULIS_PERF_AVX2 && iHostDeviceInfo.HW_AVX2 )
                        return  IMP::spec_table[i].select_AVX( iExtra );
                    else
                #endif
                #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
                    if( iPerfIntent & ULIS_PERF_SSE42 && iHostDeviceInfo.HW_SSE42 )
                        return  IMP::spec_table[i].select_SSE( iExtra );
                    else
                #endif
                        return  IMP::spec_table[i].select_MEM( iExtra );
            }
        }

        #define TMP_CALL( _TYPE_ID, _E0, _E2, _E3 ) return  QueryGeneric< _E0 >( iPerfIntent, iHostDeviceInfo, iFormatMetrics, iExtra );
        ULIS_SWITCH_FOR_ALL_DO( iFormatMetrics.TP, ULIS_FOR_ALL_TYPES_ID_DO, TMP_CALL, 0, 0, 0 )
        #undef TMP_CALL

        ULIS_ASSERT( false, "No Dispatch found." );
        return  nullptr;
    }

private:
    template< typename T >
    static ULIS_FORCEINLINE typename IMP::fpQuery QueryGeneric( uint32 iPerfIntent, const FHostDeviceInfo& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics, const typename IMP::tExtra& iExtra ) {
        #ifdef ULIS_COMPILETIME_AVX2_SUPPORT
            if( iPerfIntent & ULIS_PERF_AVX2 && iHostDeviceInfo.HW_AVX2 )
                return  IMP:: template TGenericDispatchGroup< T >::select_AVX_Generic( iExtra );
            else
        #endif
        #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
            if( iPerfIntent & ULIS_PERF_SSE42 && iHostDeviceInfo.HW_SSE42 )
                return  IMP:: template TGenericDispatchGroup< T >::select_SSE_Generic( iExtra );
            else
        #endif
                return  IMP:: template TGenericDispatchGroup< T >::select_MEM_Generic( iExtra );
    }
};

/////////////////////////////////////////////////////
// Macro Helper for Dispatcher definition
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
    #define ULIS_OLDDISPATCH_SELECT_GENAVX( TAG, AVX )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_AVX_Generic = AVX;
#else
    #define ULIS_DISPATCH_SELECT_GENAVX( TAG, AVX )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_AVX_Generic = nullptr;
#endif

#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
    #define ULIS_OLDDISPATCH_SELECT_GENSSE( TAG, SSE )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_SSE_Generic = SSE;
#else
    #define ULIS_OLDDISPATCH_SELECT_GENSSE( TAG, AVX )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_SSE_Generic = nullptr;
#endif

#define ULIS_OLDDISPATCH_SELECT_GENMEM( TAG, MEM )                                                                        \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_MEM_Generic = MEM;

#define ULIS_BEGIN_OLDDISPATCHER( TAG, FPT, EXT, GENAVX, GENSSE, GENMEM )  \
struct TAG {                                                            \
    typedef FPT fpQuery;                                                \
    typedef EXT tExtra;                                                 \
    typedef fpQuery(*fpSelect)( const tExtra& );                        \
    struct FSpecDispatchGroup {                                         \
        const fpOldCond    select_cond;                                    \
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
ULIS_OLDDISPATCH_SELECT_GENAVX( TAG, GENAVX );                             \
ULIS_OLDDISPATCH_SELECT_GENSSE( TAG, GENSSE );                             \
ULIS_OLDDISPATCH_SELECT_GENMEM( TAG, GENMEM );                             \
const typename TAG::FSpecDispatchGroup  TAG::spec_table[] = {

#define ULIS_BEGIN_OLDDISPATCHER_GENERIC( TAG, FPT, EXT, GENMEM )  ULIS_BEGIN_OLDDISPATCHER( TAG, FPT, EXT, GENMEM, GENMEM, GENMEM )

#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
    #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
        #define ULIS_DECL_OLDDISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, _AVX, _SSE, _MEM },
    #else
        #define ULIS_DECL_OLDDISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, _SSE, _MEM },
    #endif
#else
    #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
        #define ULIS_DECL_OLDDISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, _SSE, _MEM },
    #else
        #define ULIS_DECL_OLDDISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, nullptr, _MEM },
    #endif
#endif

#define ULIS_END_OLDDISPATCHER( TAG )                                                                              \
    { nullptr, nullptr, nullptr, nullptr }                                                                      \
};                                                                                                              \
const int TAG::spec_size = sizeof( TAG::spec_table ) / sizeof( TAG::FSpecDispatchGroup ) - 1;

ULIS_NAMESPACE_END

