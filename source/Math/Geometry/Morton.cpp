// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Morton.cpp
* @author       Clement Berthaud
* @brief        This file provides the macros for the Morton utilities.
* @license      Please refer to LICENSE.md
*/
#include "Math/Geometry/Morton.h"

#define ULIS_MORTON_X_MASK 0x55 // 0b01010101
#define ULIS_MORTON_Y_MASK 0xAA // 0b10101010

ULIS_NAMESPACE_BEGIN
// http://bitmath.blogspot.com/2012/11/tesseral-arithmetic-useful-snippets.html
//static
uint8
FMortonFunctionKeys8bit2D::IncX( uint8 iKey ) {
    uint8 sum = ( iKey | ULIS_MORTON_Y_MASK ) + 1;
    return ( sum & ULIS_MORTON_X_MASK ) | ( iKey & ULIS_MORTON_Y_MASK );
}

//static
uint8
FMortonFunctionKeys8bit2D::IncY( uint8 iKey ) {
    uint8 sum = ( iKey | ULIS_MORTON_X_MASK ) + 2;
    return ( sum & ULIS_MORTON_Y_MASK ) | ( iKey & ULIS_MORTON_X_MASK );
}

ULIS_NAMESPACE_END

