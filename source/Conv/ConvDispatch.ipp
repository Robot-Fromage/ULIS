// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         ConvDispatch.ipp
* @author       Clement Berthaud
* @brief        This file provides the dispatch implementation for the conversion functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/ConvBuffer.h"
#include "Conv/Generic/ToGrey.ipp"
#include "Conv/Generic/ToRGB.ipp"
#include "Conv/Generic/ToHSV.ipp"
#include "Conv/Generic/ToHSL.ipp"
#include "Conv/Generic/ToCMY.ipp"
#include "Conv/Generic/ToCMYK.ipp"
#include "Conv/Generic/ToYUV.ipp"
#include "Conv/Generic/ToLab.ipp"
#include "Conv/Generic/ToXYZ.ipp"
#include "Conv/Generic/ToYxy.ipp"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Per Model Select
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ To Grey
template< typename T1, typename T2 >
ULIS3_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectGrey( tFormat iSrcFormat, tFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS3_R_MODEL( iSrcFormat ) ) ) {
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
ULIS3_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectRGB( tFormat iSrcFormat, tFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS3_R_MODEL( iSrcFormat ) ) ) {
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
ULIS3_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectHSV( tFormat iSrcFormat, tFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS3_R_MODEL( iSrcFormat ) ) ) {
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
ULIS3_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectHSL( tFormat iSrcFormat, tFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS3_R_MODEL( iSrcFormat ) ) ) {
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
ULIS3_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectCMY( tFormat iSrcFormat, tFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS3_R_MODEL( iSrcFormat ) ) ) {
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
ULIS3_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectCMYK( tFormat iSrcFormat, tFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS3_R_MODEL( iSrcFormat ) ) ) {
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
ULIS3_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectYUV( tFormat iSrcFormat, tFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS3_R_MODEL( iSrcFormat ) ) ) {
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
ULIS3_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectLab( tFormat iSrcFormat, tFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS3_R_MODEL( iSrcFormat ) ) ) {
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
ULIS3_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectXYZ( tFormat iSrcFormat, tFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS3_R_MODEL( iSrcFormat ) ) ) {
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
ULIS3_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectYxy( tFormat iSrcFormat, tFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS3_R_MODEL( iSrcFormat ) ) ) {
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
ULIS3_FORCEINLINE
fpConversionInvocation
QueryDispatchedConversionInvocation_SelectModel( tFormat iSrcFormat, tFormat iDstFormat )
{
    switch( static_cast< eColorModel >( ULIS3_R_MODEL( iDstFormat ) ) )
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

ULIS3_NAMESPACE_END

