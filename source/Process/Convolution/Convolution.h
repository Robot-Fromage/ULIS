// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Convolution.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Convolution API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Image/Color.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Process/Conv/srgb2linear.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Args

/////////////////////////////////////////////////////
// Invocations
template< typename T >
void
InvokeConvolutionMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleConvolutionMT_MEM_Generic, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeConvolutionMT_MEM_Generic< T > )
ULIS_DECLARE_DISPATCHER( FDispatchedConvolutionInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedConvolutionInvocationSchedulerSelector, &ScheduleConvolutionMT_MEM_Generic< T > )

ULIS_NAMESPACE_END

