// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ConvDispatch.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the ConvertFormat entry point functions.
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
fpConvertFormat
QueryDispatchConvertFormatInvocation_SelectGrey( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToGrey < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToGrey  < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToGrey  < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToGrey  < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToGrey  < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToGrey < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToGrey  < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToGrey  < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToGrey  < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToGrey  < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To RGB
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchConvertFormatInvocation_SelectRGB( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToRGB  < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToRGB   < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToRGB   < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToRGB   < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToRGB   < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToRGB  < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToRGB   < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToRGB   < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToRGB   < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToRGB   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To HSV
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchConvertFormatInvocation_SelectHSV( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToHSV  < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToHSV   < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToHSV   < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToHSV   < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToHSV   < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToHSV  < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToHSV   < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToHSV   < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToHSV   < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToHSV   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To HSL
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchConvertFormatInvocation_SelectHSL( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToHSL  < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToHSL   < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToHSL   < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToHSL   < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToHSL   < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToHSL  < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToHSL   < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToHSL   < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToHSL   < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToHSL   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To CMY
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchConvertFormatInvocation_SelectCMY( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToCMY  < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToCMY   < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToCMY   < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToCMY   < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToCMY   < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToCMY  < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToCMY   < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToCMY   < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToCMY   < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToCMY   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ To CMYK
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchConvertFormatInvocation_SelectCMYK( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToCMYK < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToCMYK  < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToCMYK  < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToCMYK  < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToCMYK  < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToCMYK < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToCMYK  < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToCMYK  < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToCMYK  < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToCMYK  < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To YUV
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchConvertFormatInvocation_SelectYUV( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToYUV  < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToYUV   < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToYUV   < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToYUV   < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToYUV   < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToYUV  < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToYUV   < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToYUV   < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToYUV   < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToYUV   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To Lab
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchConvertFormatInvocation_SelectLab( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToLab  < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToLab   < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToLab   < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToLab   < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToLab   < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToLab  < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToLab   < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToLab   < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToLab   < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToLab   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To XYZ
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchConvertFormatInvocation_SelectXYZ( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToXYZ  < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToXYZ   < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToXYZ   < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToXYZ   < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToXYZ   < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToXYZ  < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToXYZ   < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToXYZ   < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToXYZ   < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToXYZ   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To Yxy
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchConvertFormatInvocation_SelectYxy( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToYxy  < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToYxy   < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToYxy   < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToYxy   < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToYxy   < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToYxy  < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToYxy   < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToYxy   < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToYxy   < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToYxy   < T1, T2 >;
    }
    return  nullptr;
}

/////////////////////////////////////////////////////
// Select Model
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchConvertFormatInvocation_SelectModel( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iDstFormat ) ) )
    {
        case CM_GREY    :   return  QueryDispatchConvertFormatInvocation_SelectGrey< T1, T2 >( iSrcFormat, iDstFormat );
        case CM_RGB     :   return  QueryDispatchConvertFormatInvocation_SelectRGB<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_HSV     :   return  QueryDispatchConvertFormatInvocation_SelectHSV<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_HSL     :   return  QueryDispatchConvertFormatInvocation_SelectHSL<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_CMY     :   return  QueryDispatchConvertFormatInvocation_SelectCMY<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_CMYK    :   return  QueryDispatchConvertFormatInvocation_SelectCMYK< T1, T2 >( iSrcFormat, iDstFormat );
        case CM_YUV     :   return  QueryDispatchConvertFormatInvocation_SelectYUV<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_Lab     :   return  QueryDispatchConvertFormatInvocation_SelectLab<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_XYZ     :   return  QueryDispatchConvertFormatInvocation_SelectXYZ<  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_Yxy     :   return  QueryDispatchConvertFormatInvocation_SelectYxy<  T1, T2 >( iSrcFormat, iDstFormat );
    }
    return  nullptr;
}

/////////////////////////////////////////////////////
// Conversion Dispatcher
fpConvertFormat QueryDispatchConvertFormatInvocation( eFormat iSrcFormat, eFormat iDstFormat )
{
        switch( static_cast< eType >( ULIS_R_TYPE( iSrcFormat ) ) ) {
        case Type_uint8: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchConvertFormatInvocation_SelectModel< uint8, uint8       >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchConvertFormatInvocation_SelectModel< uint8, uint16      >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchConvertFormatInvocation_SelectModel< uint8, uint32      >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchConvertFormatInvocation_SelectModel< uint8, ufloat      >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchConvertFormatInvocation_SelectModel< uint8, udouble     >( iSrcFormat, iDstFormat );
        }
        case Type_uint16: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchConvertFormatInvocation_SelectModel< uint16, uint8      >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchConvertFormatInvocation_SelectModel< uint16, uint16     >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchConvertFormatInvocation_SelectModel< uint16, uint32     >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchConvertFormatInvocation_SelectModel< uint16, ufloat     >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchConvertFormatInvocation_SelectModel< uint16, udouble    >( iSrcFormat, iDstFormat );
        }
        case Type_uint32: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchConvertFormatInvocation_SelectModel< uint32, uint8      >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchConvertFormatInvocation_SelectModel< uint32, uint16     >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchConvertFormatInvocation_SelectModel< uint32, uint32     >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchConvertFormatInvocation_SelectModel< uint32, ufloat     >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchConvertFormatInvocation_SelectModel< uint32, udouble    >( iSrcFormat, iDstFormat );
        }
        case Type_ufloat: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchConvertFormatInvocation_SelectModel< ufloat, uint8      >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchConvertFormatInvocation_SelectModel< ufloat, uint16     >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchConvertFormatInvocation_SelectModel< ufloat, uint32     >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchConvertFormatInvocation_SelectModel< ufloat, ufloat     >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchConvertFormatInvocation_SelectModel< ufloat, udouble    >( iSrcFormat, iDstFormat );
        }
        //DISABLED:DOUBLEcase TYPE_UDOUBLE: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
        //DISABLED:DOUBLE        case Type_uint8:    return  QueryDispatchConvertFormatInvocation_SelectModel< udouble, uint8     >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case Type_uint16:   return  QueryDispatchConvertFormatInvocation_SelectModel< udouble, uint16    >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case Type_uint32:   return  QueryDispatchConvertFormatInvocation_SelectModel< udouble, uint32    >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case Type_ufloat:   return  QueryDispatchConvertFormatInvocation_SelectModel< udouble, ufloat    >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case TYPE_UDOUBLE:  return  QueryDispatchConvertFormatInvocation_SelectModel< udouble, udouble   >( iSrcFormat, iDstFormat ); }
    }

    return  nullptr;
}

/////////////////////////////////////////////////////
// Template Instanciations
#define X_DO_01( T, U )   template ULIS_API void ConvertFormatGreyToGrey< T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_02( T, U )   template ULIS_API void ConvertFormatRGBToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_03( T, U )   template ULIS_API void ConvertFormatHSVToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_04( T, U )   template ULIS_API void ConvertFormatHSLToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_05( T, U )   template ULIS_API void ConvertFormatCMYToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_06( T, U )   template ULIS_API void ConvertFormatCMYKToGrey< T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_07( T, U )   template ULIS_API void ConvertFormatYUVToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_08( T, U )   template ULIS_API void ConvertFormatLabToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_09( T, U )   template ULIS_API void ConvertFormatXYZToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_10( T, U )   template ULIS_API void ConvertFormatYxyToGrey<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_11( T, U )   template ULIS_API void ConvertFormatGreyToRGB<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_12( T, U )   template ULIS_API void ConvertFormatRGBToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_13( T, U )   template ULIS_API void ConvertFormatHSVToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_14( T, U )   template ULIS_API void ConvertFormatHSLToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_15( T, U )   template ULIS_API void ConvertFormatCMYToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_16( T, U )   template ULIS_API void ConvertFormatCMYKToRGB<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_17( T, U )   template ULIS_API void ConvertFormatYUVToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_18( T, U )   template ULIS_API void ConvertFormatLabToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_19( T, U )   template ULIS_API void ConvertFormatXYZToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_20( T, U )   template ULIS_API void ConvertFormatYxyToRGB<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_21( T, U )   template ULIS_API void ConvertFormatGreyToHSV<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_22( T, U )   template ULIS_API void ConvertFormatRGBToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_23( T, U )   template ULIS_API void ConvertFormatHSVToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_24( T, U )   template ULIS_API void ConvertFormatHSLToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_25( T, U )   template ULIS_API void ConvertFormatCMYToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_26( T, U )   template ULIS_API void ConvertFormatCMYKToHSV<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_27( T, U )   template ULIS_API void ConvertFormatYUVToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_28( T, U )   template ULIS_API void ConvertFormatLabToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_29( T, U )   template ULIS_API void ConvertFormatXYZToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_30( T, U )   template ULIS_API void ConvertFormatYxyToHSV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_31( T, U )   template ULIS_API void ConvertFormatGreyToHSL<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_32( T, U )   template ULIS_API void ConvertFormatRGBToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_33( T, U )   template ULIS_API void ConvertFormatHSVToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_34( T, U )   template ULIS_API void ConvertFormatHSLToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_35( T, U )   template ULIS_API void ConvertFormatCMYToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_36( T, U )   template ULIS_API void ConvertFormatCMYKToHSL<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_37( T, U )   template ULIS_API void ConvertFormatYUVToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_38( T, U )   template ULIS_API void ConvertFormatLabToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_39( T, U )   template ULIS_API void ConvertFormatXYZToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_40( T, U )   template ULIS_API void ConvertFormatYxyToHSL<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_41( T, U )   template ULIS_API void ConvertFormatGreyToCMY<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_42( T, U )   template ULIS_API void ConvertFormatRGBToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_43( T, U )   template ULIS_API void ConvertFormatHSVToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_44( T, U )   template ULIS_API void ConvertFormatHSLToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_45( T, U )   template ULIS_API void ConvertFormatCMYToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_46( T, U )   template ULIS_API void ConvertFormatCMYKToCMY<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_47( T, U )   template ULIS_API void ConvertFormatYUVToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_48( T, U )   template ULIS_API void ConvertFormatLabToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_49( T, U )   template ULIS_API void ConvertFormatXYZToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_50( T, U )   template ULIS_API void ConvertFormatYxyToCMY<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_51( T, U )   template ULIS_API void ConvertFormatGreyToCMYK< T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_52( T, U )   template ULIS_API void ConvertFormatRGBToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_53( T, U )   template ULIS_API void ConvertFormatHSVToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_54( T, U )   template ULIS_API void ConvertFormatHSLToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_55( T, U )   template ULIS_API void ConvertFormatCMYToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_56( T, U )   template ULIS_API void ConvertFormatCMYKToCMYK< T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_57( T, U )   template ULIS_API void ConvertFormatYUVToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_58( T, U )   template ULIS_API void ConvertFormatLabToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_59( T, U )   template ULIS_API void ConvertFormatXYZToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_60( T, U )   template ULIS_API void ConvertFormatYxyToCMYK<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_61( T, U )   template ULIS_API void ConvertFormatGreyToYUV<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_62( T, U )   template ULIS_API void ConvertFormatRGBToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_63( T, U )   template ULIS_API void ConvertFormatHSVToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_64( T, U )   template ULIS_API void ConvertFormatHSLToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_65( T, U )   template ULIS_API void ConvertFormatCMYToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_66( T, U )   template ULIS_API void ConvertFormatCMYKToYUV<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_67( T, U )   template ULIS_API void ConvertFormatYUVToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_68( T, U )   template ULIS_API void ConvertFormatLabToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_69( T, U )   template ULIS_API void ConvertFormatXYZToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_70( T, U )   template ULIS_API void ConvertFormatYxyToYUV<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_71( T, U )   template ULIS_API void ConvertFormatGreyToLab<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_72( T, U )   template ULIS_API void ConvertFormatRGBToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_73( T, U )   template ULIS_API void ConvertFormatHSVToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_74( T, U )   template ULIS_API void ConvertFormatHSLToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_75( T, U )   template ULIS_API void ConvertFormatCMYToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_76( T, U )   template ULIS_API void ConvertFormatCMYKToLab<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_77( T, U )   template ULIS_API void ConvertFormatYUVToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_78( T, U )   template ULIS_API void ConvertFormatLabToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_79( T, U )   template ULIS_API void ConvertFormatXYZToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_80( T, U )   template ULIS_API void ConvertFormatYxyToLab<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_81( T, U )   template ULIS_API void ConvertFormatGreyToXYZ<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_82( T, U )   template ULIS_API void ConvertFormatRGBToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_83( T, U )   template ULIS_API void ConvertFormatHSVToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_84( T, U )   template ULIS_API void ConvertFormatHSLToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_85( T, U )   template ULIS_API void ConvertFormatCMYToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_86( T, U )   template ULIS_API void ConvertFormatCMYKToXYZ<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_87( T, U )   template ULIS_API void ConvertFormatYUVToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_88( T, U )   template ULIS_API void ConvertFormatLabToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_89( T, U )   template ULIS_API void ConvertFormatXYZToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_90( T, U )   template ULIS_API void ConvertFormatYxyToXYZ<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_91( T, U )   template ULIS_API void ConvertFormatGreyToYxy<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_92( T, U )   template ULIS_API void ConvertFormatRGBToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_93( T, U )   template ULIS_API void ConvertFormatHSVToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_94( T, U )   template ULIS_API void ConvertFormatHSLToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_95( T, U )   template ULIS_API void ConvertFormatCMYToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_96( T, U )   template ULIS_API void ConvertFormatCMYKToYxy<  T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_97( T, U )   template ULIS_API void ConvertFormatYUVToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_98( T, U )   template ULIS_API void ConvertFormatLabToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_99( T, U )   template ULIS_API void ConvertFormatXYZToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
#define X_DO_FF( T, U )   template ULIS_API void ConvertFormatYxyToYxy<   T, U >( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
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

