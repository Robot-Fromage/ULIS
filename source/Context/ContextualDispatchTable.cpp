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

ULIS_NAMESPACE_BEGIN
FContext::FContextualDispatchTable::FContextualDispatchTable( const FHardwareMetrics& iHardwareMetrics, eFormat iFormat, ePerformanceIntent iPerfIntent )
        : mScheduleBlendSeparable(              TDispatcher< FDispatchedBlendSeparableInvocationSchedulerSelector >             ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBlendNonSeparable(           TDispatcher< FDispatchedBlendNonSeparableInvocationSchedulerSelector >          ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBlendMisc(                   TDispatcher< FDispatchedBlendMiscInvocationSchedulerSelector >                  ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBlendSeparableSubpixel(      TDispatcher< FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector >     ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBlendNonSeparableSubpixel(   TDispatcher< FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector >  ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleBlendMiscSubpixel(           TDispatcher< FDispatchedBlendMiscSubpixelInvocationSchedulerSelector >          ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleAlphaBlend(                  TDispatcher< FDispatchedAlphaBlendSeparableInvocationSchedulerSelector >        ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleAlphaBlendSubpixel(          TDispatcher< FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector >::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTiledBlendSeparable(         TDispatcher< FDispatchedTiledBlendSeparableInvocationSchedulerSelector >        ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTiledBlendNonSeparable(      TDispatcher< FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector >     ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleTiledBlendMisc(              TDispatcher< FDispatchedTiledBlendMiscInvocationSchedulerSelector >             ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleClear(                       TDispatcher< FDispatchedClearInvocationSchedulerSelector >                      ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleCopy(                        TDispatcher< FDispatchedCopyInvocationSchedulerSelector >                       ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleConvertFormat(               TDispatcher< FDispatchedConvertFormatInvocationSchedulerSelector >              ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleFill(                        TDispatcher< FDispatchedFillInvocationSchedulerSelector >                       ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mScheduleFillPreserveAlpha(           TDispatcher< FDispatchedFillPreserveAlphaInvocationSchedulerSelector >          ::Query( iHardwareMetrics, iFormat, iPerfIntent ) )
        , mArgConvForwardBlendNonSeparable(     QueryDispatchedConvertFormatInvocation( iFormat, eFormat::Format_RGBF ) )
        , mArgConvBackwardBlendNonSeparable(    QueryDispatchedConvertFormatInvocation( eFormat::Format_RGBF, iFormat ) )
{}

FContext::FContextualDispatchTable::~FContextualDispatchTable()
{}

ULIS_NAMESPACE_END

