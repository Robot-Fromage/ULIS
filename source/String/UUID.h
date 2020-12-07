// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         UUID.h
* @author       Clement Berthaud
* @brief        This file provides declaration for weak UUID functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Weak UUID
/// @fn         GenerateWeakUUID()
/// @brief      Generate a weak UUID.
/// @details    Weak UUID is an UUID that is pseudo random but with no guarantee to be unique at all. This it can be suitable for light uses.
std::string GenerateWeakUUID( uint8 iLen );

ULIS_NAMESPACE_END

