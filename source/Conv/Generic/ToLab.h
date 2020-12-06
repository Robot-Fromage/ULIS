// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ToLab.h
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
// To Lab
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvertFormatGreyToLab( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatGreyToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvertFormatRGBToLab( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = srgb2linear( ConvType< T, ufloat >( DREF_SRC( 0 ) ) );
        ufloat g = srgb2linear( ConvType< T, ufloat >( DREF_SRC( 1 ) ) );
        ufloat b = srgb2linear( ConvType< T, ufloat >( DREF_SRC( 2 ) ) );
        float x = 0.4124f * r + 0.3576f * g + 0.1805f * b;
        float y = 0.2126f * r + 0.7152f * g + 0.0722f * b;
        float z = 0.0193f * r + 0.1192f * g + 0.9505f * b;
        cmsCIELab Lab;
        cmsCIEXYZ XYZ;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        XYZ.X = x * 100.0;
        XYZ.Y = y * 100.0;
        XYZ.Z = z * 100.0;
        cmsXYZ2Lab( &D65, &Lab, &XYZ );
        ufloat _L = static_cast< ufloat >( Lab.L / 100.f );
        ufloat _a = static_cast< ufloat >( Lab.a / 255.f + 0.5f );
        ufloat _b = static_cast< ufloat >( Lab.b / 255.f + 0.5f );
        DREF_DST( 0 ) = ConvType< ufloat, U >( _L );
        DREF_DST( 1 ) = ConvType< ufloat, U >( _a );
        DREF_DST( 2 ) = ConvType< ufloat, U >( _b );
        FWD_ALPHA;
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvertFormatHSVToLab( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvertFormatHSLToLab( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSLToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvertFormatCMYToLab( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvertFormatCMYKToLab( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYKToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvertFormatYUVToLab( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYUVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvertFormatLabToLab( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        DREF_DST( 0 ) = ConvType< T, U >( DREF_SRC( 0 ) );
        DREF_DST( 1 ) = ConvType< T, U >( DREF_SRC( 1 ) );
        DREF_DST( 2 ) = ConvType< T, U >( DREF_SRC( 2 ) );
        FWD_ALPHA;
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvertFormatXYZToLab( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIELab Lab;
        cmsCIEXYZ XYZ;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        XYZ.X = ConvType< T, ufloat >( DREF_SRC( 0 ) ) * 100.0;
        XYZ.Y = ConvType< T, ufloat >( DREF_SRC( 1 ) ) * 100.0;
        XYZ.Z = ConvType< T, ufloat >( DREF_SRC( 2 ) ) * 100.0;
        cmsXYZ2Lab( &D65, &Lab, &XYZ );
        ufloat _L = static_cast< ufloat >( Lab.L / 100.f );
        ufloat _a = static_cast< ufloat >( Lab.a / 255.f + 0.5f );
        ufloat _b = static_cast< ufloat >( Lab.b / 255.f + 0.5f );
        DREF_DST( 0 ) = ConvType< ufloat, U >( _L );
        DREF_DST( 1 ) = ConvType< ufloat, U >( _a );
        DREF_DST( 2 ) = ConvType< ufloat, U >( _b );
        FWD_ALPHA;
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvertFormatYxyToLab( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIExyY xyY;
        cmsCIEXYZ XYZ;
        xyY.Y = ConvType< T, ufloat >( DREF_SRC( 0 ) );
        xyY.x = ConvType< T, ufloat >( DREF_SRC( 1 ) );
        xyY.y = ConvType< T, ufloat >( DREF_SRC( 2 ) );
        cmsxyY2XYZ( &XYZ, &xyY );

        cmsCIELab Lab;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        XYZ.X = XYZ.X * 100.0;
        XYZ.Y = XYZ.Y * 100.0;
        XYZ.Z = XYZ.Z * 100.0;
        cmsXYZ2Lab( &D65, &Lab, &XYZ );
        ufloat _L = static_cast< ufloat >( Lab.L / 100.f );
        ufloat _a = static_cast< ufloat >( Lab.a / 255.f + 0.5f );
        ufloat _b = static_cast< ufloat >( Lab.b / 255.f + 0.5f );
        DREF_DST( 0 ) = ConvType< ufloat, U >( _L );
        DREF_DST( 1 ) = ConvType< ufloat, U >( _a );
        DREF_DST( 2 ) = ConvType< ufloat, U >( _b );
        FWD_ALPHA;
        iSrc.Next();
        iDst.Next();
    }
}

ULIS_NAMESPACE_END

