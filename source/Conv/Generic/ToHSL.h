// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ToHSL.h
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

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To HSL
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvertFormatGreyToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatGreyToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToHSL< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvertFormatRGBToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = ConvType< T, ufloat >( DREF_SRC( 0 ) );
        ufloat g = ConvType< T, ufloat >( DREF_SRC( 1 ) );
        ufloat b = ConvType< T, ufloat >( DREF_SRC( 2 ) );
        float cmin = FMath::Min3( r, g, b );
        float cmax = FMath::Max3( r, g, b );
        float delta = cmax - cmin;
        float deltaAdd = cmax + cmin;
        float h = 0.f;
        float s = 0.f;
        float l = 0.f;
        l = ( deltaAdd ) / 2.0f;
        if ( delta < FMath::kEpsilonf ){
            h = s = 0.f;
        } else {
            s = ( l < 0.5f ) ? delta / deltaAdd : delta / ( 2.0f - deltaAdd );
            float deltaR = ( ( ( cmax - r ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
            float deltaG = ( ( ( cmax - g ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
            float deltaB = ( ( ( cmax - b ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
                 if( fabs( r - cmax ) < FMath::kEpsilonf )   h = deltaB - deltaG;
            else if( fabs( g - cmax ) < FMath::kEpsilonf )   h = ( 1.0f / 3.0f ) + deltaR - deltaB;
            else if( fabs( b - cmax ) < FMath::kEpsilonf )   h = ( 2.0f / 3.0f ) + deltaG - deltaR;

            if( h < 0.0 ) h += 1.0;
            if( h > 1.0 ) h -= 1.0;
        }

        DREF_DST( 0 ) = ConvType< ufloat, U >( h );
        DREF_DST( 1 ) = ConvType< ufloat, U >( s );
        DREF_DST( 2 ) = ConvType< ufloat, U >( l );
        FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvertFormatHSVToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToHSL< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvertFormatHSLToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
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
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvertFormatCMYToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToHSL< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvertFormatCMYKToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYKToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToHSL< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvertFormatYUVToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYUVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToHSL< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvertFormatLabToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatLabToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToHSL< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvertFormatXYZToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatXYZToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToHSL< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvertFormatYxyToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYxyToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvertFormatRGBToHSL< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

ULIS_NAMESPACE_END

