// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Mix.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Conv entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS2_NAMESPACE_BEGIN
ULIS2_API FPixelValue MixNative( ufloat iT, IPixel* iA, IPixel* iB );
ULIS2_API FPixelValue MixLinearRGB( ufloat iT, IPixel* iA, IPixel* iB );
ULIS2_API FPixelValue MixRGB( ufloat iT, IPixel* iA, IPixel* iB );
ULIS2_API FPixelValue MixLab( ufloat iT, IPixel* iA, IPixel* iB );
ULIS2_NAMESPACE_END

