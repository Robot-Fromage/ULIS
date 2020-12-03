// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
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

class A;
class B;

class A
{
public:
    ~A() {}

    B Ret();
};

class B : public A
{
public:
    ~B() {};
};

B A::Ret() {
    return  B();
}

int
main() {
    A a;
    B b = a.Ret();

    return  0;
}

