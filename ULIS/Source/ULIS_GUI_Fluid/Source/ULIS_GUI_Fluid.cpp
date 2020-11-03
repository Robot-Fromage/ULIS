// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS_GUI_Fluid.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include "ULIS_GUI_Fluid.h"


int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    SWindow* w = new SWindow();
    w->show();

    int exit_code = app.exec();

    delete w;

    return  exit_code;
}

