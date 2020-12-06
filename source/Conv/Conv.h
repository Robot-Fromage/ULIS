// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Conv.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Conv entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/Conv.h"
#include "Dispatch/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Typedefs
typedef void (*fpConvertFormat)( FPixel iSrc, FPixel iDst, uint32 iLen );
ULIS_API fpConvertFormat QueryDispatchedConvertFormatInvocation( eFormat iSrcFormat, eFormat iDstFormat );

/////////////////////////////////////////////////////
// FConvCommandArgs
class FConvCommandArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FConvCommandArgs() override {}

    FConvCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , const fpConvertFormat iInvocation
    )
        : FDualBufferCommandArgs(
              iSrc
            , iDst
            , iSrcRect
            , iDstRect
            )
        , invocation( iInvocation )
        {}

    fpConvertFormat invocation;
};

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleConvertFormat_MEM_Generic );
ULIS_DECLARE_DISPATCHER( FDispatchedConvertFormatInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedConvertFormatInvocationSchedulerSelector
    , &ScheduleConvertFormat_MEM_Generic
)

/////////////////////////////////////////////////////
// Explicit Conv Entry Points
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ To Grey
template< typename T1, typename T2 > void ConvertFormatGreyToGrey( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToGrey( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToGrey( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToGrey( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToGrey( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToGrey( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToGrey( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToGrey( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToGrey( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToGrey( FPixel iSrc, FPixel iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToRGB
template< typename T1, typename T2 > void ConvertFormatGreyToRGB( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToRGB( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToRGB( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToRGB( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToRGB( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToRGB( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToRGB( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToRGB( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToRGB( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToRGB( FPixel iSrc, FPixel iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToHSV
template< typename T1, typename T2 > void ConvertFormatGreyToHSV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToHSV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToHSV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToHSV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToHSV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToHSV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToHSV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToHSV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToHSV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToHSV( FPixel iSrc, FPixel iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToHSL
template< typename T1, typename T2 > void ConvertFormatGreyToHSL( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToHSL( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToHSL( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToHSL( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToHSL( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToHSL( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToHSL( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToHSL( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToHSL( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToHSL( FPixel iSrc, FPixel iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToCMY
template< typename T1, typename T2 > void ConvertFormatGreyToCMY( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToCMY( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToCMY( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToCMY( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToCMY( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToCMY( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToCMY( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToCMY( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToCMY( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToCMY( FPixel iSrc, FPixel iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- ToCMYK
template< typename T1, typename T2 > void ConvertFormatGreyToCMYK( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToCMYK( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToCMYK( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToCMYK( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToCMYK( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToCMYK( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToCMYK( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToCMYK( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToCMYK( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToCMYK( FPixel iSrc, FPixel iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToYUV
template< typename T1, typename T2 > void ConvertFormatGreyToYUV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToYUV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToYUV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToYUV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToYUV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToYUV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToYUV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToYUV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToYUV( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToYUV( FPixel iSrc, FPixel iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToLab
template< typename T1, typename T2 > void ConvertFormatGreyToLab( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToLab( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToLab( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToLab( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToLab( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToLab( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToLab( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToLab( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToLab( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToLab( FPixel iSrc, FPixel iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToXYZ
template< typename T1, typename T2 > void ConvertFormatGreyToXYZ( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToXYZ( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToXYZ( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToXYZ( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToXYZ( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToXYZ( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToXYZ( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToXYZ( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToXYZ( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToXYZ( FPixel iSrc, FPixel iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToYxy
template< typename T1, typename T2 > void ConvertFormatGreyToYxy( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToYxy( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToYxy( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToYxy( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToYxy( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToYxy( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToYxy( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToYxy( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToYxy( FPixel iSrc, FPixel iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToYxy( FPixel iSrc, FPixel iDst, uint32 iLen );

ULIS_NAMESPACE_END

