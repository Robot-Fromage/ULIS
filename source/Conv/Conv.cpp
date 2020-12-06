// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Conv.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Conv entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Conv/Conv.h"
#include "Image/Block.h"
#include "Image/Pixel.h"
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
/////////////////////////////////////////////////////
// Invocations
void
InvokeConvertFormat_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FConvCommandArgs* cargs
)
{
    cargs->invocation(
          FPixel( jargs->src, cargs->src.Format() )
        , FPixel( jargs->dst, cargs->dst.Format() )
        , static_cast< uint32 >( jargs->size / cargs->dst.BytesPerPixel() )
    );
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleConvertFormat, FDualBufferJobArgs, FConvCommandArgs, &InvokeConvertFormat_MEM_Generic )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedConvertFormatInvocationSchedulerSelector )

/////////////////////////////////////////////////////
// Explicit Conv Entry Points
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ To Grey
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchedConvertFormatInvocation_SelectGrey( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToGrey  < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToGrey   < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToGrey   < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToGrey   < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToGrey   < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToGrey  < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToGrey   < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToGrey   < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToGrey   < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToGrey   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To RGB
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchedConvertFormatInvocation_SelectRGB( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToRGB   < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToRGB    < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToRGB    < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToRGB    < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToRGB    < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToRGB   < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToRGB    < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToRGB    < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToRGB    < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToRGB    < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To HSV
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchedConvertFormatInvocation_SelectHSV( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToHSV   < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToHSV    < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToHSV    < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToHSV    < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToHSV    < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToHSV   < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToHSV    < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToHSV    < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToHSV    < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToHSV    < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To HSL
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchedConvertFormatInvocation_SelectHSL( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToHSL   < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToHSL    < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToHSL    < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToHSL    < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToHSL    < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToHSL   < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToHSL    < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToHSL    < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToHSL    < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToHSL    < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To CMY
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchedConvertFormatInvocation_SelectCMY( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToCMY   < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToCMY    < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToCMY    < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToCMY    < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToCMY    < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToCMY   < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToCMY    < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToCMY    < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToCMY    < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToCMY    < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ To CMYK
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchedConvertFormatInvocation_SelectCMYK( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToCMYK  < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToCMYK   < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToCMYK   < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToCMYK   < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToCMYK   < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToCMYK  < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToCMYK   < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToCMYK   < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToCMYK   < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToCMYK   < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To YUV
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchedConvertFormatInvocation_SelectYUV( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToYUV   < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToYUV    < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToYUV    < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToYUV    < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToYUV    < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToYUV   < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToYUV    < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToYUV    < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToYUV    < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToYUV    < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To Lab
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchedConvertFormatInvocation_SelectLab( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToLab   < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToLab    < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToLab    < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToLab    < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToLab    < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToLab   < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToLab    < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToLab    < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToLab    < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToLab    < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To XYZ
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchedConvertFormatInvocation_SelectXYZ( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToXYZ   < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToXYZ    < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToXYZ    < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToXYZ    < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToXYZ    < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToXYZ   < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToXYZ    < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToXYZ    < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToXYZ    < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToXYZ    < T1, T2 >;
    }
    return  nullptr;
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- To Yxy
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchedConvertFormatInvocation_SelectYxy( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iSrcFormat ) ) ) {
        case CM_GREY    : return  &ConvertFormatGreyToYxy   < T1, T2 >;
        case CM_RGB     : return  &ConvertFormatRGBToYxy    < T1, T2 >;
        case CM_HSV     : return  &ConvertFormatHSVToYxy    < T1, T2 >;
        case CM_HSL     : return  &ConvertFormatHSLToYxy    < T1, T2 >;
        case CM_CMY     : return  &ConvertFormatCMYToYxy    < T1, T2 >;
        case CM_CMYK    : return  &ConvertFormatCMYKToYxy   < T1, T2 >;
        case CM_YUV     : return  &ConvertFormatYUVToYxy    < T1, T2 >;
        case CM_Lab     : return  &ConvertFormatLabToYxy    < T1, T2 >;
        case CM_XYZ     : return  &ConvertFormatXYZToYxy    < T1, T2 >;
        case CM_Yxy     : return  &ConvertFormatYxyToYxy    < T1, T2 >;
    }
    return  nullptr;
}

/////////////////////////////////////////////////////
// Select Model
template< typename T1, typename T2 >
ULIS_FORCEINLINE
fpConvertFormat
QueryDispatchedConvertFormatInvocation_SelectModel( eFormat iSrcFormat, eFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS_R_MODEL( iDstFormat ) ) )
    {
        case CM_GREY    : return  QueryDispatchedConvertFormatInvocation_SelectGrey < T1, T2 >( iSrcFormat, iDstFormat );
        case CM_RGB     : return  QueryDispatchedConvertFormatInvocation_SelectRGB  <  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_HSV     : return  QueryDispatchedConvertFormatInvocation_SelectHSV  <  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_HSL     : return  QueryDispatchedConvertFormatInvocation_SelectHSL  <  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_CMY     : return  QueryDispatchedConvertFormatInvocation_SelectCMY  <  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_CMYK    : return  QueryDispatchedConvertFormatInvocation_SelectCMYK < T1, T2 >( iSrcFormat, iDstFormat );
        case CM_YUV     : return  QueryDispatchedConvertFormatInvocation_SelectYUV  <  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_Lab     : return  QueryDispatchedConvertFormatInvocation_SelectLab  <  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_XYZ     : return  QueryDispatchedConvertFormatInvocation_SelectXYZ  <  T1, T2 >( iSrcFormat, iDstFormat );
        case CM_Yxy     : return  QueryDispatchedConvertFormatInvocation_SelectYxy  <  T1, T2 >( iSrcFormat, iDstFormat );
    }
    return  nullptr;
}

/////////////////////////////////////////////////////
// Conversion Dispatcher
fpConvertFormat QueryDispatchedConvertFormatInvocation( eFormat iSrcFormat, eFormat iDstFormat )
{
        switch( static_cast< eType >( ULIS_R_TYPE( iSrcFormat ) ) ) {
        case Type_uint8: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchedConvertFormatInvocation_SelectModel< uint8, uint8    >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchedConvertFormatInvocation_SelectModel< uint8, uint16   >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchedConvertFormatInvocation_SelectModel< uint8, uint32   >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchedConvertFormatInvocation_SelectModel< uint8, ufloat   >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchedConvertFormatInvocation_SelectModel< uint8, udouble     >( iSrcFormat, iDstFormat );
        }
        case Type_uint16: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchedConvertFormatInvocation_SelectModel< uint16, uint8   >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchedConvertFormatInvocation_SelectModel< uint16, uint16  >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchedConvertFormatInvocation_SelectModel< uint16, uint32  >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchedConvertFormatInvocation_SelectModel< uint16, ufloat  >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchedConvertFormatInvocation_SelectModel< uint16, udouble    >( iSrcFormat, iDstFormat );
        }
        case Type_uint32: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchedConvertFormatInvocation_SelectModel< uint32, uint8   >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchedConvertFormatInvocation_SelectModel< uint32, uint16  >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchedConvertFormatInvocation_SelectModel< uint32, uint32  >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchedConvertFormatInvocation_SelectModel< uint32, ufloat  >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchedConvertFormatInvocation_SelectModel< uint32, udouble    >( iSrcFormat, iDstFormat );
        }
        case Type_ufloat: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
                case Type_uint8:    return  QueryDispatchedConvertFormatInvocation_SelectModel< ufloat, uint8   >( iSrcFormat, iDstFormat );
                case Type_uint16:   return  QueryDispatchedConvertFormatInvocation_SelectModel< ufloat, uint16  >( iSrcFormat, iDstFormat );
                case Type_uint32:   return  QueryDispatchedConvertFormatInvocation_SelectModel< ufloat, uint32  >( iSrcFormat, iDstFormat );
                case Type_ufloat:   return  QueryDispatchedConvertFormatInvocation_SelectModel< ufloat, ufloat  >( iSrcFormat, iDstFormat );
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  QueryDispatchedConvertFormatInvocation_SelectModel< ufloat, udouble    >( iSrcFormat, iDstFormat );
        }
        //DISABLED:DOUBLEcase TYPE_UDOUBLE: switch( static_cast< eType >( ULIS_R_TYPE( iDstFormat ) ) ) {
        //DISABLED:DOUBLE        case Type_uint8:    return  QueryDispatchedConvertFormatInvocation_SelectModel< udouble, uint8     >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case Type_uint16:   return  QueryDispatchedConvertFormatInvocation_SelectModel< udouble, uint16    >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case Type_uint32:   return  QueryDispatchedConvertFormatInvocation_SelectModel< udouble, uint32    >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case Type_ufloat:   return  QueryDispatchedConvertFormatInvocation_SelectModel< udouble, ufloat    >( iSrcFormat, iDstFormat );
        //DISABLED:DOUBLE        case TYPE_UDOUBLE:  return  QueryDispatchedConvertFormatInvocation_SelectModel< udouble, udouble   >( iSrcFormat, iDstFormat ); }
    }

    return  nullptr;
}

/////////////////////////////////////////////////////
// Template Instanciations
#define X_EXPLICIT_TEMPLATE_SPEC_IMP( T, U, A, B ) template ULIS_API void ConvertFormat ## A ## To ## B ## < T, U >( FPixel iSrc, FPixel iDst, uint32 iLen );
#define X_EXPLICIT_TEMPLATE_SPEC( A, B ) ULIS_FOR_ALL_TYPES_COMBINATIONS_DO( X_EXPLICIT_TEMPLATE_SPEC_IMP, A, B )
ULIS_FOR_ALL_COLOR_MODELS_COMBINATIONS_DO( X_EXPLICIT_TEMPLATE_SPEC )

ULIS_NAMESPACE_END

