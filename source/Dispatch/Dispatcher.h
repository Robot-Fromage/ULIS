// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Dispatcher.h
* @author       Clement Berthaud
* @brief        This file provides implementation for the Dispatcher utilities.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Dispatch/SpecializationCondition.h"
#include "Scheduling/Command.h"
#include "System/Device.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// TDispatcher
template< typename IMP >
class TDispatcher {
public:
    static ULIS_FORCEINLINE typename fpCommandScheduler Query( const FDevice& iDevice, eFormat iFormat ) {
        for( int i = 0; i < IMP::spec_size; ++i ) {
            if( IMP::spec_table[i].select_cond( iFormat ) ) {
                #ifdef ULIS_COMPILETIME_AVX2_SUPPORT
                    if( iDevice.HasHardwareAVX2() )
                        return  IMP::spec_table[i].select_AVX;
                    else
                #endif
                #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
                    if( iDevice.HasHardwareSSE42() )
                        return  IMP::spec_table[i].select_SSE;
                    else
                #endif
                        return  IMP::spec_table[i].select_MEM;
            }
        }

        #define TMP_CALL( _TYPE_ID, _E0, _E2, _E3 ) return  QueryGeneric< _E0 >( iDevice, iFormat );
        ULIS_SWITCH_FOR_ALL_DO( static_cast< eType >( ULIS_R_TYPE( iFormat ) ), ULIS_FOR_ALL_TYPES_ID_DO, TMP_CALL, 0, 0, 0 )
        #undef TMP_CALL

        ULIS_ASSERT( false, "No Dispatch found." );
        return  nullptr;
    }

private:
    template< typename T >
    static ULIS_FORCEINLINE typename fpCommandScheduler QueryGeneric( const FDevice& iDevice, eFormat iFormat ) {
        #ifdef ULIS_COMPILETIME_AVX2_SUPPORT
            if( iDevice.HasHardwareAVX2() )
                return  IMP:: template TGenericDispatchGroup< T >::select_AVX_Generic;
            else
        #endif
        #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
            if( iDevice.HasHardwareSSE42() )
                return  IMP:: template TGenericDispatchGroup< T >::select_SSE_Generic;
            else
        #endif
                return  IMP:: template TGenericDispatchGroup< T >::select_MEM_Generic;
    }
};

/////////////////////////////////////////////////////
// Macro Helper for Dispatcher definition
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
    #define ULIS_DISPATCH_SELECT_GENAVX( TAG, AVX ) \
    template< typename T > const typename fpCommandScheduler TAG::TGenericDispatchGroup< T >::select_AVX_Generic = &ResolveScheduleCommandCall< TAG::tArgs, AVX >;
#else
    #define ULIS_DISPATCH_SELECT_GENAVX( TAG, AVX ) \
    template< typename T > const typename fpCommandScheduler TAG::TGenericDispatchGroup< T >::select_AVX_Generic = nullptr;
#endif

#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
    #define ULIS_DISPATCH_SELECT_GENSSE( TAG, SSE ) \
    template< typename T > const typename fpCommandScheduler TAG::TGenericDispatchGroup< T >::select_SSE_Generic = &ResolveScheduleCommandCall< TAG::tArgs, SSE >;
#else
    #define ULIS_DISPATCH_SELECT_GENSSE( TAG, AVX ) \
    template< typename T > const typename fpCommandScheduler TAG::TGenericDispatchGroup< T >::select_SSE_Generic = nullptr;
#endif

#define ULIS_DISPATCH_SELECT_GENMEM( TAG, MEM ) \
    template< typename T > const typename fpCommandScheduler TAG::TGenericDispatchGroup< T >::select_MEM_Generic = &ResolveScheduleCommandCall< TAG::tArgs, MEM >;

#define ULIS_DECLARE_DISPATCHER( TAG, ARG )                 \
struct TAG {                                                \
    typedef ARG tArgs;                                      \
    struct FSpecDispatchGroup {                             \
        const fpCond    select_cond;                        \
        const fpCommandScheduler   select_AVX;              \
        const fpCommandScheduler   select_SSE;              \
        const fpCommandScheduler   select_MEM;              \
    };                                                      \
    static const FSpecDispatchGroup spec_table[];           \
    static const int spec_size;                             \
    template< typename T >                                  \
    struct TGenericDispatchGroup {                          \
        static const fpCommandScheduler select_AVX_Generic; \
        static const fpCommandScheduler select_SSE_Generic; \
        static const fpCommandScheduler select_MEM_Generic; \
    };                                                      \
};

#define ULIS_DEFINE_DISPATCHER_GENERIC_GROUP( TAG, GENAVX, GENSSE, GENMEM ) \
ULIS_DISPATCH_SELECT_GENAVX( TAG, GENAVX );                                 \
ULIS_DISPATCH_SELECT_GENSSE( TAG, GENSSE );                                 \
ULIS_DISPATCH_SELECT_GENMEM( TAG, GENMEM );

#define ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( TAG, GENMEM ) ULIS_DEFINE_DISPATCHER_GENERIC_GROUP( TAG, GENMEM, GENMEM, GENMEM )

#define ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( TAG ) \
const typename TAG::FSpecDispatchGroup  TAG::spec_table[] = {

#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
    #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
        #define ULIS_DEFINE_DISPATCHER_SPECIALIZATION( _COND, _AVX, _SSE, _MEM ) { _COND, &ResolveScheduleCommandCall< tArgs, _AVX >, &ResolveScheduleCommandCall< tArgs, _SSE >, &ResolveScheduleCommandCall< tArgs, _MEM > },
    #else
        #define ULIS_DEFINE_DISPATCHER_SPECIALIZATION( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, &ResolveScheduleCommandCall< tArgs, _SSE >, &ResolveScheduleCommandCall< tArgs, _MEM > },
    #endif
#else
    #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
        #define ULIS_DEFINE_DISPATCHER_SPECIALIZATION( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, &ResolveScheduleCommandCall< tArgs, _SSE >, &ResolveScheduleCommandCall< tArgs, _MEM > },
    #else
        #define ULIS_DEFINE_DISPATCHER_SPECIALIZATION( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, nullptr, &ResolveScheduleCommandCall< tArgs, _MEM > },
    #endif
#endif

#define ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( TAG )                                    \
    { nullptr, nullptr, nullptr, nullptr }                                                      \
};                                                                                              \
const int TAG::spec_size = sizeof( TAG::spec_table ) / sizeof( TAG::FSpecDispatchGroup ) - 1;

ULIS_NAMESPACE_END

