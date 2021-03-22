// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ContextualDispatchTable.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for the FContextualDispatchTable class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/ContextualDispatchTable.h"
#include "Process/Blend/Blend.h"
#include "Process/Clear/Clear.h"
#include "Process/Copy/Copy.h"
#include "Process/Fill/Fill.h"
#include "Process/Text/Text.h"
#include "Process/Transform/Transform.h"
#include "Process/IO/Disk.h"
#include "Process/Misc/Extract.h"
#include "Process/Misc/Filter.h"
#include "Process/Misc/GammaCompress.h"
#include "Process/Misc/Premult.h"
#include "Process/Misc/Sanitize.h"
#include "Process/Misc/Swap.h"
#include "Process/SAT/SAT.h"
#include "Process/Convolution/Convolution.h"
#include "Process/Convolution/Morpho.h"
#include "Process/Raster/RasterInvocations.h"
#include "Process/Analysis/AccumulativeSampling.h"
#include "Process/Analysis/AnalyzeSmallestVisibleRect.h"


ULIS_NAMESPACE_BEGIN
FContext::FContextualDispatchTable::FContextualDispatchTable( const FHardwareMetrics& iHardwareMetrics, eFormat iFormat, ePerformanceIntent iPerfIntent )
        : mHardwareMetrics( FHardwareMetrics() )
        , mFormat( iFormat )
        , mPerfIntent( iPerfIntent )
#ifdef ULIS_FEATURE_BLEND_ENABLED
        , mScheduleBlendSeparable(                  TDispatcher< FDispatchedBlendSeparableInvocationSchedulerSelector                   >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBlendNonSeparable(               TDispatcher< FDispatchedBlendNonSeparableInvocationSchedulerSelector                >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBlendMisc(                       TDispatcher< FDispatchedBlendMiscInvocationSchedulerSelector                        >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBlendSeparableSubpixel(          TDispatcher< FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector           >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBlendNonSeparableSubpixel(       TDispatcher< FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector        >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBlendMiscSubpixel(               TDispatcher< FDispatchedBlendMiscSubpixelInvocationSchedulerSelector                >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleAlphaBlend(                      TDispatcher< FDispatchedAlphaBlendSeparableInvocationSchedulerSelector              >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleAlphaBlendSubpixel(              TDispatcher< FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector      >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTiledBlendSeparable(             TDispatcher< FDispatchedTiledBlendSeparableInvocationSchedulerSelector              >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTiledBlendNonSeparable(          TDispatcher< FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector           >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTiledBlendMisc(                  TDispatcher< FDispatchedTiledBlendMiscInvocationSchedulerSelector                   >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_BLEND_ENABLED

#ifdef ULIS_FEATURE_CLEAR_ENABLED
        , mScheduleClear(                           TDispatcher< FDispatchedClearInvocationSchedulerSelector                            >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_CLEAR_ENABLED

#ifdef ULIS_FEATURE_COPY_ENABLED
        , mScheduleCopy(                            TDispatcher< FDispatchedCopyInvocationSchedulerSelector                             >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_COPY_ENABLED

#ifdef ULIS_FEATURE_CONV_ENABLED
        , mScheduleConvertFormat(                   TDispatcher< FDispatchedConvertFormatInvocationSchedulerSelector                    >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_CONV_ENABLED

#ifdef ULIS_FEATURE_FILL_ENABLED
        , mScheduleFill(                            TDispatcher< FDispatchedFillInvocationSchedulerSelector                             >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleFillPreserveAlpha(               TDispatcher< FDispatchedFillPreserveAlphaInvocationSchedulerSelector                >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_FILL_ENABLED

#ifdef ULIS_FEATURE_TEXT_ENABLED
        , mScheduleRasterText(                      TDispatcher< FDispatchedRasterTextInvocationSchedulerSelector                       >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleRasterTextAA(                    TDispatcher< FDispatchedRasterTextAAInvocationSchedulerSelector                     >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_TEXT_ENABLED

#ifdef ULIS_FEATURE_TRANSFORM_ENABLED
        , mScheduleResizeArea(                      TDispatcher< FDispatchedResizeAreaInvocationSchedulerSelector                       >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleResizeBicubic(                   TDispatcher< FDispatchedResizeBicubicInvocationSchedulerSelector                    >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleResizeBilinear(                  TDispatcher< FDispatchedResizeBilinearInvocationSchedulerSelector                   >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleResizeNN(                        TDispatcher< FDispatchedResizeNNInvocationSchedulerSelector                         >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformAffineBicubic(          TDispatcher< FDispatchedTransformAffineBicubicInvocationSchedulerSelector           >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformAffineBilinear(         TDispatcher< FDispatchedTransformAffineBilinearInvocationSchedulerSelector          >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformAffineNN(               TDispatcher< FDispatchedTransformAffineNNInvocationSchedulerSelector                >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformAffineTiledBicubic(     TDispatcher< FDispatchedTransformAffineTiledBicubicInvocationSchedulerSelector      >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformAffineTiledBilinear(    TDispatcher< FDispatchedTransformAffineTiledBilinearInvocationSchedulerSelector     >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformAffineTiledNN(          TDispatcher< FDispatchedTransformAffineTiledNNInvocationSchedulerSelector           >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformBezierBicubic(          TDispatcher< FDispatchedTransformBezierBicubicInvocationSchedulerSelector           >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformBezierBilinear(         TDispatcher< FDispatchedTransformBezierBilinearInvocationSchedulerSelector          >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformBezierNN(               TDispatcher< FDispatchedTransformBezierNNInvocationSchedulerSelector                >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformPerspectiveBicubic(     TDispatcher< FDispatchedTransformPerspectiveBicubicInvocationSchedulerSelector      >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformPerspectiveBilinear(    TDispatcher< FDispatchedTransformPerspectiveBilinearInvocationSchedulerSelector     >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTransformPerspectiveNN(          TDispatcher< FDispatchedTransformPerspectiveNNInvocationSchedulerSelector           >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )

        , mScheduleProcessBezierDeformField(        TDispatcher< FDispatchedProcessBezierDeformFieldSchedulerSelector                   >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_TRANSFORM_ENABLED

#ifdef ULIS_FEATURE_IO_ENABLED
        , mScheduleFileLoad(                        TDispatcher< FDispatchedLoadFromFileInvocationSchedulerSelector                     >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleFileSave(                        TDispatcher< FDispatchedSaveToFileInvocationSchedulerSelector                       >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_IO_ENABLED

        , mScheduleExtract(                         TDispatcher< FDispatchedExtractInvocationSchedulerSelector                          >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleFilter(                          TDispatcher< FDispatchedFilterInvocationSchedulerSelector                           >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleFilterInPlace(                   TDispatcher< FDispatchedFilterInPlaceInvocationSchedulerSelector                    >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleFilterInto(                      TDispatcher< FDispatchedFilterIntoInvocationSchedulerSelector                       >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mSchedulesRGBToLinear(                    TDispatcher< FDispatchedApplysRGB2LinearInvocationSchedulerSelector                 >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleLinearTosRGB(                    TDispatcher< FDispatchedApplyLinear2sRGBInvocationSchedulerSelector                 >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mSchedulePremultiply(                     TDispatcher< FDispatchedPremultInvocationSchedulerSelector                          >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleUnpremultiply(                   TDispatcher< FDispatchedUnpremultInvocationSchedulerSelector                        >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleSanitize(                        TDispatcher< FDispatchedSanitizeZeroAlphaInvocationSchedulerSelector                >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleSwap(                            TDispatcher< FDispatchedSwapInvocationSchedulerSelector                             >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )

        , mScheduleBuildSATXPass(                   TDispatcher< FDispatchedBuildSATXPassInvocationSchedulerSelector                    >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBuildSATYPass(                   TDispatcher< FDispatchedBuildSATYPassInvocationSchedulerSelector                    >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBuildPremultipliedSATXPass(      TDispatcher< FDispatchedBuildPremultSATXPassInvocationSchedulerSelector             >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBuildPremultipliedSATYPass(      TDispatcher< FDispatchedBuildPremultSATYPassInvocationSchedulerSelector             >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )

        , mScheduleAnalyzeSmallestVisibleRectXPass( TDispatcher< FDispatchedAnalyzeSmallestVisibleRectXPassInvocationSchedulerSelector  >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleAnalyzeSmallestVisibleRectYPass( TDispatcher< FDispatchedAnalyzeSmallestVisibleRectYPassInvocationSchedulerSelector  >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleAccumulativeSamplingXPass(       TDispatcher< FDispatchedAccumulativeSamplingXPassInvocationSchedulerSelector        >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleAccumulativeSamplingYPass(       TDispatcher< FDispatchedAccumulativeSamplingYPassInvocationSchedulerSelector        >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )

        , mScheduleConvolve(                        TDispatcher< FDispatchedConvolutionInvocationSchedulerSelector                      >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleMorphologicalProcess(            TDispatcher< FDispatchedMorphoInvocationSchedulerSelector                           >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        
        , mScheduleDrawLine(                        TDispatcher< FDispatchedDrawLineInvocationSchedulerSelector                         >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawLineAA(                      TDispatcher< FDispatchedDrawLineAAInvocationSchedulerSelector                       >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawLineSP(                      TDispatcher< FDispatchedDrawLineSPInvocationSchedulerSelector                       >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawCircleAndres(                TDispatcher< FDispatchedDrawCircleAndresInvocationSchedulerSelector                 >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawCircleAndresAA(              TDispatcher< FDispatchedDrawCircleAndresAAInvocationSchedulerSelector               >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawCircleAndresSP(              TDispatcher< FDispatchedDrawCircleAndresSPInvocationSchedulerSelector               >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawCircleBresenham(             TDispatcher< FDispatchedDrawCircleBresenhamInvocationSchedulerSelector              >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawCircleBresenhamAA(           TDispatcher< FDispatchedDrawCircleBresenhamAAInvocationSchedulerSelector            >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawCircleBresenhamSP(           TDispatcher< FDispatchedDrawCircleBresenhamSPInvocationSchedulerSelector            >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawArcAndres(                   TDispatcher< FDispatchedDrawArcAndresInvocationSchedulerSelector                    >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawArcAndresAA(                 TDispatcher< FDispatchedDrawArcAndresAAInvocationSchedulerSelector                  >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawArcAndresSP(                 TDispatcher< FDispatchedDrawArcAndresSPInvocationSchedulerSelector                  >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawArcBresenham(                TDispatcher< FDispatchedDrawArcBresenhamInvocationSchedulerSelector                 >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawArcBresenhamAA(              TDispatcher< FDispatchedDrawArcBresenhamAAInvocationSchedulerSelector               >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawArcBresenhamSP(              TDispatcher< FDispatchedDrawArcBresenhamSPInvocationSchedulerSelector               >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawEllipse(                     TDispatcher< FDispatchedDrawEllipseInvocationSchedulerSelector                      >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawEllipseAA(                   TDispatcher< FDispatchedDrawEllipseAAInvocationSchedulerSelector                    >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawEllipseSP(                   TDispatcher< FDispatchedDrawEllipseSPInvocationSchedulerSelector                    >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawRotatedEllipse(              TDispatcher< FDispatchedDrawRotatedEllipseInvocationSchedulerSelector               >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawRotatedEllipseAA(            TDispatcher< FDispatchedDrawRotatedEllipseAAInvocationSchedulerSelector             >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawRotatedEllipseSP(            TDispatcher< FDispatchedDrawRotatedEllipseSPInvocationSchedulerSelector             >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawRectangle(                   TDispatcher< FDispatchedDrawRectangleInvocationSchedulerSelector                    >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawPolygon(                     TDispatcher< FDispatchedDrawPolygonInvocationSchedulerSelector                      >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawPolygonAA(                   TDispatcher< FDispatchedDrawPolygonAAInvocationSchedulerSelector                    >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawPolygonSP(                   TDispatcher< FDispatchedDrawPolygonSPInvocationSchedulerSelector                    >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawQuadraticBezier(             TDispatcher< FDispatchedDrawQuadraticBezierInvocationSchedulerSelector              >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawQuadraticBezierAA(           TDispatcher< FDispatchedDrawQuadraticBezierAAInvocationSchedulerSelector            >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleDrawQuadraticBezierSP(           TDispatcher< FDispatchedDrawQuadraticBezierSPInvocationSchedulerSelector            >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )


#if defined( ULIS_FEATURE_CONV_ENABLED ) && defined( ULIS_FEATURE_BLEND_ENABLED )
        , mArgConvForwardBlendNonSeparable(     QueryDispatchedConvertFormatInvocation( iFormat, eFormat::Format_RGBF ) )
        , mArgConvBackwardBlendNonSeparable(    QueryDispatchedConvertFormatInvocation( eFormat::Format_RGBF, iFormat ) )
#endif // defined( ULIS_FEATURE_CONV_ENABLED ) && defined( ULIS_FEATURE_BLEND_ENABLED )
{}

FContext::FContextualDispatchTable::~FContextualDispatchTable()
{}

ULIS_NAMESPACE_END

