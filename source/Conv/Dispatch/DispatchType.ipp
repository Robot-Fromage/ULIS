// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         DispatchType.ipp
* @author       Clement Berthaud
* @brief        This file provides the definition for the type dispatcher for the conv entry point functions
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/ConvBuffer.h"
//Dispatch
#include "Conv/Dispatch/DispatchModel.ipp"

ULIS2_NAMESPACE_BEGIN
fpDispatchedConvInvoke QueryDispatchedConvInvokeForParameters( uint32 iSrcFormat, uint32 iDstFormat )
{
        switch( static_cast< eType >( ULIS2_R_TYPE( iSrcFormat ) ) ) {
        case TYPE_UINT8: switch( static_cast< eType >( ULIS2_R_TYPE( iDstFormat ) ) ) {
                case TYPE_UINT8:    return  QueryDispatchedConvInvokeForParameters_SelectModel< uint8, uint8       >( iSrcFormat, iDstFormat );
                case TYPE_UINT16:   return  QueryDispatchedConvInvokeForParameters_SelectModel< uint8, uint16      >( iSrcFormat, iDstFormat );
                case TYPE_UINT32:   return  QueryDispatchedConvInvokeForParameters_SelectModel< uint8, uint32      >( iSrcFormat, iDstFormat );
                case TYPE_UFLOAT:   return  QueryDispatchedConvInvokeForParameters_SelectModel< uint8, ufloat      >( iSrcFormat, iDstFormat );
                case TYPE_UDOUBLE:  return  QueryDispatchedConvInvokeForParameters_SelectModel< uint8, udouble     >( iSrcFormat, iDstFormat ); }
        case TYPE_UINT16: switch( static_cast< eType >( ULIS2_R_TYPE( iDstFormat ) ) ) {
                case TYPE_UINT8:    return  QueryDispatchedConvInvokeForParameters_SelectModel< uint16, uint8      >( iSrcFormat, iDstFormat );
                case TYPE_UINT16:   return  QueryDispatchedConvInvokeForParameters_SelectModel< uint16, uint16     >( iSrcFormat, iDstFormat );
                case TYPE_UINT32:   return  QueryDispatchedConvInvokeForParameters_SelectModel< uint16, uint32     >( iSrcFormat, iDstFormat );
                case TYPE_UFLOAT:   return  QueryDispatchedConvInvokeForParameters_SelectModel< uint16, ufloat     >( iSrcFormat, iDstFormat );
                case TYPE_UDOUBLE:  return  QueryDispatchedConvInvokeForParameters_SelectModel< uint16, udouble    >( iSrcFormat, iDstFormat ); }
        case TYPE_UINT32: switch( static_cast< eType >( ULIS2_R_TYPE( iDstFormat ) ) ) {
                case TYPE_UINT8:    return  QueryDispatchedConvInvokeForParameters_SelectModel< uint32, uint8      >( iSrcFormat, iDstFormat );
                case TYPE_UINT16:   return  QueryDispatchedConvInvokeForParameters_SelectModel< uint32, uint16     >( iSrcFormat, iDstFormat );
                case TYPE_UINT32:   return  QueryDispatchedConvInvokeForParameters_SelectModel< uint32, uint32     >( iSrcFormat, iDstFormat );
                case TYPE_UFLOAT:   return  QueryDispatchedConvInvokeForParameters_SelectModel< uint32, ufloat     >( iSrcFormat, iDstFormat );
                case TYPE_UDOUBLE:  return  QueryDispatchedConvInvokeForParameters_SelectModel< uint32, udouble    >( iSrcFormat, iDstFormat ); }
        case TYPE_UFLOAT: switch( static_cast< eType >( ULIS2_R_TYPE( iDstFormat ) ) ) {
                case TYPE_UINT8:    return  QueryDispatchedConvInvokeForParameters_SelectModel< ufloat, uint8      >( iSrcFormat, iDstFormat );
                case TYPE_UINT16:   return  QueryDispatchedConvInvokeForParameters_SelectModel< ufloat, uint16     >( iSrcFormat, iDstFormat );
                case TYPE_UINT32:   return  QueryDispatchedConvInvokeForParameters_SelectModel< ufloat, uint32     >( iSrcFormat, iDstFormat );
                case TYPE_UFLOAT:   return  QueryDispatchedConvInvokeForParameters_SelectModel< ufloat, ufloat     >( iSrcFormat, iDstFormat );
                case TYPE_UDOUBLE:  return  QueryDispatchedConvInvokeForParameters_SelectModel< ufloat, udouble    >( iSrcFormat, iDstFormat ); }
        case TYPE_UDOUBLE: switch( static_cast< eType >( ULIS2_R_TYPE( iDstFormat ) ) ) {
                case TYPE_UINT8:    return  QueryDispatchedConvInvokeForParameters_SelectModel< udouble, uint8     >( iSrcFormat, iDstFormat );
                case TYPE_UINT16:   return  QueryDispatchedConvInvokeForParameters_SelectModel< udouble, uint16    >( iSrcFormat, iDstFormat );
                case TYPE_UINT32:   return  QueryDispatchedConvInvokeForParameters_SelectModel< udouble, uint32    >( iSrcFormat, iDstFormat );
                case TYPE_UFLOAT:   return  QueryDispatchedConvInvokeForParameters_SelectModel< udouble, ufloat    >( iSrcFormat, iDstFormat );
                case TYPE_UDOUBLE:  return  QueryDispatchedConvInvokeForParameters_SelectModel< udouble, udouble   >( iSrcFormat, iDstFormat ); }
    }

    return  nullptr;
}
ULIS2_NAMESPACE_END

