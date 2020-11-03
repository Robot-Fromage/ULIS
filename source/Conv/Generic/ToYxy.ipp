// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         ToYxy.ipp
* @author       Clement Berthaud
* @brief        This file provides the definition for model conversion functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/ConvBuffer.h"
#include "Conv/ConvHelpers.h"
#include "Data/FormatInfo.h"
#include "Data/Pixel.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To Yxy
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T1, typename T2 > void ConvBufferGreyToYxy(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferGreyToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToYxy< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T1, typename T2 > void ConvBufferRGBToYxy(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        ufloat r = srgb2linear( ConvType< T1, ufloat >( U2_DREF_SRC( 0 ) ) );
        ufloat g = srgb2linear( ConvType< T1, ufloat >( U2_DREF_SRC( 1 ) ) );
        ufloat b = srgb2linear( ConvType< T1, ufloat >( U2_DREF_SRC( 2 ) ) );
        cmsCIEXYZ XYZ;
        XYZ.X = 0.4124f * r + 0.3576f * g + 0.1805f * b;
        XYZ.Y = 0.2126f * r + 0.7152f * g + 0.0722f * b;
        XYZ.Z = 0.0193f * r + 0.1192f * g + 0.9505f * b;
        cmsCIExyY xyY;
        cmsXYZ2xyY( &xyY, &XYZ );
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( static_cast< ufloat >( xyY.Y ) );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( static_cast< ufloat >( xyY.x ) );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( static_cast< ufloat >( xyY.y ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T1, typename T2 > void ConvBufferHSVToYxy(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferHSVToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToYxy< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T1, typename T2 > void ConvBufferHSLToYxy(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferHSLToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToYxy< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T1, typename T2 > void ConvBufferCMYToYxy(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferCMYToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToYxy< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T1, typename T2 > void ConvBufferCMYKToYxy(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferCMYKToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToYxy< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T1, typename T2 > void ConvBufferYUVToYxy(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferYUVToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToYxy< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T1, typename T2 > void ConvBufferLabToYxy(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        cmsCIELab Lab;
        cmsCIEXYZ XYZ;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        Lab.L = ConvType< T1, udouble >( U2_DREF_SRC( 0 ) ) * 100.0;
        Lab.a = ( ConvType< T1, udouble >( U2_DREF_SRC( 1 ) ) - 0.5 ) * 255.0;
        Lab.b = ( ConvType< T1, udouble >( U2_DREF_SRC( 2 ) ) - 0.5 ) * 255.0;
        cmsLab2XYZ( &D65, &XYZ, &Lab );
        cmsCIExyY xyY;
        cmsXYZ2xyY( &xyY, &XYZ );
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( static_cast< ufloat >( xyY.Y ) );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( static_cast< ufloat >( xyY.x ) );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( static_cast< ufloat >( xyY.y ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T1, typename T2 > void ConvBufferXYZToYxy(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        cmsCIEXYZ XYZ;
            cmsCIExyY xyY;
        XYZ.X = ConvType< T1, ufloat >( U2_DREF_SRC( 0 ) );
        XYZ.Y = ConvType< T1, ufloat >( U2_DREF_SRC( 1 ) );
        XYZ.Z = ConvType< T1, ufloat >( U2_DREF_SRC( 2 ) );
        cmsXYZ2xyY( &xyY, &XYZ );
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( static_cast< ufloat >( xyY.Y ) );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( static_cast< ufloat >( xyY.x ) );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( static_cast< ufloat >( xyY.y ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T1, typename T2 > void ConvBufferYxyToYxy(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        U2_DREF_DST( 0 ) = ConvType< T1, T2 >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 1 ) = ConvType< T1, T2 >( U2_DREF_SRC( 1 ) );
        U2_DREF_DST( 2 ) = ConvType< T1, T2 >( U2_DREF_SRC( 2 ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

ULIS3_NAMESPACE_END

