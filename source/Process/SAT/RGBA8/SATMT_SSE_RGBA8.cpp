// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SATMT_SSE_RGBA8.h
* @author       Clement Berthaud
* @brief        This file provides the definitions for a SAT specialization as described in the title.
* @license      Please refer to LICENSE.md
*/
#include "Process/SAT/RGBA8/SATMT_SSE_RGBA8.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
void
InvokeBuildSATXPassMT_SSE_RGBA8(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
}

void
InvokeBuildSATYPassMT_SSE_RGBA8(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
}

void
InvokeBuildPremultSATXPassMT_SSE_RGBA8(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
}

void
InvokeBuildPremultSATYPassMT_SSE_RGBA8(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
}

ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildSATXPassMT_SSE_RGBA8, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildSATXPassMT_SSE_RGBA8 );
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildSATYPassMT_SSE_RGBA8, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildSATYPassMT_SSE_RGBA8 );
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildPremultSATXPassMT_SSE_RGBA8, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildPremultSATXPassMT_SSE_RGBA8 );
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildPremultSATYPassMT_SSE_RGBA8, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildPremultSATYPassMT_SSE_RGBA8 );

ULIS_NAMESPACE_END

