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
typedef void (*fpConvertFormat)( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
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
template< typename T1, typename T2 > void ConvertFormatGreyToGrey( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToGrey( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToGrey( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToGrey( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToGrey( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToGrey( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToGrey( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToGrey( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToGrey( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToGrey( const FPixel& iSrc, FPixel& iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToRGB
template< typename T1, typename T2 > void ConvertFormatGreyToRGB( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToRGB( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToRGB( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToRGB( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToRGB( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToRGB( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToRGB( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToRGB( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToRGB( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToRGB( const FPixel& iSrc, FPixel& iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToHSV
template< typename T1, typename T2 > void ConvertFormatGreyToHSV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToHSV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToHSV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToHSV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToHSV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToHSV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToHSV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToHSV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToHSV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToHSV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToHSL
template< typename T1, typename T2 > void ConvertFormatGreyToHSL( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToHSL( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToHSL( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToHSL( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToHSL( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToHSL( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToHSL( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToHSL( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToHSL( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToHSL( const FPixel& iSrc, FPixel& iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToCMY
template< typename T1, typename T2 > void ConvertFormatGreyToCMY( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToCMY( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToCMY( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToCMY( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToCMY( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToCMY( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToCMY( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToCMY( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToCMY( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToCMY( const FPixel& iSrc, FPixel& iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- ToCMYK
template< typename T1, typename T2 > void ConvertFormatGreyToCMYK( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToCMYK( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToCMYK( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToCMYK( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToCMYK( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToCMYK( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToCMYK( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToCMYK( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToCMYK( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToCMYK( const FPixel& iSrc, FPixel& iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToYUV
template< typename T1, typename T2 > void ConvertFormatGreyToYUV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToYUV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToYUV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToYUV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToYUV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToYUV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToYUV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToYUV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToYUV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToYUV( const FPixel& iSrc, FPixel& iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToLab
template< typename T1, typename T2 > void ConvertFormatGreyToLab( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToLab( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToLab( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToLab( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToLab( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToLab( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToLab( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToLab( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToLab( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToLab( const FPixel& iSrc, FPixel& iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToXYZ
template< typename T1, typename T2 > void ConvertFormatGreyToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToXYZ( const FPixel& iSrc, FPixel& iDst, uint32 iLen );

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- ToYxy
template< typename T1, typename T2 > void ConvertFormatGreyToYxy( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatRGBToYxy( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSVToYxy( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatHSLToYxy( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYToYxy( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatCMYKToYxy( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYUVToYxy( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatLabToYxy( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatXYZToYxy( const FPixel& iSrc, FPixel& iDst, uint32 iLen );
template< typename T1, typename T2 > void ConvertFormatYxyToYxy( const FPixel& iSrc, FPixel& iDst, uint32 iLen );

ULIS_NAMESPACE_END

