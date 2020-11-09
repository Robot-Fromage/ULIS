// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Clear.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Clear invocations.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Clear/ClearArgs.h"

ULIS_NAMESPACE_BEGIN
void
InvokeClearMTProcessScanline_AX2(
      const FClearJobArgs* jargs
    , const FClearCommandArgs* cargs
);

void
InvokeClearMTProcessScanline_SSE4_2(
      const FClearJobArgs* jargs
    , const FClearCommandArgs* cargs
);

void
InvokeClearMTProcessScanline_MEM(
      const FClearJobArgs* jargs
    , const FClearCommandArgs* cargs
);

void
ScheduleClearMT_AX2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
);

void
ScheduleClearMT_SSE4_2(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
);

void
ScheduleClearMT_MEM(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
);

ULIS_NAMESPACE_END

