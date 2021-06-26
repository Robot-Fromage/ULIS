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
using namespace ::ULIS;

int main( int argc, char *argv[] ) {

    //srand( time( NULL ) );
    // Test1: Many random allocs and degrag
    using namespace ULIS::units_literals;
    constexpr uint64 elems = 10;
    constexpr uint64 numArenas = 10;
    FFixedAllocMemoryPool mem( 8_B, elems, 200_MB );
    for( int i = 0; i < numArenas; ++i )
        mem.AllocOneArenaIfNecessary();

    tClient a[elems][numArenas];
    for( int i = 0; i < elems; ++i )
        for( int j = 0; j < numArenas; ++j )
            a[i][j] = mem.Malloc();

    int del = ( elems * numArenas ) / 2;
    for( int i = 0; i < del; ++i ) {
        int x = rand() % ( elems - 1 );
        int y = rand() % ( numArenas - 1 );
        tClient client = a[x][y];
        while( FFixedAllocArena::IsFree( client ) ) {
            rand() % ( elems - 1 );
            rand() % ( numArenas - 1 );
            client = a[rand()%elems][rand()%numArenas];
        }
    }

    mem.DebugPrint();
    mem.UnsafeFreeAll();
    //mem.DefragForce();
    //mem.Print();

    // Test2: Proper client update after defrag
    /*
    FFixedAllocMemoryPool mem( 10, 1, 10 );
    uint8** a_client[10];

    for( int i = 0; i < 5; ++i ) {
        a_client[i] = mem.Malloc();
        **a_client[i] = 0;
    }

    uint8** data_client = mem.Malloc();
    **data_client = 5;

    for( int i = 0; i < 5; ++i )
        mem.Free( a_client[i] );

    mem.Print();
    std::cout << (int)**data_client << std::endl;
    mem.DefragForce();
    mem.Print();
    std::cout << (int)**data_client << std::endl;

    for( int i = 0; i < 5; ++i ) {
        a_client[i] = mem.Malloc();
        **a_client[i] = 0;
    }
    mem.Print();
    std::cout << (int)**data_client << std::endl;

    for( int i = 0; i < 5; ++i )
        mem.Free( a_client[i] );
    mem.Free( data_client );
    */

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

