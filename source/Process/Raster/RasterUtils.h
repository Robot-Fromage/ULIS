// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterUtils.h
* @author       Thomas Schmitt
* @brief        This file provides Utility struct and functions for the other raster API files
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Image/Block.h"
#include <map>

ULIS_NAMESPACE_BEGIN
namespace eClippingZone
{
const uint8 kInside   = 0b0000;
const uint8 kLeft     = 0b0001;
const uint8 kRight    = 0b0010;
const uint8 kBot      = 0b0100;
const uint8 kTop      = 0b1000;
};
typedef uint8 tClippingCode;

int InternalComputeCodeForPoint( const FVec2I& iPoint,const FRectI& iClippingRect );
bool InternalCropLineToRectangle( FVec2I& ioP0,FVec2I& ioP1,const FRectI& iClippingRect );
float InternalGetPixelBaseAlphaFromCoord( const FVec2F& iPt );

ULIS_NAMESPACE_END

