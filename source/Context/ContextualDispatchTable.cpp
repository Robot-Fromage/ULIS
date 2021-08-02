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
#include "Process/Noise/Noise.h"
#include "Process/SAT/SAT.h"
#include "Process/Convolution/Convolution.h"
#include "Process/Convolution/Morpho.h"
#include "Process/Raster/RasterInvocations.h"
#include "Process/Analysis/AccumulativeSampling.h"
#include "Process/Analysis/AnalyzeSmallestVisibleRect.h"
#include "Process/Gradient/Gradient.h"


ULIS_NAMESPACE_BEGIN
FContext::FContextualDispatchTable::FContextualDispatchTable( eFormat iFormat, ePerformanceIntent iPerfIntent )
        : mFormat( iFormat )
        , mPerfIntent( iPerfIntent )
#ifdef ULIS_FEATURE_BLEND_ENABLED
        , mScheduleBlendSeparable(                  TDispatcher< FDispatchedBlendSeparableInvocationSchedulerSelector                   >::Query( iFormat, iPerfIntent ) )
        , mScheduleBlendNonSeparable(               TDispatcher< FDispatchedBlendNonSeparableInvocationSchedulerSelector                >::Query( iFormat, iPerfIntent ) )
        , mScheduleBlendMisc(                       TDispatcher< FDispatchedBlendMiscInvocationSchedulerSelector                        >::Query( iFormat, iPerfIntent ) )
        , mScheduleBlendSeparableSubpixel(          TDispatcher< FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector           >::Query( iFormat, iPerfIntent ) )
        , mScheduleBlendNonSeparableSubpixel(       TDispatcher< FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector        >::Query( iFormat, iPerfIntent ) )
        , mScheduleBlendMiscSubpixel(               TDispatcher< FDispatchedBlendMiscSubpixelInvocationSchedulerSelector                >::Query( iFormat, iPerfIntent ) )
        , mScheduleAlphaBlend(                      TDispatcher< FDispatchedAlphaBlendSeparableInvocationSchedulerSelector              >::Query( iFormat, iPerfIntent ) )
        , mScheduleAlphaBlendSubpixel(              TDispatcher< FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector      >::Query( iFormat, iPerfIntent ) )
        , mScheduleTiledBlendSeparable(             TDispatcher< FDispatchedTiledBlendSeparableInvocationSchedulerSelector              >::Query( iFormat, iPerfIntent ) )
        , mScheduleTiledBlendNonSeparable(          TDispatcher< FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector           >::Query( iFormat, iPerfIntent ) )
        , mScheduleTiledBlendMisc(                  TDispatcher< FDispatchedTiledBlendMiscInvocationSchedulerSelector                   >::Query( iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_BLEND_ENABLED

#ifdef ULIS_FEATURE_CLEAR_ENABLED
        , mScheduleClear(                           TDispatcher< FDispatchedClearInvocationSchedulerSelector                            >::Query( iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_CLEAR_ENABLED

#ifdef ULIS_FEATURE_COPY_ENABLED
        , mScheduleCopy(                            TDispatcher< FDispatchedCopyInvocationSchedulerSelector                             >::Query( iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_COPY_ENABLED

#ifdef ULIS_FEATURE_CONV_ENABLED
        , mScheduleConvertFormat(                   TDispatcher< FDispatchedConvertFormatInvocationSchedulerSelector                    >::Query( iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_CONV_ENABLED

#ifdef ULIS_FEATURE_FILL_ENABLED
        , mScheduleFill(                            TDispatcher< FDispatchedFillInvocationSchedulerSelector                             >::Query( iFormat, iPerfIntent ) )
        , mScheduleFillPreserveAlpha(               TDispatcher< FDispatchedFillPreserveAlphaInvocationSchedulerSelector                >::Query( iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_FILL_ENABLED

#ifdef ULIS_FEATURE_TEXT_ENABLED
        , mScheduleRasterText(                      TDispatcher< FDispatchedRasterTextInvocationSchedulerSelector                       >::Query( iFormat, iPerfIntent ) )
        , mScheduleRasterTextAA(                    TDispatcher< FDispatchedRasterTextAAInvocationSchedulerSelector                     >::Query( iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_TEXT_ENABLED

#ifdef ULIS_FEATURE_TRANSFORM_ENABLED
        , mScheduleResizeArea(                      TDispatcher< FDispatchedResizeAreaInvocationSchedulerSelector                       >::Query( iFormat, iPerfIntent ) )
        , mScheduleResizeBicubic(                   TDispatcher< FDispatchedResizeBicubicInvocationSchedulerSelector                    >::Query( iFormat, iPerfIntent ) )
        , mScheduleResizeBilinear(                  TDispatcher< FDispatchedResizeBilinearInvocationSchedulerSelector                   >::Query( iFormat, iPerfIntent ) )
        , mScheduleResizeNN(                        TDispatcher< FDispatchedResizeNNInvocationSchedulerSelector                         >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformAffineBicubic(          TDispatcher< FDispatchedTransformAffineBicubicInvocationSchedulerSelector           >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformAffineBilinear(         TDispatcher< FDispatchedTransformAffineBilinearInvocationSchedulerSelector          >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformAffineNN(               TDispatcher< FDispatchedTransformAffineNNInvocationSchedulerSelector                >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformAffineTiledBicubic(     TDispatcher< FDispatchedTransformAffineTiledBicubicInvocationSchedulerSelector      >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformAffineTiledBilinear(    TDispatcher< FDispatchedTransformAffineTiledBilinearInvocationSchedulerSelector     >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformAffineTiledNN(          TDispatcher< FDispatchedTransformAffineTiledNNInvocationSchedulerSelector           >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformBezierBicubic(          TDispatcher< FDispatchedTransformBezierBicubicInvocationSchedulerSelector           >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformBezierBilinear(         TDispatcher< FDispatchedTransformBezierBilinearInvocationSchedulerSelector          >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformBezierNN(               TDispatcher< FDispatchedTransformBezierNNInvocationSchedulerSelector                >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformPerspectiveBicubic(     TDispatcher< FDispatchedTransformPerspectiveBicubicInvocationSchedulerSelector      >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformPerspectiveBilinear(    TDispatcher< FDispatchedTransformPerspectiveBilinearInvocationSchedulerSelector     >::Query( iFormat, iPerfIntent ) )
        , mScheduleTransformPerspectiveNN(          TDispatcher< FDispatchedTransformPerspectiveNNInvocationSchedulerSelector           >::Query( iFormat, iPerfIntent ) )

        , mScheduleProcessBezierDeformField(        TDispatcher< FDispatchedProcessBezierDeformFieldSchedulerSelector                   >::Query( iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_TRANSFORM_ENABLED

#ifdef ULIS_FEATURE_IO_ENABLED
        , mScheduleFileLoad(                        TDispatcher< FDispatchedLoadFromFileInvocationSchedulerSelector                     >::Query( iFormat, iPerfIntent ) )
        , mScheduleFileSave(                        TDispatcher< FDispatchedSaveToFileInvocationSchedulerSelector                       >::Query( iFormat, iPerfIntent ) )
#endif // ULIS_FEATURE_IO_ENABLED

        , mScheduleExtract(                         TDispatcher< FDispatchedExtractInvocationSchedulerSelector                          >::Query( iFormat, iPerfIntent ) )
        , mScheduleFilter(                          TDispatcher< FDispatchedFilterInvocationSchedulerSelector                           >::Query( iFormat, iPerfIntent ) )
        , mScheduleFilterInPlace(                   TDispatcher< FDispatchedFilterInPlaceInvocationSchedulerSelector                    >::Query( iFormat, iPerfIntent ) )
        , mScheduleFilterInto(                      TDispatcher< FDispatchedFilterIntoInvocationSchedulerSelector                       >::Query( iFormat, iPerfIntent ) )
        , mSchedulesRGBToLinear(                    TDispatcher< FDispatchedApplysRGB2LinearInvocationSchedulerSelector                 >::Query( iFormat, iPerfIntent ) )
        , mScheduleLinearTosRGB(                    TDispatcher< FDispatchedApplyLinear2sRGBInvocationSchedulerSelector                 >::Query( iFormat, iPerfIntent ) )
        , mSchedulePremultiply(                     TDispatcher< FDispatchedPremultInvocationSchedulerSelector                          >::Query( iFormat, iPerfIntent ) )
        , mScheduleUnpremultiply(                   TDispatcher< FDispatchedUnpremultInvocationSchedulerSelector                        >::Query( iFormat, iPerfIntent ) )
        , mScheduleSanitize(                        TDispatcher< FDispatchedSanitizeZeroAlphaInvocationSchedulerSelector                >::Query( iFormat, iPerfIntent ) )
        , mScheduleSwap(                            TDispatcher< FDispatchedSwapInvocationSchedulerSelector                             >::Query( iFormat, iPerfIntent ) )

        , mScheduleBuildSATXPass(                   TDispatcher< FDispatchedBuildSATXPassInvocationSchedulerSelector                    >::Query( iFormat, iPerfIntent ) )
        , mScheduleBuildSATYPass(                   TDispatcher< FDispatchedBuildSATYPassInvocationSchedulerSelector                    >::Query( iFormat, iPerfIntent ) )
        , mScheduleBuildPremultipliedSATXPass(      TDispatcher< FDispatchedBuildPremultSATXPassInvocationSchedulerSelector             >::Query( iFormat, iPerfIntent ) )
        , mScheduleBuildPremultipliedSATYPass(      TDispatcher< FDispatchedBuildPremultSATYPassInvocationSchedulerSelector             >::Query( iFormat, iPerfIntent ) )

        , mScheduleAnalyzeSmallestVisibleRectXPass( TDispatcher< FDispatchedAnalyzeSmallestVisibleRectXPassInvocationSchedulerSelector  >::Query( iFormat, iPerfIntent ) )
        , mScheduleAnalyzeSmallestVisibleRectYPass( TDispatcher< FDispatchedAnalyzeSmallestVisibleRectYPassInvocationSchedulerSelector  >::Query( iFormat, iPerfIntent ) )
        , mScheduleAccumulativeSamplingXPass(       TDispatcher< FDispatchedAccumulativeSamplingXPassInvocationSchedulerSelector        >::Query( iFormat, iPerfIntent ) )
        , mScheduleAccumulativeSamplingYPass(       TDispatcher< FDispatchedAccumulativeSamplingYPassInvocationSchedulerSelector        >::Query( iFormat, iPerfIntent ) )

        , mScheduleConvolve(                        TDispatcher< FDispatchedConvolutionInvocationSchedulerSelector                      >::Query( iFormat, iPerfIntent ) )
        , mScheduleConvolveMax(                     TDispatcher< FDispatchedConvolutionMaxInvocationSchedulerSelector                   >::Query( iFormat, iPerfIntent ) )
        , mScheduleConvolveMin(                     TDispatcher< FDispatchedConvolutionMinInvocationSchedulerSelector                   >::Query( iFormat, iPerfIntent ) )
        , mScheduleConvolvePremult(                 TDispatcher< FDispatchedConvolutionPremultInvocationSchedulerSelector               >::Query( iFormat, iPerfIntent ) )
        , mScheduleConvolvePremultMax(             TDispatcher< FDispatchedConvolutionPremultMaxInvocationSchedulerSelector             >::Query( iFormat, iPerfIntent ) )
        , mScheduleConvolvePremultMin(             TDispatcher< FDispatchedConvolutionPremultMinInvocationSchedulerSelector             >::Query( iFormat, iPerfIntent ) )
        , mScheduleMorphologicalProcess(            TDispatcher< FDispatchedMorphoInvocationSchedulerSelector                           >::Query( iFormat, iPerfIntent ) )
        
        , mScheduleDrawLine(                        TDispatcher< FDispatchedDrawLineInvocationSchedulerSelector                         >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawLineAA(                      TDispatcher< FDispatchedDrawLineAAInvocationSchedulerSelector                       >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawLineSP(                      TDispatcher< FDispatchedDrawLineSPInvocationSchedulerSelector                       >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawCircle(                      TDispatcher< FDispatchedDrawCircleInvocationSchedulerSelector                       >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawCircleAA(                    TDispatcher< FDispatchedDrawCircleAAInvocationSchedulerSelector                     >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawCircleSP(                    TDispatcher< FDispatchedDrawCircleSPInvocationSchedulerSelector                     >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawArc(                         TDispatcher< FDispatchedDrawArcInvocationSchedulerSelector                          >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawArcAA(                       TDispatcher< FDispatchedDrawArcAAInvocationSchedulerSelector                        >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawArcSP(                       TDispatcher< FDispatchedDrawArcSPInvocationSchedulerSelector                        >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawEllipse(                     TDispatcher< FDispatchedDrawEllipseInvocationSchedulerSelector                      >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawEllipseAA(                   TDispatcher< FDispatchedDrawEllipseAAInvocationSchedulerSelector                    >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawEllipseSP(                   TDispatcher< FDispatchedDrawEllipseSPInvocationSchedulerSelector                    >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawRotatedEllipse(              TDispatcher< FDispatchedDrawRotatedEllipseInvocationSchedulerSelector               >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawRotatedEllipseAA(            TDispatcher< FDispatchedDrawRotatedEllipseAAInvocationSchedulerSelector             >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawRotatedEllipseSP(            TDispatcher< FDispatchedDrawRotatedEllipseSPInvocationSchedulerSelector             >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawRectangle(                   TDispatcher< FDispatchedDrawRectangleInvocationSchedulerSelector                    >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawPolygon(                     TDispatcher< FDispatchedDrawPolygonInvocationSchedulerSelector                      >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawPolygonAA(                   TDispatcher< FDispatchedDrawPolygonAAInvocationSchedulerSelector                    >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawPolygonSP(                   TDispatcher< FDispatchedDrawPolygonSPInvocationSchedulerSelector                    >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawQuadraticBezier(             TDispatcher< FDispatchedDrawQuadraticBezierInvocationSchedulerSelector              >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawQuadraticBezierAA(           TDispatcher< FDispatchedDrawQuadraticBezierAAInvocationSchedulerSelector            >::Query( iFormat, iPerfIntent ) )
        , mScheduleDrawQuadraticBezierSP(           TDispatcher< FDispatchedDrawQuadraticBezierSPInvocationSchedulerSelector            >::Query( iFormat, iPerfIntent ) )

        , mScheduleRasterGradientLinear(            TDispatcher< FDispatchedGradientLinearInvocationSchedulerSelector                   >::Query( iFormat, iPerfIntent ) )
        , mScheduleRasterGradientRadial(            TDispatcher< FDispatchedGradientRadialInvocationSchedulerSelector                   >::Query( iFormat, iPerfIntent ) )
        , mScheduleRasterGradientAngular(           TDispatcher< FDispatchedGradientAngularInvocationSchedulerSelector                  >::Query( iFormat, iPerfIntent ) )
        , mScheduleRasterGradientReflected(         TDispatcher< FDispatchedGradientReflectedInvocationSchedulerSelector                >::Query( iFormat, iPerfIntent ) )
        , mScheduleRasterGradientDiamond(           TDispatcher< FDispatchedGradientDiamondInvocationSchedulerSelector                  >::Query( iFormat, iPerfIntent ) )

        , mScheduleBrownianNoise(                   TDispatcher< FDispatchedBrownianNoiseInvocationSchedulerSelector                    >::Query( iFormat, iPerfIntent ) )
        , mScheduleValueNoise(                      TDispatcher< FDispatchedValueNoiseInvocationSchedulerSelector                       >::Query( iFormat, iPerfIntent ) )
        , mScheduleVoronoiNoise(                    TDispatcher< FDispatchedVoronoiNoiseInvocationSchedulerSelector                     >::Query( iFormat, iPerfIntent ) )
        , mScheduleWhiteNoise(                      TDispatcher< FDispatchedWhiteNoiseInvocationSchedulerSelector                       >::Query( iFormat, iPerfIntent ) )

#if defined( ULIS_FEATURE_CONV_ENABLED ) && defined( ULIS_FEATURE_BLEND_ENABLED )
        , mArgConvForwardBlendNonSeparable(     QueryDispatchedConvertFormatInvocation( iFormat, eFormat::Format_RGBF ) )
        , mArgConvBackwardBlendNonSeparable(    QueryDispatchedConvertFormatInvocation( eFormat::Format_RGBF, iFormat ) )
#endif // defined( ULIS_FEATURE_CONV_ENABLED ) && defined( ULIS_FEATURE_BLEND_ENABLED )
{}

FContext::FContextualDispatchTable::~FContextualDispatchTable()
{}

ULIS_NAMESPACE_END

