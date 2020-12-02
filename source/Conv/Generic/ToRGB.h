// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ToRGB.h
* @author       Clement Berthaud
* @brief        This file provides the definition for model conversion functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/ConvDispatch.h"
#include "Conv/ConvHelpers.h"
#include "Image/Color.h"
#include "Image/Format.h"
#include "Image/Pixel.h"
#include "Image/Sample.h"
#include "Conv/srgb2linear.h"
#include <lcms2.h>
#include <cmath>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To RGB
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvBufferGreyToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        U grey = ConvType< T, U >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 0 ) = grey;
        U2_DREF_DST( 1 ) = grey;
        U2_DREF_DST( 2 ) = grey;
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvBufferRGBToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        U2_DREF_DST( 0 ) = ConvType< T, U >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 1 ) = ConvType< T, U >( U2_DREF_SRC( 1 ) );
        U2_DREF_DST( 2 ) = ConvType< T, U >( U2_DREF_SRC( 2 ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvBufferHSVToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        T _H = U2_DREF_SRC( 0 );
        T _S = U2_DREF_SRC( 1 );
        T _V = U2_DREF_SRC( 2 );
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
        U2_DREF_DST( 0 ) = ConvType< ufloat, U >( r );
        U2_DREF_DST( 1 ) = ConvType< ufloat, U >( g );
        U2_DREF_DST( 2 ) = ConvType< ufloat, U >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvBufferHSLToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        T _H = U2_DREF_SRC( 0 );
        T _S = U2_DREF_SRC( 1 );
        T _L = U2_DREF_SRC( 2 );
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
        U2_DREF_DST( 0 ) = ConvType< ufloat, U >( r + m );
        U2_DREF_DST( 1 ) = ConvType< ufloat, U >( g + m );
        U2_DREF_DST( 2 ) = ConvType< ufloat, U >( b + m );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvBufferCMYToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        U max = MaxType< U >();
        U2_DREF_DST( 0 ) = max - ConvType< T, U >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 1 ) = max - ConvType< T, U >( U2_DREF_SRC( 1 ) );
        U2_DREF_DST( 2 ) = max - ConvType< T, U >( U2_DREF_SRC( 2 ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvBufferCMYKToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat c = ConvType< T, ufloat >( U2_DREF_SRC( 0 ) );
        ufloat m = ConvType< T, ufloat >( U2_DREF_SRC( 1 ) );
        ufloat y = ConvType< T, ufloat >( U2_DREF_SRC( 2 ) );
        ufloat k = ConvType< T, ufloat >( U2_DREF_SRC( 3 ) );
        float r = 1.f - ( c * ( 1.f - k ) + k );
        float g = 1.f - ( m * ( 1.f - k ) + k );
        float b = 1.f - ( y * ( 1.f - k ) + k );
        U2_DREF_DST( 0 ) = ConvType< ufloat, U >( r );
        U2_DREF_DST( 1 ) = ConvType< ufloat, U >( g );
        U2_DREF_DST( 2 ) = ConvType< ufloat, U >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvBufferYUVToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat y = ConvType< T, ufloat >( U2_DREF_SRC( 0 ) );
        ufloat u = ConvType< T, ufloat >( U2_DREF_SRC( 1 ) );
        ufloat v = ConvType< T, ufloat >( U2_DREF_SRC( 2 ) );
        float r = linear2srgb( y + 1.14f * v );
        float g = linear2srgb( y - 0.395f * u - 0.581f * v );
        float b = linear2srgb( y + 2.033f * u );
        U2_DREF_DST( 0 ) = ConvType< ufloat, U >( r );
        U2_DREF_DST( 1 ) = ConvType< ufloat, U >( g );
        U2_DREF_DST( 2 ) = ConvType< ufloat, U >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvBufferLabToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIELab Lab;
        cmsCIEXYZ XYZ;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        Lab.L = ConvType< T, udouble >( U2_DREF_SRC( 0 ) ) * 100.0;
        Lab.a = ( ConvType< T, udouble >( U2_DREF_SRC( 1 ) ) - 0.5 ) * 255.0;
        Lab.b = ( ConvType< T, udouble >( U2_DREF_SRC( 2 ) ) - 0.5 ) * 255.0;
        cmsLab2XYZ( &D65, &XYZ, &Lab );
        ufloat x = static_cast< ufloat >( XYZ.X ) / 100.f;
        ufloat y = static_cast< ufloat >( XYZ.Y ) / 100.f;
        ufloat z = static_cast< ufloat >( XYZ.Z ) / 100.f;
        float r = linear2srgb( +3.2404542f * x - 1.5371385f * y - 0.4985314f * z );
        float g = linear2srgb( -0.9692660f * x + 1.8760108f * y + 0.0415560f * z );
        float b = linear2srgb( +0.0556434f * x - 0.2040259f * y + 1.0572252f * z );
        U2_DREF_DST( 0 ) = ConvType< ufloat, U >( r );
        U2_DREF_DST( 1 ) = ConvType< ufloat, U >( g );
        U2_DREF_DST( 2 ) = ConvType< ufloat, U >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvBufferXYZToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat x = ConvType< T, ufloat >( U2_DREF_SRC( 0 ) );
        ufloat y = ConvType< T, ufloat >( U2_DREF_SRC( 1 ) );
        ufloat z = ConvType< T, ufloat >( U2_DREF_SRC( 2 ) );
        float r = linear2srgb( +3.2404542f * x - 1.5371385f * y - 0.4985314f * z );
        float g = linear2srgb( -0.9692660f * x + 1.8760108f * y + 0.0415560f * z );
        float b = linear2srgb( +0.0556434f * x - 0.2040259f * y + 1.0572252f * z );
        U2_DREF_DST( 0 ) = ConvType< float, U >( r );
        U2_DREF_DST( 1 ) = ConvType< float, U >( g );
        U2_DREF_DST( 2 ) = ConvType< float, U >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvBufferYxyToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIExyY xyY;
        cmsCIEXYZ XYZ;
        xyY.Y = ConvType< T, udouble >( U2_DREF_SRC( 0 ) );
        xyY.x = ConvType< T, udouble >( U2_DREF_SRC( 1 ) );
        xyY.y = ConvType< T, udouble >( U2_DREF_SRC( 2 ) );
        cmsxyY2XYZ( &XYZ, &xyY );
        ufloat x = static_cast< ufloat >( XYZ.X );
        ufloat y = static_cast< ufloat >( XYZ.Y );
        ufloat z = static_cast< ufloat >( XYZ.Z );
        float r = linear2srgb( +3.2404542f * x - 1.5371385f * y - 0.4985314f * z );
        float g = linear2srgb( -0.9692660f * x + 1.8760108f * y + 0.0415560f * z );
        float b = linear2srgb( +0.0556434f * x - 0.2040259f * y + 1.0572252f * z );
        U2_DREF_DST( 0 ) = ConvType< float, U >( r );
        U2_DREF_DST( 1 ) = ConvType< float, U >( g );
        U2_DREF_DST( 2 ) = ConvType< float, U >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

ULIS_NAMESPACE_END

