// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Widgets.GraphScene.h
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the declaration for the SGraphScene class.
 */
#pragma once

#include <QGraphicsScene>

namespace  nApplication {
namespace  nWidgets {


class SGraphScene : public QGraphicsScene
{
    Q_OBJECT

    typedef  QGraphicsScene tSuperClass;
    typedef  SGraphScene    tSelf;

public:
    // Construction / Destruction
    virtual  ~SGraphScene();
    SGraphScene( QObject* iParent = NULL );

};


} // namespace  nWidgets
} // namespace  nApplication

