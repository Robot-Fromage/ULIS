// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         StringUtils.cpp
* @author       Clement Berthaud
* @brief        This file provides string utility tools.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Base/StringUtils.h"
#include <iostream>
#include <cstring>
#include <cassert>
#include <algorithm>

namespace std {   };
using namespace std;

ULIS3_NAMESPACE_BEGIN
bool replace_imp( std::string& str, const std::string& from, const std::string& to ) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void ReplaceAllOccurences( std::string& str, const std::string& from, const std::string& to ) {
    while( replace_imp( str, from, to ) ) {}
}

size_t
LevenshteinDistance( const char* s, size_t n, const char* t, size_t m )
{
    ++n; ++m;
    size_t* d = new size_t[n * m];
    memset(d, 0, sizeof(size_t) * n * m);
    for( size_t i = 1, im = 0; i < m; ++i, ++im ) {
        for( size_t j = 1, jn = 0; j < n; ++j, ++jn ) {
            if( s[jn] == t[im] ) {
            d[(i * n) + j] = d[((i - 1) * n) + (j - 1)];
            } else {
            d[(i * n) + j] = min(d[(i - 1) * n + j] + 1,        /* A deletion. */
                                min(d[i * n + (j - 1)] + 1,        /* An insertion. */
                                d[(i - 1) * n + (j - 1)] + 1));    /* A substitution. */
            }
        }
    }
    size_t r = d[n * m - 1];
    delete [] d;
    return  r;
}

ULIS3_NAMESPACE_END

