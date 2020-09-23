// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         AlphaBlendMT_AVX_RGBA8.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/BlendArgs.h"

ULIS_NAMESPACE_BEGIN
void
InvokeAlphaBlendMTProcessScanline_Separable_AVX_RGBA8_Subpixel(
      const uint8* iSrc
    , uint8* iBdp
    , int32 iLine
    , const uint32 iSrcBps
    , const FBlendCommandArgs* iArgs
);

void
ScheduleAlphaBlendMT_Separable_AVX_RGBA8_Subpixel(
      const FBlendCommandArgs* iArgs
    , const FSchedulePolicy& iPolicy
    , FThreadPool& iPool
);

void
InvokeAlphaBlendMTProcessScanline_Separable_AVX_RGBA8(
      const uint8* iSrc
    , uint8* iBdp
    , int32 iLine
    , const FBlendCommandArgs* iArgs
);

void
ScheduleAlphaBlendMT_Separable_AVX_RGBA8(
      const FBlendCommandArgs* iArgs
    , const FSchedulePolicy& iPolicy
    , FThreadPool& iPool
);

ULIS_NAMESPACE_END

