// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BlendMT_NonSeparable_SSE_RGBA8.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/BlendArgs.h"

ULIS_NAMESPACE_BEGIN

void
InvokeBlendMT_NonSeparable_SSE_RGBA8_Subpixel(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
);

void
InvokeBlendMT_NonSeparable_SSE_RGBA8(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
);

ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleBlendMT_NonSeparable_SSE_RGBA8_Subpixel );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleBlendMT_NonSeparable_SSE_RGBA8 );

ULIS_NAMESPACE_END

