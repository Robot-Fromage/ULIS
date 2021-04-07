// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SAT.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementations for the SAT API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/SAT/SAT.h"

// Include SSE RGBA8 Implementation
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
//#include "Process/SAT/RGBA8/SATMT_SSE_RGBA8.h"
#endif // ULIS_COMPILETIME_SSE_SUPPORT

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Dispatch / Schedule
// SAT X PASS
/*
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBuildSATXPassInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBuildSATXPassMT_SSE_RGBA8
        , &ScheduleBuildSATXPassMT_SSE_RGBA8
        , &ScheduleBuildSATXPassMT_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBuildSATXPassInvocationSchedulerSelector )
// SAT Y PASS
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBuildSATYPassInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBuildSATYPassMT_SSE_RGBA8
        , &ScheduleBuildSATYPassMT_SSE_RGBA8
        , &ScheduleBuildSATYPassMT_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBuildSATYPassInvocationSchedulerSelector )
// Premultiplied SAT X PASS
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBuildPremultSATXPassInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBuildPremultSATXPassMT_SSE_RGBA8
        , &ScheduleBuildPremultSATXPassMT_SSE_RGBA8
        , &ScheduleBuildPremultSATXPassMT_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBuildPremultSATXPassInvocationSchedulerSelector )
// Premultiplied SAT Y PASS
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBuildPremultSATYPassInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleBuildPremultSATYPassMT_SSE_RGBA8
        , &ScheduleBuildPremultSATYPassMT_SSE_RGBA8
        , &ScheduleBuildPremultSATYPassMT_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedBuildPremultSATYPassInvocationSchedulerSelector )
*/
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedBuildSATXPassInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedBuildSATYPassInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedBuildPremultSATXPassInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedBuildPremultSATYPassInvocationSchedulerSelector )
ULIS_NAMESPACE_END

