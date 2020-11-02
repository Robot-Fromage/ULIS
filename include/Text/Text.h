// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Text.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Text entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Geometry/Transformation2D.h"

ULIS_NAMESPACE_BEGIN
ULIS_API void RenderText(
      FOldThreadPool* iOldThreadPool
    , bool iBlocking
    , uint32 iPerfIntent
    , const FHostDeviceInfo& iHostDeviceInfo
    , bool iCallCB
    , FBlock* iDestination
    , const std::wstring iText
    , const FFont& iFont
    , int iSize
    , const ISample& iColor
    , const FTransformation2D& iTransform
    , bool iAntialiasing
);

ULIS_API FRectI TextMetrics(
      std::wstring iText
    , const FFont& iFont
    , int iSize
    , const FTransformation2D& iTransform
);
ULIS_NAMESPACE_END

