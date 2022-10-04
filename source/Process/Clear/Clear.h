// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Clear.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for the Clear API.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleClearMT_AVX );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleClearMT_SSE );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleClearMT_MEM );
ULIS_DECLARE_DISPATCHER( FDispatchedClearInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP(
      FDispatchedClearInvocationSchedulerSelector
    , &ScheduleClearMT_AVX
    , &ScheduleClearMT_SSE
    , &ScheduleClearMT_MEM
)

ULIS_NAMESPACE_END

