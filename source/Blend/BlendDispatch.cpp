// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         BlendDispatch.cpp
* @author       Clement Berthaud
* @brief        This file provides the dispatch mechanism for Blend invocations.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Blend/BlendDispatch.h"
#include "Blend/Modes.h"

// Include SSE RGBA8 Implementation
#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
#include "Blend/RGBA8/BlendMT_Separable_SSE_RGBA8.h"
#include "Blend/RGBA8/BlendMT_NonSeparable_SSE_RGBA8.h"
#include "Blend/RGBA8/AlphaBlendMT_SSE_RGBA8.h"
#include "Blend/RGBA8/TiledBlendMT_Separable_SSE_RGBA8.h"
#include "Blend/RGBA8/TiledBlendMT_NonSeparable_SSE_RGBA8.h"
#endif // ULIS_COMPILETIME_SSE42_SUPPORT

// Include AVX RGBA8 Implementation
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
#include "Blend/RGBA8/BlendMT_Separable_AVX_RGBA8.h"
#include "Blend/RGBA8/AlphaBlendMT_AVX_RGBA8.h"
#endif // ULIS_COMPILETIME_AVX2_SUPPORT

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Blend
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendSeparableInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBlendMT_Separable_AVX_RGBA8
        , &ScheduleBlendMT_Separable_SSE_RGBA8
        , &ScheduleBlendMT_Separable_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendSeparableInvocationSchedulerSelector )

ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendNonSeparableInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBlendMT_NonSeparable_SSE_RGBA8
        , &ScheduleBlendMT_NonSeparable_SSE_RGBA8
        , &ScheduleBlendMT_NonSeparable_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendNonSeparableInvocationSchedulerSelector )

ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendMiscInvocationSchedulerSelector )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendMiscInvocationSchedulerSelector )

/////////////////////////////////////////////////////
// Blend Subpixel
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBlendMT_Separable_AVX_RGBA8_Subpixel
        , &ScheduleBlendMT_Separable_SSE_RGBA8_Subpixel
        , &ScheduleBlendMT_Separable_MEM_Generic_Subpixel< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector )

ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBlendMT_NonSeparable_SSE_RGBA8_Subpixel
        , &ScheduleBlendMT_NonSeparable_SSE_RGBA8_Subpixel
        , &ScheduleBlendMT_NonSeparable_MEM_Generic_Subpixel< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector )

ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendMiscSubpixelInvocationSchedulerSelector )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendMiscSubpixelInvocationSchedulerSelector )

ULIS_NAMESPACE_END

