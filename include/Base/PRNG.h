// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         PRNG.h
* @author       Clement Berthaud
* @brief        This file provides declaration for weak PRNG functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS2_NAMESPACE_BEGIN
ULIS2_API void ResetWeakPRNGSeed();
ULIS2_API void SetWeakPRNGSeed( uint32 iVal );
ULIS2_API uint32 GetWeakPRNGSeed();
ULIS2_API uint32 GenerateWeakPRNG();
ULIS2_NAMESPACE_END

