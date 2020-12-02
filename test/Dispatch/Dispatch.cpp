// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/**
*
*   ULIS
*__________________
*
* @file         Dispatch.cpp
* @author       Clement Berthaud
* @brief        Dispatch application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>
#include <functional>
#include <map>
using namespace ::ul3;

/////////////////////////////////////////////////////
// Imp
#define INVOCATION_X_ARGS

typedef void (*fpDispatchedXFunc)( INVOCATION_X_ARGS );
template< typename T > void InvokeXGenericAVX0( INVOCATION_X_ARGS ) { std::cout << "InvokeXGenericAVX0 called." << std::endl; }
template< typename T > void InvokeXGenericAVX1( INVOCATION_X_ARGS ) { std::cout << "InvokeXGenericAVX1 called." << std::endl; }
template< typename T > void InvokeXGenericAVX2( INVOCATION_X_ARGS ) { std::cout << "InvokeXGenericAVX2 called." << std::endl; }
template< typename T > void InvokeXGenericSSE0( INVOCATION_X_ARGS ) { std::cout << "InvokeXGenericSSE0 called." << std::endl; }
template< typename T > void InvokeXGenericSSE1( INVOCATION_X_ARGS ) { std::cout << "InvokeXGenericSSE1 called." << std::endl; }
template< typename T > void InvokeXGenericSSE2( INVOCATION_X_ARGS ) { std::cout << "InvokeXGenericSSE2 called." << std::endl; }
template< typename T > void InvokeXGenericMEM0( INVOCATION_X_ARGS ) { std::cout << "InvokeXGenericMEM0 called." << std::endl; }
template< typename T > void InvokeXGenericMEM1( INVOCATION_X_ARGS ) { std::cout << "InvokeXGenericMEM1 called." << std::endl; }
template< typename T > void InvokeXGenericMEM2( INVOCATION_X_ARGS ) { std::cout << "InvokeXGenericMEM2 called." << std::endl; }
void InvokeXRGBA8AVX0( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBA8AVX0 called." << std::endl; }
void InvokeXRGBA8AVX1( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBA8AVX1 called." << std::endl; }
void InvokeXRGBA8AVX2( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBA8AVX2 called." << std::endl; }
void InvokeXRGBA8SSE0( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBA8SSE0 called." << std::endl; }
void InvokeXRGBA8SSE1( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBA8SSE1 called." << std::endl; }
void InvokeXRGBA8SSE2( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBA8SSE2 called." << std::endl; }
void InvokeXRGBA8MEM0( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBA8MEM0 called." << std::endl; }
void InvokeXRGBA8MEM1( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBA8MEM1 called." << std::endl; }
void InvokeXRGBA8MEM2( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBA8MEM2 called." << std::endl; }
void InvokeXRGBAFAVX0( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBAFAVX0 called." << std::endl; }
void InvokeXRGBAFAVX1( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBAFAVX1 called." << std::endl; }
void InvokeXRGBAFAVX2( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBAFAVX2 called." << std::endl; }
void InvokeXRGBAFSSE0( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBAFSSE0 called." << std::endl; }
void InvokeXRGBAFSSE1( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBAFSSE1 called." << std::endl; }
void InvokeXRGBAFSSE2( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBAFSSE2 called." << std::endl; }
void InvokeXRGBAFMEM0( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBAFMEM0 called." << std::endl; }
void InvokeXRGBAFMEM1( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBAFMEM1 called." << std::endl; }
void InvokeXRGBAFMEM2( INVOCATION_X_ARGS ) { std::cout << "InvokeXRGBAFMEM2 called." << std::endl; }

/////////////////////////////////////////////////////
// Table
static ULIS_FORCEINLINE bool OldDispatchTestIsUnorderedRGBA8( const FFormatMetrics& iFormatMetrics ) { return  ( iFormatMetrics.TP == Type_uint8 ) && ( iFormatMetrics.HEA ) && ( iFormatMetrics.NCC == 3 ) && ( iFormatMetrics.CM == CM_RGB ); }
static ULIS_FORCEINLINE bool OldDispatchTestIsUnorderedRGBAF( const FFormatMetrics& iFormatMetrics ) { return  ( iFormatMetrics.TP == Type_ufloat ) && ( iFormatMetrics.HEA ) && ( iFormatMetrics.NCC == 3 ) && ( iFormatMetrics.CM == CM_RGB ); }
typedef bool (*fpOldCond)( const FFormatMetrics& iFormatMetrics );

/*
template< typename F, typename T, typename E >
struct TMultiDispatchTable {
    static const TMultiDispatchGroup< F, E > spec_table[];
    static const typename TMultiDispatchGroup< F, E >::fpSelect select_AVX_Generic;
    static const typename TMultiDispatchGroup< F, E >::fpSelect select_SSE_Generic;
    static const typename TMultiDispatchGroup< F, E >::fpSelect select_MEM_Generic;
    static const int spec_size;
};
*/

/////////////////////////////////////////////////////
// Dispatcher
template< typename D >
class TOldDispatcher {
public:
    static ULIS_FORCEINLINE typename D::fpQuery Query( uint32 iPerfIntent, const FHardwareMetrics& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics, const typename D::tExtra& iExtra ) {
        for( int i = 0; i < D::spec_size; ++i ) {
            if( D::spec_table[i].select_cond( iFormatMetrics ) ) {
                #ifdef ULIS_COMPILETIME_AVX2_SUPPORT
                    if( iPerfIntent & ULIS_PERF_AVX2 && iHostDeviceInfo.HW_AVX2 )
                        return  D::spec_table[i].select_AVX( iExtra );
                    else
                #endif
                #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
                    if( iPerfIntent & ULIS_PERF_SSE42 && iHostDeviceInfo.HW_SSE42 )
                        return  D::spec_table[i].select_SSE( iExtra );
                    else
                #endif
                        return  D::spec_table[i].select_MEM( iExtra );
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
    static ULIS_FORCEINLINE typename D::fpQuery QueryGeneric( uint32 iPerfIntent, const FHardwareMetrics& iHostDeviceInfo, const FFormatMetrics& iFormatMetrics, const typename D::tExtra& iExtra ) {
        #ifdef ULIS_COMPILETIME_AVX2_SUPPORT
            if( iPerfIntent & ULIS_PERF_AVX2 && iHostDeviceInfo.HW_AVX2 )
                return  D:: template TGenericDispatchGroup< T >::select_AVX_Generic( iExtra );
            else
        #endif
        #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
            if( iPerfIntent & ULIS_PERF_SSE42 && iHostDeviceInfo.HW_SSE42 )
                return  D:: template TGenericDispatchGroup< T >::select_SSE_Generic( iExtra );
            else
        #endif
                return  D:: template TGenericDispatchGroup< T >::select_MEM_Generic( iExtra );
    }
};

/////////////////////////////////////////////////////
// Spec
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
    #define ULIS_DISPATCH_SELECT_GENAVX( TAG, AVX )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_AVX_Generic = AVX;
#else
    #define ULIS_DISPATCH_SELECT_GENAVX( TAG, AVX )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_AVX_Generic = nullptr;
#endif

#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
    #define ULIS_OLDDISPATCH_SELECT_GENSSE( TAG, SSE )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_SSE_Generic = SSE;
#else
    #define ULIS_DISPATCH_SELECT_GENAVX( TAG, AVX )                                                                    \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_SSE_Generic = nullptr;
#endif

#define ULIS_OLDDISPATCH_SELECT_GENMEM( TAG, MEM )                                                                        \
    template< typename T > const typename TAG::fpSelect TAG::TGenericDispatchGroup< T >::select_MEM_Generic = MEM;

#define ULIS_BEGIN_OLDDISPATCHER( TAG, FPT, EXT, GENAVX, GENSSE, GENMEM ) \
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
ULIS_DISPATCH_SELECT_GENAVX( TAG, GENAVX );                            \
ULIS_OLDDISPATCH_SELECT_GENSSE( TAG, GENSSE );                            \
ULIS_OLDDISPATCH_SELECT_GENMEM( TAG, GENMEM );                            \
const typename TAG::FSpecDispatchGroup  TAG::spec_table[] = {

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

#define ULIS_END_OLDDISPATCHER( TAG )                                                                             \
    { nullptr, nullptr, nullptr, nullptr }                                                                      \
};                                                                                                              \
const int TAG::spec_size = sizeof( TAG::spec_table ) / sizeof( TAG::FSpecDispatchGroup ) - 1;

/////////////////////////////////////////////////////
// Imp Dispatcher
#define IMP_DISPATCH_SELECT( N, a, b, c )   \
fpDispatchedXFunc N ( const int& iInfo ) {  \
    switch( iInfo ) {                       \
        case 0  : return  a;                \
        case 1  : return  b;                \
        case 2  : return  c;                \
        default : return  nullptr;          \
    }                                       \
}
template< typename T > IMP_DISPATCH_SELECT( SelectDispatchMEMGeneric, &InvokeXGenericMEM0< T >, &InvokeXGenericMEM1< T >, &InvokeXGenericMEM2< T > )
template< typename T > IMP_DISPATCH_SELECT( SelectDispatchSSEGeneric, &InvokeXGenericSSE0< T >, &InvokeXGenericSSE1< T >, &InvokeXGenericSSE2< T > )
template< typename T > IMP_DISPATCH_SELECT( SelectDispatchAVXGeneric, &InvokeXGenericAVX0< T >, &InvokeXGenericAVX1< T >, &InvokeXGenericAVX2< T > )
IMP_DISPATCH_SELECT( SelectDispatchMEMRGBA8, &InvokeXRGBA8MEM0, &InvokeXRGBA8MEM1, &InvokeXRGBA8MEM2 )
IMP_DISPATCH_SELECT( SelectDispatchSSERGBA8, &InvokeXRGBA8SSE0, &InvokeXRGBA8SSE1, &InvokeXRGBA8SSE2 )
IMP_DISPATCH_SELECT( SelectDispatchAVXRGBA8, &InvokeXRGBA8AVX0, &InvokeXRGBA8AVX1, &InvokeXRGBA8AVX2 )
IMP_DISPATCH_SELECT( SelectDispatchMEMRGBAF, &InvokeXRGBAFMEM0, &InvokeXRGBAFMEM1, &InvokeXRGBAFMEM2 )
IMP_DISPATCH_SELECT( SelectDispatchSSERGBAF, &InvokeXRGBAFSSE0, &InvokeXRGBAFSSE1, &InvokeXRGBAFSSE2 )
IMP_DISPATCH_SELECT( SelectDispatchAVXRGBAF, &InvokeXRGBAFAVX0, &InvokeXRGBAFAVX1, &InvokeXRGBAFAVX2 )

ULIS_BEGIN_OLDDISPATCHER( FBlendDispatchTable, fpDispatchedXFunc, int, &SelectDispatchAVXGeneric< T >, &SelectDispatchSSEGeneric< T >, &SelectDispatchMEMGeneric< T > )
    ULIS_DECL_OLDDISPATCH_SPEC( &OldDispatchTestIsUnorderedRGBA8, &SelectDispatchAVXRGBA8, &SelectDispatchSSERGBA8, &SelectDispatchMEMRGBA8 )
    ULIS_DECL_OLDDISPATCH_SPEC( &OldDispatchTestIsUnorderedRGBAF, &SelectDispatchAVXRGBAF, &SelectDispatchSSERGBAF, &SelectDispatchMEMRGBAF )
ULIS_END_OLDDISPATCHER( FBlendDispatchTable )

/////////////////////////////////////////////////////
// Main
int
main() {
    const size_t size0 = FBlendDispatchTable::spec_size;

    uint32 intent = ULIS_PERF_SSE42;
    FHardwareMetrics host = FHardwareMetrics::Detect();
    FFormatMetrics format( ULIS_FORMAT_RGBA16 );

    int extra = 1;

    fpDispatchedXFunc fptr = TOldDispatcher< FBlendDispatchTable >::Query( intent, host, format, extra );
    if( fptr )
        fptr();

    return  0;
}

