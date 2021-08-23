// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         TransformDispatch.ipp
* @author       Clement Berthaud
* @brief        This file provides the dispatch mechanism for Transform invocations.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Transform/Methods.h"
#include "Transform/TransformArgs.h"
#include "Dispatch/Dispatch.h"

// Include MEM Generic Implementation
#include "Transform/Generic/ResizeMT_NN_MEM_Generic.ipp"
#include "Transform/Generic/ResizeMT_Bilinear_MEM_Generic.ipp"
#include "Transform/Generic/ResizeMT_Bicubic_MEM_Generic.ipp"
#include "Transform/Generic/ResizeMT_Area_MEM_Generic.ipp"
#include "Transform/Generic/TransformAffineMT_NN_MEM_Generic.ipp"
#include "Transform/Generic/TransformAffineMT_Bilinear_MEM_Generic.ipp"
#include "Transform/Generic/TransformAffineMT_Bicubic_MEM_Generic.ipp"
#include "Transform/Generic/TransformAffineTiledMT_NN_MEM_Generic.ipp"
#include "Transform/Generic/TransformAffineTiledMT_Bilinear_MEM_Generic.ipp"
#include "Transform/Generic/TransformAffineTiledMT_Bicubic_MEM_Generic.ipp"
#include "Transform/Generic/TransformPerspectiveMT_NN_MEM_Generic.ipp"
#include "Transform/Generic/TransformPerspectiveMT_Bilinear_MEM_Generic.ipp"
#include "Transform/Generic/TransformPerspectiveMT_Bicubic_MEM_Generic.ipp"
#include "Transform/Generic/TransformBezier_NN_MEM_Generic.ipp"
#include "Transform/Generic/TransformBezier_Bilinear_MEM_Generic.ipp"
#include "Transform/Generic/TransformBezier_Bicubic_MEM_Generic.ipp"

// Include SSE RGBA8 Implementation
#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
#include "Transform/RGBA8/ResizeMT_NN_SSE_RGBA8.ipp"
#include "Transform/RGBA8/ResizeMT_Bilinear_SSE_RGBA8.ipp"
#include "Transform/RGBA8/ResizeMT_Bicubic_SSE_RGBA8.ipp"
#include "Transform/RGBA8/ResizeMT_Area_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformAffineMT_NN_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformAffineMT_Bilinear_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformAffineMT_Bicubic_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformAffineTiledMT_NN_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformAffineTiledMT_Bilinear_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformAffineTiledMT_Bicubic_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformPerspectiveMT_NN_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformPerspectiveMT_Bilinear_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformPerspectiveMT_Bicubic_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformBezier_NN_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformBezier_Bilinear_SSE_RGBA8.ipp"
#include "Transform/RGBA8/TransformBezier_Bicubic_SSE_RGBA8.ipp"
#endif

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Typedefs
typedef void (*fpTransformInvocation)( std::shared_ptr< const FTransformArgs > );
typedef void (*fpResizeInvocation)( std::shared_ptr< const FResizeArgs > );
typedef void (*fpBezierTransformInvocation)( std::shared_ptr< const FTransformArgs >, std::shared_ptr< const FBlock >, std::shared_ptr< const FBlock > );

/////////////////////////////////////////////////////
// Implementors
#define ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( RET, TAG, ARG, NN, BILINEAR, BICUBIC, AREA )   \
RET TAG ( const ARG & iInfo ) {                                                                 \
    switch( iInfo.method ) {                                                                    \
        case INTERP_NN          : return  NN;                                                   \
        case INTERP_BILINEAR    : return  BILINEAR;                                             \
        case INTERP_BICUBIC     : return  BICUBIC;                                              \
        case INTERP_AREA        : return  AREA;                                                 \
        default                 : return  nullptr;                                              \
    }                                                                                           \
}

/////////////////////////////////////////////////////
// TransformAffine Invocation Selectors
template< typename T >
ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpTransformInvocation, SelectTransformAffineInvocationMEMGeneric, FTransformArgs
    , &TransformAffineMT_NN_MEM_Generic< T >
    , &TransformAffineMT_Bilinear_MEM_Generic< T >
    , &TransformAffineMT_Bicubic_MEM_Generic< T >
    , &TransformAffineMT_Bilinear_MEM_Generic< T > )

#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpTransformInvocation, SelectTransformAffineInvocationSSERGBA8, FTransformArgs
    , &TransformAffineMT_NN_SSE_RGBA8
    , &TransformAffineMT_Bilinear_SSE_RGBA8
    , &TransformAffineMT_Bicubic_SSE_RGBA8
    , &TransformAffineMT_Bilinear_SSE_RGBA8 )
#endif // ULIS3_COMPILETIME_SSE42_SUPPORT

ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpTransformInvocation, SelectTransformAffineInvocationMEMRGBA8, FTransformArgs
    , &TransformAffineMT_NN_MEM_Generic< uint8 >
    , &TransformAffineMT_Bilinear_MEM_Generic< uint8 >
    , &TransformAffineMT_Bicubic_MEM_Generic< uint8 >
    , &TransformAffineMT_Bilinear_MEM_Generic< uint8 > )

/////////////////////////////////////////////////////
// TransformAffineTiled Invocation Selectors
template< typename T >
ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpTransformInvocation, SelectTransformAffineTiledInvocationMEMGeneric, FTransformArgs
    , &TransformAffineTiledMT_NN_MEM_Generic< T >
    , &TransformAffineTiledMT_Bilinear_MEM_Generic< T >
    , &TransformAffineTiledMT_Bicubic_MEM_Generic< T >
    , &TransformAffineTiledMT_Bilinear_MEM_Generic< T > )

#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpTransformInvocation, SelectTransformAffineTiledInvocationSSERGBA8, FTransformArgs
    , &TransformAffineTiledMT_NN_SSE_RGBA8
    , &TransformAffineTiledMT_Bilinear_SSE_RGBA8
    , &TransformAffineTiledMT_Bicubic_SSE_RGBA8
    , &TransformAffineTiledMT_Bilinear_SSE_RGBA8 )
#endif // ULIS3_COMPILETIME_SSE42_SUPPORT

ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpTransformInvocation, SelectTransformAffineTiledInvocationMEMRGBA8, FTransformArgs
    , &TransformAffineTiledMT_NN_MEM_Generic< uint8 >
    , &TransformAffineTiledMT_Bilinear_MEM_Generic< uint8 >
    , &TransformAffineTiledMT_Bicubic_MEM_Generic< uint8 >
    , &TransformAffineTiledMT_Bilinear_MEM_Generic< uint8 > )

/////////////////////////////////////////////////////
// TransformPerspective Invocation Selectors
template< typename T >
ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpTransformInvocation, SelectTransformPerspectiveInvocationMEMGeneric, FTransformArgs
    , &TransformPerspectiveMT_NN_MEM_Generic< T >
    , &TransformPerspectiveMT_Bilinear_MEM_Generic< T >
    , &TransformPerspectiveMT_Bicubic_MEM_Generic< T >
    , &TransformPerspectiveMT_Bilinear_MEM_Generic< T > )

#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpTransformInvocation, SelectTransformPerspectiveInvocationSSERGBA8, FTransformArgs
    , &TransformPerspectiveMT_NN_SSE_RGBA8
    , &TransformPerspectiveMT_Bilinear_SSE_RGBA8
    , &TransformPerspectiveMT_Bicubic_SSE_RGBA8
    , &TransformPerspectiveMT_Bilinear_SSE_RGBA8 )
#endif // ULIS3_COMPILETIME_SSE42_SUPPORT

ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpTransformInvocation, SelectTransformPerspectiveInvocationMEMRGBA8, FTransformArgs
    , &TransformPerspectiveMT_NN_MEM_Generic< uint8 >
    , &TransformPerspectiveMT_Bilinear_MEM_Generic< uint8 >
    , &TransformPerspectiveMT_Bicubic_MEM_Generic< uint8 >
    , &TransformPerspectiveMT_Bilinear_MEM_Generic< uint8 > )

/////////////////////////////////////////////////////
// TransformBezier Invocation Selectors
template< typename T >
ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpBezierTransformInvocation, SelectTransformBezierInvocationMEMGeneric, FTransformArgs
    , &TransformBezierMT_NN_MEM_Generic< T >
    , &TransformBezierMT_Bilinear_MEM_Generic< T >
    , &TransformBezierMT_Bicubic_MEM_Generic< T >
    , &TransformBezierMT_Bilinear_MEM_Generic< T > )

#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpBezierTransformInvocation, SelectTransformBezierInvocationSSERGBA8, FTransformArgs
    , &TransformBezierMT_NN_SSE_RGBA8
    , &TransformBezierMT_Bilinear_SSE_RGBA8
    , &TransformBezierMT_Bicubic_SSE_RGBA8
    , &TransformBezierMT_Bilinear_SSE_RGBA8 )
#endif // ULIS3_COMPILETIME_SSE42_SUPPORT

ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpBezierTransformInvocation, SelectTransformBezierInvocationMEMRGBA8, FTransformArgs
    , &TransformBezierMT_NN_MEM_Generic< uint8 >
    , &TransformBezierMT_Bilinear_MEM_Generic< uint8 >
    , &TransformBezierMT_Bicubic_MEM_Generic< uint8 >
    , &TransformBezierMT_Bilinear_MEM_Generic< uint8 > )

/////////////////////////////////////////////////////
// Resize Invocation Selectors
template< typename T >
ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpResizeInvocation, SelectResizeInvocationMEMGeneric, FResizeArgs
    , &ResizeMT_NN_MEM_Generic< T >
    , &ResizeMT_Bilinear_MEM_Generic< T >
    , &ResizeMT_Bicubic_MEM_Generic< T >
    , &ResizeMT_Area_MEM_Generic< T > )

#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpResizeInvocation, SelectResizeInvocationSSERGBA8, FResizeArgs
    , &ResizeMT_NN_SSE_RGBA8
    , &ResizeMT_Bilinear_SSE_RGBA8
    , &ResizeMT_Bicubic_SSE_RGBA8
    , &ResizeMT_Area_SSE_RGBA8 )
#endif // ULIS3_COMPILETIME_SSE42_SUPPORT

ULIS3_IMP_TRANSFORM_INVOCATION_SELECTOR( fpResizeInvocation, SelectResizeInvocationMEMRGBA8, FResizeArgs
    , &ResizeMT_NN_MEM_Generic< uint8 >
    , &ResizeMT_Bilinear_MEM_Generic< uint8 >
    , &ResizeMT_Bicubic_MEM_Generic< uint8 >
    , &ResizeMT_Area_MEM_Generic< uint8 > )

/////////////////////////////////////////////////////
// TransformAffine Dispatcher
ULIS3_BEGIN_DISPATCHER_GENERIC( FTransformAffineInvocationSelector, fpTransformInvocation, FTransformArgs, &SelectTransformAffineInvocationMEMGeneric< T > )
    ULIS3_DECL_DISPATCH_SPEC( &DispatchTestIsUnorderedRGBA8, &SelectTransformAffineInvocationSSERGBA8, &SelectTransformAffineInvocationSSERGBA8, &SelectTransformAffineInvocationMEMRGBA8 )
ULIS3_END_DISPATCHER( FTransformAffineInvocationSelector )

/////////////////////////////////////////////////////
// TransformAffineTiled Dispatcher
ULIS3_BEGIN_DISPATCHER_GENERIC( FTransformAffineTiledInvocationSelector, fpTransformInvocation, FTransformArgs, &SelectTransformAffineTiledInvocationMEMGeneric< T > )
    ULIS3_DECL_DISPATCH_SPEC( &DispatchTestIsUnorderedRGBA8, &SelectTransformAffineTiledInvocationSSERGBA8, &SelectTransformAffineTiledInvocationSSERGBA8, &SelectTransformAffineTiledInvocationMEMRGBA8 )
ULIS3_END_DISPATCHER( FTransformAffineTiledInvocationSelector )

/////////////////////////////////////////////////////
// TransformPerspective Dispatcher
ULIS3_BEGIN_DISPATCHER_GENERIC( FTransformPerspectiveInvocationSelector, fpTransformInvocation, FTransformArgs, &SelectTransformPerspectiveInvocationMEMGeneric< T > )
    ULIS3_DECL_DISPATCH_SPEC( &DispatchTestIsUnorderedRGBA8, &SelectTransformPerspectiveInvocationSSERGBA8, &SelectTransformPerspectiveInvocationSSERGBA8, &SelectTransformPerspectiveInvocationMEMRGBA8 )
ULIS3_END_DISPATCHER( FTransformPerspectiveInvocationSelector )

/////////////////////////////////////////////////////
// TransformBezier Dispatcher
ULIS3_BEGIN_DISPATCHER_GENERIC( FTransformBezierInvocationSelector, fpBezierTransformInvocation, FTransformArgs, &SelectTransformBezierInvocationMEMGeneric< T > )
    ULIS3_DECL_DISPATCH_SPEC( &DispatchTestIsUnorderedRGBA8, &SelectTransformBezierInvocationSSERGBA8, &SelectTransformBezierInvocationSSERGBA8, &SelectTransformBezierInvocationMEMRGBA8 )
ULIS3_END_DISPATCHER( FTransformBezierInvocationSelector )

/////////////////////////////////////////////////////
// Resize Dispatcher
ULIS3_BEGIN_DISPATCHER_GENERIC( FResizeInvocationSelector, fpResizeInvocation, FResizeArgs, &SelectResizeInvocationMEMGeneric< T > )
    ULIS3_DECL_DISPATCH_SPEC( &DispatchTestIsUnorderedRGBA8, &SelectResizeInvocationSSERGBA8, &SelectResizeInvocationSSERGBA8, &SelectResizeInvocationMEMRGBA8 )
ULIS3_END_DISPATCHER( FResizeInvocationSelector )

ULIS3_NAMESPACE_END

