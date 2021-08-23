// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Gradient.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Gradient API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Gradient/GradientArgs.h"

// Include MEM Generic Implementation
#include "Process/Gradient/Generic/GradientLinearMT_MEM_Generic.h"
#include "Process/Gradient/Generic/GradientRadialMT_MEM_Generic.h"
#include "Process/Gradient/Generic/GradientAngularMT_MEM_Generic.h"
#include "Process/Gradient/Generic/GradientReflectedMT_MEM_Generic.h"
#include "Process/Gradient/Generic/GradientDiamondMT_MEM_Generic.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Dispatchers
ULIS_DECLARE_DISPATCHER( FDispatchedGradientLinearInvocationSchedulerSelector  )
ULIS_DECLARE_DISPATCHER( FDispatchedGradientRadialInvocationSchedulerSelector  )
ULIS_DECLARE_DISPATCHER( FDispatchedGradientAngularInvocationSchedulerSelector  )
ULIS_DECLARE_DISPATCHER( FDispatchedGradientReflectedInvocationSchedulerSelector  )
ULIS_DECLARE_DISPATCHER( FDispatchedGradientDiamondInvocationSchedulerSelector  )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedGradientLinearInvocationSchedulerSelector,    &ScheduleGradientLinearMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedGradientRadialInvocationSchedulerSelector,    &ScheduleGradientRadialMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedGradientAngularInvocationSchedulerSelector,   &ScheduleGradientAngularMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedGradientReflectedInvocationSchedulerSelector, &ScheduleGradientReflectedMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedGradientDiamondInvocationSchedulerSelector,   &ScheduleGradientDiamondMT_MEM_Generic< T > )
ULIS_NAMESPACE_END

