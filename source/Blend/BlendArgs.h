// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
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
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FBlendArgs
struct FBlendArgs {
    FThreadPool*            pool;
    bool                    blocking;
    const FHostDeviceInfo*  hostDeviceInfo;
    uint32                  perfIntent;
    const FRasterImage2D*           source;
    FRasterImage2D*                 backdrop;
    FRectI                   sourceRect;
    FVec2F                  subpixelComponent;
    FVec2F                  buspixelComponent;
    bool                    subpixelFlag;
    eBlendingMode           blendingMode;
    eAlphaMode              alphaMode;
    ufloat                  opacityValue;
    FVec2I                  shift;
    FVec2I                  backdropCoverage;
    FRectI                   backdropWorkingRect;
};

ULIS_NAMESPACE_END

