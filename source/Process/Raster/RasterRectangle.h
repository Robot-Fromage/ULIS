// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterRectangle.h
* @author       Thomas Schmitt
* @brief        This file provides the actual functions for the raster of Rectangle
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN

void DrawRectangle(        FBlock&                        iBlock
                         , const FVec2I&                  iTopLeft
                         , const FVec2I&                  iBottomRight
                         , const FColor&                  iColor
                         //, const bool                     iFilled // Done in invocation
                         , const FRectI&                  iClippingRect );

ULIS_NAMESPACE_END

