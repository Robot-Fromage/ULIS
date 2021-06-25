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
#include <ctime>

int main( int argc, char *argv[] ) {
    using namespace ::ULIS;
    using namespace ::ULIS::units;
    std::cout << FLibInfo::LibraryInformationString().Data() << std::endl;
    srand( time( NULL ) );
    // Test1: Many random allocs and degrag
    /*
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
    */

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

    std::cout << (uint64)bit_t( bit_t( 1 ) ) << std::endl;
    std::cout << (uint64)bit_t( kbit_t( 1 ) ) << std::endl;
    std::cout << (uint64)bit_t( Mbit_t( 1 ) ) << std::endl;
    std::cout << (uint64)bit_t( Gbit_t( 1 ) ) << std::endl;
    std::cout << std::endl;
    std::cout << (uint64)bit_t( bit_t( 1 ) ) << std::endl;
    std::cout << (uint64)bit_t( Kibit_t( 1 ) ) << std::endl;
    std::cout << (uint64)bit_t( Mibit_t( 1 ) ) << std::endl;
    std::cout << (uint64)bit_t( Gibit_t( 1 ) ) << std::endl;
    std::cout << std::endl;
    std::cout << (double)bit_t( bit_t( 1 ) ) << std::endl;
    std::cout << (double)kbit_t( bit_t( 1 ) ) << std::endl;
    std::cout << (double)Mbit_t( bit_t( 1 ) ) << std::endl;
    std::cout << (double)Gbit_t( bit_t( 1 ) ) << std::endl;
    std::cout << std::endl;
    std::cout << (double)bit_t( bit_t( 1 ) ) << std::endl;
    std::cout << (double)Kibit_t( bit_t( 1 ) ) << std::endl;
    std::cout << (double)Mibit_t( bit_t( 1 ) ) << std::endl;
    std::cout << (double)Gibit_t( bit_t( 1 ) ) << std::endl;
    std::cout << std::endl;
    return  0;
}

