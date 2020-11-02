// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS
*__________________
*
* @file         Sandbox.cpp
* @author       Clement Berthaud
* @brief        Sandbox program without ULIS.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <cstdint>
#include <iostream>
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

uint8 Downscale( uint16 iValue )  { return ( iValue + 1 + ( iValue>>8 ) ) >> 8; }

int
main() {
    std::cout << (int)( 65535 >> 8 );
    return  0;
}

