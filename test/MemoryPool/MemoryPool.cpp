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
#include <ctime>
#include <ULIS>
using namespace ::ULIS;
using namespace ULIS::units_literals;

void TestFixedDefragRate() {
    // Test1: Many random allocs / deallocs and degraf
    // Bake constants and variables
    byte_t allocSize = 8_B;
    constexpr uint64 numCellsPerArena = 10;
    constexpr uint64 numArenas = 10;
    tClient a[numCellsPerArena][numArenas];
    bool b[numCellsPerArena][numArenas];

    // Init pool
    FFixedAllocMemoryPool mem( allocSize, numCellsPerArena, 200_MB );
    for( int i = 0; i < 10; ++i )
        mem.AllocOneArenaIfNecessary();

    // Fill with allocs
    for( int i = 0; i < numCellsPerArena; ++i ) {
        for( int j = 0; j < numArenas; ++j ) {
            a[i][j] = mem.Malloc();
            b[i][j] = true;
        }
    }

    // Print
    mem.DebugPrint();

    // Free half of allocs at random positions.
    int del = ( numCellsPerArena * numArenas ) / 2;
    for( int i = 0; i < del; ++i ) {
        int x = rand() % ( numCellsPerArena );
        int y = rand() % ( numArenas );
        tClient client = a[x][y];
        while( b[x][y] == false ) {
            x = rand() % ( numCellsPerArena );
            y = rand() % ( numArenas );
            client = a[x][y];
        }
        mem.Free( client );
        b[x][y] = false;
    }

    // Print
    mem.DebugPrint();

    // Trigger Defrag
    mem.DefragForce();

    // Print
    mem.DebugPrint();

    // Free all remaining unsafe but it's ok because we're done.
    mem.UnsafeFreeAll();

    // Print
    mem.DebugPrint();
}

void TestFixedClientUpdateAfterDefrag() {
    // Test2: Proper client update after defrag
    FFixedAllocMemoryPool mem( 10_B, 10, 1_MB );
    mem.AllocOneArenaIfNecessary();
    mem.AllocOneArenaIfNecessary();
    uint8** a_client[20];

    for( int i = 0; i < 15; ++i ) {
        a_client[i] = mem.Malloc();
        **(a_client[i]) = 0;
    }

    uint8** data_client = mem.Malloc();
    **data_client = 5;

    for( int i = 0; i < 15; ++i )
        mem.Free( a_client[i] );

    mem.DebugPrint();
    std::cout << (int)**data_client << std::endl;
    mem.DefragForce();
    mem.DebugPrint();
    std::cout << (int)**data_client << std::endl;

    for( int i = 0; i < 15; ++i ) {
        a_client[i] = mem.Malloc();
        **a_client[i] = 0;
    }
    mem.DebugPrint();
    std::cout << (int)**data_client << std::endl;

    for( int i = 0; i < 10; ++i )
        mem.Free( a_client[i] );
    mem.DebugPrint();
    std::cout << (int)**data_client << std::endl;

    mem.DefragForce();
    mem.DebugPrint();
    std::cout << (int)**data_client << std::endl;

    mem.Free( data_client );
    mem.DebugPrint();
    mem.UnsafeFreeAll();
    mem.DebugPrint();
}

int main( int argc, char *argv[] ) {
    srand( time( NULL ) );
    //TestFixedDefragRate();
    //TestFixedClientUpdateAfterDefrag();

    /*
    // Test3: Basic Shrinkable
    // metapad = 12
    /*
    int allocSize = 100;
    constexpr int numAllocs = 10;
    FShrinkableAllocArena mem( numAllocs * allocSize + numAllocs * 12, allocSize );
    mem.Print();

    uint8** a[numAllocs];
    for( int i = 0; i < numAllocs; ++i ) {
        a[i] = mem.Malloc();
    }
    mem.Print();

    for( int i = 0; i < numAllocs; ++i ) {
        int s = ( rand()% ( allocSize / 2 - 2 ) ) + 1;
        bool shrank = mem.Shrink( *a[i], s );
        ULIS_ASSERT( shrank, "cool" );
    }
    mem.Print();

    for( int i = 0; i < 10; ++i )
        if( !FShrinkableAllocArena::IsFree( (const uint8**)a[i] ) )
            mem.Free( a[i] );

    for( int i = 0; i < numAllocs; ++i ) {
        a[i] = mem.Malloc();
    }

    for( int i = 0; i < numAllocs; ++i ) {
        int s = ( rand()% ( allocSize / 2 - 2 ) ) + 1;
        bool shrank = mem.Shrink( *a[i], s );
        ULIS_ASSERT( shrank, "cool" );
    }
    mem.Print();
    */
    return  0;
}

