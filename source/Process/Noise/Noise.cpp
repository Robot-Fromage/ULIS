// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Noise.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Noise API.
* @license      Please refer to LICENSE.md
*/
#include "Process/Noise/Noise.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Gradient Sep
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedBrownianNoiseInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedValueNoiseInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedVoronoiNoiseInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedWhiteNoiseInvocationSchedulerSelector )
ULIS_NAMESPACE_END

