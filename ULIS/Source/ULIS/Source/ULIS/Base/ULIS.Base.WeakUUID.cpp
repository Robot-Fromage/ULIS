// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Base.WeakUUID.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definitions for the Weak UUID utility functions.
 */
#include "ULIS/Base/ULIS.Base.WeakUUID.h"
#include <iostream>
#include <sstream>
#include <random>

namespace ULIS {
/////////////////////////////////////////////////////
// Weak UUID
//--------------------------------------------------------------------------------------
//---------------------------------------------------------- Private Unit Implementation
unsigned char random_char() {
    return  static_cast< unsigned  char >( rand()%256 );
}


std::string generate_hex( const  unsigned  int  len ) {
    std::stringstream ss;
    for( unsigned  int i = 0; i < len; i++) {
        auto rc = random_char();
        std::stringstream hexstream;
        hexstream << std::hex << int( rc );
        auto hex = hexstream.str(); 
        ss << ( hex.length() < 2 ? '0' + hex : hex );
    }
    return  ss.str();
}


//--------------------------------------------------------------------------------------
//---------------------------------------------------------------- Public Implementation
std::string GenerateWeakUUID( uint8 len )
{
    return  generate_hex( len );
}


} // namespace ULIS

