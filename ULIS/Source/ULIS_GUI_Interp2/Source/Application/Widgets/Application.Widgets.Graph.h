// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Widgets.Graph.h
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the declaration for the SGraph class.
 */
#pragma once

#include <QWidget>

namespace  nApplication { namespace  nWidgets { class SGraphView; } }
namespace  nApplication { namespace  nWidgets { class SGraphScene; } }

namespace  nApplication {
namespace  nWidgets {


class SGraph : public QWidget
{
    Q_OBJECT

    typedef  QWidget    tSuperClass;
    typedef  SGraph      tSelf;

public:
    // Construction / Destruction
    virtual  ~SGraph();
    SGraph( QWidget* iParent = NULL );

protected:
    // Event
    virtual  void  resizeEvent( QResizeEvent*   event )  override;

private:
    // GUI utilities
    void  Init();
    void  Build();
    void  Compose();
    void  Destroy();

private:
    // Private Data Members
    SGraphView*     mView;
    SGraphScene*    mScene;

};


} // namespace  nWidgets
} // namespace  nApplication
