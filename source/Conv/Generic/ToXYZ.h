// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ToXYZ.h
* @author       Clement Berthaud
* @brief        This file provides the definition for model conversion functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/Conv.h"
#include "Conv/ConvHelpers.h"
#include "Image/Color.h"
#include "Image/Format.h"
#include "Image/Pixel.h"
#include "Image/Sample.h"
#include "Math/Math.h"
#include <lcms2.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To XYZ
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvertFormatGreyToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatGreyToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToXYZ< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvertFormatRGBToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = srgb2linear( ConvType< T, ufloat >( DREF_SRC( 0 ) ) );
        ufloat g = srgb2linear( ConvType< T, ufloat >( DREF_SRC( 1 ) ) );
        ufloat b = srgb2linear( ConvType< T, ufloat >( DREF_SRC( 2 ) ) );
        float x = 0.4124f * r + 0.3576f * g + 0.1805f * b;
        float y = 0.2126f * r + 0.7152f * g + 0.0722f * b;
        float z = 0.0193f * r + 0.1192f * g + 0.9505f * b;
        DREF_DST( 0 ) = ConvType< ufloat, U >( x );
        DREF_DST( 1 ) = ConvType< ufloat, U >( y );
        DREF_DST( 2 ) = ConvType< ufloat, U >( z );
        FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvertFormatHSVToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToXYZ< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvertFormatHSLToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSLToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToXYZ< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvertFormatCMYToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToXYZ< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvertFormatCMYKToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYKToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToXYZ< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvertFormatYUVToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYUVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToXYZ< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvertFormatLabToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIELab Lab;
        cmsCIEXYZ XYZ;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        Lab.L = ConvType< T, udouble >( DREF_SRC( 0 ) ) * 100.0;
        Lab.a = ( ConvType< T, udouble >( DREF_SRC( 1 ) ) - 0.5 ) * 255.0;
        Lab.b = ( ConvType< T, udouble >( DREF_SRC( 2 ) ) - 0.5 ) * 255.0;
        cmsLab2XYZ( &D65, &XYZ, &Lab );
        ufloat x = static_cast< ufloat >( XYZ.X );
        ufloat y = static_cast< ufloat >( XYZ.Y );
        ufloat z = static_cast< ufloat >( XYZ.Z );
        DREF_DST( 0 ) = ConvType< ufloat, U >( x );
        DREF_DST( 1 ) = ConvType< ufloat, U >( y );
        DREF_DST( 2 ) = ConvType< ufloat, U >( z );
        FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvertFormatXYZToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen )
{
    while( iLen-- )
    {
        DREF_DST( 0 ) = ConvType< T, U >( DREF_SRC( 0 ) );
        DREF_DST( 1 ) = ConvType< T, U >( DREF_SRC( 1 ) );
        DREF_DST( 2 ) = ConvType< T, U >( DREF_SRC( 2 ) );
        FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvertFormatYxyToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIExyY xyY;
        cmsCIEXYZ XYZ;
        xyY.Y = ConvType< T, udouble >( DREF_SRC( 0 ) );
        xyY.x = ConvType< T, udouble >( DREF_SRC( 1 ) );
        xyY.y = ConvType< T, udouble >( DREF_SRC( 2 ) );
        cmsxyY2XYZ( &XYZ, &xyY );
        ufloat x = static_cast< ufloat >( XYZ.X );
        ufloat y = static_cast< ufloat >( XYZ.Y );
        ufloat z = static_cast< ufloat >( XYZ.Z );
        DREF_DST( 0 ) = ConvType< ufloat, U >( x );
        DREF_DST( 1 ) = ConvType< ufloat, U >( y );
        DREF_DST( 2 ) = ConvType< ufloat, U >( z );
        FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

ULIS_NAMESPACE_END

