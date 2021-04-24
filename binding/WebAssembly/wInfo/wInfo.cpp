// IDDN FR.001.250001.004.S.X.2019.000.00000
/**
*
*   ULIS
*__________________
*
* @file         wInfo.cpp
* @author       Clement Berthaud
* @brief        Formats application for wasm ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>

int main() {
    std::cout << ::ULIS::FullLibraryInformationString().Data() << std::endl;
    return  0;
}

