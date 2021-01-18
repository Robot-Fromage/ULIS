// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PRNG.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Blend PRNG entry point functions.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Blend PRNG for pseudo random modes like Dissolve
/*! Reset the PRNG Seed for pseudo random Blend modes like Dissolve. */
void ResetBlendPRNGSeed();

/*! Set the PRNG Seed for pseudo random Blend modes like Dissolve. */
void SetBlendPRNGSeed( uint32 iVal );

/*! Get the PRNG Seed for pseudo random Blend modes like Dissolve. */
uint32 GetBlendPRNGSeed();

/*! Generate a pseudo random number for Blend modes like Dissolve. */
uint32 GenerateBlendPRNG();

ULIS_NAMESPACE_END

