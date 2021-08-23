// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         ToGrey.ipp
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

ULIS2_NAMESPACE_BEGIN
template< typename T1, typename T2 > void ConvBufferGreyToGrey( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        U2_DREF_DST( 0 ) = ConvType< T1, T2 >( U2_DREF_SRC( 0 ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferRGBToGrey(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        ufloat r = ConvType< T1, ufloat >( U2_DREF_SRC( 0 ) );
        ufloat g = ConvType< T1, ufloat >( U2_DREF_SRC( 1 ) );
        ufloat b = ConvType< T1, ufloat >( U2_DREF_SRC( 2 ) );
        ufloat grey = 0.3f * r + 0.59f * g + 0.11f * b;
        U2_DREF_DST( 0 ) = ConvType< ufloat, T2 >( grey );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferHSVToGrey(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferHSVToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToGrey< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferHSLToGrey(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferHSLToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToGrey< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferCMYToGrey(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferCMYToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToGrey< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferCMYKToGrey( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferCMYKToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToGrey< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferYUVToGrey(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferYUVToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToGrey< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferLabToGrey(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferLabToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToGrey< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferXYZToGrey(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferXYZToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToGrey< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferYxyToGrey(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferYxyToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToGrey< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}

ULIS2_NAMESPACE_END
