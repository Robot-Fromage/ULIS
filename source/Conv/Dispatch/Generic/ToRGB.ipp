// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         ToRGB.ipp
* @author       Clement Berthaud
* @brief        This file provides the definition for model conversion functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/ConvBuffer.h"
#include "Data/FormatInfo.h"
#include "Data/Pixel.h"
#include "Color/srgb2linear.h"
#include "lcms2.h"

ULIS2_NAMESPACE_BEGIN
template< typename T1, typename T2 > void ConvBufferGreyToRGB( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        T2 grey = ConvType< T1, T2 >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 0 ) = grey;
        U2_DREF_DST( 1 ) = grey;
        U2_DREF_DST( 2 ) = grey;
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferRGBToRGB(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        U2_DREF_DST( 0 ) = ConvType< T1, T2 >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 1 ) = ConvType< T1, T2 >( U2_DREF_SRC( 1 ) );
        U2_DREF_DST( 2 ) = ConvType< T1, T2 >( U2_DREF_SRC( 2 ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferHSVToRGB(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        T1 _H = U2_DREF_SRC( 0 );
        T1 _S = U2_DREF_SRC( 1 );
        T1 _V = U2_DREF_SRC( 2 );
        ufloat h = ConvType< T1, ufloat >( _H );
        ufloat s = ConvType< T1, ufloat >( _S );
        ufloat v = ConvType< T1, ufloat >( _V );

        float r, g, b;
        r = g = b = 0.f;
        float var_h, var_i, var_1, var_2, var_3;
        var_h = var_i = var_1 = var_2 = var_3 = 0.f;
        if ( s == 0 ) {
            r = g = b = v;
        } else {
            var_h = h * 6;
            if ( var_h == 6 ) var_h = 0 ;
            var_i = var_h;
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
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( r );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( g );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferHSLToRGB(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        T1 _H = U2_DREF_SRC( 0 );
        T1 _S = U2_DREF_SRC( 1 );
        T1 _L = U2_DREF_SRC( 2 );
        ufloat h = ConvType< T1, ufloat >( _H );
        ufloat s = ConvType< T1, ufloat >( _S );
        ufloat l = ConvType< T1, ufloat >( _L );
        float r, g, b;
        r = g = b = 0.f;
        float var_h = h / 60;
        if ( var_h == 6 ) var_h = 0;
        int var_i = int( var_h );
        float C = ( 1.f - fabs( 2.f * l - 1.f ) ) * s;
        float X = float( C * ( 1 - fabs( fmod( var_h, 2 ) - 1 ) ) );
        float m = l - C / 2;
        if      ( var_i == 0 ) { r = C; g = X; b = 0; }
        else if ( var_i == 1 ) { r = X; g = C; b = 0; }
        else if ( var_i == 2 ) { r = 0; g = C; b = X; }
        else if ( var_i == 3 ) { r = 0; g = X; b = C; }
        else if ( var_i == 4 ) { r = X; g = 0; b = C; }
        else                   { r = C; g = 0; b = X; }
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( r + m );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( g + m );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( b + m );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferCMYToRGB(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        T2 max = MaxType< T2 >();
        U2_DREF_DST( 0 ) = max - ConvType< T1, T2 >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 1 ) = max - ConvType< T1, T2 >( U2_DREF_SRC( 1 ) );
        U2_DREF_DST( 2 ) = max - ConvType< T1, T2 >( U2_DREF_SRC( 2 ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferCMYKToRGB( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        ufloat c = ConvType< T1, ufloat >( U2_DREF_SRC( 0 ) );
        ufloat m = ConvType< T1, ufloat >( U2_DREF_SRC( 1 ) );
        ufloat y = ConvType< T1, ufloat >( U2_DREF_SRC( 2 ) );
        ufloat k = ConvType< T1, ufloat >( U2_DREF_SRC( 3 ) );
        float r = 1.f - ( c * ( 1.f - k ) + k );
        float g = 1.f - ( m * ( 1.f - k ) + k );
        float b = 1.f - ( y * ( 1.f - k ) + k );
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( r );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( g );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferYUVToRGB(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        ufloat y = ConvType< T1, ufloat >( U2_DREF_SRC( 0 ) );
        ufloat u = ConvType< T1, ufloat >( U2_DREF_SRC( 1 ) );
        ufloat v = ConvType< T1, ufloat >( U2_DREF_SRC( 2 ) );
        float r = linear2srgb( y + 1.14f * v );
        float g = linear2srgb( y - 0.395f * u - 0.581f * v );
        float b = linear2srgb( y + 2.033f * u );
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( r );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( g );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferLabToRGB(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        cmsCIELab Lab; 
        cmsCIEXYZ XYZ;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        Lab.L = ConvType< T1, udouble >( U2_DREF_SRC( 0 ) ) * 100.0;
        Lab.a = ( ConvType< T1, udouble >( U2_DREF_SRC( 1 ) ) - 0.5 ) * 255.0;
        Lab.b = ( ConvType< T1, udouble >( U2_DREF_SRC( 2 ) ) - 0.5 ) * 255.0;
        cmsLab2XYZ( &D65, &XYZ, &Lab );
        ufloat x = static_cast< ufloat >( XYZ.X ) / 100.f;
        ufloat y = static_cast< ufloat >( XYZ.Y ) / 100.f;
        ufloat z = static_cast< ufloat >( XYZ.Z ) / 100.f;
        float r = linear2srgb( +3.2404542f * x - 1.5371385f * y - 0.4985314f * z );
        float g = linear2srgb( -0.9692660f * x + 1.8760108f * y + 0.0415560f * z );
        float b = linear2srgb( +0.0556434f * x - 0.2040259f * y + 1.0572252f * z );
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( r );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( g );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferXYZToRGB(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        ufloat x = ConvType< T1, ufloat >( U2_DREF_SRC( 0 ) );
        ufloat y = ConvType< T1, ufloat >( U2_DREF_SRC( 1 ) );
        ufloat z = ConvType< T1, ufloat >( U2_DREF_SRC( 2 ) );
        float r = linear2srgb( +3.2404542f * x - 1.5371385f * y - 0.4985314f * z );
        float g = linear2srgb( -0.9692660f * x + 1.8760108f * y + 0.0415560f * z );
        float b = linear2srgb( +0.0556434f * x - 0.2040259f * y + 1.0572252f * z );
        U2_DREF_DST( 0 ) = ConvType< float, T2 >( r );
        U2_DREF_DST( 1 ) = ConvType< float, T2 >( g );
        U2_DREF_DST( 2 ) = ConvType< float, T2 >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferYxyToRGB(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        cmsCIExyY xyY;
        cmsCIEXYZ XYZ;
        xyY.Y = ConvType< T1, udouble >( U2_DREF_SRC( 0 ) );
        xyY.x = ConvType< T1, udouble >( U2_DREF_SRC( 1 ) );
        xyY.y = ConvType< T1, udouble >( U2_DREF_SRC( 2 ) );
        cmsxyY2XYZ( &XYZ, &xyY );
        ufloat x = static_cast< ufloat >( XYZ.X );
        ufloat y = static_cast< ufloat >( XYZ.Y );
        ufloat z = static_cast< ufloat >( XYZ.Z );
        float r = linear2srgb( +3.2404542f * x - 1.5371385f * y - 0.4985314f * z );
        float g = linear2srgb( -0.9692660f * x + 1.8760108f * y + 0.0415560f * z );
        float b = linear2srgb( +0.0556434f * x - 0.2040259f * y + 1.0572252f * z );
        U2_DREF_DST( 0 ) = ConvType< float, T2 >( r );
        U2_DREF_DST( 1 ) = ConvType< float, T2 >( g );
        U2_DREF_DST( 2 ) = ConvType< float, T2 >( b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


ULIS2_NAMESPACE_END
