// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         wULIS.cpp
* @author       Clement Berthaud
* @brief        Binding of ULIS for JavaScript / WebAssembly.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
using namespace ::ULIS;

#include <emscripten/bind.h>
using namespace emscripten;

/////////
// Context Utils
/*
template< typename ... Ts, typename F >
auto ctxCallAdapter( F fptr ) {
    return  [fptr]( FContext* ctx, Ts ... args, py::list iWaitList, FEvent* iEvent ) {
        TArray< FEvent > arr;
        arr.Reserve( iWaitList.size() );
        for( auto it = iWaitList.begin(); it != iWaitList.end(); ++it )
            arr.PushBack( (*it).cast< FEvent >() );
        (ctx->*fptr)( args ..., static_cast< uint32 >( arr.Size() ), arr.Data(), iEvent );
    };
}
*/

/////////
// wULIS4
float lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

EMSCRIPTEN_BINDINGS( wULIS4 ) {
    /////////
    // ulError codes
    constant( "NO_ERROR"                        , ULIS_NO_ERROR                         );
    constant( "WARNING_NO_OP"                   , ULIS_WARNING_NO_OP                    );
    constant( "WARNING_NO_OP_GEOMETRY"          , ULIS_WARNING_NO_OP_GEOMETRY           );
    constant( "WARNING_NO_OP_BAD_FILE_FORMAT"   , ULIS_WARNING_NO_OP_BAD_FILE_FORMAT    );
    constant( "WARNING_NO_OP_BAD_INPUT_DATA"    , ULIS_WARNING_NO_OP_BAD_INPUT_DATA     );
    constant( "ERROR_FORMATS_MISMATCH"          , ULIS_ERROR_FORMATS_MISMATCH           );
    constant( "ERROR_CONCURRENT_DATA"           , ULIS_ERROR_CONCURRENT_DATA            );
    constant( "ERROR_BAD_INPUT_DATA"            , ULIS_ERROR_BAD_INPUT_DATA             );



    /////////
    // AXI
    function( "VersionString", optional_override( [](){ return  std::string( VersionString().Data() ); } ) );
}

