// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Gradient.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Gradient API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/Gradient/Gradient.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Gradient Sep
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedGradientLinearInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedGradientRadialInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedGradientAngularInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedGradientReflectedInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedGradientDiamondInvocationSchedulerSelector )

ULIS_NAMESPACE_END

