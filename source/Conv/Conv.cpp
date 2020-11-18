// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Conv.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Conv entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Conv/Conv.h"
#include "Conv/ConvHelpers.h"
#include "Copy/Copy.h"
#include "Image/Block.h"
#include "Image/Pixel.h"
#include "Math/Math.h"
#include "lcms2.h"

ULIS_NAMESPACE_BEGIN
void Conv( const ISample& iSrc, ISample& iDst ) {
    if( iSrc.Format() == iDst.Format() ) {
        memcpy( iDst.Bits(), iSrc.Bits(), iDst.BytesPerPixel() );
    } else {
        fpConversionInvocation fptr = QueryDispatchedConversionInvocation( iSrc.Format(), iDst.Format() );
        fptr( iSrc.FormatMetrics(), iSrc.Bits(), iDst.FormatMetrics(), iDst.Bits(), 1 );
    }
}

FColor Conv( const ISample& iSrc, eFormat iDst ) {
    FColor dst( iDst );
    if( iSrc.Format() == iDst ) {
        memcpy( dst.Bits(), iSrc.Bits(), dst.BytesPerPixel() );
    } else {
        fpConversionInvocation fptr = QueryDispatchedConversionInvocation( iSrc.Format(), iDst );
        fptr( iSrc.FormatMetrics(), iSrc.Bits(), dst.FormatMetrics(), dst.Bits(), 1 );
    }
    return  dst;
}

void
InvokeConvertFormat(
      const FConvJobArgs* jargs
    , const FConvCommandArgs* cargs
)
{
    cargs->invocation(
          cargs->src.FormatMetrics()
        , jargs->src
        , cargs->dst.FormatMetrics()
        , jargs->dst
        , static_cast< uint32 >( jargs->size )
    );
}

void
ScheduleConvertFormat(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    BuildConvJobs< &InvokeConvertFormat >( iCommand, iPolicy );
}

ULIS_NAMESPACE_END

