// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Copy.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Copy entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Copy/CopyArgs.h"

ULIS_NAMESPACE_BEGIN

void
InvokeCopyMTProcessScanline_AX2(
      const FCopyJobArgs* jargs
    , const FCopyCommandArgs* cargs
);

void
InvokeCopyMTProcessScanline_SSE4_2(
      const FCopyJobArgs* jargs
    , const FCopyCommandArgs* cargs
);

void
InvokeCopyMTProcessScanline_MEM(
      const FCopyJobArgs* jargs
    , const FCopyCommandArgs* cargs
);

void
ScheduleCopyMT_AX2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
);

void
ScheduleCopyMT_SSE4_2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
);

void
ScheduleCopyMT_MEM(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
);

ULIS_NAMESPACE_END

