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
FContext::FContextualDispatchTable::FContextualDispatchTable( const FHardwareMetrics& iHardwareMetrics, eFormat iFormat )
        : mScheduleBlendSeparable(              TDispatcher< FDispatchedBlendSeparableInvocationSchedulerSelector >             ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleBlendNonSeparable(           TDispatcher< FDispatchedBlendNonSeparableInvocationSchedulerSelector >          ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleBlendMisc(                   TDispatcher< FDispatchedBlendMiscInvocationSchedulerSelector >                  ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleBlendSeparableSubpixel(      TDispatcher< FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector >     ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleBlendNonSeparableSubpixel(   TDispatcher< FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector >  ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleBlendMiscSubpixel(           TDispatcher< FDispatchedBlendMiscSubpixelInvocationSchedulerSelector >          ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleAlphaBlend(                  TDispatcher< FDispatchedAlphaBlendSeparableInvocationSchedulerSelector >        ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleAlphaBlendSubpixel(          TDispatcher< FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector >::Query( iHardwareMetrics, iFormat ) )
        , mScheduleTiledBlendSeparable(         TDispatcher< FDispatchedTiledBlendSeparableInvocationSchedulerSelector >        ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleTiledBlendNonSeparable(      TDispatcher< FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector >     ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleTiledBlendMisc(              TDispatcher< FDispatchedTiledBlendMiscInvocationSchedulerSelector >             ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleClear(                       TDispatcher< FDispatchedClearInvocationSchedulerSelector >                      ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleCopy(                        TDispatcher< FDispatchedCopyInvocationSchedulerSelector >                       ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleConvertFormat(               TDispatcher< FDispatchedConvertFormatInvocationSchedulerSelector >              ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleFill(                        TDispatcher< FDispatchedFillInvocationSchedulerSelector >                       ::Query( iHardwareMetrics, iFormat ) )
        , mScheduleFillPreserveAlpha(           TDispatcher< FDispatchedFillPreserveAlphaInvocationSchedulerSelector >          ::Query( iHardwareMetrics, iFormat ) )
        , mArgConvForwardBlendNonSeparable(     QueryDispatchedConvertFormatInvocation( iFormat, eFormat::Format_RGBF ) )
        , mArgConvBackwardBlendNonSeparable(    QueryDispatchedConvertFormatInvocation( eFormat::Format_RGBF, iFormat ) )
{}

FContext::FContextualDispatchTable::~FContextualDispatchTable()
{}

ULIS_NAMESPACE_END

