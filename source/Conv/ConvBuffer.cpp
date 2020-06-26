// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         ConvBuffer.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the ConvBuffer entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Conv/ConvBuffer.h"
#include "Conv/ConvDispatch.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Conversion Dispatcher
fpConversionInvocation QueryDispatchedConversionInvocation( uint32 iSrcFormat, uint32 iDstFormat )
{
        switch( static_cast< eType >( ULIS3_R_TYPE( iSrcFormat ) ) ) {
        case TYPE_UINT8: switch( static_cast< eType >( ULIS3_R_TYPE( iDstFormat ) ) ) {
                case TYPE_UINT8:    return  QueryDispatchedConversionInvocation_SelectModel< uint8, uint8       >( iSrcFormat, iDstFormat );
                case TYPE_UINT16:   return  QueryDispatchedConversionInvocation_SelectModel< uint8, uint16      >( iSrcFormat, iDstFormat );
                case TYPE_UINT32:   return  QueryDispatchedConversionInvocation_SelectModel< uint8, uint32      >( iSrcFormat, iDstFormat );
                case TYPE_UFLOAT:   return  QueryDispatchedConversionInvocation_SelectModel< uint8, ufloat      >( iSrcFormat, iDstFormat );
                case TYPE_UDOUBLE:  return  QueryDispatchedConversionInvocation_SelectModel< uint8, udouble     >( iSrcFormat, iDstFormat ); }
        case TYPE_UINT16: switch( static_cast< eType >( ULIS3_R_TYPE( iDstFormat ) ) ) {
                case TYPE_UINT8:    return  QueryDispatchedConversionInvocation_SelectModel< uint16, uint8      >( iSrcFormat, iDstFormat );
                case TYPE_UINT16:   return  QueryDispatchedConversionInvocation_SelectModel< uint16, uint16     >( iSrcFormat, iDstFormat );
                case TYPE_UINT32:   return  QueryDispatchedConversionInvocation_SelectModel< uint16, uint32     >( iSrcFormat, iDstFormat );
                case TYPE_UFLOAT:   return  QueryDispatchedConversionInvocation_SelectModel< uint16, ufloat     >( iSrcFormat, iDstFormat );
                case TYPE_UDOUBLE:  return  QueryDispatchedConversionInvocation_SelectModel< uint16, udouble    >( iSrcFormat, iDstFormat ); }
        case TYPE_UINT32: switch( static_cast< eType >( ULIS3_R_TYPE( iDstFormat ) ) ) {
                case TYPE_UINT8:    return  QueryDispatchedConversionInvocation_SelectModel< uint32, uint8      >( iSrcFormat, iDstFormat );
                case TYPE_UINT16:   return  QueryDispatchedConversionInvocation_SelectModel< uint32, uint16     >( iSrcFormat, iDstFormat );
                case TYPE_UINT32:   return  QueryDispatchedConversionInvocation_SelectModel< uint32, uint32     >( iSrcFormat, iDstFormat );
                case TYPE_UFLOAT:   return  QueryDispatchedConversionInvocation_SelectModel< uint32, ufloat     >( iSrcFormat, iDstFormat );
                case TYPE_UDOUBLE:  return  QueryDispatchedConversionInvocation_SelectModel< uint32, udouble    >( iSrcFormat, iDstFormat ); }
        case TYPE_UFLOAT: switch( static_cast< eType >( ULIS3_R_TYPE( iDstFormat ) ) ) {
                case TYPE_UINT8:    return  QueryDispatchedConversionInvocation_SelectModel< ufloat, uint8      >( iSrcFormat, iDstFormat );
                case TYPE_UINT16:   return  QueryDispatchedConversionInvocation_SelectModel< ufloat, uint16     >( iSrcFormat, iDstFormat );
                case TYPE_UINT32:   return  QueryDispatchedConversionInvocation_SelectModel< ufloat, uint32     >( iSrcFormat, iDstFormat );
                case TYPE_UFLOAT:   return  QueryDispatchedConversionInvocation_SelectModel< ufloat, ufloat     >( iSrcFormat, iDstFormat );
                case TYPE_UDOUBLE:  return  QueryDispatchedConversionInvocation_SelectModel< ufloat, udouble    >( iSrcFormat, iDstFormat ); }
        case TYPE_UDOUBLE: switch( static_cast< eType >( ULIS3_R_TYPE( iDstFormat ) ) ) {
                case TYPE_UINT8:    return  QueryDispatchedConversionInvocation_SelectModel< udouble, uint8     >( iSrcFormat, iDstFormat );
                case TYPE_UINT16:   return  QueryDispatchedConversionInvocation_SelectModel< udouble, uint16    >( iSrcFormat, iDstFormat );
                case TYPE_UINT32:   return  QueryDispatchedConversionInvocation_SelectModel< udouble, uint32    >( iSrcFormat, iDstFormat );
                case TYPE_UFLOAT:   return  QueryDispatchedConversionInvocation_SelectModel< udouble, ufloat    >( iSrcFormat, iDstFormat );
                case TYPE_UDOUBLE:  return  QueryDispatchedConversionInvocation_SelectModel< udouble, udouble   >( iSrcFormat, iDstFormat ); }
    }

    return  nullptr;
}

/////////////////////////////////////////////////////
// Template Instanciations
#define X_DO_01( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferGreyToGrey<   t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_02( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferRGBToGrey<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_03( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSVToGrey<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_04( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSLToGrey<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_05( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYToGrey<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_06( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYKToGrey<   t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_07( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYUVToGrey<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_08( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferLabToGrey<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_09( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferXYZToGrey<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_10( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYxyToGrey<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_11( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferGreyToRGB<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_12( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferRGBToRGB<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_13( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSVToRGB<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_14( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSLToRGB<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_15( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYToRGB<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_16( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYKToRGB<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_17( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYUVToRGB<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_18( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferLabToRGB<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_19( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferXYZToRGB<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_20( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYxyToRGB<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_21( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferGreyToHSV<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_22( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferRGBToHSV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_23( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSVToHSV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_24( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSLToHSV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_25( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYToHSV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_26( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYKToHSV<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_27( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYUVToHSV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_28( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferLabToHSV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_29( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferXYZToHSV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_30( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYxyToHSV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_31( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferGreyToHSL<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_32( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferRGBToHSL<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_33( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSVToHSL<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_34( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSLToHSL<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_35( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYToHSL<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_36( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYKToHSL<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_37( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYUVToHSL<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_38( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferLabToHSL<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_39( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferXYZToHSL<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_40( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYxyToHSL<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_41( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferGreyToCMY<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_42( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferRGBToCMY<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_43( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSVToCMY<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_44( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSLToCMY<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_45( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYToCMY<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_46( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYKToCMY<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_47( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYUVToCMY<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_48( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferLabToCMY<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_49( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferXYZToCMY<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_50( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYxyToCMY<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_51( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferGreyToCMYK<   t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_52( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferRGBToCMYK<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_53( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSVToCMYK<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_54( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSLToCMYK<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_55( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYToCMYK<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_56( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYKToCMYK<   t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_57( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYUVToCMYK<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_58( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferLabToCMYK<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_59( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferXYZToCMYK<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_60( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYxyToCMYK<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_61( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferGreyToYUV<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_62( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferRGBToYUV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_63( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSVToYUV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_64( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSLToYUV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_65( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYToYUV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_66( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYKToYUV<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_67( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYUVToYUV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_68( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferLabToYUV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_69( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferXYZToYUV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_70( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYxyToYUV<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_71( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferGreyToLab<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_72( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferRGBToLab<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_73( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSVToLab<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_74( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSLToLab<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_75( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYToLab<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_76( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYKToLab<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_77( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYUVToLab<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_78( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferLabToLab<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_79( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferXYZToLab<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_80( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYxyToLab<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_81( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferGreyToXYZ<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_82( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferRGBToXYZ<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_83( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSVToXYZ<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_84( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSLToXYZ<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_85( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYToXYZ<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_86( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYKToXYZ<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_87( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYUVToXYZ<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_88( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferLabToXYZ<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_89( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferXYZToXYZ<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_90( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYxyToXYZ<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_91( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferGreyToYxy<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_92( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferRGBToYxy<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_93( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSVToYxy<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_94( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferHSLToYxy<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_95( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYToYxy<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_96( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferCMYKToYxy<    t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_97( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYUVToYxy<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_98( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferLabToYxy<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_99( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferXYZToYxy<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
#define X_DO_FF( t1, t2 )   ULIS3_API_TEMPLATE void ConvBufferYxyToYxy<     t1, t2 >( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen );
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_01 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_02 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_03 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_04 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_05 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_06 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_07 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_08 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_09 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_10 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_11 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_12 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_13 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_14 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_15 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_16 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_17 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_18 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_19 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_20 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_21 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_22 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_23 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_24 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_25 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_26 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_27 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_28 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_29 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_30 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_31 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_32 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_33 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_34 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_35 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_36 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_37 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_38 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_39 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_40 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_41 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_42 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_43 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_44 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_45 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_46 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_47 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_48 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_49 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_50 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_51 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_52 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_53 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_54 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_55 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_56 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_57 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_58 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_59 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_60 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_61 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_62 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_63 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_64 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_65 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_66 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_67 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_68 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_69 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_70 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_71 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_72 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_73 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_74 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_75 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_76 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_77 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_78 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_79 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_80 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_81 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_82 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_83 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_84 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_85 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_86 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_87 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_88 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_89 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_90 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_91 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_92 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_93 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_94 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_95 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_96 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_97 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_98 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_99 )
ULIS3_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_FF )
#undef X_DO_01
#undef X_DO_02
#undef X_DO_03
#undef X_DO_04
#undef X_DO_05
#undef X_DO_06
#undef X_DO_07
#undef X_DO_08
#undef X_DO_09
#undef X_DO_10
#undef X_DO_11
#undef X_DO_12
#undef X_DO_13
#undef X_DO_14
#undef X_DO_15
#undef X_DO_16
#undef X_DO_17
#undef X_DO_18
#undef X_DO_19
#undef X_DO_20
#undef X_DO_21
#undef X_DO_22
#undef X_DO_23
#undef X_DO_24
#undef X_DO_25
#undef X_DO_26
#undef X_DO_27
#undef X_DO_28
#undef X_DO_29
#undef X_DO_30
#undef X_DO_31
#undef X_DO_32
#undef X_DO_33
#undef X_DO_34
#undef X_DO_35
#undef X_DO_36
#undef X_DO_37
#undef X_DO_38
#undef X_DO_39
#undef X_DO_40
#undef X_DO_41
#undef X_DO_42
#undef X_DO_43
#undef X_DO_44
#undef X_DO_45
#undef X_DO_46
#undef X_DO_47
#undef X_DO_48
#undef X_DO_49
#undef X_DO_50
#undef X_DO_51
#undef X_DO_52
#undef X_DO_53
#undef X_DO_54
#undef X_DO_55
#undef X_DO_56
#undef X_DO_57
#undef X_DO_58
#undef X_DO_59
#undef X_DO_60
#undef X_DO_61
#undef X_DO_62
#undef X_DO_63
#undef X_DO_64
#undef X_DO_65
#undef X_DO_66
#undef X_DO_67
#undef X_DO_68
#undef X_DO_69
#undef X_DO_70
#undef X_DO_71
#undef X_DO_72
#undef X_DO_73
#undef X_DO_74
#undef X_DO_75
#undef X_DO_76
#undef X_DO_77
#undef X_DO_78
#undef X_DO_79
#undef X_DO_80
#undef X_DO_81
#undef X_DO_82
#undef X_DO_83
#undef X_DO_84
#undef X_DO_85
#undef X_DO_86
#undef X_DO_87
#undef X_DO_88
#undef X_DO_89
#undef X_DO_90
#undef X_DO_91
#undef X_DO_92
#undef X_DO_93
#undef X_DO_94
#undef X_DO_95
#undef X_DO_96
#undef X_DO_97
#undef X_DO_98
#undef X_DO_99
#undef X_DO_FF

ULIS3_NAMESPACE_END

