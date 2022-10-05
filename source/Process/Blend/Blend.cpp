// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Blend.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Blend API.
* @license      Please refer to LICENSE.md
*/
#include "Process/Blend/Blend.h"

// Include SSE RGBA8 Implementation
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
#include "Process/Blend/RGBA8/BlendMT_Separable_SSE_RGBA8.h"
#include "Process/Blend/RGBA8/BlendMT_NonSeparable_SSE_RGBA8.h"
#include "Process/Blend/RGBA8/AlphaBlendMT_SSE_RGBA8.h"
#include "Process/Blend/RGBA8/TiledBlendMT_Separable_SSE_RGBA8.h"
#include "Process/Blend/RGBA8/TiledBlendMT_NonSeparable_SSE_RGBA8.h"
#endif // ULIS_COMPILETIME_SSE_SUPPORT

// Include AVX RGBA8 Implementation
#ifdef ULIS_COMPILETIME_AVX_SUPPORT
#include "Process/Blend/RGBA8/BlendMT_Separable_AVX_RGBA8.h"
#include "Process/Blend/RGBA8/AlphaBlendMT_AVX_RGBA8.h"
#endif // ULIS_COMPILETIME_AVX_SUPPORT

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Blend Sep
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendSeparableInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBlendMT_Separable_AVX_RGBA8
        , &ScheduleBlendMT_Separable_SSE_RGBA8
        , &ScheduleBlendMT_Separable_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendSeparableInvocationSchedulerSelector )
// Blend NSep
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendNonSeparableInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBlendMT_NonSeparable_SSE_RGBA8
        , &ScheduleBlendMT_NonSeparable_SSE_RGBA8
        , &ScheduleBlendMT_NonSeparable_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendNonSeparableInvocationSchedulerSelector )
// Blend Misc
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedBlendMiscInvocationSchedulerSelector )
// Blend Subpixel Sep
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBlendMT_Separable_AVX_RGBA8_Subpixel
        , &ScheduleBlendMT_Separable_SSE_RGBA8_Subpixel
        , &ScheduleBlendMT_Separable_MEM_Generic_Subpixel< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector )
// Blend Subpixel NSep
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBlendMT_NonSeparable_SSE_RGBA8_Subpixel
        , &ScheduleBlendMT_NonSeparable_SSE_RGBA8_Subpixel
        , &ScheduleBlendMT_NonSeparable_MEM_Generic_Subpixel< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector )
// Blend Subpixel Misc
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedBlendMiscSubpixelInvocationSchedulerSelector )
// AlphaBlend
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedAlphaBlendSeparableInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleAlphaBlendMT_Separable_AVX_RGBA8
        , &ScheduleAlphaBlendMT_Separable_SSE_RGBA8
        , &ScheduleAlphaBlendMT_Separable_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedAlphaBlendSeparableInvocationSchedulerSelector )
// AlphaBlend Subpixel
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleAlphaBlendMT_Separable_AVX_RGBA8_Subpixel
        , &ScheduleAlphaBlendMT_Separable_SSE_RGBA8_Subpixel
        , &ScheduleAlphaBlendMT_Separable_MEM_Generic_Subpixel< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector )
// TiledBlend Sep
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTiledBlendSeparableInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTiledBlendMT_Separable_SSE_RGBA8
        , &ScheduleTiledBlendMT_Separable_SSE_RGBA8
        , &ScheduleTiledBlendMT_Separable_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTiledBlendSeparableInvocationSchedulerSelector )
// TiledBlend NSep
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTiledBlendMT_NonSeparable_SSE_RGBA8
        , &ScheduleTiledBlendMT_NonSeparable_SSE_RGBA8
        , &ScheduleTiledBlendMT_NonSeparable_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector )
// TiledBlend Misc
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedTiledBlendMiscInvocationSchedulerSelector )

ULIS_NAMESPACE_END

