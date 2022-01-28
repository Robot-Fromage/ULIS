// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformPerspectiveMT_Bicubic_SSE_RGBA8.h
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
#include "Process/Transform/TransformArgs.h"

ULIS_NAMESPACE_BEGIN
void
InvokeTransformPerspectiveMT_Bicubic_SSE_RGBA8(
      const FTransformJobArgs* jargs
    , const FTransformCommandArgs* cargs
);

ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleTransformPerspectiveMT_Bicubic_SSE_RGBA8 );

ULIS_NAMESPACE_END
#endif // ULIS_COMPILETIME_SSE_SUPPORT

