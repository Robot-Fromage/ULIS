// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Color.h"
#include "Image/Pixel.h"
#include "Image/Sample.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/SchedulePolicy.h"
#include "System/Device.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FContext
/// @brief      The FContext class provides a monolithic context for
///             calling image raster and composition functions for a given format.
/// @details    FContext works on FBlock and is bound to a specific format.
///             It prefetches dispatched implementation to select the fastest
///             method according to the format, its optional  specializations,
///             and the runtime device support for multi-threading or SIMD.
///
///             There can be multiple FContext instances for different formats.
///             Keep in mind this object is rather heavy as it caches paths to
///             the implementations of each feature it exposes.
///
///             FContext can be expected to reach a size in the Ko magnitude,
///             there is a small overhead during instanciation at runtime.
///
///             \sa FBlock
///             \sa FThreadPool
///             \sa FHardwareMetrics
///             \sa FCommandQueue
class ULIS_API FContext
{
private:
    struct FContextualDispatchTable;

public:
    /*!
        Destructor. Clean up the contextual dispatch table.
    */
    ~FContext();

    /*!
        Constructor. Builds the contextual dispatch table. Construction has
        significant overhead, preferably construct a context once at the beginning.
        A queue can be shared beetween multiple contexts with no issue.
    */
    FContext(
          FCommandQueue& iQueue
        , eFormat iFormat
        , ePerformanceIntent iPerfIntent = ePerformanceIntent::PerformanceIntent_Max
    );

    FContext( const FContext& ) = delete;
    FContext& operator=( const FContext& ) = delete;

public:
/////////////////////////////////////////////////////
// Control Flow
    /*!
        Issue all commands and return immediately.
    */
    void Flush();

    /*!
        Issue all commands and wait for completion
    */
    void Finish();

    /*!
        Wait for completion of all already issued commands
    */
    void Fence();

    /*!
        Wait for completion of a given event.

        \warning The event must be associated to a flushed command, otherwise
        the program will block forever in a loop.
    */
    void Wait( const FEvent& iEvent );

    /*!
        Getter for the context format
    */
    eFormat Format() const;

private:
    /*!
        Internal tool for notifying an user event the task is a no-op
    */
    void FinishEventNo_OP( FEvent* iEvent );

public:
/////////////////////////////////////////////////////
// Blend
    /*!
        Perform a blend operation with iSource composited on top of iBackdrop.
        iBackdrop is modified to receive the result of the operation, while
        iSource is left untouched.

        You can specify a sub-portion of the iSource image by specifying the
        iSourceRect to the desired part of the picture. If you want to blend the
        whole image, use the FBlock::Rect() method on the iSource block.
        You can also specify where in iBackdrop the iSource FBlock should be
        composited, in integer coordinates.

        If the iSourceRect and/or iPosition lead to a destination geometry that
        does not intersect the rectangular geometry of iBackdrop, the call will
        not perform any computation and will return safely, so it is safe to
        specify out-of-bounds positions.

        \sa BlendAA()
    */
    void
    Blend(
          const FBlock& iSource
        , FBlock& iBackdrop
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FVec2I& iPosition = FVec2I( 0, 0 )
        , eBlendMode iBlendingMode = Blend_Normal
        , eAlphaMode iAlphaMode = Alpha_Normal
        , ufloat iOpacity = 1.0f
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform an antialiased blend operation with iSource composited on top of
        iBackdrop. iBackdrop is modified to receive the result of the operation,
        while iSource is left untouched.

        An antialiasing blend accepts floating point coordinates and performs
        antialiasing at the same time as it performs the compositing, this is
        useful for drawing, sometimes also referred to as subpixel blending.

        You can specify a sub-portion of the iSource image by specifying the
        iSourceRect to the desired part of the picture. If you want to blend the
        whole image, use the FBlock::Rect() method on the iSource block.
        You can also specify where in iBackdrop the iSource FBlock should be
        composited, in floating point coordinates.

        If the iSourceRect and/or iPosition lead to a destination geometry that
        does not intersect the rectangular geometry of iBackdrop, the call will
        not perform any computation and will return safely, so it is safe to
        specify out-of-bounds positions.

        \sa Blend()
    */
    void
    BlendAA(
          const FBlock& iSource
        , FBlock& iBackdrop
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FVec2F& iPosition = FVec2F( 0.f, 0.f )
        , eBlendMode iBlendingMode = Blend_Normal
        , eAlphaMode iAlphaMode = Alpha_Normal
        , ufloat iOpacity = 1.0f
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform an alpha blend operation with iSource composited on top of
        iBackdrop. iBackdrop is modified to receive the result of the
        operation, while iSource is left untouched.

        You can specify a sub-portion of the iSource image by specifying the
        iSourceRect to the desired part of the picture. If you want to blend the
        whole image, use the FBlock::Rect() method on the iSource block.
        You can also specify where in iBackdrop the iSource FBlock should be
        composited, in integer coordinates.

        If the iSourceRect and/or iPosition lead to a destination geometry that
        does not intersect the rectangular geometry of iBackdrop, the call will
        not perform any computation and will return safely, so it is safe to
        specify out-of-bounds positions.

        \sa AlphaBlendAA()
        \sa Blend()
    */
    void
    AlphaBlend(
          const FBlock& iSource
        , FBlock& iBackdrop
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FVec2I& iPosition = FVec2I( 0, 0 )
        , ufloat iOpacity = 1.0f
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform an antialiased alpha blend operation with iSource composited on
        top of iBackdrop. iBackdrop is modified to receive the result of the
        operation, while iSource is left untouched.

        An antialiasing blend accepts floating point coordinates and performs
        antialiasing at the same time as it performs the compositing, this is
        useful for drawing, sometimes also referred to as subpixel blending.

        You can specify a sub-portion of the iSource image by specifying the
        iSourceRect to the desired part of the picture. If you want to blend the
        whole image, use the FBlock::Rect() method on the iSource block.
        You can also specify where in iBackdrop the iSource FBlock should be
        composited, in floating point coordinates.

        If the iSourceRect and/or iPosition lead to a destination geometry that
        does not intersect the rectangular geometry of iBackdrop, the call will
        not perform any computation and will return safely, so it is safe to
        specify out-of-bounds positions.

        \sa AlphaBlend()
        \sa BlendAA()
    */
    void
    AlphaBlendAA(
          const FBlock& iSource
        , FBlock& iBackdrop
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FVec2F& iPosition = FVec2F( 0.f, 0.f )
        , ufloat iOpacity = 1.0f
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform a tiled blend operation with iSource composited on top of
        iBackdrop. iBackdrop is modified to receive the result of the
        operation, while iSource is left untouched. iSource is tiled over the
        specified surface to cover iBackdrop.

        You can specify a sub-portion of the iSource image by specifying the
        iSourceRect to the desired part of the picture. If you want to blend the
        whole image, use the FBlock::Rect() method on the iSource block.
        You can also specify where in iBackdrop the iSource FBlock should be
        composited, in integer coordinates.

        If the iSourceRect and/or iPosition lead to a destination geometry that
        does not intersect the rectangular geometry of iBackdrop, the call will
        not perform any computation and will return safely, so it is safe to
        specify out-of-bounds positions.

        \sa Blend()
    */
    void
    BlendTiled(
          const FBlock& iSource
        , FBlock& iBackdrop
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FRectI& iBackdropRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FVec2I& iPosition = FVec2I( 0, 0 )
        , eBlendMode iBlendingMode = Blend_Normal
        , eAlphaMode iAlphaMode = Alpha_Normal
        , ufloat iOpacity = 1.0f
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform a blend operation of a single plain color composited on top of
        iBackdrop. iBackdrop is modified to receive the result of the
        operation, while iColor is left untouched.

        If the specified rect and/or position lead to a destination geometry
        that does not intersect the rectangular geometry of iBackdrop, the call
        will not perform any computation and will return safely, so it is safe
        to specify out-of-bounds positions.

        \sa BlendTiled()
    */
    void
    BlendColor(
          const ISample& iColor
        , FBlock& iBackdrop
        , const FRectI& iBackdropRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , eBlendMode iBlendingMode = Blend_Normal
        , eAlphaMode iAlphaMode = Alpha_Normal
        , ufloat iOpacity = 1.0f
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

/////////////////////////////////////////////////////
// Clear
    /*!
        Perform a clear operation on the input block.
        iBlock is modified to be cleared.

        You can specify a sub-portion of the iBlock image by specifying the
        iRect to the desired part of the picture. If you want to clear the
        whole image, use the FBlock::Rect() method on the iBlock.

        If the iRect lead to a destination geometry that does not intersect the
        rectangular geometry of iBlock, the call will not perform any
        computation and will return safely, so it is safe to specify
        out-of-bounds positions.
    */
    void
    Clear(
          FBlock& iBlock
        , const FRectI& iRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

/////////////////////////////////////////////////////
// Conv
    /*!
        Perform a conv operation with iSource converted into iDestination.
        iDestination is modified to receive the result of the operation, while
        iSource is left untouched.

        You can specify a sub-portion of the iSource image by specifying the
        iSourceRect to the desired part of the picture. If you want to convert
        the whole image, use the FBlock::Rect() method on the iSource block.
        You can also specify where in iDestination the iSource FBlock should be
        converted, in integer coordinates.

        If the iSourceRect and/or iPosition lead to a destination geometry that
        does not intersect the rectangular geometry of iDestination, the call will
        not perform any computation and will return safely, so it is safe to
        specify out-of-bounds positions.
    */
    void
    ConvertFormat(
          const FBlock& iSource
        , FBlock& iDestination
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FVec2I& iPosition = FVec2I( 0, 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

/////////////////////////////////////////////////////
// Copy
    /*!
        Perform a copy operation with iSource copied into iDestination.
        iDestination is modified to receive the result of the operation, while
        iSource is left untouched.

        You can specify a sub-portion of the iSource image by specifying the
        iSourceRect to the desired part of the picture. If you want to convert
        the whole image, use the FBlock::Rect() method on the iSource block.
        You can also specify where in iDestination the iSource FBlock should be
        copied, in integer coordinates.

        If the iSourceRect and/or iPosition lead to a destination geometry that
        does not intersect the rectangular geometry of iDestination, the call will
        not perform any computation and will return safely, so it is safe to
        specify out-of-bounds positions.
    */
    void
    Copy(
          const FBlock& iSource
        , FBlock& iDestination
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FVec2I& iPosition = FVec2I( 0, 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

/////////////////////////////////////////////////////
// Fill
    /*!
        Perform a fill operation in iBlock.
        iBlock is modified to receive the result of the operation.

        You can specify a sub-portion of the iBlock image by specifying the
        iRect to the desired part of the picture. If you want to fill
        the whole image, use the FBlock::Rect() method on the iBlock.

        If the iRect lead to a destination geometry that does not intersect the
        rectangular geometry of iBlock, the call will not perform any
        computation and will return safely, so it is safe to specify
        out-of-bounds positions.
    */
    void
    Fill(
          FBlock& iBlock
        , const FRectI& iRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const ISample& iColor = FColor::RGBA8( 0, 0, 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform a fill operation in iBlock while preserving alpha.
        The color will change but the alpha will remain intact.
        iBlock is modified to receive the result of the operation.

        You can specify a sub-portion of the iBlock image by specifying the
        iRect to the desired part of the picture. If you want to fill
        the whole image, use the FBlock::Rect() method on the iBlock.

        If the iRect lead to a destination geometry that does not intersect the
        rectangular geometry of iBlock, the call will not perform any
        computation and will return safely, so it is safe to specify
        out-of-bounds positions.
    */
    void
    FillPreserveAlpha(
          FBlock& iBlock
        , const FRectI& iRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const ISample& iColor = FColor::RGBA8( 0, 0, 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

/////////////////////////////////////////////////////
// Text
    /*!
        Perform a text raster operation in iBlock.
        iBlock is modified to receive the result of the operation.

        If the text leads to a destination geometry that does not intersect the
        rectangular geometry of iBlock, the call will not perform any
        computation and will return safely. It is also safe to specify
        out-of-bounds positions, but the text can be cropped.

        You can use TextMetrics() in order to retrieve the geometry before drawing.

        \warning the FFont input object must remain valid until the async command
        is complete. The other input objects don't have such constraints.

        \warning The same FFont input object cannot be used by multiple threads,
        so you cannot issue two concurrent RasterText commands with the same
        Font at the same time, you have to ensure a Fence or a wait event.

        \sa RasterTextAA()
        \sa TextMetrics()
    */
    void
    RasterText(
          FBlock& iBlock
        , const std::wstring& iText
        , const FFont& iFont
        , uint32 iFontSize = 12
        , const FMat3F& iTransform = FMat3F()
        , const ISample& iColor = FColor::RGBA8( 0, 0, 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform a text raster operation in iBlock with text antialiasing.
        iBlock is modified to receive the result of the operation.

        If the text leads to a destination geometry that does not intersect the
        rectangular geometry of iBlock, the call will not perform any
        computation and will return safely. It is also safe to specify
        out-of-bounds positions, but the text can be cropped.

        You can use TextMetrics() in order to retrieve the geometry before drawing.

        \warning the FFont input object must remain valid until the async command
        is complete. The other input objects don't have such constraints.

        \warning The same FFont input object cannot be used by multiple threads,
        so you cannot issue two concurrent RasterText commands with the same
        Font at the same time, you have to ensure a Fence or a wait event.

        \sa RasterText()
        \sa TextMetrics()
    */
    void
    RasterTextAA(
          FBlock& iBlock
        , const std::wstring& iText
        , const FFont& iFont
        , uint32 iFontSize = 12
        , const FMat3F& iTransform = FMat3F()
        , const ISample& iColor = FColor::RGBA8( 0, 0, 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Collect text metrics before a text raster operation.
        This can help you position your text geometry beforehand.
        Unlike the other methods of FContext, this doesn't work on images and
        returns immediately, it is not scheduled in the Queue.

        \sa RasterText()
        \sa TextMetrics()
    */
    static
    FRectI
    TextMetrics(
          const std::wstring& iText
        , const FFont& iFont
        , uint32 iFontSize = 12
        , const FMat3F& iTransform = FMat3F()
    );

/////////////////////////////////////////////////////
// Transform
    /*!
        Perform an affine transform operation using iSource as input. The result
        is written in the iDestination block.

        An affine transform is a linear transformation such as a translation,
        scale, rotation, skew, or any combination of these.

        You can use TransformAffineMetrics() in order to compute the geometry of
        the transformed output before actually scheduling the operation.

        You can specify a sub-rectangular area in the source input with the
        iSourceRect input parameter.

        \sa TransformAffine()
        \sa TransformPerspective()
        \sa TransformBezier()
        \sa TransformAffineMetrics()
        \sa TransformPerspectiveMetrics()
        \sa TransformBezierMetrics()
    */
    void
    TransformAffine(
          const FBlock& iSource
        , FBlock& iDestination
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FMat3F& iTransformMatrix = FMat3F()
        , eResamplingMethod iResamplingMethod = eResamplingMethod::Resampling_Bilinear
        , eBorderMode iBorderMode = eBorderMode::Border_Transparent
        , const ISample& iBorderValue = FColor::RGBA8( 0, 0, 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform an affine transform operation using iSource as input. The result
        is written in the iDestination block.

        An affine transform is a linear transformation such as a translation,
        scale, rotation, skew, or any combination of these.

        You can use TransformAffineMetrics() in order to compute the geometry of
        the transformed output before actually scheduling the operation.

        You can specify a sub-rectangular area in the source input with the
        iSourceRect input parameter.

        This tiled version allows to cover the area specified by iDestinationRect
        by repeating the transformed source in a pattern inside the destination.

        \sa TransformAffine()
        \sa TransformPerspective()
        \sa TransformBezier()
        \sa TransformAffineMetrics()
        \sa TransformPerspectiveMetrics()
        \sa TransformBezierMetrics()
    */
    void
    TransformAffineTiled(
          const FBlock& iSource
        , FBlock& iDestination
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FRectI& iDestinationRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FMat3F& iTransformMatrix = FMat3F()
        , eResamplingMethod iResamplingMethod = eResamplingMethod::Resampling_Bilinear
        , eBorderMode iBorderMode = eBorderMode::Border_Transparent
        , const ISample& iBorderValue = FColor::RGBA8( 0, 0, 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform an homography or perspective transform operation using iSource
        as input. The result is written in the iDestination block.

        An homography is a distortion transform that doesn't necessarilly preserve
        features such as parallel lines.

        You can use TransformPerspectiveMetrics() in order to compute the geometry of
        the transformed output before actually scheduling the operation.

        You can specify a sub-rectangular area in the source input with the
        iSourceRect input parameter.

        \warning The input iTransformMatrix should be built from the
        FMat3F::MakeHomography() method, an homography matrix differs from an
        affine transform matrix, despite both being stored in a 3x3 matrix.

        \sa TransformAffine()
        \sa TransformPerspective()
        \sa TransformBezier()
        \sa TransformAffineMetrics()
        \sa TransformPerspectiveMetrics()
        \sa TransformBezierMetrics()
    */
    void
    TransformPerspective(
          const FBlock& iSource
        , FBlock& iDestination
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FMat3F& iTransformMatrix = FMat3F()
        , eResamplingMethod iResamplingMethod = eResamplingMethod::Resampling_Bilinear
        , eBorderMode iBorderMode = eBorderMode::Border_Transparent
        , const ISample& iBorderValue = FColor::RGBA8( 0, 0, 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform a bezier transform operation using iSource as input. The result
        is written in the iDestination block.

        A bezier transform is a distortion of an input source covering a 2D bezier
        surface shape. Think about a flag.

        You can use TransformBezierMetrics() in order to compute the geometry of
        the transformed output before actually scheduling the operation.

        You can specify a sub-rectangular area in the source input with the
        iSourceRect input parameter.

        \warning The input iControlPoints array should have size 4 and control
        points define the corners of a bezier path with four control points,
        each one of them using two tangents control points. Two adjacent
        control points define a cubic bezier curve.

        \sa TransformAffine()
        \sa TransformPerspective()
        \sa TransformBezier()
        \sa TransformAffineMetrics()
        \sa TransformPerspectiveMetrics()
        \sa TransformBezierMetrics()
    */
    void
    TransformBezier(
          const FBlock& iSource
        , FBlock& iDestination
        , const TArray< FCubicBezierControlPoint >& iControlPoints
        , float iThreshold = 1.f
        , uint32 iPlotSize = 1
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , eResamplingMethod iResamplingMethod = eResamplingMethod::Resampling_Bilinear
        , eBorderMode iBorderMode = eBorderMode::Border_Transparent
        , const ISample& iBorderValue = FColor::RGBA8( 0, 0, 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform a resize operation using iSource as input. The result
        is written in the iDestination block.

        A resize is simply a scale of an input source.

        You can specify a sub-rectangular area in the source input with the
        iSourceRect input parameter.

        It is advised to use Resize() instead of TransformAffine() when you only
        need a rescale, because the implementation can be made faster by relying
        on this assumption. This is also the only method that can handle the
        more advanced resampling methods, such as eResamplingMethod::Resampling_Area.

        \sa TransformAffine()
        \sa TransformPerspective()
        \sa TransformBezier()
        \sa TransformAffineMetrics()
        \sa TransformPerspectiveMetrics()
        \sa TransformBezierMetrics()
    */
    void
    Resize(
          const FBlock& iSource
        , FBlock& iDestination
        , const FRectI& iSourceRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FRectF& iDestinationRect = FRectF( 0.f, 0.f, float( INT_MAX ), float( INT_MAX ) )
        , eResamplingMethod iResamplingMethod = eResamplingMethod::Resampling_Bilinear
        , eBorderMode iBorderMode = eBorderMode::Border_Transparent
        , const ISample& iBorderValue = FColor::RGBA8( 0, 0, 0 )
        , const FBlock* iOptionalSummedAreaTable = nullptr
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Collect metrics before a TransformAffine() operation.

        \sa TransformAffine()
        \sa TransformPerspective()
        \sa TransformBezier()
        \sa TransformAffineMetrics()
        \sa TransformPerspectiveMetrics()
        \sa TransformBezierMetrics()
    */
    static
    FRectI
    TransformAffineMetrics(
          const FRectI& iSourceRect
        , const FMat3F& iTransform
    );

    /*!
        Collect metrics before a TransformPerspective() operation.

        \sa TransformAffine()
        \sa TransformPerspective()
        \sa TransformBezier()
        \sa TransformAffineMetrics()
        \sa TransformPerspectiveMetrics()
        \sa TransformBezierMetrics()
    */
    static
    FRectI
    TransformPerspectiveMetrics(
          const FRectI& iSourceRect
        , const FMat3F& iTransform
    );

    /*!
        Collect metrics before a TransformBezier() operation.

        \sa TransformAffine()
        \sa TransformPerspective()
        \sa TransformBezier()
        \sa TransformAffineMetrics()
        \sa TransformPerspectiveMetrics()
        \sa TransformBezierMetrics()
    */
    static
    FRectI
    TransformBezierMetrics(
          const FRectI& iSourceRect
        , const TArray< FCubicBezierControlPoint >& iControlPoints
    );

/////////////////////////////////////////////////////
// IO
    /*!
        Perform a load operation into the input ioBlock. The result of the load
        is written in the ioBlock.

        The internal size and formats of ioBlock will be changed to match that
        of the file. The method might reallocate the internal data of the
        ioBlock asynchronously, but the ioBlock reference itself is not
        invalidated. It is only safe to access the ioBlock fields after the
        command has actually completed, you can ensure that with a Fence, a
        Finish, a Wait on the associated FEvent, or with an FEvent callback.

        If the internals are invalidated, the cleanup function of the internal
        data will be called and could destroy the already present memory so make
        sure it is not referenced elsewhere.
    */
    void
    LoadFromFile(
          FBlock& ioBlock
        , const std::string& iPath
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform a save operation of the input iBlock at the specified path.
        iQuality is only used for jpeg files and is beetween 0 and 100
    */
    void
    SaveToFile(
          const FBlock& iBlock
        , const std::string& iPath
        , eFileFormat iFileFormat = eFileFormat::FileFormat_png
        , int iQuality = 100
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform a load operation into the input ioBlock. The result of the load
        is written in the ioBlock.

        The internal size and formats of ioBlock will be changed to match that
        of the clipboard. The method might reallocate the internal data of the
        ioBlock asynchronously, but the ioBlock reference itself is not
        invalidated. It is only safe to access the ioBlock fields after the
        command has actually completed, you can ensure that with a Fence, a
        Finish, a Wait on the associated FEvent, or with an FEvent callback.

        If the internals are invalidated, the cleanup function of the internal
        data will be called and could destroy the already present memory so make
        sure it is not referenced elsewhere.

        \warning This feature is only available on windows at the moment.
    */
    void
    LoadFromClipboard(
          FBlock& ioBlock
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Perform a store operation of the input iBlock into the clipboard.

        \warning This feature is only available on windows at the moment.
    */
    void
    SaveToClipboard(
          const FBlock& iBlock
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );

    /*!
        Collect metrics before a LoadFromClipboard call
    */
    static
    bool
    ClipboardHasImage();

private:
    FCommandQueue& mCommandQueue;
    const FContextualDispatchTable* mContextualDispatchTable;
};

ULIS_NAMESPACE_END

