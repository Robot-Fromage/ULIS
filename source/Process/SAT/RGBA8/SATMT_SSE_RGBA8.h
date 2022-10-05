// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SATMT_SSE_RGBA8.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for a SAT specialization as described in the title.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/Dispatcher.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
void
InvokeBuildSATXPassMT_SSE_RGBA8(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
);

void
InvokeBuildSATYPassMT_SSE_RGBA8(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
);

void
InvokeBuildPremultSATXPassMT_SSE_RGBA8(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
);

void
InvokeBuildPremultSATYPassMT_SSE_RGBA8(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
);

ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleBuildSATXPassMT_SSE_RGBA8 );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleBuildSATYPassMT_SSE_RGBA8 );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleBuildPremultSATXPassMT_SSE_RGBA8 );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleBuildPremultSATYPassMT_SSE_RGBA8 );

ULIS_NAMESPACE_END

