// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Pixels.cpp
* @author       Clement Berthaud
* @brief        Pixels application for ULIS3.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>
using namespace ::ul3;

FColor GetOneColor() {
    FColor aa = FColor::FromRGBA8( 255, 0, 0 );
    return  aa;
}

int
main() {

    {
        FColor colorA( ULIS_FORMAT_RGBA8, { 0, 0, 0, 0 } );
        FColor colorB = Conv( FColor::FromRGBA8( 0, 0, 0, 255 ), ULIS_FORMAT_HSVA8 );
        colorA = colorB;
        FColor colorC = GetOneColor();
        auto dummy = 0;
    }

    return  0;
}

