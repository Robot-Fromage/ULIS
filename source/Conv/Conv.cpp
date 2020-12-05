// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Conv.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Conv entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Conv/Conv.h"
#include "Image/Block.h"
#include "Image/Pixel.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
void
InvokeConvertFormat_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FConvCommandArgs* cargs
)
{
    cargs->invocation(
          cargs->src.FormatMetrics()
        , jargs->src
        , cargs->dst.FormatMetrics()
        , jargs->dst
        , static_cast< uint32 >( jargs->size )
    );
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleConvertFormat, FDualBufferJobArgs, FConvCommandArgs, &InvokeConvertFormat_MEM_Generic )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedConvertFormatInvocationSchedulerSelector )

ULIS_NAMESPACE_END

