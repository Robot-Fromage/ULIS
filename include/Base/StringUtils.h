// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         StringUtils.h
* @author       Clement Berthaud
* @brief        This file provides string utility tools.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS2_NAMESPACE_BEGIN
ULIS2_API void ReplaceAllOccurences( std::string& str, const std::string& from, const std::string& to );
ULIS2_API size_t LevenshteinDistance( const char* s, size_t n, const char* t, size_t m );
ULIS2_NAMESPACE_END

