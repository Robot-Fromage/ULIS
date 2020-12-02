// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ToYxy.h
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

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To Yxy
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvBufferGreyToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferGreyToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYxy< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvBufferRGBToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = srgb2linear( ConvType< T, ufloat >( U2_DREF_SRC( 0 ) ) );
        ufloat g = srgb2linear( ConvType< T, ufloat >( U2_DREF_SRC( 1 ) ) );
        ufloat b = srgb2linear( ConvType< T, ufloat >( U2_DREF_SRC( 2 ) ) );
        cmsCIEXYZ XYZ;
        XYZ.X = 0.4124f * r + 0.3576f * g + 0.1805f * b;
        XYZ.Y = 0.2126f * r + 0.7152f * g + 0.0722f * b;
        XYZ.Z = 0.0193f * r + 0.1192f * g + 0.9505f * b;
        cmsCIExyY xyY;
        cmsXYZ2xyY( &xyY, &XYZ );
        U2_DREF_DST( 0 ) = ConvType< ufloat, U >( static_cast< ufloat >( xyY.Y ) );
        U2_DREF_DST( 1 ) = ConvType< ufloat, U >( static_cast< ufloat >( xyY.x ) );
        U2_DREF_DST( 2 ) = ConvType< ufloat, U >( static_cast< ufloat >( xyY.y ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvBufferHSVToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferHSVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYxy< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvBufferHSLToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferHSLToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYxy< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvBufferCMYToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferCMYToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYxy< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvBufferCMYKToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferCMYKToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYxy< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvBufferYUVToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferYUVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYxy< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvBufferLabToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
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
        cmsCIExyY xyY;
        cmsXYZ2xyY( &xyY, &XYZ );
        U2_DREF_DST( 0 ) = ConvType< ufloat, U >( static_cast< ufloat >( xyY.Y ) );
        U2_DREF_DST( 1 ) = ConvType< ufloat, U >( static_cast< ufloat >( xyY.x ) );
        U2_DREF_DST( 2 ) = ConvType< ufloat, U >( static_cast< ufloat >( xyY.y ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvBufferXYZToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        cmsCIEXYZ XYZ;
            cmsCIExyY xyY;
        XYZ.X = ConvType< T, ufloat >( U2_DREF_SRC( 0 ) );
        XYZ.Y = ConvType< T, ufloat >( U2_DREF_SRC( 1 ) );
        XYZ.Z = ConvType< T, ufloat >( U2_DREF_SRC( 2 ) );
        cmsXYZ2xyY( &xyY, &XYZ );
        U2_DREF_DST( 0 ) = ConvType< ufloat, U >( static_cast< ufloat >( xyY.Y ) );
        U2_DREF_DST( 1 ) = ConvType< ufloat, U >( static_cast< ufloat >( xyY.x ) );
        U2_DREF_DST( 2 ) = ConvType< ufloat, U >( static_cast< ufloat >( xyY.y ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvBufferYxyToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
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

ULIS_NAMESPACE_END

