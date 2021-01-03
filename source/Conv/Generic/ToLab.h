// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ToLab.h
* @author       Clement Berthaud
* @brief        This file provides the definition for model conversion functions.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/Conv.h"
#include "Image/Color.h"
#include "Image/Pixel.h"
#include "Math/Math.h"
#include <lcms2.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To Lab
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvertFormatGreyToLab( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatGreyToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvertFormatRGBToLab( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = srgb2linear( ConvType< T, ufloat >( iSrc.ChannelT< T >( 0 ) ) );
        ufloat g = srgb2linear( ConvType< T, ufloat >( iSrc.ChannelT< T >( 1 ) ) );
        ufloat b = srgb2linear( ConvType< T, ufloat >( iSrc.ChannelT< T >( 2 ) ) );
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
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( _L ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( _a ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( _b ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvertFormatHSVToLab( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSVToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvertFormatHSLToLab( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSLToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvertFormatCMYToLab( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvertFormatCMYKToLab( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYKToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvertFormatYUVToLab( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYUVToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToLab< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvertFormatLabToLab( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        iDst.SetChannelT< U >( 0, ConvType< T, U >( iSrc.ChannelT< T >( 0 ) ) );
        iDst.SetChannelT< U >( 1, ConvType< T, U >( iSrc.ChannelT< T >( 1 ) ) );
        iDst.SetChannelT< U >( 2, ConvType< T, U >( iSrc.ChannelT< T >( 2 ) ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvertFormatXYZToLab( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIELab Lab;
        cmsCIEXYZ XYZ;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        XYZ.X = ConvType< T, ufloat >( iSrc.ChannelT< T >( 0 ) ) * 100.0;
        XYZ.Y = ConvType< T, ufloat >( iSrc.ChannelT< T >( 1 ) ) * 100.0;
        XYZ.Z = ConvType< T, ufloat >( iSrc.ChannelT< T >( 2 ) ) * 100.0;
        cmsXYZ2Lab( &D65, &Lab, &XYZ );
        ufloat _L = static_cast< ufloat >( Lab.L / 100.f );
        ufloat _a = static_cast< ufloat >( Lab.a / 255.f + 0.5f );
        ufloat _b = static_cast< ufloat >( Lab.b / 255.f + 0.5f );
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( _L ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( _a ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( _b ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvertFormatYxyToLab( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIExyY xyY;
        cmsCIEXYZ XYZ;
        xyY.Y = ConvType< T, ufloat >( iSrc.ChannelT< T >( 0 ) );
        xyY.x = ConvType< T, ufloat >( iSrc.ChannelT< T >( 1 ) );
        xyY.y = ConvType< T, ufloat >( iSrc.ChannelT< T >( 2 ) );
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
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( _L ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( _a ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( _b ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

ULIS_NAMESPACE_END

