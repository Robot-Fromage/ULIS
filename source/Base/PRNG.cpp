// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         PRNG.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for weak PRNG functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Base/PRNG.h"

ULIS3_NAMESPACE_BEGIN
static uint32 sWeakPRNGSeed = 5323; // Arbitrary

void ResetWeakPRNGSeed() {
    sWeakPRNGSeed = 5323;
}

void SetWeakPRNGSeed( uint32 iVal ) {
    sWeakPRNGSeed = iVal;
}

uint32 GetWeakPRNGSeed() {
    return  sWeakPRNGSeed;
}

uint32 GenerateWeakPRNG() {
    sWeakPRNGSeed = 8253729 * sWeakPRNGSeed + 2396403;
    return sWeakPRNGSeed % 65537;
}

ULIS3_NAMESPACE_END

