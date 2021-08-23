// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         ToCMY.ipp
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
template< typename T1, typename T2 > void ConvBufferGreyToCMY( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferGreyToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMY< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferRGBToCMY(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
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


template< typename T1, typename T2 > void ConvBufferHSVToCMY(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferHSVToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMY< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferHSLToCMY(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferHSLToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMY< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferCMYToCMY(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    while( iLen-- ) {
        U2_DREF_DST( 0 ) = ConvType< T1, T2 >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 1 ) = ConvType< T1, T2 >( U2_DREF_SRC( 1 ) );
        U2_DREF_DST( 2 ) = ConvType< T1, T2 >( U2_DREF_SRC( 2 ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferCMYKToCMY( const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferCMYKToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMY< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferYUVToCMY(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferYUVToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMY< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferLabToCMY(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferLabToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMY< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferXYZToCMY(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferXYZToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMY< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


template< typename T1, typename T2 > void ConvBufferYxyToCMY(  const FFormatInfo* iSrcFormat, const tByte* iSrc, const FFormatInfo* iDstFormat, tByte* iDst, tSize iLen ) {
    FPixelValue temp( ULIS2_FORMAT_RGBAF );
    while( iLen-- ) {
        ConvBufferYxyToRGB< T1, ufloat >( iSrcFormat, iSrc, &temp.FormatInfo(), temp.Ptr(), 1 );
        ConvBufferRGBToCMY< ufloat, T2 >( &temp.FormatInfo(), temp.Ptr(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat->BPP;
        iDst += iDstFormat->BPP;
    }
}


ULIS2_NAMESPACE_END
