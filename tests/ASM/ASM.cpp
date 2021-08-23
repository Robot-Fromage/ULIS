// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         ASM.cpp
* @author       Clement Berthaud
* @brief        Assembly test application for ULIS2.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <iostream>

int power2( int num, int power ) {
    /*
    __asm {
        mov eax, num    ; Get first argument
        mov ecx, power  ; Get second argument
        shl eax, cl     ; EAX = EAX * ( 2 to the power of CL )
    }
    // Return with result in EAX
    */
    return  0;
}

int main() {
    std::cout << power2( 3, 5 );
    return  0;
}

