// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#include <GL/glew.h>
#include <QApplication>
#include <QOpenGLWidget>
#include <QFile>
#include <QStyle>
#include "MainWindow.h"

#include <cstdlib>
#include <ctime>
int
main( int argc, char *argv[] ) {
    srand( time( NULL ) );
    QApplication app( argc, argv );
    SMainWindow* window = new SMainWindow();
    QFile stylesheet("style.qss");

    if( !stylesheet.open( QFile::ReadOnly ) ) {
        stylesheet.setFileName("C:/Users/PRAXINOS/work/ULIS_Workspace/Versions/dev/ULIS/binding/Python/interactive/res/style/style.qss");
        stylesheet.open( QFile::ReadOnly );
    }

    app.setStyleSheet( stylesheet.readAll() );

    window->setWindowTitle( "pyULIS4 Interactive Demo" );
    window->show();
    int exit_code = app.exec();
    delete  window;
    return  exit_code;
}

