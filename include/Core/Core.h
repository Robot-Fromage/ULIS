// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Core.h
* @author       Clement Berthaud
* @brief        This file gathers core definitions for the ULIS2 library.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
/////////////////////////////////////////////////////
// Includes
#include "Core/Platform.h"
#include "Core/Types.h"
#include "Core/FDecl.h"
#include "Core/Utils.h"
#include "Core/Format.h"

/////////////////////////////////////////////////////
// glm FORCE extensions, before any glm related includes
#define GLM_FORCE_SSE42
#define GLM_FORCE_INTRINSICS
#define GLM_FORCE_SWIZZLE
