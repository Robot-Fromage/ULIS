// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        Particles application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "Particles.h"
#include <QApplication>

#include <cstdlib>
#include <ctime>
int
main( int argc, char *argv[] ) {
    srand( time( NULL ) );
    QApplication app( argc, argv );
    SWindow* window = new SWindow();
    window->show();
    int exit_code = app.exec();
    delete  window;
    return  exit_code;
}

