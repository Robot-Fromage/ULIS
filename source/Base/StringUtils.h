// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         StringUtils.h
* @author       Clement Berthaud
* @brief        This file provides string utility tools.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS3_NAMESPACE_BEGIN
void ReplaceAllOccurences( std::string& str, const std::string& from, const std::string& to );
size_t LevenshteinDistance( const char* s, size_t n, const char* t, size_t m );
ULIS3_NAMESPACE_END

