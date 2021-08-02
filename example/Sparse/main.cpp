// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        Sparse terminal curses debug application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <curses.h>
using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    initscr();
    int x, y;
    x = y = 10;
    move( x, y );
    printw( "Hello World!" );
    refresh();
    int c = getch();
    clear();

    mvprintw( 0, 0, "%d", c );
    getch();
    endwin();

    return  0;
}

