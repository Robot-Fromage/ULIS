// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SAT.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the SAT API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"

// Include MEM Generic Implementation
#include "Process/SAT/Generic/SATMT_MEM_Generic.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_DISPATCHER( FDispatchedBuildSATXPassInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedBuildSATYPassInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedBuildPremultSATXPassInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedBuildPremultSATYPassInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBuildSATXPassInvocationSchedulerSelector,          &ScheduleBuildSATXPassMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBuildSATYPassInvocationSchedulerSelector,          &ScheduleBuildSATYPassMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBuildPremultSATXPassInvocationSchedulerSelector,   &ScheduleBuildPremultSATXPassMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBuildPremultSATYPassInvocationSchedulerSelector,   &ScheduleBuildPremultSATYPassMT_MEM_Generic< T > )

ULIS_NAMESPACE_END

