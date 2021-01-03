// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ToRGB.h
* @author       Clement Berthaud
* @brief        This file provides the definition for model conversion functions.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/Conv.h"
#include "Conv/srgb2linear.h"
#include "Image/Color.h"
#include "Image/Pixel.h"
#include "Math/Math.h"
#include <lcms2.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To RGB
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvertFormatGreyToRGB( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        U grey = ConvType< T, U >( iSrc.ChannelT< T >( 0 ) );
        iDst.SetChannelT< U >( 0, grey );
        iDst.SetChannelT< U >( 1, grey );
        iDst.SetChannelT< U >( 2, grey );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvertFormatRGBToRGB( const FPixel iSrc, FPixel iDst, uint32 iLen )
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
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvertFormatHSVToRGB( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        T _H = iSrc.ChannelT< T >( 0 );
        T _S = iSrc.ChannelT< T >( 1 );
        T _V = iSrc.ChannelT< T >( 2 );
        ufloat h = ConvType< T, ufloat >( _H );
        ufloat s = ConvType< T, ufloat >( _S );
        ufloat v = ConvType< T, ufloat >( _V );

        float r, g, b;
        r = g = b = 0.f;
        float var_h, var_i, var_1, var_2, var_3;
        var_h = var_i = var_1 = var_2 = var_3 = 0.f;
        if ( s == 0 ) {
            r = g = b = v;
        } else {
            var_h = h * 6;
            if ( var_h == 6 ) var_h = 0 ;
            var_i = floor( var_h );
            var_1 = v * ( 1 - s );
            var_2 = v * ( 1 - s * ( var_h - var_i ) );
            var_3 = v * ( 1 - s * ( 1 - ( var_h - var_i ) ) );

            if      ( var_i == 0 ) { r = v     ; g = var_3 ; b = var_1 ; }
            else if ( var_i == 1 ) { r = var_2 ; g = v     ; b = var_1 ; }
            else if ( var_i == 2 ) { r = var_1 ; g = v     ; b = var_3 ; }
            else if ( var_i == 3 ) { r = var_1 ; g = var_2 ; b = v     ; }
            else if ( var_i == 4 ) { r = var_3 ; g = var_1 ; b = v     ; }
            else                   { r = v     ; g = var_1 ; b = var_2 ; }
        }
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( r ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( g ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( b ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvertFormatHSLToRGB( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        T _H = iSrc.ChannelT< T >( 0 );
        T _S = iSrc.ChannelT< T >( 1 );
        T _L = iSrc.ChannelT< T >( 2 );
        ufloat h = ConvType< T, ufloat >( _H );
        ufloat s = ConvType< T, ufloat >( _S );
        ufloat l = ConvType< T, ufloat >( _L );
        float r, g, b;
        r = g = b = 0.f;
        float var_h = h * 6;
        if ( var_h == 6 ) var_h = 0;
        int var_i = static_cast< int >( var_h );
        float C = ( 1.f - fabs( 2.f * l - 1.f ) ) * s;
        float X = float( C * ( 1 - fabs( fmod( var_h, 2 ) - 1 ) ) );
        float m = l - C / 2;
        if      ( var_i == 0 ) { r = C; g = X; b = 0; }
        else if ( var_i == 1 ) { r = X; g = C; b = 0; }
        else if ( var_i == 2 ) { r = 0; g = C; b = X; }
        else if ( var_i == 3 ) { r = 0; g = X; b = C; }
        else if ( var_i == 4 ) { r = X; g = 0; b = C; }
        else                   { r = C; g = 0; b = X; }
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( r + m ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( g + m ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( b + m ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvertFormatCMYToRGB( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        U max = MaxType< U >();
        iDst.SetChannelT< U >( 0, max - ConvType< T, U >( iSrc.ChannelT< T >( 0 ) ) );
        iDst.SetChannelT< U >( 1, max - ConvType< T, U >( iSrc.ChannelT< T >( 1 ) ) );
        iDst.SetChannelT< U >( 2, max - ConvType< T, U >( iSrc.ChannelT< T >( 2 ) ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvertFormatCMYKToRGB( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat c = ConvType< T, ufloat >( iSrc.ChannelT< T >( 0 ) );
        ufloat m = ConvType< T, ufloat >( iSrc.ChannelT< T >( 1 ) );
        ufloat y = ConvType< T, ufloat >( iSrc.ChannelT< T >( 2 ) );
        ufloat k = ConvType< T, ufloat >( iSrc.ChannelT< T >( 3 ) );
        float r = 1.f - ( c * ( 1.f - k ) + k );
        float g = 1.f - ( m * ( 1.f - k ) + k );
        float b = 1.f - ( y * ( 1.f - k ) + k );
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( r ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( g ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( b ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvertFormatYUVToRGB( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat y = ConvType< T, ufloat >( iSrc.ChannelT< T >( 0 ) );
        ufloat u = ConvType< T, ufloat >( iSrc.ChannelT< T >( 1 ) );
        ufloat v = ConvType< T, ufloat >( iSrc.ChannelT< T >( 2 ) );
        float r = linear2srgb( y + 1.14f * v );
        float g = linear2srgb( y - 0.395f * u - 0.581f * v );
        float b = linear2srgb( y + 2.033f * u );
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( r ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( g ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( b ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvertFormatLabToRGB( const FPixel iSrc, FPixel iDst, uint32 iLen )
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
        ufloat x = static_cast< ufloat >( XYZ.X ) / 100.f;
        ufloat y = static_cast< ufloat >( XYZ.Y ) / 100.f;
        ufloat z = static_cast< ufloat >( XYZ.Z ) / 100.f;
        float r = linear2srgb( +3.2404542f * x - 1.5371385f * y - 0.4985314f * z );
        float g = linear2srgb( -0.9692660f * x + 1.8760108f * y + 0.0415560f * z );
        float b = linear2srgb( +0.0556434f * x - 0.2040259f * y + 1.0572252f * z );
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( r ) );
        iDst.SetChannelT< U >( 1, ConvType< ufloat, U >( g ) );
        iDst.SetChannelT< U >( 2, ConvType< ufloat, U >( b ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvertFormatXYZToRGB( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat x = ConvType< T, ufloat >( iSrc.ChannelT< T >( 0 ) );
        ufloat y = ConvType< T, ufloat >( iSrc.ChannelT< T >( 1 ) );
        ufloat z = ConvType< T, ufloat >( iSrc.ChannelT< T >( 2 ) );
        float r = linear2srgb( +3.2404542f * x - 1.5371385f * y - 0.4985314f * z );
        float g = linear2srgb( -0.9692660f * x + 1.8760108f * y + 0.0415560f * z );
        float b = linear2srgb( +0.0556434f * x - 0.2040259f * y + 1.0572252f * z );
        iDst.SetChannelT< U >( 0, ConvType< float, U >( r ) );
        iDst.SetChannelT< U >( 1, ConvType< float, U >( g ) );
        iDst.SetChannelT< U >( 2, ConvType< float, U >( b ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvertFormatYxyToRGB( const FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIExyY xyY;
        cmsCIEXYZ XYZ;
        xyY.Y = ConvType< T, udouble >( iSrc.ChannelT< T >( 0 ) );
        xyY.x = ConvType< T, udouble >( iSrc.ChannelT< T >( 1 ) );
        xyY.y = ConvType< T, udouble >( iSrc.ChannelT< T >( 2 ) );
        cmsxyY2XYZ( &XYZ, &xyY );
        ufloat x = static_cast< ufloat >( XYZ.X );
        ufloat y = static_cast< ufloat >( XYZ.Y );
        ufloat z = static_cast< ufloat >( XYZ.Z );
        float r = linear2srgb( +3.2404542f * x - 1.5371385f * y - 0.4985314f * z );
        float g = linear2srgb( -0.9692660f * x + 1.8760108f * y + 0.0415560f * z );
        float b = linear2srgb( +0.0556434f * x - 0.2040259f * y + 1.0572252f * z );
        iDst.SetChannelT< U >( 0, ConvType< float, U >( r ) );
        iDst.SetChannelT< U >( 1, ConvType< float, U >( g ) );
        iDst.SetChannelT< U >( 2, ConvType< float, U >( b ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

ULIS_NAMESPACE_END

