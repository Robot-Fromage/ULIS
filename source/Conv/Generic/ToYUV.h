// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ToYUV.h
* @author       Clement Berthaud
* @brief        This file provides the definition for model conversion functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/ConvertFormatInvocations.h"
#include "Conv/ConvHelpers.h"
#include "Image/Color.h"
#include "Image/Format.h"
#include "Image/Pixel.h"
#include "Image/Sample.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To YUV
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvBufferGreyToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferGreyToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYUV< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvBufferRGBToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = srgb2linear( ConvType< T, ufloat >( DREF_SRC( 0 ) ) );
        ufloat g = srgb2linear( ConvType< T, ufloat >( DREF_SRC( 1 ) ) );
        ufloat b = srgb2linear( ConvType< T, ufloat >( DREF_SRC( 2 ) ) );
        float y = 0.299f * r + 0.587f * g + 0.114f * b;
        float u = 0.492f * ( b - y ); // + 0.435912 * ( 1 / ( 0.886 + 0.435912 ) )
        float v = 0.877f * ( r - y ); // + 0.621787 * ( 1 / ( 0.701 + 0.621787 ) )
        DREF_DST( 0 ) = ConvType< ufloat, U >( y );
        DREF_DST( 1 ) = ConvType< ufloat, U >( u );
        DREF_DST( 2 ) = ConvType< ufloat, U >( v );
        FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvBufferHSVToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferHSVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYUV< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvBufferHSLToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferHSLToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYUV< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvBufferCMYToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferCMYToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYUV< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvBufferCMYKToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferCMYKToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYUV< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvBufferYUVToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferYUVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYUV< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvBufferLabToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferLabToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYUV< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvBufferXYZToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferXYZToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYUV< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvBufferYxyToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferYxyToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToYUV< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

ULIS_NAMESPACE_END

