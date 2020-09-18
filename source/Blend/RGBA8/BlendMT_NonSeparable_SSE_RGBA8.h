// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         BlendMT_NonSeparable_SSE_RGBA8.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/BlendArgs.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN

void
InvokeBlendMTProcessScanline_NonSeparable_SSE_RGBA8_Subpixel(
      const uint8* iSrc
    , uint8* iBdp
    , int32 iLine
    , const uint32 iSrcBps
    , const FBlendArgs* iArgs
    , const Vec4i iIDT
);

void
ScheduleBlendMT_NonSeparable_SSE_RGBA8_Subpixel(
    const FBlendArgs* iArgs
);

void
InvokeBlendMTProcessScanline_NonSeparable_SSE_RGBA8(
      const uint8* iSrc
    , uint8* iBdp
    , int32 iLine
    , const FBlendArgs* iArgs
    , const Vec4i iIDT
);

void
ScheduleBlendMT_NonSeparable_SSE_RGBA8(
    const FBlendArgs* iArgs
);

ULIS_NAMESPACE_END

