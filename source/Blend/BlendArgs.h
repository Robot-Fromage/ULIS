// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         BlendArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FBlendArgs structure.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Modes.h"
#include "Maths/Geometry.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FBlendArgs
struct FBlendArgs {
    FThreadPool*            pool;
    bool                    blocking;
    const FHostDeviceInfo*  hostDeviceInfo;
    uint32                  perfIntent;
    const FBlock*           source;
    FBlock*                 backdrop;
    FRect                   sourceRect;
    FVec2F                  subpixelComponent;
    FVec2F                  buspixelComponent;
    bool                    subpixelFlag;
    eBlendingMode           blendingMode;
    eAlphaMode              alphaMode;
    ufloat                  opacityValue;
    FVec2I                  shift;
    FVec2I                  backdropCoverage;
    FRect                   backdropWorkingRect;
};

ULIS3_NAMESPACE_END

