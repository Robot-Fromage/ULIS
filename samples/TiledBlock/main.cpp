// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         main.cpp
* @author       Clement Berthaud
* @brief        TiledBlock application for ULIS2.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Canvas.h"
#include <QApplication>

#include <cstdlib>
#include <ctime>
int
main( int argc, char *argv[] ) {
    srand( time( NULL ) );
    QApplication app( argc, argv );
    SCanvas* canvas = new SCanvas();
    canvas->show();
    int exit_code = app.exec();
    delete  canvas;
    return  exit_code;
}

