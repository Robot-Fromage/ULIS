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
    using namespace ::ULIS;
    std::cout << FLibInfo::LibraryInformationString().Data() << std::endl;

    constexpr int arenaSize = 90;
    constexpr int numArenas = 50;
    FFixedAllocMemoryPool mem( arenaSize, 1, arenaSize * numArenas );
    for( int i = 0; i < numArenas-1; ++i )
        mem.AllocOneArenaIfNecessary();

    uint8* a[arenaSize][numArenas];
    for( int i = 0; i < arenaSize; ++i )
        for( int j = 0; j < numArenas; ++j )
            a[i][j] = mem.Malloc();

    int del = ( arenaSize * numArenas ) / 2;
    for( int i = 0; i < del; ++i ) {
        uint8* ptr = a[rand()%arenaSize][rand()%numArenas];
        while( FFixedAllocArena::IsFree( ptr ) )
            ptr = a[rand()%arenaSize][rand()%numArenas];
        mem.Free( ptr );
    }

    mem.Print();
    mem.DefragForce();
    mem.Print();

    return  0;
}

