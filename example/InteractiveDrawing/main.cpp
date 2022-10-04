// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "MainWindow.h"
#include "ULISLoader.h"
#include <QApplication>
#include <cstdlib>
#include <ctime>

int
main( int argc, char *argv[] ) {
    srand( time( NULL ) );
    QApplication app( argc, argv );
    FULISLoader handle;
    SMainWindow* window = new SMainWindow( handle );
    window->show();
    int exit_code = app.exec();
    delete  window;
    return  exit_code;
}

