// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ToYxy.h
* @author       Clement Berthaud
* @brief        This file provides the definition for model conversion functions.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Conv/Conv.h"
#include "Image/Color.h"
#include "Image/Pixel.h"
#include "Math/Math.h"
#include <lcms2.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To Yxy
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvertFormatGreyToYxy( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatGreyToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYxy< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvertFormatRGBToYxy( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = srgb2linear( ConvType< T, ufloat >( iSrc.ChannelT< T >( 0 ) ) );
        ufloat g = srgb2linear( ConvType< T, ufloat >( iSrc.ChannelT< T >( 1 ) ) );
        ufloat b = srgb2linear( ConvType< T, ufloat >( iSrc.ChannelT< T >( 2 ) ) );
        cmsCIEXYZ XYZ;
        XYZ.X = 0.4124f * r + 0.3576f * g + 0.1805f * b;
        XYZ.Y = 0.2126f * r + 0.7152f * g + 0.0722f * b;
        XYZ.Z = 0.0193f * r + 0.1192f * g + 0.9505f * b;
        cmsCIExyY xyY;
        cmsXYZ2xyY( &xyY, &XYZ );
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( static_cast< ufloat >( xyY.Y ) ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( static_cast< ufloat >( xyY.x ) ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( static_cast< ufloat >( xyY.y ) ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvertFormatHSVToYxy( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSVToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYxy< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvertFormatHSLToYxy( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSLToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYxy< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvertFormatCMYToYxy( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYxy< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvertFormatCMYKToYxy( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYKToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYxy< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvertFormatYUVToYxy( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYUVToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYxy< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvertFormatLabToYxy( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIELab Lab;
        cmsCIEXYZ XYZ;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        Lab.L = ConvType< T, udouble >( iSrc.ChannelT< T >( 0 ) ) * 100.0;
        Lab.a = ( ConvType< T, udouble >( iSrc.ChannelT< T >( 1 ) ) - 0.5 ) * 255.0;
        Lab.b = ( ConvType< T, udouble >( iSrc.ChannelT< T >( 2 ) ) - 0.5 ) * 255.0;
        cmsLab2XYZ( &D65, &XYZ, &Lab );
        cmsCIExyY xyY;
        cmsXYZ2xyY( &xyY, &XYZ );
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( static_cast< ufloat >( xyY.Y ) ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( static_cast< ufloat >( xyY.x ) ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( static_cast< ufloat >( xyY.y ) ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvertFormatXYZToYxy( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIEXYZ XYZ;
            cmsCIExyY xyY;
        XYZ.X = ConvType< T, ufloat >( iSrc.ChannelT< T >( 0 ) );
        XYZ.Y = ConvType< T, ufloat >( iSrc.ChannelT< T >( 1 ) );
        XYZ.Z = ConvType< T, ufloat >( iSrc.ChannelT< T >( 2 ) );
        cmsXYZ2xyY( &xyY, &XYZ );
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( static_cast< ufloat >( xyY.Y ) ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( static_cast< ufloat >( xyY.x ) ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( static_cast< ufloat >( xyY.y ) ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvertFormatYxyToYxy( const FPixel iSrc, FPixel iDst, uint32 iLen )
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

ULIS_NAMESPACE_END

