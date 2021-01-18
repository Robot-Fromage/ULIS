// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Transform.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the Transform API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Transform/Transform.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
void
TransformAffine(
      const FBlock& iSource
    , FBlock& iDestination
    , const FRectI& iSourceRect
    , const FMat3F& iTransformMatrix
    , eResamplingMethod iResamplingMethod
    , eBorderMode iBorderMode
    , const ISample& iBorderValue
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

void
TransformAffineTiled(
      const FBlock& iSource
    , FBlock& iDestination
    , const FRectI& iSourceRect
    , const FRectI& iDestinationRect
    , const FMat3F& iTransformMatrix
    , eResamplingMethod iResamplingMethod
    , eBorderMode iBorderMode
    , const ISample& iBorderValue
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

void
TransformPerspective(
      const FBlock& iSource
    , FBlock& iDestination
    , const FRectI& iSourceRect
    , const FMat3F& iTransformMatrix
    , eResamplingMethod iResamplingMethod
    , eBorderMode iBorderMode
    , const ISample& iBorderValue
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

void
TransformBezier(
      const FBlock& iSource
    , FBlock& iDestination
    , const TArray< FCubicBezierControlPoint >& iControlPoints
    , float iThreshold
    , uint32 iPlotSize
    , const FRectI& iSourceRect
    , eResamplingMethod iResamplingMethod
    , eBorderMode iBorderMode
    , const ISample& iBorderValue
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

void
Resize(
      const FBlock& iSource
    , FBlock& iDestination
    , const FRectI& iSourceRect
    , const FVec2F& iSize
    , const FVec2F& iPos
    , eResamplingMethod iResamplingMethod
    , eBorderMode iBorderMode
    , const ISample& iBorderValue
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

//static
FRectI
TransformAffineMetrics(
      const FRectI& iSourceRect
    , const FMat3F& iTransform
)
{
    return  iSourceRect.TransformedAffine( iTransform );

    // Old imp:
    /*
    FRectI trans = iSourceRect.TransformedAffine( iTransform );
    if( iMethod == INTERP_BILINEAR || iMethod == INTERP_BICUBIC || iMethod == INTERP_AREA ) {
        float tx, ty, r, sx, sy, skx, sky;
        iTransform.Matrix().Decompose( &tx, &ty, &r, &sx, &sy, &skx, &sky );
        float angle = FMath::Max( abs( cos( r ) ), abs( sin( r ) ) );
        float scale = FMath::Max( sx, sy );
        int overflow = static_cast< int >( ceil( angle * scale ) );
        trans.x -= overflow;
        trans.y -= overflow;
        trans.w += overflow * 2;
        trans.h += overflow * 2;
    }
    return  trans;
    */
}

//static
FRectI
TransformPerspectiveMetrics(
      const FRectI& iSourceRect
    , const FMat3F& iTransform
)
{
    return  iSourceRect.TransformedPerspective( iTransform );
    // Old imp:
    /*
    FRectI trans = iSourceRect.TransformedPerspective( iTransform );
    if( iMethod == INTERP_BILINEAR || iMethod == INTERP_BICUBIC || iMethod == INTERP_AREA  ) {
        float tx, ty, r, sx, sy, skx, sky;
        iTransform.Matrix().Decompose( &tx, &ty, &r, &sx, &sy, &skx, &sky );
        trans.x -= static_cast< int >( ceil( sx ) );
        trans.y -= static_cast< int >( ceil( sy ) );
        trans.w += static_cast< int >( ceil( sx ) );
        trans.h += static_cast< int >( ceil( sy ) );
    }
    return  trans;
    */
}

//static
FRectI
TransformBezierMetrics(
      const FRectI& iSourceRect
    , const TArray< FCubicBezierControlPoint >& iControlPoints
)
{
    ULIS_ASSERT( iControlPoints.Size() == 4, "Bad control points size" );
    return  FRectI::FromMinMax(
          static_cast< int >( FMath::VMin( iControlPoints[0].point.x, iControlPoints[0].ctrlCW.x, iControlPoints[0].ctrlCCW.x
                                         , iControlPoints[1].point.x, iControlPoints[1].ctrlCW.x, iControlPoints[1].ctrlCCW.x
                                         , iControlPoints[2].point.x, iControlPoints[2].ctrlCW.x, iControlPoints[2].ctrlCCW.x
                                         , iControlPoints[3].point.x, iControlPoints[3].ctrlCW.x, iControlPoints[3].ctrlCCW.x ) )
        , static_cast< int >( FMath::VMin( iControlPoints[0].point.y, iControlPoints[0].ctrlCW.y, iControlPoints[0].ctrlCCW.y
                                         , iControlPoints[1].point.y, iControlPoints[1].ctrlCW.y, iControlPoints[1].ctrlCCW.y
                                         , iControlPoints[2].point.y, iControlPoints[2].ctrlCW.y, iControlPoints[2].ctrlCCW.y
                                         , iControlPoints[3].point.y, iControlPoints[3].ctrlCW.y, iControlPoints[3].ctrlCCW.y ) )
        , static_cast< int >( FMath::VMax( iControlPoints[0].point.x, iControlPoints[0].ctrlCW.x, iControlPoints[0].ctrlCCW.x
                                         , iControlPoints[1].point.x, iControlPoints[1].ctrlCW.x, iControlPoints[1].ctrlCCW.x
                                         , iControlPoints[2].point.x, iControlPoints[2].ctrlCW.x, iControlPoints[2].ctrlCCW.x
                                         , iControlPoints[3].point.x, iControlPoints[3].ctrlCW.x, iControlPoints[3].ctrlCCW.x ) )
        , static_cast< int >( FMath::VMax( iControlPoints[0].point.y, iControlPoints[0].ctrlCW.y, iControlPoints[0].ctrlCCW.y
                                         , iControlPoints[1].point.y, iControlPoints[1].ctrlCW.y, iControlPoints[1].ctrlCCW.y
                                         , iControlPoints[2].point.y, iControlPoints[2].ctrlCW.y, iControlPoints[2].ctrlCCW.y
                                         , iControlPoints[3].point.y, iControlPoints[3].ctrlCW.y, iControlPoints[3].ctrlCCW.y ) ) );
}

ULIS_NAMESPACE_END

