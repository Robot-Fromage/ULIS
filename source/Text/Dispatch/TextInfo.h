// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Dispatch.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic Text entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Maths/Geometry.h"

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS2_NAMESPACE_BEGIN
struct ULIS2_API _FPrivateTextInfo {
    FThreadPool*            pool;
    bool                    blocking;
    uint32                  perfIntent;
    const FHostDeviceInfo*  hostDeviceInfo;
    FBlock*                 destination;
    const std::wstring*     text;
    const FFont*            font;
    int                     size;
    tByte                   color[64];
    FT_Matrix               matrix;
    FVec2I                  position;
    bool                    antialiasing;
};
ULIS2_NAMESPACE_END

