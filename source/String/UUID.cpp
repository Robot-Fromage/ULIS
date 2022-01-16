// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         UUID.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for weak UUID functions.
* @license      Please refer to LICENSE.md
*/
#include "String/UUID.h"
#include <iostream>
#include <sstream>
#include <random>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Weak UUID
//--------------------------------------------------------------------------------------
//---------------------------------------------------------- Private Unit Implementation
unsigned char random_char() {
    return  static_cast< unsigned  char >( rand() % 256 );
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


ULIS_NAMESPACE_END

