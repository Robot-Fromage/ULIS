// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Info.cpp
* @author       Clement Berthaud
* @brief        Info application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <iostream>
#include <ULIS>
#include <bitset>

int main( int argc, char *argv[] ) {
    std::cout << ::ULIS::FLibInfo::LibraryInformationString().Data() << std::endl;
    return  0;
}

