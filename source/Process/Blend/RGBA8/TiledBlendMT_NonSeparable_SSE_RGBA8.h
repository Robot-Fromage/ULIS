// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TiledBlendMT_NonSeparable_SSE_RGBA8.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for a Blend specialization as described in the title.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
#include "Process/Blend/BlendArgs.h"

ULIS_NAMESPACE_BEGIN
void
InvokeTiledBlendMT_NonSeparable_SSE_RGBA8(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
);

ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleTiledBlendMT_NonSeparable_SSE_RGBA8 );

ULIS_NAMESPACE_END
#endif // ULIS_COMPILETIME_SSE_SUPPORT

