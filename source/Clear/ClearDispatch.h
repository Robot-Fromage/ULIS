// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ClearDispatch.h
* @author       Clement Berthaud
* @brief        This file provides the dispatch mechanism for Clear invocations.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Clear/Clear.h"
#include "Dispatch/Dispatcher.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Blend
ULIS_DECLARE_DISPATCHER( FDispatchedClearInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP(
      FDispatchedClearInvocationSchedulerSelector
    , &ScheduleClearMT_MEM
    , &ScheduleClearMT_SSE4_2
    , &ScheduleClearMT_AX2
)
ULIS_NAMESPACE_END

