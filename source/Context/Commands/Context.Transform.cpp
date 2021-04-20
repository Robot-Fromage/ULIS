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
          iSource.Format() == iDestination.Format() && iSource.Format() == Format()
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
          iSource.Format() == iDestination.Format() && iSource.Format() == Format()
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
          iSource.Format() == iDestination.Format() && iSource.Format() == Format()
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
          iSource.Format() == iDestination.Format() && iSource.Format() == Format()
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

    FBlock* field = new FBlock();
    FBlock* mask = new FBlock();
    FEvent preprocess;
    XProcessBezierDisplacementField(
          iSource
        , iDestination
        , *field
        , *mask
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
        , &preprocess
    );

    FEvent event(
        FOnEventComplete(
            [field, mask]( const FRectI& ) {
                delete  field;
                delete  mask;
            }
        )
    );
    mCommandQueue.d->Push(
        new FCommand(
                mContextualDispatchTable->QueryScheduleTransformBezier( iResamplingMethod )
            , new FTransformBezierCommandArgs(
                  iSource
                , iDestination
                , src_roi
                , dst_roi
                , *field
                , *mask
                , iResamplingMethod
                , iBorderMode
                , iBorderValue.ToFormat( iDestination.Format() )
            )
            , iPolicy
            , false // Non-contiguous, disable chunks, force scanlines.
            , false // No need to force mono.
            , 1
            , &preprocess
            , &event
            , dst_roi
        )
    );
    Dummy_OP( 1, &event, iEvent );

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
          iSource.Format() == iDestination.Format() && iSource.Format() == Format()
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
FContext::BezierDisplacementFieldMetrics( const FBlock& iSource )
{
    return  eFormat::Format_GAF;
}

//static
eFormat
FContext::BezierDisplacementMaskMetrics( const FBlock& iSource )
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
    TArray< FEvent > events( 3 );
    XAllocateBlockData( iField, dst_roi.w, dst_roi.h, Format_GAF, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ), FSchedulePolicy::MonoChunk, iNumWait, iWaitList, &events[0] );
    XAllocateBlockData( iMask, dst_roi.w, dst_roi.h, Format_G8, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ), FSchedulePolicy::MonoChunk, 0, nullptr, &events[1] );
    // Only load fake geometry to avoid return on hollow block.
    iMask.LoadFromData( nullptr, dst_roi.w, dst_roi.h, Format_G8, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ) );
    Clear( iMask, roi, FSchedulePolicy::AsyncCacheEfficient, 1, &events[1], &events[2] );

    FRectI trans = TransformBezierMetrics( src_roi, iControlPoints );
    FVec2F shift = trans.Position();
    auto cargs = new FProcessBezierDeformFieldArgs(
          iField
        , iMask
        , dst_roi
        , iResamplingMethod
        , iBorderMode
        , iBorderValue.ToFormat( iDestination.Format() )
        , iThreshold
        , iPlotSize
    );
    cargs->points.Reserve( 4 );
    for( int i = 0; i < 4; ++i )
        cargs->points.PushBack( FCubicBezierControlPoint{ iControlPoints[i].point - shift, iControlPoints[i].ctrlCW - shift, iControlPoints[i].ctrlCCW - shift } );

    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleProcessBezierDeformField
            , cargs
            , iPolicy
            , false
            , true // Force mono.
            , 3
            , &events[0]
            , iEvent
            , dst_roi
        )
    );

    return  ULIS_NO_ERROR;
}

//static
FRectI
FContext::MipMapMetrics(
    const FRectI& iSource
)
{
    const FRectI src_rect = iSource.Sanitized();
    return  FRectI( 0, 0, static_cast< int >( iSource.w * 1.5f ), iSource.h );
}

//static
FRectI
FContext::MipLevelMetrics( const FRectI& iSource, uint8 iLevel )
{
    const FRectI src_rect = iSource.Sanitized();
    const FRectI total = MipMapMetrics( src_rect );

    if( iLevel == 0 )
        return  src_rect;

    FVec2I pos = src_rect.Position();
    FVec2I size = src_rect.Size();
    FVec2I botright = pos + size;
    FRectI result = FRectI::FromPositionAndSize( FVec2I( botright.x, 0 ), size / 2 );

    for( int i = 2; i <= iLevel; ++i ) {
        pos = result.Position();
        size = result.Size();
        botright = pos + size;
        result = FRectI::FromPositionAndSize( FVec2I( pos.x, botright.y ), size / 2 );
    }

    return  result;
}

//static
void
FContext::MipRectsMetrics( const FRectI& iSource, uint8 iMaxLevel, TArray< FRectI >* oArray )
{
    ULIS_ASSERT( oArray, "Bad input" );
    oArray->Clear();
    const FRectI rect = iSource.Sanitized();
    oArray->Reserve( iMaxLevel + 1 );
    oArray->PushBack( rect );
    FVec2I pos = rect.Position();
    FVec2I size = rect.Size();
    FVec2I botright = pos + size;
    FRectI result = FRectI::FromPositionAndSize( FVec2I( botright.x, 0 ), size / 2 );
    oArray->PushBack( result );

    for( int i = 2; i <= iMaxLevel; ++i ) {
        pos = result.Position();
        size = result.Size();
        botright = pos + size;
        result = FRectI::FromPositionAndSize( FVec2I( pos.x, botright.y ), size / 2 );
        oArray->PushBack( result );
    }
}

//static
uint8
FContext::MaxMipLevelMetrics( const FRectI& iSource )
{
    FRectI rect = iSource.Sanitized();
    return  static_cast< uint8 >( log2f( static_cast< float >( rect.w ) ) );
}

ulError
FContext::XBuildMipMap(
      const FBlock& iSource
    , FBlock& iDestination
    , int iMaxMipLevel
    , const FRectI& iSourceRect
    , eResamplingMethod iResamplingMethod
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR(
          iSource.Format() == Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );
    // Sanitize geometry
    const FRectI src_rect = iSource.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_roi = MipMapMetrics( src_roi );

    if( iResamplingMethod == Resampling_Area )
        iResamplingMethod = Resampling_Bilinear;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    if( iMaxMipLevel == -1 )
        iMaxMipLevel = MaxMipLevelMetrics( src_roi );

    TArray< FRectI > mipsRects;
    MipRectsMetrics( src_roi, iMaxMipLevel, &mipsRects );

    TArray< FEvent > events( iMaxMipLevel + 2 );
    XAllocateBlockData( iDestination, dst_roi.w, dst_roi.h, iSource.Format(), nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ), FSchedulePolicy::MonoChunk, iNumWait, iWaitList, &events[0] );
    // Only load fake geometry to avoid return on hollow block.
    iDestination.LoadFromData( nullptr, dst_roi.w, dst_roi.h, iSource.Format(), nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ) );
    //Clear( iDestination, FRectI( src_roi.w, mipsRects[1].h, dst_roi.w - src_roi.w, dst_roi.h - mipsRects[1].h ), FSchedulePolicy::AsyncMultiScanlines, 1, &events[0], &events[1] );
    Copy( iSource, iDestination, src_roi, FVec2I( 0 ), FSchedulePolicy::AsyncMultiScanlines, 1, &events[0], &events[1] );

    for( int i = 1; i <= iMaxMipLevel; ++i ) {
        ulError err = Resize(
              i == 1 ? iSource : iDestination
            , iDestination
            , mipsRects[i - 1]
            , mipsRects[i]
            , iResamplingMethod
            , Border_Transparent
            , FColor::Transparent
            , nullptr
            , FSchedulePolicy::AsyncMultiScanlines
            , 1
            , &events[i]
            , &events[i+1]
        );
        ULIS_ASSERT( !( err > 0 ), "Error during mip computation level " << i );
    }

    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

