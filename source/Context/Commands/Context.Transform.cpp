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
}

//static
FRectI
TransformPerspectiveMetrics(
      const FRectI& iSourceRect
    , const FMat3F& iTransform
)
{
}

//static
FRectI
TransformBezierMetrics(
      const FRectI& iSourceRect
    , const TArray< FCubicBezierControlPoint >& iControlPoints
)
{
}

ULIS_NAMESPACE_END

