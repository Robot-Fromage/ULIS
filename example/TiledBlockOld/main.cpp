// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        TiledBlock application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "Canvas.h"
#include "ULISLoader.h"
#include <QApplication>

#include <cstdlib>
#include <ctime>
int
main( int argc, char *argv[] ) {
    srand( time( NULL ) );
    QApplication app( argc, argv );
    FULISLoader handle;
    SCanvas* canvas = new SCanvas( handle );
    canvas->show();
    int exit_code = app.exec();
    delete  canvas;
    return  exit_code;
}

