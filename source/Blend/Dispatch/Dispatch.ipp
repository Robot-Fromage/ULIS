// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Dispatch.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic Blend entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Base/HostDeviceInfo.h"
#include "Blend/Modes.h"
#include "Blend/Dispatch/BlendInfo.h"
#include "Blend/Dispatch/Generic/BlendMT_Separable_MEM_Generic.ipp"
#include "Blend/Dispatch/Generic/BlendMT_NonSeparable_MEM_Generic.ipp"
#include "Blend/Dispatch/Generic/BlendMT_Misc_MEM_Generic.ipp"
#include "Blend/Dispatch/RGBA8/BlendMT_Separable_SSE_RGBA8.ipp"
#include "Blend/Dispatch/RGBA8/BlendMT_NonSeparable_SSE_RGBA8.ipp"
#include "Blend/Dispatch/RGBA8/BlendMT_Separable_AVX_RGBA8.ipp"

#define ULIS2_SELECT_COMP_OP(   iSubpixel, _FUNCTION )      iSubpixel ? & _FUNCTION ## _Subpixel : & _FUNCTION
#define ULIS2_SELECT_COMP_OPT(  iSubpixel, _FUNCTION, _T )  iSubpixel ? & _FUNCTION ## _Subpixel < _T > : & _FUNCTION < _T >

ULIS2_NAMESPACE_BEGIN
typedef void (*fpDispatchedBlendFunc)( std::shared_ptr< const _FBlendInfoPrivate > iBlendParams );

template< typename T >
fpDispatchedBlendFunc
QueryDispatchedBlendFunctionForParameters_Generic( const _FBlendInfoPrivate& iInfo ) {
    switch( BlendingModeQualifier( iInfo.blendingMode ) ) {
        case BMQ_MISC           : return  ULIS2_SELECT_COMP_OPT( iInfo.subpixelFlag, BlendMT_Misc_MEM_Generic,          T );
        case BMQ_SEPARABLE      : return  ULIS2_SELECT_COMP_OPT( iInfo.subpixelFlag, BlendMT_Separable_MEM_Generic,     T );
        case BMQ_NONSEPARABLE   : return  ULIS2_SELECT_COMP_OPT( iInfo.subpixelFlag, BlendMT_NonSeparable_MEM_Generic,  T );
    }
    return  nullptr;
}


fpDispatchedBlendFunc
QueryDispatchedBlendFunctionForParameters_RGBA8( const _FBlendInfoPrivate& iInfo ) {
    switch( BlendingModeQualifier( iInfo.blendingMode ) ) {
        case BMQ_MISC: {
            return  ULIS2_SELECT_COMP_OPT( iInfo.subpixelFlag, BlendMT_Misc_MEM_Generic, uint8  );
        }

        case BMQ_SEPARABLE: {
            #ifdef __AVX2__
                if( iInfo.perfIntent & ULIS2_PERF_AVX2 && iInfo.hostDeviceInfo->HW_AVX2 )
                    return  ULIS2_SELECT_COMP_OP( iInfo.subpixelFlag, BlendMT_Separable_AVX_RGBA8 );
                else
            #endif
            #ifdef __SSE4_2__
                if( iInfo.hostDeviceInfo->HW_SSE42 )
                    return  ULIS2_SELECT_COMP_OP( iInfo.subpixelFlag, BlendMT_Separable_SSE_RGBA8 );
                else
            #endif
                    return  ULIS2_SELECT_COMP_OPT( iInfo.subpixelFlag, BlendMT_Separable_MEM_Generic,  uint8 );
        }

        case BMQ_NONSEPARABLE: {
            #ifdef __SSE4_2__
                if( iInfo.hostDeviceInfo->HW_SSE42 )
                    return  ULIS2_SELECT_COMP_OP( iInfo.subpixelFlag, BlendMT_NonSeparable_SSE_RGBA8 );
                else
            #endif
                    return  ULIS2_SELECT_COMP_OPT( iInfo.subpixelFlag, BlendMT_NonSeparable_MEM_Generic, uint8 );
        }
    }
    return  nullptr;
}


template< typename T >
fpDispatchedBlendFunc
QueryDispatchedBlendFunctionForParameters_imp( const _FBlendInfoPrivate& iInfo ) {
    return  QueryDispatchedBlendFunctionForParameters_Generic< T >( iInfo );
}


template<>
fpDispatchedBlendFunc
QueryDispatchedBlendFunctionForParameters_imp< uint8 >( const _FBlendInfoPrivate& iInfo ) {
    // RGBA8 Signature, any layout
    if( iInfo.source->HasAlpha()
     && iInfo.source->NumColorChannels()    == 3
     && iInfo.source->Model()               == CM_RGB
     && iInfo.perfIntent & ULIS2_PERF_TSPEC
     && ( iInfo.perfIntent & ULIS2_PERF_SSE42 || iInfo.perfIntent & ULIS2_PERF_AVX2 )
     && ( iInfo.hostDeviceInfo->HW_SSE42 || iInfo.hostDeviceInfo->HW_AVX2 ) ) {
        return  QueryDispatchedBlendFunctionForParameters_RGBA8( iInfo );
    }

    // Generic Fallback
    return  QueryDispatchedBlendFunctionForParameters_Generic< uint8 >( iInfo );
}


fpDispatchedBlendFunc
QueryDispatchedBlendFunctionForParameters( const _FBlendInfoPrivate& iInfo ) {
    switch( iInfo.source->Type() ) {
        case TYPE_UINT8     : return  QueryDispatchedBlendFunctionForParameters_imp< uint8   >( iInfo );
        case TYPE_UINT16    : return  QueryDispatchedBlendFunctionForParameters_imp< uint16  >( iInfo );
        case TYPE_UINT32    : return  QueryDispatchedBlendFunctionForParameters_imp< uint32  >( iInfo );
        case TYPE_UFLOAT    : return  QueryDispatchedBlendFunctionForParameters_imp< ufloat  >( iInfo );
        case TYPE_UDOUBLE   : return  QueryDispatchedBlendFunctionForParameters_imp< udouble >( iInfo );
    }
    return  nullptr;
}

ULIS2_NAMESPACE_END

