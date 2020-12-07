// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ToHSV.h
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

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To HSV
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvertFormatGreyToHSV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatGreyToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToHSV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvertFormatRGBToHSV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = ConvType< T, ufloat >( iSrc.ChannelT< T >( 0 ) );
        ufloat g = ConvType< T, ufloat >( iSrc.ChannelT< T >( 1 ) );
        ufloat b = ConvType< T, ufloat >( iSrc.ChannelT< T >( 2 ) );
        ufloat cmin = FMath::Min3( r, g, b );
        ufloat cmax = FMath::Max3( r, g, b );
        ufloat delta = cmax - cmin;

        float h = 0.0;
        float s = 0.0;
        float v = 0.0;
        v = cmax;
        if ( delta < FMath::kEpsilonf ){
            h = s = 0.f;
        } else {
            s = (delta / cmax );
            float deltaR = ( ( ( cmax - r ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
            float deltaG = ( ( ( cmax - g ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
            float deltaB = ( ( ( cmax - b ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
            if( fabs( r - cmax ) < FMath::kEpsilonf )        h = deltaB - deltaG;
            else if( fabs( g - cmax ) < FMath::kEpsilonf )   h = ( 1.0f / 3.0f ) + deltaR - deltaB;
            else if( fabs( b - cmax ) < FMath::kEpsilonf )   h = ( 2.0f / 3.0f ) + deltaG - deltaR;
            if( h < 0.0f ) h += 1.0f;
            if( h > 1.0f ) h -= 1.0f;
        }

        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( h ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( s ) );
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
ConvertFormatHSVToHSV( const FPixel iSrc, FPixel iDst, uint32 iLen )
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
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvertFormatHSLToHSV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSLToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToHSV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvertFormatCMYToHSV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToHSV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvertFormatCMYKToHSV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYKToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToHSV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvertFormatYUVToHSV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYUVToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToHSV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvertFormatLabToHSV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatLabToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToHSV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvertFormatXYZToHSV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatXYZToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToHSV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvertFormatYxyToHSV( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYxyToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToHSV< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

ULIS_NAMESPACE_END

