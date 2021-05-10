// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         jsULIS.cpp
* @author       Clement Berthaud
* @brief        Binding of ULIS for JavaScript / WebAssembly.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
using namespace ::ULIS;

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
// jsULIS4
#include <emscripten/bind.h>
using namespace emscripten;

float lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("lerp", &lerp);
}

