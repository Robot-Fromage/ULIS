// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ConvDispatch.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the ConvBuffer entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Conv/ConvertFormatInvocations.h"
#include "Conv/Generic/ToGrey.h"
#include "Conv/Generic/ToRGB.h"
#include "Conv/Generic/ToHSV.h"
#include "Conv/Generic/ToHSL.h"
#include "Conv/Generic/ToCMY.h"
#include "Conv/Generic/ToCMYK.h"
#include "Conv/Generic/ToYUV.h"
#include "Conv/Generic/ToLab.h"
#include "Conv/Generic/ToXYZ.h"
#include "Conv/Generic/ToYxy.h"

ULIS_NAMESPACE_BEGIN
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ To Grey
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectGrey( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvBufferGreyToGrey < T1, T2 >;
        case CM_RGB     : return  &ConvBufferRGBToGrey  < T1, T2 >;
        case CM_HSV     : return  &ConvBufferHSVToGrey  < T1, T2 >;
        case CM_HSL     : return  &ConvBufferHSLToGrey  < T1, T2 >;
        case CM_CMY     : return  &ConvBufferCMYToGrey  < T1, T2 >;
        case CM_CMYK    : return  &ConvBufferCMYKToGrey < T1, T2 >;
        case CM_YUV     : return  &ConvBufferYUVToGrey  < T1, T2 >;
        case CM_Lab     : return  &ConvBufferLabToGrey  < T1, T2 >;
        case CM_XYZ     : return  &ConvBufferXYZToGrey  < T1, T2 >;
        case CM_Yxy     : return  &ConvBufferYxyToGrey  < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To RGB
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectRGB( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvBufferGreyToRGB  < T1, T2 >;
        case CM_RGB     : return  &ConvBufferRGBToRGB   < T1, T2 >;
        case CM_HSV     : return  &ConvBufferHSVToRGB   < T1, T2 >;
        case CM_HSL     : return  &ConvBufferHSLToRGB   < T1, T2 >;
        case CM_CMY     : return  &ConvBufferCMYToRGB   < T1, T2 >;
        case CM_CMYK    : return  &ConvBufferCMYKToRGB  < T1, T2 >;
        case CM_YUV     : return  &ConvBufferYUVToRGB   < T1, T2 >;
        case CM_Lab     : return  &ConvBufferLabToRGB   < T1, T2 >;
        case CM_XYZ     : return  &ConvBufferXYZToRGB   < T1, T2 >;
        case CM_Yxy     : return  &ConvBufferYxyToRGB   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To HSV
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectHSV( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvBufferGreyToHSV  < T1, T2 >;
        case CM_RGB     : return  &ConvBufferRGBToHSV   < T1, T2 >;
        case CM_HSV     : return  &ConvBufferHSVToHSV   < T1, T2 >;
        case CM_HSL     : return  &ConvBufferHSLToHSV   < T1, T2 >;
        case CM_CMY     : return  &ConvBufferCMYToHSV   < T1, T2 >;
        case CM_CMYK    : return  &ConvBufferCMYKToHSV  < T1, T2 >;
        case CM_YUV     : return  &ConvBufferYUVToHSV   < T1, T2 >;
        case CM_Lab     : return  &ConvBufferLabToHSV   < T1, T2 >;
        case CM_XYZ     : return  &ConvBufferXYZToHSV   < T1, T2 >;
        case CM_Yxy     : return  &ConvBufferYxyToHSV   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To HSL
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectHSL( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvBufferGreyToHSL  < T1, T2 >;
        case CM_RGB     : return  &ConvBufferRGBToHSL   < T1, T2 >;
        case CM_HSV     : return  &ConvBufferHSVToHSL   < T1, T2 >;
        case CM_HSL     : return  &ConvBufferHSLToHSL   < T1, T2 >;
        case CM_CMY     : return  &ConvBufferCMYToHSL   < T1, T2 >;
        case CM_CMYK    : return  &ConvBufferCMYKToHSL  < T1, T2 >;
        case CM_YUV     : return  &ConvBufferYUVToHSL   < T1, T2 >;
        case CM_Lab     : return  &ConvBufferLabToHSL   < T1, T2 >;
        case CM_XYZ     : return  &ConvBufferXYZToHSL   < T1, T2 >;
        case CM_Yxy     : return  &ConvBufferYxyToHSL   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To CMY
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectCMY( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvBufferGreyToCMY  < T1, T2 >;
        case CM_RGB     : return  &ConvBufferRGBToCMY   < T1, T2 >;
        case CM_HSV     : return  &ConvBufferHSVToCMY   < T1, T2 >;
        case CM_HSL     : return  &ConvBufferHSLToCMY   < T1, T2 >;
        case CM_CMY     : return  &ConvBufferCMYToCMY   < T1, T2 >;
        case CM_CMYK    : return  &ConvBufferCMYKToCMY  < T1, T2 >;
        case CM_YUV     : return  &ConvBufferYUVToCMY   < T1, T2 >;
        case CM_Lab     : return  &ConvBufferLabToCMY   < T1, T2 >;
        case CM_XYZ     : return  &ConvBufferXYZToCMY   < T1, T2 >;
        case CM_Yxy     : return  &ConvBufferYxyToCMY   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ To CMYK
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectCMYK( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvBufferGreyToCMYK < T1, T2 >;
        case CM_RGB     : return  &ConvBufferRGBToCMYK  < T1, T2 >;
        case CM_HSV     : return  &ConvBufferHSVToCMYK  < T1, T2 >;
        case CM_HSL     : return  &ConvBufferHSLToCMYK  < T1, T2 >;
        case CM_CMY     : return  &ConvBufferCMYToCMYK  < T1, T2 >;
        case CM_CMYK    : return  &ConvBufferCMYKToCMYK < T1, T2 >;
        case CM_YUV     : return  &ConvBufferYUVToCMYK  < T1, T2 >;
        case CM_Lab     : return  &ConvBufferLabToCMYK  < T1, T2 >;
        case CM_XYZ     : return  &ConvBufferXYZToCMYK  < T1, T2 >;
        case CM_Yxy     : return  &ConvBufferYxyToCMYK  < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To YUV
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectYUV( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvBufferGreyToYUV  < T1, T2 >;
        case CM_RGB     : return  &ConvBufferRGBToYUV   < T1, T2 >;
        case CM_HSV     : return  &ConvBufferHSVToYUV   < T1, T2 >;
        case CM_HSL     : return  &ConvBufferHSLToYUV   < T1, T2 >;
        case CM_CMY     : return  &ConvBufferCMYToYUV   < T1, T2 >;
        case CM_CMYK    : return  &ConvBufferCMYKToYUV  < T1, T2 >;
        case CM_YUV     : return  &ConvBufferYUVToYUV   < T1, T2 >;
        case CM_Lab     : return  &ConvBufferLabToYUV   < T1, T2 >;
        case CM_XYZ     : return  &ConvBufferXYZToYUV   < T1, T2 >;
        case CM_Yxy     : return  &ConvBufferYxyToYUV   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To Lab
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectLab( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvBufferGreyToLab  < T1, T2 >;
        case CM_RGB     : return  &ConvBufferRGBToLab   < T1, T2 >;
        case CM_HSV     : return  &ConvBufferHSVToLab   < T1, T2 >;
        case CM_HSL     : return  &ConvBufferHSLToLab   < T1, T2 >;
        case CM_CMY     : return  &ConvBufferCMYToLab   < T1, T2 >;
        case CM_CMYK    : return  &ConvBufferCMYKToLab  < T1, T2 >;
        case CM_YUV     : return  &ConvBufferYUVToLab   < T1, T2 >;
        case CM_Lab     : return  &ConvBufferLabToLab   < T1, T2 >;
        case CM_XYZ     : return  &ConvBufferXYZToLab   < T1, T2 >;
        case CM_Yxy     : return  &ConvBufferYxyToLab   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To XYZ
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectXYZ( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvBufferGreyToXYZ  < T1, T2 >;
        case CM_RGB     : return  &ConvBufferRGBToXYZ   < T1, T2 >;
        case CM_HSV     : return  &ConvBufferHSVToXYZ   < T1, T2 >;
        case CM_HSL     : return  &ConvBufferHSLToXYZ   < T1, T2 >;
        case CM_CMY     : return  &ConvBufferCMYToXYZ   < T1, T2 >;
        case CM_CMYK    : return  &ConvBufferCMYKToXYZ  < T1, T2 >;
        case CM_YUV     : return  &ConvBufferYUVToXYZ   < T1, T2 >;
        case CM_Lab     : return  &ConvBufferLabToXYZ   < T1, T2 >;
        case CM_XYZ     : return  &ConvBufferXYZToXYZ   < T1, T2 >;
        case CM_Yxy     : return  &ConvBufferYxyToXYZ   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To Yxy
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectYxy( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvBufferGreyToYxy  < T1, T2 >;
        case CM_RGB     : return  &ConvBufferRGBToYxy   < T1, T2 >;
        case CM_HSV     : return  &ConvBufferHSVToYxy   < T1, T2 >;
        case CM_HSL     : return  &ConvBufferHSLToYxy   < T1, T2 >;
        case CM_CMY     : return  &ConvBufferCMYToYxy   < T1, T2 >;
        case CM_CMYK    : return  &ConvBufferCMYKToYxy  < T1, T2 >;
        case CM_YUV     : return  &ConvBufferYUVToYxy   < T1, T2 >;
        case CM_Lab     : return  &ConvBufferLabToYxy   < T1, T2 >;
        case CM_XYZ     : return  &ConvBufferXYZToYxy   < T1, T2 >;
        case CM_Yxy     : return  &ConvBufferYxyToYxy   < T1, T2 >;
    }
    return  nullptr;
}

/////////////////////////////////////////////////////
// Select Model
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectModel( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iDstFormat ) ) )
    {
        case CM_GREY    :   return  QueryDispatchedConversionInvocation_SelectGrey< T1, T2 >( iSrcFormat, iDstFormat );
        case CM_RGB     :   return  QueryDispatchedConversionInvocation_SelectRGB<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_HSV     :   return  QueryDispatchedConversionInvocation_SelectHSV<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_HSL     :   return  QueryDispatchedConversionInvocation_SelectHSL<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_CMY     :   return  QueryDispatchedConversionInvocation_SelectCMY<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_CMYK    :   return  QueryDispatchedConversionInvocation_SelectCMYK< T1, T2 >( iSrcFormat, iDstFormat );
        case CM_YUV     :   return  QueryDispatchedConversionInvocation_SelectYUV<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_Lab     :   return  QueryDispatchedConversionInvocation_SelectLab<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_XYZ     :   return  QueryDispatchedConversionInvocation_SelectXYZ<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_Yxy     :   return  QueryDispatchedConversionInvocation_SelectYxy<  T1, T2 >( iSrcFormat, iDstFormat );
    }
    return  nullptr;
}

/////////////////////////////////////////////////////
// Conversion Dispatcher
fpConversionInvocation QueryDispatchedConversionInvocation( eFormat iSrcFormat, eFormat iDstFormat )
{
        switch( static_cast< eType >( ULIS_R_TYPE( iSrcFormat ) ) ) {
        case Type_uint8: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchedConversionInvocation_SelectModel< uint8, uint8       >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchedConversionInvocation_SelectModel< uint8, uint16      >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchedConversionInvocation_SelectModel< uint8, uint32      >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchedConversionInvocation_SelectModel< uint8, ufloat      >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchedConversionInvocation_SelectModel< uint8, udouble     >( iSrcFormat, iDstFormat );
        }
        case Type_uint16: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchedConversionInvocation_SelectModel< uint16, uint8      >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchedConversionInvocation_SelectModel< uint16, uint16     >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchedConversionInvocation_SelectModel< uint16, uint32     >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchedConversionInvocation_SelectModel< uint16, ufloat     >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchedConversionInvocation_SelectModel< uint16, udouble    >( iSrcFormat, iDstFormat );
        }
        case Type_uint32: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchedConversionInvocation_SelectModel< uint32, uint8      >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchedConversionInvocation_SelectModel< uint32, uint16     >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchedConversionInvocation_SelectModel< uint32, uint32     >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchedConversionInvocation_SelectModel< uint32, ufloat     >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchedConversionInvocation_SelectModel< uint32, udouble    >( iSrcFormat, iDstFormat );
        }
        case Type_ufloat: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchedConversionInvocation_SelectModel< ufloat, uint8      >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchedConversionInvocation_SelectModel< ufloat, uint16     >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchedConversionInvocation_SelectModel< ufloat, uint32     >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchedConversionInvocation_SelectModel< ufloat, ufloat     >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchedConversionInvocation_SelectModel< ufloat, udouble    >( iSrcFormat, iDstFormat );
        }
        //DISABLED:DOUBLEcase TYPE_UDOUBLE: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
        //DISABLED:DOUBLE        case Type_uint8:    return  QueryDispatchedConversionInvocation_SelectModel< udouble, uint8     >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case Type_uint16:   return  QueryDispatchedConversionInvocation_SelectModel< udouble, uint16    >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case Type_uint32:   return  QueryDispatchedConversionInvocation_SelectModel< udouble, uint32    >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case Type_ufloat:   return  QueryDispatchedConversionInvocation_SelectModel< udouble, ufloat    >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case TYPE_UDOUBLE:  return  QueryDispatchedConversionInvocation_SelectModel< udouble, udouble   >( iSrcFormat, iDstFormat ); }
    }

    return  nullptr;
}

/////////////////////////////////////////////////////
// Template Instanciations
#define X_DO_01( T, U )   template ULIS_API void ConvBufferGreyToGrey< T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_02( T, U )   template ULIS_API void ConvBufferRGBToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_03( T, U )   template ULIS_API void ConvBufferHSVToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_04( T, U )   template ULIS_API void ConvBufferHSLToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_05( T, U )   template ULIS_API void ConvBufferCMYToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_06( T, U )   template ULIS_API void ConvBufferCMYKToGrey< T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_07( T, U )   template ULIS_API void ConvBufferYUVToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_08( T, U )   template ULIS_API void ConvBufferLabToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_09( T, U )   template ULIS_API void ConvBufferXYZToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_10( T, U )   template ULIS_API void ConvBufferYxyToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_11( T, U )   template ULIS_API void ConvBufferGreyToRGB<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_12( T, U )   template ULIS_API void ConvBufferRGBToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_13( T, U )   template ULIS_API void ConvBufferHSVToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_14( T, U )   template ULIS_API void ConvBufferHSLToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_15( T, U )   template ULIS_API void ConvBufferCMYToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_16( T, U )   template ULIS_API void ConvBufferCMYKToRGB<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_17( T, U )   template ULIS_API void ConvBufferYUVToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_18( T, U )   template ULIS_API void ConvBufferLabToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_19( T, U )   template ULIS_API void ConvBufferXYZToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_20( T, U )   template ULIS_API void ConvBufferYxyToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_21( T, U )   template ULIS_API void ConvBufferGreyToHSV<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_22( T, U )   template ULIS_API void ConvBufferRGBToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_23( T, U )   template ULIS_API void ConvBufferHSVToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_24( T, U )   template ULIS_API void ConvBufferHSLToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_25( T, U )   template ULIS_API void ConvBufferCMYToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_26( T, U )   template ULIS_API void ConvBufferCMYKToHSV<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_27( T, U )   template ULIS_API void ConvBufferYUVToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_28( T, U )   template ULIS_API void ConvBufferLabToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_29( T, U )   template ULIS_API void ConvBufferXYZToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_30( T, U )   template ULIS_API void ConvBufferYxyToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_31( T, U )   template ULIS_API void ConvBufferGreyToHSL<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_32( T, U )   template ULIS_API void ConvBufferRGBToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_33( T, U )   template ULIS_API void ConvBufferHSVToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_34( T, U )   template ULIS_API void ConvBufferHSLToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_35( T, U )   template ULIS_API void ConvBufferCMYToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_36( T, U )   template ULIS_API void ConvBufferCMYKToHSL<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_37( T, U )   template ULIS_API void ConvBufferYUVToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_38( T, U )   template ULIS_API void ConvBufferLabToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_39( T, U )   template ULIS_API void ConvBufferXYZToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_40( T, U )   template ULIS_API void ConvBufferYxyToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_41( T, U )   template ULIS_API void ConvBufferGreyToCMY<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_42( T, U )   template ULIS_API void ConvBufferRGBToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_43( T, U )   template ULIS_API void ConvBufferHSVToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_44( T, U )   template ULIS_API void ConvBufferHSLToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_45( T, U )   template ULIS_API void ConvBufferCMYToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_46( T, U )   template ULIS_API void ConvBufferCMYKToCMY<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_47( T, U )   template ULIS_API void ConvBufferYUVToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_48( T, U )   template ULIS_API void ConvBufferLabToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_49( T, U )   template ULIS_API void ConvBufferXYZToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_50( T, U )   template ULIS_API void ConvBufferYxyToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_51( T, U )   template ULIS_API void ConvBufferGreyToCMYK< T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_52( T, U )   template ULIS_API void ConvBufferRGBToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_53( T, U )   template ULIS_API void ConvBufferHSVToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_54( T, U )   template ULIS_API void ConvBufferHSLToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_55( T, U )   template ULIS_API void ConvBufferCMYToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_56( T, U )   template ULIS_API void ConvBufferCMYKToCMYK< T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_57( T, U )   template ULIS_API void ConvBufferYUVToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_58( T, U )   template ULIS_API void ConvBufferLabToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_59( T, U )   template ULIS_API void ConvBufferXYZToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_60( T, U )   template ULIS_API void ConvBufferYxyToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_61( T, U )   template ULIS_API void ConvBufferGreyToYUV<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_62( T, U )   template ULIS_API void ConvBufferRGBToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_63( T, U )   template ULIS_API void ConvBufferHSVToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_64( T, U )   template ULIS_API void ConvBufferHSLToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_65( T, U )   template ULIS_API void ConvBufferCMYToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_66( T, U )   template ULIS_API void ConvBufferCMYKToYUV<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_67( T, U )   template ULIS_API void ConvBufferYUVToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_68( T, U )   template ULIS_API void ConvBufferLabToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_69( T, U )   template ULIS_API void ConvBufferXYZToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_70( T, U )   template ULIS_API void ConvBufferYxyToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_71( T, U )   template ULIS_API void ConvBufferGreyToLab<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_72( T, U )   template ULIS_API void ConvBufferRGBToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_73( T, U )   template ULIS_API void ConvBufferHSVToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_74( T, U )   template ULIS_API void ConvBufferHSLToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_75( T, U )   template ULIS_API void ConvBufferCMYToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_76( T, U )   template ULIS_API void ConvBufferCMYKToLab<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_77( T, U )   template ULIS_API void ConvBufferYUVToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_78( T, U )   template ULIS_API void ConvBufferLabToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_79( T, U )   template ULIS_API void ConvBufferXYZToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_80( T, U )   template ULIS_API void ConvBufferYxyToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_81( T, U )   template ULIS_API void ConvBufferGreyToXYZ<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_82( T, U )   template ULIS_API void ConvBufferRGBToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_83( T, U )   template ULIS_API void ConvBufferHSVToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_84( T, U )   template ULIS_API void ConvBufferHSLToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_85( T, U )   template ULIS_API void ConvBufferCMYToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_86( T, U )   template ULIS_API void ConvBufferCMYKToXYZ<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_87( T, U )   template ULIS_API void ConvBufferYUVToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_88( T, U )   template ULIS_API void ConvBufferLabToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_89( T, U )   template ULIS_API void ConvBufferXYZToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_90( T, U )   template ULIS_API void ConvBufferYxyToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_91( T, U )   template ULIS_API void ConvBufferGreyToYxy<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_92( T, U )   template ULIS_API void ConvBufferRGBToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_93( T, U )   template ULIS_API void ConvBufferHSVToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_94( T, U )   template ULIS_API void ConvBufferHSLToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_95( T, U )   template ULIS_API void ConvBufferCMYToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_96( T, U )   template ULIS_API void ConvBufferCMYKToYxy<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_97( T, U )   template ULIS_API void ConvBufferYUVToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_98( T, U )   template ULIS_API void ConvBufferLabToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_99( T, U )   template ULIS_API void ConvBufferXYZToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_FF( T, U )   template ULIS_API void ConvBufferYxyToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_01 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_02 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_03 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_04 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_05 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_06 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_07 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_08 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_09 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_10 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_11 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_12 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_13 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_14 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_15 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_16 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_17 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_18 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_19 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_20 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_21 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_22 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_23 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_24 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_25 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_26 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_27 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_28 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_29 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_30 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_31 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_32 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_33 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_34 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_35 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_36 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_37 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_38 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_39 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_40 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_41 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_42 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_43 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_44 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_45 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_46 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_47 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_48 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_49 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_50 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_51 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_52 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_53 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_54 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_55 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_56 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_57 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_58 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_59 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_60 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_61 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_62 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_63 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_64 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_65 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_66 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_67 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_68 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_69 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_70 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_71 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_72 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_73 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_74 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_75 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_76 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_77 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_78 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_79 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_80 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_81 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_82 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_83 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_84 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_85 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_86 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_87 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_88 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_89 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_90 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_91 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_92 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_93 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_94 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_95 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_96 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_97 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_98 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_99 )
ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_DO_FF )
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

ULIS_NAMESPACE_END

