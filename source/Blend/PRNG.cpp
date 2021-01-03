// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PRNG.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Blend PRNG entry point functions.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Blend/PRNG.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Blend PRNG for pseudo random modes like Dissolve
namespace detail {
/*! Arbitrary seed. */
static uint32 sgBlendPRNGSeed = 5323;
} // namespace detail

void ResetBlendPRNGSeed() {
    detail::sgBlendPRNGSeed = 5323;
}

void SetBlendPRNGSeed( uint32 iVal ) {
    detail::sgBlendPRNGSeed = iVal;
}

uint32 GetBlendPRNGSeed() {
    return  detail::sgBlendPRNGSeed;
}

uint32 GenerateBlendPRNG() {
    detail::sgBlendPRNGSeed = 8253729 * detail::sgBlendPRNGSeed + 2396403;
    return detail::sgBlendPRNGSeed % 65537;
}

ULIS_NAMESPACE_END

