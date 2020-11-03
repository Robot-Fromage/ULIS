// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         ToLab.ipp
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
// To Lab
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T1, typename T2 > void ConvBufferGreyToLab(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferGreyToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToLab< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T1, typename T2 > void ConvBufferRGBToLab(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        ufloat r = srgb2linear( ConvType< T1, ufloat >( U2_DREF_SRC( 0 ) ) );
        ufloat g = srgb2linear( ConvType< T1, ufloat >( U2_DREF_SRC( 1 ) ) );
        ufloat b = srgb2linear( ConvType< T1, ufloat >( U2_DREF_SRC( 2 ) ) );
        float x = 0.4124f * r + 0.3576f * g + 0.1805f * b;
        float y = 0.2126f * r + 0.7152f * g + 0.0722f * b;
        float z = 0.0193f * r + 0.1192f * g + 0.9505f * b;
        cmsCIELab Lab;
        cmsCIEXYZ XYZ;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        XYZ.X = x * 100.0;
        XYZ.Y = y * 100.0;
        XYZ.Z = z * 100.0;
        cmsXYZ2Lab( &D65, &Lab, &XYZ );
        ufloat _L = static_cast< ufloat >( Lab.L / 100.f );
        ufloat _a = static_cast< ufloat >( Lab.a / 255.f + 0.5f );
        ufloat _b = static_cast< ufloat >( Lab.b / 255.f + 0.5f );
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( _L );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( _a );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( _b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T1, typename T2 > void ConvBufferHSVToLab(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferHSVToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToLab< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T1, typename T2 > void ConvBufferHSLToLab(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferHSLToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToLab< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T1, typename T2 > void ConvBufferCMYToLab(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferCMYToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToLab< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T1, typename T2 > void ConvBufferCMYKToLab(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferCMYKToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToLab< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T1, typename T2 > void ConvBufferYUVToLab(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS3_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferYUVToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToLab< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T1, typename T2 > void ConvBufferLabToLab(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        U2_DREF_DST( 0 ) = ConvType< T1, T2 >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 1 ) = ConvType< T1, T2 >( U2_DREF_SRC( 1 ) );
        U2_DREF_DST( 2 ) = ConvType< T1, T2 >( U2_DREF_SRC( 2 ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T1, typename T2 > void ConvBufferXYZToLab(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        cmsCIELab Lab;
        cmsCIEXYZ XYZ;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        XYZ.X = ConvType< T1, ufloat >( U2_DREF_SRC( 0 ) ) * 100.0;
        XYZ.Y = ConvType< T1, ufloat >( U2_DREF_SRC( 1 ) ) * 100.0;
        XYZ.Z = ConvType< T1, ufloat >( U2_DREF_SRC( 2 ) ) * 100.0;
        cmsXYZ2Lab( &D65, &Lab, &XYZ );
        ufloat _L = static_cast< ufloat >( Lab.L / 100.f );
        ufloat _a = static_cast< ufloat >( Lab.a / 255.f + 0.5f );
        ufloat _b = static_cast< ufloat >( Lab.b / 255.f + 0.5f );
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( _L );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( _a );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( _b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T1, typename T2 > void ConvBufferYxyToLab(   const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        cmsCIExyY xyY;
        cmsCIEXYZ XYZ;
        xyY.Y = ConvType< T1, ufloat >( U2_DREF_SRC( 0 ) );
        xyY.x = ConvType< T1, ufloat >( U2_DREF_SRC( 1 ) );
        xyY.y = ConvType< T1, ufloat >( U2_DREF_SRC( 2 ) );
        cmsxyY2XYZ( &XYZ, &xyY );

        cmsCIELab Lab;
        cmsCIEXYZ D65 = { 95.047f, 100.00f, 108.883f };
        XYZ.X = XYZ.X * 100.0;
        XYZ.Y = XYZ.Y * 100.0;
        XYZ.Z = XYZ.Z * 100.0;
        cmsXYZ2Lab( &D65, &Lab, &XYZ );
        ufloat _L = static_cast< ufloat >( Lab.L / 100.f );
        ufloat _a = static_cast< ufloat >( Lab.a / 255.f + 0.5f );
        ufloat _b = static_cast< ufloat >( Lab.b / 255.f + 0.5f );
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( _L );
        U2_DREF_DST( 1 ) = ConvType< ufloat, T2 >( _a );
        U2_DREF_DST( 2 ) = ConvType< ufloat, T2 >( _b );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

ULIS3_NAMESPACE_END

