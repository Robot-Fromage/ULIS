// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         CopyDispatch.h
* @author       Clement Berthaud
* @brief        This file provides the dispatch mechanism for Copy invocations.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Copy/Copy.h"
#include "Dispatch/Dispatcher.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Copy
ULIS_DECLARE_DISPATCHER( FDispatchedCopyInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP(
      FDispatchedCopyInvocationSchedulerSelector
    , &ScheduleCopyMT_AX2
    , &ScheduleCopyMT_SSE4_2
    , &ScheduleCopyMT_MEM
)
ULIS_NAMESPACE_END

