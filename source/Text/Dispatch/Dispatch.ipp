// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Dispatch.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic Text entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Text/Dispatch/TextInfo.h"
#include "Text/Dispatch/Generic/TraceTextMono_Generic.ipp"

ULIS3_NAMESPACE_BEGIN
// Dispatch Typedefs ( implemented in dispatch.ipp but available from public API )
typedef void (*fpDispatchedTextFunc)( std::shared_ptr< _FPrivateTextInfo > );

// Generic Dispatcher
template< typename T >
fpDispatchedTextFunc
QueryDispatchedTextFunctionForParameters_Generic() {
    return  &TraceTextMono_Generic< T >;
}

// Generic Dispatcher Selector
template< typename T >
fpDispatchedTextFunc
QueryDispatchedTextFunctionForParameters_imp() {
    return  QueryDispatchedTextFunctionForParameters_Generic< T >();
}

// Type Dispatcher Selector
fpDispatchedTextFunc
QueryDispatchedTextFunctionForParameters( eType iType ) {
    switch( iType ) {
        case TYPE_UINT8     : return  QueryDispatchedTextFunctionForParameters_imp< uint8   >();
        case TYPE_UINT16    : return  QueryDispatchedTextFunctionForParameters_imp< uint16  >();
        case TYPE_UINT32    : return  QueryDispatchedTextFunctionForParameters_imp< uint32  >();
        case TYPE_UFLOAT    : return  QueryDispatchedTextFunctionForParameters_imp< ufloat  >();
        case TYPE_UDOUBLE   : return  QueryDispatchedTextFunctionForParameters_imp< udouble >();
    }
    return  nullptr;
}

ULIS3_NAMESPACE_END

