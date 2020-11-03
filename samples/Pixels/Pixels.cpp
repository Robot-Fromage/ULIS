// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS3
*__________________
*
* @file         Pixels.cpp
* @author       Clement Berthaud
* @brief        Pixels application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>
using namespace ::ul3;

FPixelValue GetOneColor() {
    FPixelValue aa = FPixelValue::FromRGBA8( 255, 0, 0 );
    return  aa;
}

int
main() {

    {
        FPixelValue colorA( ULIS3_FORMAT_RGBA8, { 0, 0, 0, 0 } );
        FPixelValue colorB = Conv( FPixelValue::FromRGBA8( 0, 0, 0, 255 ), ULIS3_FORMAT_HSVA8 );
        colorA = colorB;
        FPixelValue colorC = GetOneColor();
        auto dummy = 0;
    }

    return  0;
}

