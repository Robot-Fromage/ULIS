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

class A
{
public:
    virtual ~A() = 0 {}
};

class B : public A
{
public:
    virtual ~B() override {};
};

class C final : public B
{
public:
    ~C() override {};
};

void F( B& i )
{
    auto dummy = 0;
}

template< typename T, typename U, void (*IMP)( U& ) >
void
FF( T& i )
{
    IMP( i );
}


int
main() {
    C obj;
    FF< C, B, &F >( obj );

    return  0;
}

