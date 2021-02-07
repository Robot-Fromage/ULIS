// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to AnalyzeSmallestVisibleRectright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnalyzeSmallestVisibleRect.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the AnalyzeSmallestVisibleRect entry point functions.
* @AnalyzeSmallestVisibleRectright    AnalyzeSmallestVisibleRectright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( AnalyzeSmallestVisibleRect );
ULIS_DECLARE_DISPATCHER( FDispatchedAnalyzeSmallestVisibleRectInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedAnalyzeSmallestVisibleRectInvocationSchedulerSelector
    , &ScheduleAnalyzeSmallestVisibleRectMT_MEM
)

ULIS_NAMESPACE_END

