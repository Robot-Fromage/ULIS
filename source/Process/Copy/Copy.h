// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Copy.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Copy entry point functions.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleCopyMT_AVX );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleCopyMT_SSE );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleCopyMT_MEM );
ULIS_DECLARE_DISPATCHER( FDispatchedCopyInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP(
      FDispatchedCopyInvocationSchedulerSelector
    , &ScheduleCopyMT_AVX
    , &ScheduleCopyMT_SSE
    , &ScheduleCopyMT_MEM
)

ULIS_NAMESPACE_END

