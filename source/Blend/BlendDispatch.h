// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         BlendDispatch.h
* @author       Clement Berthaud
* @brief        This file provides the dispatch mechanism for Blend invocations.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/BlendArgs.h"
#include "Dispatch/Dispatcher.h"

// Include MEM Generic Implementation
#include "Blend/Generic/BlendMT_Separable_MEM_Generic.h"
#include "Blend/Generic/BlendMT_NonSeparable_MEM_Generic.h"
#include "Blend/Generic/BlendMT_Misc_MEM_Generic.h"
#include "Blend/Generic/AlphaBlendMT_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_Separable_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_NonSeparable_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_Misc_MEM_Generic.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Typedef scheduler
typedef void (*fpBlendScheduler)( const FBlendArgs* iArgs );

/////////////////////////////////////////////////////
// Blend
ULIS_DECLARE_DISPATCHER( FDispatchedBlendSeparableInvocationSchedulerSelector,      FBlendArgs )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendNonSeparableInvocationSchedulerSelector,   FBlendArgs )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendMiscInvocationSchedulerSelector,           FBlendArgs )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendSeparableInvocationSchedulerSelector,    &ScheduleBlendMT_Separable_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendNonSeparableInvocationSchedulerSelector, &ScheduleBlendMT_NonSeparable_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendMiscInvocationSchedulerSelector,         &ScheduleBlendMT_Misc_MEM_Generic< T > )

/////////////////////////////////////////////////////
// Blend Subpixel
ULIS_DECLARE_DISPATCHER( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector,      FBlendArgs )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector,   FBlendArgs )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendMiscSubpixelInvocationSchedulerSelector,           FBlendArgs )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector,    &ScheduleBlendMT_Separable_MEM_Generic_Subpixel< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector, &ScheduleBlendMT_NonSeparable_MEM_Generic_Subpixel< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendMiscSubpixelInvocationSchedulerSelector,         &ScheduleBlendMT_Misc_MEM_Generic_Subpixel< T > )


ULIS_NAMESPACE_END

