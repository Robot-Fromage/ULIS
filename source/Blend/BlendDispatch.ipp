// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         BlendDispatch.ipp
* @author       Clement Berthaud
* @brief        This file provides the dispatch mechanism for Blend invocations.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Modes.h"
#include "Blend/BlendArgs.h"
#include "Dispatch/Dispatch.h"

// Include MEM Generic Implementation
#include "Blend/Generic/BlendMT_Separable_MEM_Generic.ipp"
#include "Blend/Generic/BlendMT_NonSeparable_MEM_Generic.ipp"
#include "Blend/Generic/BlendMT_Misc_MEM_Generic.ipp"
#include "Blend/Generic/AlphaBlendMT_MEM_Generic.ipp"
#include "Blend/Generic/TiledBlendMT_Separable_MEM_Generic.ipp"
#include "Blend/Generic/TiledBlendMT_NonSeparable_MEM_Generic.ipp"
#include "Blend/Generic/TiledBlendMT_Misc_MEM_Generic.ipp"

// Include SSE RGBA8 Implementation
#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
#include "Blend/RGBA8/BlendMT_Separable_SSE_RGBA8.ipp"
#include "Blend/RGBA8/BlendMT_NonSeparable_SSE_RGBA8.ipp"
#include "Blend/RGBA8/AlphaBlendMT_SSE_RGBA8.ipp"
#include "Blend/RGBA8/TiledBlendMT_Separable_SSE_RGBA8.ipp"
#include "Blend/RGBA8/TiledBlendMT_NonSeparable_SSE_RGBA8.ipp"
#endif // ULIS3_COMPILETIME_SSE42_SUPPORT

// Include AVX RGBA8 Implementation
#ifdef ULIS3_COMPILETIME_AVX2_SUPPORT
#include "Blend/RGBA8/BlendMT_Separable_AVX_RGBA8.ipp"
#include "Blend/RGBA8/AlphaBlendMT_AVX_RGBA8.ipp"
#endif // ULIS3_COMPILETIME_AVX2_SUPPORT

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Typedefs
typedef void (*fpBlendInvocation)( std::shared_ptr< const FBlendArgs > iBlendParams );

/////////////////////////////////////////////////////
// Implementors
#define ULIS3_IMP_BLEND_INVOCATION_SELECTOR( TAG, MISC, MISCAA, SEP, SEPAA, NSEP, NSEPAA )      \
fpBlendInvocation TAG ( const FBlendArgs& iInfo ) {                                             \
    switch( BlendingModeQualifier( iInfo.blendingMode ) ) {                                     \
        case BMQ_MISC           : return  iInfo.subpixelFlag ? MISCAA   : MISC;                 \
        case BMQ_SEPARABLE      : return  iInfo.subpixelFlag ? SEPAA    : SEP;                  \
        case BMQ_NONSEPARABLE   : return  iInfo.subpixelFlag ? NSEPAA   : NSEP;                 \
        default                 : return  nullptr;                                              \
    }                                                                                           \
}

#define ULIS3_IMP_ALPHABLEND_INVOCATION_SELECTOR( TAG, SEP, SEPAA ) \
fpBlendInvocation TAG ( const FBlendArgs& iInfo ) {                 \
    return  iInfo.subpixelFlag ? SEPAA    : SEP;                    \
}

#define ULIS3_IMP_TILEDBLEND_INVOCATION_SELECTOR( TAG, MISC, SEP, NSEP )    \
fpBlendInvocation TAG ( const FBlendArgs& iInfo ) {                         \
    switch( BlendingModeQualifier( iInfo.blendingMode ) ) {                 \
        case BMQ_MISC           : return  MISC;                             \
        case BMQ_SEPARABLE      : return  SEP;                              \
        case BMQ_NONSEPARABLE   : return  NSEP;                             \
        default                 : return  nullptr;                          \
    }                                                                       \
}

/////////////////////////////////////////////////////
// Blend Invocation Selectors
template< typename T > ULIS3_IMP_BLEND_INVOCATION_SELECTOR( SelectBlendInvocationMEMGeneric
    , &BlendMT_Misc_MEM_Generic< T >                , &BlendMT_Misc_MEM_Generic_Subpixel< T >
    , &BlendMT_Separable_MEM_Generic< T >           , &BlendMT_Separable_MEM_Generic_Subpixel< T >
    , &BlendMT_NonSeparable_MEM_Generic< T >        , &BlendMT_NonSeparable_MEM_Generic_Subpixel< T > )
#ifdef ULIS3_COMPILETIME_AVX2_SUPPORT
ULIS3_IMP_BLEND_INVOCATION_SELECTOR( SelectBlendInvocationAVXRGBA8
    , &BlendMT_Misc_MEM_Generic< uint8 >            , &BlendMT_Misc_MEM_Generic_Subpixel< uint8 >
    , &BlendMT_Separable_AVX_RGBA8                  , &BlendMT_Separable_AVX_RGBA8_Subpixel
    , &BlendMT_NonSeparable_SSE_RGBA8               , &BlendMT_NonSeparable_SSE_RGBA8_Subpixel )
#endif // ULIS3_COMPILETIME_AVX2_SUPPORT
#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
ULIS3_IMP_BLEND_INVOCATION_SELECTOR( SelectBlendInvocationSSERGBA8
    , &BlendMT_Misc_MEM_Generic< uint8 >            , &BlendMT_Misc_MEM_Generic_Subpixel< uint8 >
    , &BlendMT_Separable_SSE_RGBA8                  , &BlendMT_Separable_SSE_RGBA8_Subpixel
    , &BlendMT_NonSeparable_SSE_RGBA8               , &BlendMT_NonSeparable_SSE_RGBA8_Subpixel )
#endif // ULIS3_COMPILETIME_SSE42_SUPPORT
ULIS3_IMP_BLEND_INVOCATION_SELECTOR( SelectBlendInvocationMEMRGBA8
    , &BlendMT_Misc_MEM_Generic< uint8 >            , &BlendMT_Misc_MEM_Generic_Subpixel< uint8 >
    , &BlendMT_Separable_MEM_Generic< uint8 >       , &BlendMT_Separable_MEM_Generic_Subpixel< uint8 >
    , &BlendMT_NonSeparable_MEM_Generic< uint8 >    , &BlendMT_NonSeparable_MEM_Generic_Subpixel< uint8 > )

/////////////////////////////////////////////////////
// AlphaBlend Invocation Selectors
template< typename T > ULIS3_IMP_ALPHABLEND_INVOCATION_SELECTOR( SelectAlphaBlendInvocationMEMGeneric
    , &AlphaBlendMT_Separable_MEM_Generic< T >      , &AlphaBlendMT_Separable_MEM_Generic_Subpixel< T > )
#ifdef ULIS3_COMPILETIME_AVX2_SUPPORT
ULIS3_IMP_ALPHABLEND_INVOCATION_SELECTOR( SelectAlphaBlendInvocationAVXRGBA8
    , &AlphaBlendMT_Separable_AVX_RGBA8             , &AlphaBlendMT_Separable_AVX_RGBA8_Subpixel )
#endif // ULIS3_COMPILETIME_AVX2_SUPPORT
#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
ULIS3_IMP_ALPHABLEND_INVOCATION_SELECTOR( SelectAlphaBlendInvocationSSERGBA8
    , &AlphaBlendMT_Separable_SSE_RGBA8             , &AlphaBlendMT_Separable_SSE_RGBA8_Subpixel )
#endif // ULIS3_COMPILETIME_SSE42_SUPPORT
ULIS3_IMP_ALPHABLEND_INVOCATION_SELECTOR( SelectAlphaBlendInvocationMEMRGBA8
    , &AlphaBlendMT_Separable_MEM_Generic< uint8 >  , &AlphaBlendMT_Separable_MEM_Generic_Subpixel< uint8 > )

/////////////////////////////////////////////////////
// TiledBlend Invocation Selectors
template< typename T > ULIS3_IMP_TILEDBLEND_INVOCATION_SELECTOR( SelectTiledBlendInvocationMEMGeneric
    , &TiledBlendMT_Misc_MEM_Generic< T >, &TiledBlendMT_Separable_MEM_Generic< T >, &TiledBlendMT_NonSeparable_MEM_Generic< T > )
#ifdef ULIS3_COMPILETIME_AVX2_SUPPORT
ULIS3_IMP_TILEDBLEND_INVOCATION_SELECTOR( SelectTiledBlendInvocationAVXRGBA8
    , &TiledBlendMT_Misc_MEM_Generic< uint8 >, &TiledBlendMT_NonSeparable_SSE_RGBA8, &TiledBlendMT_NonSeparable_SSE_RGBA8 )
#endif // ULIS3_COMPILETIME_AVX2_SUPPORT
#ifdef ULIS3_COMPILETIME_SSE42_SUPPORT
ULIS3_IMP_TILEDBLEND_INVOCATION_SELECTOR( SelectTiledBlendInvocationSSERGBA8
    , &TiledBlendMT_Misc_MEM_Generic< uint8 >, &TiledBlendMT_Separable_SSE_RGBA8, &TiledBlendMT_NonSeparable_SSE_RGBA8 )
#endif // ULIS3_COMPILETIME_SSE42_SUPPORT
ULIS3_IMP_TILEDBLEND_INVOCATION_SELECTOR( SelectTiledBlendInvocationMEMRGBA8
    , &TiledBlendMT_Misc_MEM_Generic< uint8 >, &TiledBlendMT_Separable_MEM_Generic< uint8 >, &TiledBlendMT_NonSeparable_MEM_Generic< uint8 > )

/////////////////////////////////////////////////////
// Blend Dispatcher
ULIS3_BEGIN_DISPATCHER_GENERIC( FBlendInvocationSelector, fpBlendInvocation, FBlendArgs, &SelectBlendInvocationMEMGeneric< T > )
    ULIS3_DECL_DISPATCH_SPEC( &DispatchTestIsUnorderedRGBA8, &SelectBlendInvocationAVXRGBA8, &SelectBlendInvocationSSERGBA8, &SelectBlendInvocationMEMRGBA8 )
ULIS3_END_DISPATCHER( FBlendInvocationSelector )

/////////////////////////////////////////////////////
// AlphaBlend Dispatcher
ULIS3_BEGIN_DISPATCHER_GENERIC( FAlphaBlendInvocationSelector, fpBlendInvocation, FBlendArgs, &SelectAlphaBlendInvocationMEMGeneric< T > )
    ULIS3_DECL_DISPATCH_SPEC( &DispatchTestIsUnorderedRGBA8, &SelectAlphaBlendInvocationAVXRGBA8, &SelectAlphaBlendInvocationSSERGBA8, &SelectAlphaBlendInvocationMEMRGBA8 )
ULIS3_END_DISPATCHER( FAlphaBlendInvocationSelector )

/////////////////////////////////////////////////////
// TiledBlend Dispatcher
ULIS3_BEGIN_DISPATCHER_GENERIC( FTiledBlendInvocationSelector, fpBlendInvocation, FBlendArgs, &SelectTiledBlendInvocationMEMGeneric< T > )
    ULIS3_DECL_DISPATCH_SPEC( &DispatchTestIsUnorderedRGBA8, &SelectTiledBlendInvocationAVXRGBA8, &SelectTiledBlendInvocationSSERGBA8, &SelectTiledBlendInvocationMEMRGBA8 )
ULIS3_END_DISPATCHER( FTiledBlendInvocationSelector )

ULIS3_NAMESPACE_END

