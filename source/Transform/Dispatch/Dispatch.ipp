// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Dispatch.ipp
* @author       Clement Berthaud
* @brief        This file provides the dispatched for the transform entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Transform/Methods.h"
#include "Transform/Dispatch/TransformInfo.h"
#include "Transform/Dispatch/Generic/TransformAffineMT_NN_MEM_Generic.ipp"
#include "Transform/Dispatch/Generic/TransformAffineMT_Bilinear_MEM_Generic.ipp"
#include "Transform/Dispatch/Generic/TransformAffineMT_Bicubic_MEM_Generic.ipp"
#include "Transform/Dispatch/Generic/TransformPerspectiveMT_NN_MEM_Generic.ipp"
#include "Transform/Dispatch/Generic/TransformPerspectiveMT_Bilinear_MEM_Generic.ipp"
#include "Transform/Dispatch/Generic/TransformPerspectiveMT_Bicubic_MEM_Generic.ipp"
#include "Transform/Dispatch/Generic/TransformBezier_NN_MEM_Generic.ipp"
#include "Transform/Dispatch/Generic/TransformBezier_Bilinear_MEM_Generic.ipp"
#include "Transform/Dispatch/Generic/TransformBezier_Bicubic_MEM_Generic.ipp"

ULIS2_NAMESPACE_BEGIN
typedef void (*fpDispatchedTransformFunc)( std::shared_ptr< const _FTransformInfoPrivate > );
typedef void (*fpDispatchedBezierTransformFunc)( std::shared_ptr< const _FTransformInfoPrivate >, std::shared_ptr< const FBlock >, std::shared_ptr< const FBlock > );
/////////////////////////////////////////////////////
// AFFINE
// Generic Dispatcher
template< typename T >
fpDispatchedTransformFunc
QueryDispatchedTransformAffineFunctionForParameters_Generic( const _FTransformInfoPrivate& iInfo ) {
    switch( iInfo.method ) {
        case INTERP_NN          : return  TransformAffineMT_NN_MEM_Generic< T >;
        case INTERP_BILINEAR    : return  TransformAffineMT_Bilinear_MEM_Generic< T >;
        case INTERP_BICUBIC     : return  TransformAffineMT_Bicubic_MEM_Generic< T >;
    }
    return  nullptr;
}

// Generic Dispatcher Selector
template< typename T >
fpDispatchedTransformFunc
QueryDispatchedTransformAffineFunctionForParameters_imp( const _FTransformInfoPrivate& iInfo ) {
    return  QueryDispatchedTransformAffineFunctionForParameters_Generic< T >( iInfo );
}

// Type Dispatcher Selector
fpDispatchedTransformFunc
QueryDispatchedTransformAffineFunctionForParameters( const _FTransformInfoPrivate& iInfo ) {
    switch( iInfo.source->Type() ) {
        case TYPE_UINT8     : return  QueryDispatchedTransformAffineFunctionForParameters_imp< uint8   >( iInfo ); break;
        case TYPE_UINT16    : return  QueryDispatchedTransformAffineFunctionForParameters_imp< uint16  >( iInfo ); break;
        case TYPE_UINT32    : return  QueryDispatchedTransformAffineFunctionForParameters_imp< uint32  >( iInfo ); break;
        case TYPE_UFLOAT    : return  QueryDispatchedTransformAffineFunctionForParameters_imp< ufloat  >( iInfo ); break;
        case TYPE_UDOUBLE   : return  QueryDispatchedTransformAffineFunctionForParameters_imp< udouble >( iInfo ); break;
    }
    return  nullptr;
}



/////////////////////////////////////////////////////
// PERSPECTIVE
// Generic Dispatcher
template< typename T >
fpDispatchedTransformFunc
QueryDispatchedTransformPerspectiveFunctionForParameters_Generic( const _FTransformInfoPrivate& iInfo ) {
    switch( iInfo.method ) {
        case INTERP_NN          : return  TransformPerspectiveMT_NN_MEM_Generic< T >;
        case INTERP_BILINEAR    : return  TransformPerspectiveMT_Bilinear_MEM_Generic< T >;
        case INTERP_BICUBIC     : return  TransformPerspectiveMT_Bicubic_MEM_Generic< T >;
    }
    return  nullptr;
}

// Generic Dispatcher Selector
template< typename T >
fpDispatchedTransformFunc
QueryDispatchedTransformPerspectiveFunctionForParameters_imp( const _FTransformInfoPrivate& iInfo ) {
    return  QueryDispatchedTransformPerspectiveFunctionForParameters_Generic< T >( iInfo );
}

// Type Dispatcher Selector
fpDispatchedTransformFunc
QueryDispatchedTransformPerspectiveFunctionForParameters( const _FTransformInfoPrivate& iInfo ) {
    switch( iInfo.source->Type() ) {
        case TYPE_UINT8     : return  QueryDispatchedTransformPerspectiveFunctionForParameters_imp< uint8   >( iInfo ); break;
        case TYPE_UINT16    : return  QueryDispatchedTransformPerspectiveFunctionForParameters_imp< uint16  >( iInfo ); break;
        case TYPE_UINT32    : return  QueryDispatchedTransformPerspectiveFunctionForParameters_imp< uint32  >( iInfo ); break;
        case TYPE_UFLOAT    : return  QueryDispatchedTransformPerspectiveFunctionForParameters_imp< ufloat  >( iInfo ); break;
        case TYPE_UDOUBLE   : return  QueryDispatchedTransformPerspectiveFunctionForParameters_imp< udouble >( iInfo ); break;
    }
    return  nullptr;
}


/////////////////////////////////////////////////////
// BEZIER
// Generic Dispatcher
template< typename T >
fpDispatchedBezierTransformFunc
QueryDispatchedTransformBezierFunctionForParameters_Generic( const _FTransformInfoPrivate& iInfo ) {
    switch( iInfo.method ) {
        case INTERP_NN          : return  TransformBezierMT_NN_MEM_Generic< T >;
        case INTERP_BILINEAR    : return  TransformBezierMT_Bilinear_MEM_Generic< T >;
        case INTERP_BICUBIC     : return  TransformBezierMT_Bicubic_MEM_Generic< T >;
    }
    return  nullptr;
}

// Generic Dispatcher Selector
template< typename T >
fpDispatchedBezierTransformFunc
QueryDispatchedTransformBezierFunctionForParameters_imp( const _FTransformInfoPrivate& iInfo ) {
    return  QueryDispatchedTransformBezierFunctionForParameters_Generic< T >( iInfo );
}

// Type Dispatcher Selector
fpDispatchedBezierTransformFunc
QueryDispatchedTransformBezierFunctionForParameters( const _FTransformInfoPrivate& iInfo ) {
    switch( iInfo.source->Type() ) {
        case TYPE_UINT8     : return  QueryDispatchedTransformBezierFunctionForParameters_imp< uint8   >( iInfo ); break;
        case TYPE_UINT16    : return  QueryDispatchedTransformBezierFunctionForParameters_imp< uint16  >( iInfo ); break;
        case TYPE_UINT32    : return  QueryDispatchedTransformBezierFunctionForParameters_imp< uint32  >( iInfo ); break;
        case TYPE_UFLOAT    : return  QueryDispatchedTransformBezierFunctionForParameters_imp< ufloat  >( iInfo ); break;
        case TYPE_UDOUBLE   : return  QueryDispatchedTransformBezierFunctionForParameters_imp< udouble >( iInfo ); break;
    }
    return  nullptr;
}
ULIS2_NAMESPACE_END

