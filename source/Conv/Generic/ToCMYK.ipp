// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
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
#include "Conv/ConvBuffer.h"
#include "Data/FormatInfo.h"
#include "Data/Pixel.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To CMYK
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T1, typename T2 > void ConvBufferGreyToCMYK( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferGreyToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMYK< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T1, typename T2 > void ConvBufferRGBToCMYK(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        ufloat r = ConvType< T1, ufloat >( U2_DREF_SRC( 0 ) );
        ufloat g = ConvType< T1, ufloat >( U2_DREF_SRC( 1 ) );
        ufloat b = ConvType< T1, ufloat >( U2_DREF_SRC( 2 ) );
        float ik = FMaths::Max3( r, g, b );
        float k = 1.f - ik;
        if( ik == 0 ) ik = 1;
        float c = ( ( 1.f - r ) - k ) / ( ik );
        float m = ( ( 1.f - g ) - k ) / ( ik );
        float y = ( ( 1.f - b ) - k ) / ( ik );
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( c );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( m );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( y );
        U2_DREF_DST( 3 ) = ConvType< ufloat, T2 >( k );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T1, typename T2 > void ConvBufferHSVToCMYK(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferHSVToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMYK< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T1, typename T2 > void ConvBufferHSLToCMYK(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferHSLToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMYK< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T1, typename T2 > void ConvBufferCMYToCMYK(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferCMYToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMYK< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T1, typename T2 > void ConvBufferCMYKToCMYK( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        U2_DREF_DST( 0 ) = ConvType< T1, T2 >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 1 ) = ConvType< T1, T2 >( U2_DREF_SRC( 1 ) );
        U2_DREF_DST( 2 ) = ConvType< T1, T2 >( U2_DREF_SRC( 2 ) );
        U2_DREF_DST( 3 ) = ConvType< T1, T2 >( U2_DREF_SRC( 3 ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T1, typename T2 > void ConvBufferYUVToCMYK(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferYUVToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMYK< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T1, typename T2 > void ConvBufferLabToCMYK(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferLabToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMYK< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T1, typename T2 > void ConvBufferXYZToCMYK(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferXYZToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMYK< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T1, typename T2 > void ConvBufferYxyToCMYK(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferYxyToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMYK< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

ULIS3_NAMESPACE_END

