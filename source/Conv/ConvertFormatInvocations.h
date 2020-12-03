// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ConvDispatch.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the ConvBuffer entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Typedefs
typedef void (*fpConversionInvocation)( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
ULIS_API fpConversionInvocation QueryDispatchedConversionInvocation( eFormat iSrcFormat, eFormat iDstFormat );

/////////////////////////////////////////////////////
// Explicit Conv Entry Points
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ To Grey
template< typename T1, typename T2 > void ConvBufferGreyToGrey( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferRGBToGrey( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSVToGrey( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSLToGrey( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYToGrey( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYKToGrey( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYUVToGrey( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferLabToGrey( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferXYZToGrey( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYxyToGrey( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToRGB
template< typename T1, typename T2 > void ConvBufferGreyToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferRGBToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSVToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSLToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYKToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYUVToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferLabToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferXYZToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYxyToRGB( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToHSV
template< typename T1, typename T2 > void ConvBufferGreyToHSV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferRGBToHSV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSVToHSV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSLToHSV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYToHSV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYKToHSV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYUVToHSV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferLabToHSV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferXYZToHSV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYxyToHSV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToHSL
template< typename T1, typename T2 > void ConvBufferGreyToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferRGBToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSVToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSLToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYKToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYUVToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferLabToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferXYZToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYxyToHSL( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToCMY
template< typename T1, typename T2 > void ConvBufferGreyToCMY( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferRGBToCMY( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSVToCMY( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSLToCMY( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYToCMY( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYKToCMY( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYUVToCMY( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferLabToCMY( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferXYZToCMY( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYxyToCMY( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- ToCMYK
template< typename T1, typename T2 > void ConvBufferGreyToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferRGBToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSVToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSLToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYKToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYUVToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferLabToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferXYZToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYxyToCMYK( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToYUV
template< typename T1, typename T2 > void ConvBufferGreyToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferRGBToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSVToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSLToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYKToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYUVToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferLabToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferXYZToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYxyToYUV( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToLab
template< typename T1, typename T2 > void ConvBufferGreyToLab( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferRGBToLab( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSVToLab( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSLToLab( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYToLab( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYKToLab( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYUVToLab( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferLabToLab( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferXYZToLab( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYxyToLab( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToXYZ
template< typename T1, typename T2 > void ConvBufferGreyToXYZ( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferRGBToXYZ( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSVToXYZ( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSLToXYZ( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYToXYZ( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYKToXYZ( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYUVToXYZ( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferLabToXYZ( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferXYZToXYZ( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYxyToXYZ( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToYxy
template< typename T1, typename T2 > void ConvBufferGreyToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferRGBToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSVToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferHSLToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferCMYKToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYUVToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferLabToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferXYZToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvBufferYxyToYxy( const FFormatMetrics& iSrcFormat, const uint8* iSrc, const FFormatMetrics& iDstFormat, uint8* iDst, uint32 iLen );

ULIS_NAMESPACE_END

