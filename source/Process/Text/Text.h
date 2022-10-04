// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Text.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Text API.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Process/Text/TextArgs.h"
#include "Process/Text/Generic/RasterTextMono_Generic.h"

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleRasterTextMono_MEM_Generic, FSimpleBufferJobArgs, FTextCommandArgs, &InvokeRasterTextMono_MEM_Generic< T > )
ULIS_DECLARE_DISPATCHER( FDispatchedRasterTextInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedRasterTextInvocationSchedulerSelector
    , &ScheduleRasterTextMono_MEM_Generic< T >
)

ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleRasterTextAAMono_MEM_Generic, FSimpleBufferJobArgs, FTextCommandArgs, &InvokeRasterTextAAMono_MEM_Generic< T > )
ULIS_DECLARE_DISPATCHER( FDispatchedRasterTextAAInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedRasterTextAAInvocationSchedulerSelector
    , &ScheduleRasterTextAAMono_MEM_Generic< T >
)

ULIS_NAMESPACE_END

