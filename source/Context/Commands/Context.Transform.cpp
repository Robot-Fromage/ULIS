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
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Transform/Transform.h"
#include "Image/Block.h"
#include "Math/Interpolation/Bezier.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
ulError
FContext::TransformAffine(
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
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iDestination
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iDestination.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_aim = TransformAffineMetrics( src_roi, iTransformMatrix );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleTransformAffine( iResamplingMethod )
            , new FTransformCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
                , iResamplingMethod
                , iBorderMode
                , iBorderValue.ToFormat( iDestination.Format() )
                , iTransformMatrix.Inverse()
            )
            , iPolicy
            , false // Non-contiguous, disable chunks, force scanlines.
            , false // No need to force mono.
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::TransformAffineTiled(
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
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iDestination
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iDestination.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_roi = iDestinationRect.Sanitized() & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleTransformAffineTiled( iResamplingMethod )
            , new FTransformCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
                , iResamplingMethod
                , iBorderMode
                , iBorderValue.ToFormat( iDestination.Format() )
                , iTransformMatrix.Inverse()
                , true // Tiled
            )
            , iPolicy
            , false // Non-contiguous, disable chunks, force scanlines.
            , false // No need to force mono.
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::TransformPerspective(
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
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iDestination
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iDestination.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_aim = TransformPerspectiveMetrics( src_roi, iTransformMatrix );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->QueryScheduleTransformPerspective( iResamplingMethod )
            , new FTransformCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
                , iResamplingMethod
                , iBorderMode
                , iBorderValue.ToFormat( iDestination.Format() )
                , iTransformMatrix.Inverse()
            )
            , iPolicy
            , false // Non-contiguous, disable chunks, force scanlines.
            , false // No need to force mono.
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::TransformBezier(
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
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iDestination
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iDestination.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iControlPoints.Size() == 4
        , "Bad control points size"
        , FinishEventNo_OP( iEvent, ULIS_ERROR_BAD_INPUT_DATA )
    );

    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    FRectI dst_aim = TransformBezierMetrics( src_roi, iControlPoints );
    int plotSize = FMath::Clamp( iPlotSize, uint32( 1 ), uint32( 8 ) );
    dst_aim.w += plotSize;
    dst_aim.h += plotSize;
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    FBlock field;
    FBlock mask;
    XProcessBezierDisplacementField(
          iSource
        , iDestination
        , field
        , mask
        , iControlPoints
        , iThreshold
        , iPlotSize
        , iSourceRect
        , iResamplingMethod
        , iBorderMode
        , iBorderValue
        , iPolicy
        , iNumWait
        , iWaitList
        , iEvent
    );
    /*
    mCommandQueue.d->Push(
        new FCommand(
                mContextualDispatchTable->QueryScheduleTransformBezier( iResamplingMethod )
            , new FTransformBezierCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
                , iResamplingMethod
                , iBorderMode
                , iBorderValue.ToFormat( iDestination.Format() )
                , inverseScale
                , dst_roi.Position()
                , iOptionalSummedAreaTable // SAT
            )
            , iPolicy
            , false // Non-contiguous, disable chunks, force scanlines.
            , false // No need to force mono.
            , iNumWait
            , iWaitList
            , iEvent
            , dst_roi
        )
    );
    */
    return  ULIS_NO_ERROR;
}

ulError
FContext::Resize(
      const FBlock& iSource
    , FBlock& iDestination
    , const FRectI& iSourceRect
    , const FRectF& iDestinationRect
    , eResamplingMethod iResamplingMethod
    , eBorderMode iBorderMode
    , const ISample& iBorderValue
    , const FBlock* iOptionalSummedAreaTable
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR(
          &iSource != &iDestination
        , "Source and Backdrop are the same block."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA )
    );
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == iDestination.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectF src_rect = iSource.Rect();
    const FRectF dst_rect = iDestination.Rect();
    const FRectF src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectF dst_roi = iDestinationRect.Sanitized() & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0.f )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Forward Arguments Baking
    FVec2F inverseScale = src_roi.Size() / dst_roi.Size();

    // If AREA and SAT is not provided, build it.
    if( iResamplingMethod == Resampling_Area && iOptionalSummedAreaTable == nullptr ) {
        FEvent event_alloc;
        FBlock* sat = new FBlock(); // Hollow
        XAllocateBlockData( *sat, iSource.Width(), iSource.Height(), SummedAreaTableMetrics( iSource ), nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ), iPolicy, iNumWait, iWaitList, &event_alloc );
        FEvent event_sat;
        BuildSummedAreaTable( iSource, *sat, iPolicy, 1, &event_alloc, &event_sat );

        FEvent resize_event(
            FOnEventComplete(
                [sat]( const FRectI& ) {
                    delete  sat;
                }
            )
        );
        // Bake and push command
        mCommandQueue.d->Push(
            new FCommand(
                  mContextualDispatchTable->mScheduleResizeArea
                , new FResizeCommandArgs(
                      iSource
                    , iDestination
                    , src_roi
                    , dst_roi
                    , iResamplingMethod
                    , iBorderMode
                    , iBorderValue.ToFormat( iDestination.Format() )
                    , inverseScale
                    , dst_roi.Position()
                    , sat
                )
                , iPolicy
                , false // Non-contiguous, disable chunks, force scanlines.
                , false // No need to force mono.
                , 1
                , &event_sat
                , &resize_event
                , dst_roi
            )
        );
        Dummy_OP( 1, &resize_event, iEvent );
    } else {
        // Bake and push command
        mCommandQueue.d->Push(
            new FCommand(
                  mContextualDispatchTable->QueryScheduleResize( iResamplingMethod )
                , new FResizeCommandArgs(
                      iSource
                    , iDestination
                    , src_roi
                    , dst_roi
                    , iResamplingMethod
                    , iBorderMode
                    , iBorderValue.ToFormat( iDestination.Format() )
                    , inverseScale
                    , dst_roi.Position()
                    , iOptionalSummedAreaTable // SAT
                )
                , iPolicy
                , false // Non-contiguous, disable chunks, force scanlines.
                , false // No need to force mono.
                , iNumWait
                , iWaitList
                , iEvent
                , dst_roi
            )
        );
    }

    return  ULIS_NO_ERROR;
}

//static
FRectI
FContext::TransformAffineMetrics(
      const FRectI& iSourceRect
    , const FMat3F& iTransform
)
{
    return  iSourceRect.TransformedAffine( iTransform );
}

//static
FRectI
FContext::TransformPerspectiveMetrics(
      const FRectI& iSourceRect
    , const FMat3F& iTransform
)
{
    return  iSourceRect.TransformedPerspective( iTransform );
}

//static
FRectI
FContext::TransformBezierMetrics(
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

//static
eFormat
FContext::BezierDisplacmentFieldMetrics( const FBlock& iSource )
{
    return  eFormat::Format_GAF;
}

//static
eFormat
FContext::BezierDisplacmentMaskMetrics( const FBlock& iSource )
{
    return  eFormat::Format_G8;
}

ulError
FContext::XProcessBezierDisplacementField(
      const FBlock& iSource
    , const FBlock& iDestination
    , FBlock& iField
    , FBlock& iMask
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
    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI dst_rect = iDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    FRectI dst_aim = TransformBezierMetrics( src_roi, iControlPoints );
    int plotSize = FMath::Clamp( iPlotSize, uint32( 1 ), uint32( 8 ) );
    dst_aim.w += plotSize;
    dst_aim.h += plotSize;
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    FRectI roi = FRectI::FromPositionAndSize( FVec2I(), dst_roi.Size() );
    TArray< FEvent > eventAllocation( 2 );
    XAllocateBlockData( iField, dst_roi.w, dst_roi.h, Format_GAF, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ), iPolicy, iNumWait, iWaitList, &eventAllocation[0] );
    XAllocateBlockData( iMask, dst_roi.w, dst_roi.h, Format_G8, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ), iPolicy, 0, nullptr, &eventAllocation[1] );
    FEvent eventClear;
    Clear( iMask, roi, iPolicy, 1, &eventAllocation[1], &eventClear );

    /*
    mCommandQueue.d->Push(
        new FCommand(
                mContextualDispatchTable->QueryScheduleTransformBezier( iResamplingMethod )
            , new FTransformBezierCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
                , iResamplingMethod
                , iBorderMode
                , iBorderValue.ToFormat( iDestination.Format() )
                , inverseScale
                , dst_roi.Position()
                , iOptionalSummedAreaTable // SAT
            )
            , iPolicy
            , false
            , true // Force mono.
            , 1
            , &eventClear
            , iEvent
            , dst_roi
        )
    );
    */

    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

