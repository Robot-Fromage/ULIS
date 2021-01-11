// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Dispatcher.h
* @author       Clement Berthaud
* @brief        This file provides implementation for the Dispatcher utilities.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/SpecializationCondition.h"
#include "Scheduling/Command.h"
#include "Scheduling/Job.h"
#include "System/Device.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// TDispatcher
template< typename IMP >
class TDispatcher {
public:
    static ULIS_FORCEINLINE typename fpCommandScheduler Query( const FHardwareMetrics& iDevice, eFormat iFormat, ePerformanceIntent iPerfIntent ) {
        for( int i = 0; i < IMP::spec_size; ++i ) {
            if( IMP::spec_table[i].select_cond( iFormat ) ) {
                #ifdef ULIS_COMPILETIME_AVX_SUPPORT
                    if( iDevice.HasHardwareAVX2() && bool( iPerfIntent & ePerformanceIntent::PerformanceIntent_AVX ) )
                        return  IMP::spec_table[i].select_AVX;
                    else
                #endif
                #ifdef ULIS_COMPILETIME_SSE_SUPPORT
                    if( iDevice.HasHardwareSSE42() && bool( iPerfIntent & ePerformanceIntent::PerformanceIntent_SSE ) )
                        return  IMP::spec_table[i].select_SSE;
                    else
                #endif
                        return  IMP::spec_table[i].select_MEM;
            }
        }

        #define TMP_CALL( _TYPE_ID, _TYPE, _E2, _E3 ) return  QueryGeneric< _TYPE >( iDevice, iFormat, iPerfIntent );
        ULIS_SWITCH_FOR_ALL_DO( static_cast< eType >( ULIS_R_TYPE( iFormat ) ), ULIS_FOR_ALL_TYPES_ID_DO, TMP_CALL, 0, 0, 0 )
        #undef TMP_CALL

        ULIS_ASSERT( false, "No Dispatch found." );
        return  nullptr;
    }

private:
    template< typename T >
    static ULIS_FORCEINLINE typename fpCommandScheduler QueryGeneric( const FHardwareMetrics& iDevice, eFormat iFormat, ePerformanceIntent iPerfIntent ) {
        #ifdef ULIS_COMPILETIME_AVX_SUPPORT
            if( iDevice.HasHardwareAVX2() && bool( iPerfIntent & ePerformanceIntent::PerformanceIntent_AVX ) )
                return  IMP:: template TGenericDispatchGroup< T >::select_AVX_Generic;
            else
        #endif
        #ifdef ULIS_COMPILETIME_SSE_SUPPORT
            if( iDevice.HasHardwareSSE42() && bool( iPerfIntent & ePerformanceIntent::PerformanceIntent_SSE ) )
                return  IMP:: template TGenericDispatchGroup< T >::select_SSE_Generic;
            else
        #endif
                return  IMP:: template TGenericDispatchGroup< T >::select_MEM_Generic;
    }
};

/////////////////////////////////////////////////////
// Macro Helper for Dispatcher definition
#ifdef ULIS_COMPILETIME_AVX_SUPPORT
    #define ULIS_DISPATCH_SELECT_GENAVX( TAG, AVX ) \
    template< typename T > const typename fpCommandScheduler TAG::TGenericDispatchGroup< T >::select_AVX_Generic = AVX;
#else
    #define ULIS_DISPATCH_SELECT_GENAVX( TAG, AVX ) \
    template< typename T > const typename fpCommandScheduler TAG::TGenericDispatchGroup< T >::select_AVX_Generic = nullptr;
#endif

#ifdef ULIS_COMPILETIME_SSE_SUPPORT
    #define ULIS_DISPATCH_SELECT_GENSSE( TAG, SSE ) \
    template< typename T > const typename fpCommandScheduler TAG::TGenericDispatchGroup< T >::select_SSE_Generic = SSE;
#else
    #define ULIS_DISPATCH_SELECT_GENSSE( TAG, AVX ) \
    template< typename T > const typename fpCommandScheduler TAG::TGenericDispatchGroup< T >::select_SSE_Generic = nullptr;
#endif

#define ULIS_DISPATCH_SELECT_GENMEM( TAG, MEM ) \
    template< typename T > const typename fpCommandScheduler TAG::TGenericDispatchGroup< T >::select_MEM_Generic = MEM;

#define ULIS_DECLARE_DISPATCHER( TAG )                      \
struct TAG {                                                \
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

#ifdef ULIS_COMPILETIME_AVX_SUPPORT
    #ifdef ULIS_COMPILETIME_SSE_SUPPORT
        #define ULIS_DEFINE_DISPATCHER_SPECIALIZATION( _COND, _AVX, _SSE, _MEM )    \
            { _COND                                                                 \
            , _AVX                                                                  \
            , _SSE                                                                  \
            , _MEM },
    #else
        #define ULIS_DEFINE_DISPATCHER_SPECIALIZATION( _COND, _AVX, _SSE, _MEM )    \
            { _COND                                                                 \
            , nullptr                                                               \
            , _SSE                                                                  \
            , _MEM },
    #endif
#else
    #ifdef ULIS_COMPILETIME_SSE_SUPPORT
        #define ULIS_DEFINE_DISPATCHER_SPECIALIZATION( _COND, _AVX, _SSE, _MEM )    \
            { _COND                                                                 \
            , nullptr                                                               \
            , _SSE                                                                  \
            , _MEM },
    #else
        #define ULIS_DEFINE_DISPATCHER_SPECIALIZATION( _COND, _AVX, _SSE, _MEM )    \
            { _COND                                                                 \
            , nullptr                                                               \
            , nullptr                                                               \
            , _MEM },
    #endif
#endif

#define ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( TAG )                                    \
    { nullptr, nullptr, nullptr, nullptr }                                                      \
};                                                                                              \
const int TAG::spec_size = sizeof( TAG::spec_table ) / sizeof( TAG::FSpecDispatchGroup ) - 1;

#define ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( TAG ) \
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( TAG )      \
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( TAG )

#define ULIS_DECLARE_COMMAND_SCHEDULER( iName ) void iName ( FCommand* iCommand, const FSchedulePolicy& iPolicy, bool iContiguous = false );

ULIS_NAMESPACE_END

