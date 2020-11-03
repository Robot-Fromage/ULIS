// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         TransformArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Transform and Resize Args structures.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Transform/Methods.h"
#include "Maths/Geometry.h"
#include <glm/matrix.hpp>

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FTransformArgs
struct FTransformArgs {
    FThreadPool*            pool;
    bool                    blocking;
    const FHostDeviceInfo*  hostDeviceInfo;
    uint32                  perfIntent;
    const FBlock*           source;
    FBlock*                 destination;
    FRect                   src_roi;
    FRect                   dst_roi;
    eResamplingMethod       method;
    glm::mat3               inverseTransform;
};

/////////////////////////////////////////////////////
// FResizeArgs
struct FResizeArgs {
    FThreadPool*                        pool;
    bool                                blocking;
    const FHostDeviceInfo*              hostDeviceInfo;
    uint32                              perfIntent;
    const FBlock*                       source;
    FBlock*                             destination;
    FRect                               src_roi;
    FRect                               dst_roi;
    eResamplingMethod                   method;
    FVec2F                              inverseScale;
    FVec2F                              shift;
    std::shared_ptr< const FBlock >     optionalSAT;
};

ULIS3_NAMESPACE_END

