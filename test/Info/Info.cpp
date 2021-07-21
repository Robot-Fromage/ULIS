// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Info.cpp
* @author       Clement Berthaud
* @brief        Info application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <iostream>
#include <ULIS>
#include <bitset>

int main( int argc, char *argv[] ) {
    std::cout << ::ULIS::FLibInfo::LibraryInformationString().Data() << std::endl;
    constexpr ::ULIS::TMortonEncodeKeys8bit< 16, 0 > sgMortonEncodeKeys8bit_2D_16_X;
    constexpr ::ULIS::TMortonEncodeKeys8bit< 16, 1 > sgMortonEncodeKeys8bit_2D_16_Y;
    constexpr ::ULIS::TMortonDecodeKeys8bit2D< 256 > sgMortonDecodeKeys8bit_2D_16_XY;
    for( int i = 0; i < 256; ++i ) {
        std::cout << std::bitset< 8 >( i ) << " " << int(sgMortonDecodeKeys8bit_2D_16_XY.keys[i].x) << " " << int(sgMortonDecodeKeys8bit_2D_16_XY.keys[i].y) << std::endl;
        ULIS_ASSERT( ( sgMortonEncodeKeys8bit_2D_16_X.keys[ sgMortonDecodeKeys8bit_2D_16_XY.keys[i].x ] | sgMortonEncodeKeys8bit_2D_16_Y.keys[ sgMortonDecodeKeys8bit_2D_16_XY.keys[i].y ] ) == i, "ok" );
    }
    return  0;
}

