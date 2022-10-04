// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Noise.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Noise API.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Noise/NoiseArgs.h"

// Include MEM Generic Implementation
#include "Process/Noise/Generic/BrownianNoiseMT_MEM_Generic.h"
#include "Process/Noise/Generic/ValueNoiseMT_MEM_Generic.h"
#include "Process/Noise/Generic/VoronoiNoiseMT_MEM_Generic.h"
#include "Process/Noise/Generic/WhiteNoiseMT_MEM_Generic.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Dispatchers
ULIS_DECLARE_DISPATCHER( FDispatchedBrownianNoiseInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedValueNoiseInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedVoronoiNoiseInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedWhiteNoiseInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBrownianNoiseInvocationSchedulerSelector, &ScheduleBrownianNoiseMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedValueNoiseInvocationSchedulerSelector,    &ScheduleValueNoiseMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedVoronoiNoiseInvocationSchedulerSelector,  &ScheduleVoronoiNoiseMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedWhiteNoiseInvocationSchedulerSelector,    &ScheduleWhiteNoiseMT_MEM_Generic< T > )
ULIS_NAMESPACE_END

