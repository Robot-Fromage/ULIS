// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
// IDDN.FR.001.250001.002.S.P.2019.000.00000
/*
*   ULIS
*__________________
* @file         TransformBezier_Bilinear_SSE_RGBA8.h
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
InvokeTransformBezierMT_Bilinear_SSE_RGBA8(
      const FTransformJobArgs* jargs
    , const FTransformBezierCommandArgs* cargs
);

ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleTransformBezierMT_Bilinear_SSE_RGBA8 );

ULIS_NAMESPACE_END
#endif // ULIS_COMPILETIME_SSE_SUPPORT

