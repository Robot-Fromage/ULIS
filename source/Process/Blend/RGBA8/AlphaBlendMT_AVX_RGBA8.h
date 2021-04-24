// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AlphaBlendMT_AVX_RGBA8.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#ifdef ULIS_COMPILETIME_AVX_SUPPORT
#include "Process/Blend/BlendArgs.h"

ULIS_NAMESPACE_BEGIN
void
InvokeAlphaBlendMT_Separable_AVX_RGBA8_Subpixel(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
);

void
InvokeAlphaBlendMT_Separable_AVX_RGBA8(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
);

ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleAlphaBlendMT_Separable_AVX_RGBA8_Subpixel );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleAlphaBlendMT_Separable_AVX_RGBA8 );

ULIS_NAMESPACE_END
#endif ULIS_COMPILETIME_AVX_SUPPORT

