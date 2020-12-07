// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Utils.cpp
* @author       Clement Berthaud
* @brief        This file provides string utility tools.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "String/Utils.h"
#include <iostream>
#include <cstring>
#include <cassert>
#include <algorithm>

namespace std {   };
using namespace std;

ULIS_NAMESPACE_BEGIN
bool replace_imp( std::string& iStr, const std::string& iFrom, const std::string& iTo ) {
    size_t start_pos = iStr.find( iFrom );
    if( start_pos == std::string::npos )
        return  false;
    iStr.replace( start_pos, iFrom.length(), iTo );
    return  true;
}

void ReplaceAllOccurences( std::string& iStr, const std::string& iFrom, const std::string& iTo ) {
    while( replace_imp( iStr, iFrom, iTo ) ) {}
}

size_t
LevenshteinDistance( const char* iA, size_t iLenA, const char* iB, size_t iLenB )
{
    ++iLenA; ++iLenB;
    size_t* diff = new size_t[ iLenA * iLenB ];
    memset( diff, 0, sizeof( size_t ) * iLenA * iLenB );
    for( size_t i = 1, im = 0; i < iLenB; ++i, ++im ) {
        for( size_t j = 1, jn = 0; j < iLenA; ++j, ++jn ) {
            if( iA[ jn ] == iB[ im ] ) {
                diff[ ( i * iLenA ) + j ] = diff[ ( ( i - 1 ) * iLenA ) + ( j - 1 ) ];
            } else {
                diff[ ( i * iLenA ) + j ] = min( diff[ ( i - 1 ) * iLenA + j ] + 1, min( diff[ i * iLenA + ( j - 1 ) ] + 1, diff[ ( i - 1 ) * iLenA + ( j - 1 ) ] + 1 ) );
            }
        }
    }
    size_t r = diff[ iLenA * iLenB - 1 ];
    delete [] diff;
    return  r;
}

ULIS_NAMESPACE_END

