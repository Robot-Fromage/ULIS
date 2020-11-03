// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         UUID.h
* @author       Clement Berthaud
* @brief        This file provides declaration for weak UUID functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Weak UUID
/// @fn         GenerateWeakUUID()
/// @brief      Generate a weak UUID.
/// @details    Weak UUID is an UUID that is pseudo random but with no guarantee to be unique at all. This it can be suitable for light uses.
ULIS2_API std::string GenerateWeakUUID( uint8 iLen );

ULIS2_NAMESPACE_END

