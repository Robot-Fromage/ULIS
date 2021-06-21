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

int main( int argc, char *argv[] ) {
    std::cout << ::ULIS::FLibInfo::LibraryInformationString().Data() << std::endl;
    return  0;
}

