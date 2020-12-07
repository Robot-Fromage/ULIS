// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ToYUV.h
* @author       Clement Berthaud
* @brief        This file provides the definition for model conversion functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
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
// To YUV
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvertFormatGreyToYUV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatGreyToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYUV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvertFormatRGBToYUV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = srgb2linear( ConvType< T, ufloat >( iSrc.ChannelT< T >( 0 ) ) );
        ufloat g = srgb2linear( ConvType< T, ufloat >( iSrc.ChannelT< T >( 1 ) ) );
        ufloat b = srgb2linear( ConvType< T, ufloat >( iSrc.ChannelT< T >( 2 ) ) );
        float y = 0.299f * r + 0.587f * g + 0.114f * b;
        float u = 0.492f * ( b - y ); // + 0.435912 * ( 1 / ( 0.886 + 0.435912 ) )
        float v = 0.877f * ( r - y ); // + 0.621787 * ( 1 / ( 0.701 + 0.621787 ) )
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( y ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( u ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( v ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvertFormatHSVToYUV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSVToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYUV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvertFormatHSLToYUV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSLToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYUV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvertFormatCMYToYUV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYUV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvertFormatCMYKToYUV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYKToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYUV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvertFormatYUVToYUV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYUVToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYUV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvertFormatLabToYUV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatLabToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYUV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvertFormatXYZToYUV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatXYZToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYUV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvertFormatYxyToYUV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYxyToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToYUV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

ULIS_NAMESPACE_END

