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
#pragma once
#include "Context/ContextualDispatchTable.h"
#include "Process/Blend/Blend.h"
#include "Process/Clear/Clear.h"
#include "Process/Copy/Copy.h"
#include "Process/Fill/Fill.h"
#include "Process/Text/Text.h"
#include "Process/Transform/Transform.h"

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
#endif // ULIS_FEATURE_TRANSFORM_ENABLED

#if defined( ULIS_FEATURE_CONV_ENABLED ) && defined( ULIS_FEATURE_BLEND_ENABLED )
        , mArgConvForwardBlendNonSeparable(     QueryDispatchedConvertFormatInvocation( iFormat, eFormat::Format_RGBF ) )
        , mArgConvBackwardBlendNonSeparable(    QueryDispatchedConvertFormatInvocation( eFormat::Format_RGBF, iFormat ) )
#endif // defined( ULIS_FEATURE_CONV_ENABLED ) && defined( ULIS_FEATURE_BLEND_ENABLED )
{}

FContext::FContextualDispatchTable::~FContextualDispatchTable()
{}

ULIS_NAMESPACE_END

