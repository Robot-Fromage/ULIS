// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS_GUI_Interp.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       Main file for project ULIS_GUI_Interp.
 */

#include <QApplication>
#include <QWidget>
#include <ULIS_CORE>
#include "Application/Window/Application.Window.MainWindow.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    QWidget* w  = new ::nApplication::nWindow::SMainWindow();
    w->resize( 800, 600 );
    w->show();

    int exit_code = app.exec();

    delete w;
    return  exit_code;
}

