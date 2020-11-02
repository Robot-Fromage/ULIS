// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Methods.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the resampling methods enums.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// eResamplingMethod
enum eResamplingMethod
{
      INTERP_NN
    , INTERP_BILINEAR
    , INTERP_BICUBIC
    , INTERP_AREA
};

static const char* kwResamplingMethod[] =
{
      "NearestNeighbour"
    , "Bilinear"
    , "Bicubic"
    , "Area"
};

/////////////////////////////////////////////////////
// eMipsLayout
enum eMipsLayout
{
      MPL_STANDARD
    , MPL_MIPSONLY
};

static const char* kwMipsLayout[] =
{
      "Standard"
    , "MipsOnly"
};

ULIS_NAMESPACE_END

