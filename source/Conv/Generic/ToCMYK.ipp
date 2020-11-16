// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ToCMYK.ipp
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
// To CMYK
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvBufferGreyToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferGreyToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToCMYK< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvBufferRGBToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = ConvType< T, ufloat >( U2_DREF_SRC( 0 ) );
        ufloat g = ConvType< T, ufloat >( U2_DREF_SRC( 1 ) );
        ufloat b = ConvType< T, ufloat >( U2_DREF_SRC( 2 ) );
        float ik = FMath::Max3( r, g, b );
        float k = 1.f - ik;
        if( ik == 0 ) ik = 1;
        float c = ( ( 1.f - r ) - k ) / ( ik );
        float m = ( ( 1.f - g ) - k ) / ( ik );
        float y = ( ( 1.f - b ) - k ) / ( ik );
        U2_DREF_DST( 0 ) = ConvType< ufloat, U >( c );
        U2_DREF_DST( 1 ) = ConvType< ufloat, U >( m );
        U2_DREF_DST( 2 ) = ConvType< ufloat, U >( y );
        U2_DREF_DST( 3 ) = ConvType< ufloat, U >( k );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvBufferHSVToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferHSVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToCMYK< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvBufferHSLToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferHSLToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToCMYK< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvBufferCMYToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferCMYToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToCMYK< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvBufferCMYKToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        U2_DREF_DST( 0 ) = ConvType< T, U >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 1 ) = ConvType< T, U >( U2_DREF_SRC( 1 ) );
        U2_DREF_DST( 2 ) = ConvType< T, U >( U2_DREF_SRC( 2 ) );
        U2_DREF_DST( 3 ) = ConvType< T, U >( U2_DREF_SRC( 3 ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvBufferYUVToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferYUVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToCMYK< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvBufferLabToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferLabToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToCMYK< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvBufferXYZToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferXYZToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToCMYK< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvBufferYxyToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferYxyToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatMetrics(), temp.Bits(), 1 );
        ConvBufferRGBToCMYK< ufloat, U >( temp.FormatMetrics(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

ULIS_NAMESPACE_END

