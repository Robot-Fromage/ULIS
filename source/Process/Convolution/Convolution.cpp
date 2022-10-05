// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Convolution.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Convolution API.
* @license      Please refer to LICENSE.md
*/
#include "Process/Convolution/Convolution.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Dispatch
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedConvolutionInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedConvolutionMaxInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedConvolutionMinInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedConvolutionPremultInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedConvolutionPremultMaxInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedConvolutionPremultMinInvocationSchedulerSelector )

ULIS_NAMESPACE_END

