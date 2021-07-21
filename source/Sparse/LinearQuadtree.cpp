// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LinearQuadtree.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FLinearQuadtree class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/LinearQuadtree.h"
#include "Math/Geometry/Morton.h"

ULIS_NAMESPACE_BEGIN
namespace details {
constexpr TMortonEncodeKeys8bit< 16, 0 > sgMortonEncodeKeys8bit_2D_16_X; // 16 bytes
constexpr TMortonEncodeKeys8bit< 16, 1 > sgMortonEncodeKeys8bit_2D_16_Y; // 16 bytes
constexpr TMortonDecodeKeys8bit2D< 256 > sgMortonDecodeKeys8bit_2D_16_XY; // 256 bytes
} // namespace details

ULIS_NAMESPACE_END

