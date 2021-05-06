// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QStyle>

#include <cstdlib>
#include <ctime>
int
main( int argc, char *argv[] ) {
    srand( time( NULL ) );
    QApplication app( argc, argv );
    FMainWindow* window = new FMainWindow();
    QFile stylesheet("C:/Users/PRAXINOS/work/ULIS_Workspace/Versions/dev/ULIS/binding/Python/interactive/res/style/style.qss");
    stylesheet.open( QFile::ReadOnly );
    app.setStyleSheet( stylesheet.readAll() );

    window->setWindowTitle( "pyULIS4 Interactive Demo" );
    window->show();
    int exit_code = app.exec();
    delete  window;
    return  exit_code;
}

