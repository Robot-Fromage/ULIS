// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Dispatch.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic Text entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS_NAMESPACE_BEGIN
struct _FPrivateTextInfo {
    FOldThreadPool*            pool;
    bool                    blocking;
    uint32                  perfIntent;
    const FHostDeviceInfo*  hostDeviceInfo;
    FBlock*                 destination;
    const std::wstring*     text;
    const FFont*            font;
    int                     size;
    uint8                   color[64];
    FT_Matrix               matrix;
    FVec2I                  position;
    bool                    antialiasing;
};
ULIS_NAMESPACE_END

