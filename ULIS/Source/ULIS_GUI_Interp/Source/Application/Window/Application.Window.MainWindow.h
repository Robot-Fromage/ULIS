// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Window.MainWindow.h
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the declaration for the SMainWindow class.
 */
#pragma once

#include <QWidget>

namespace  nApplication { namespace  nWidgets { class SCanvas; } }
namespace  nApplication { namespace  nWidgets { class SSlider; } }

namespace  nApplication {
namespace  nWindow {


class SMainWindow : public QWidget
{
    Q_OBJECT

    typedef  QWidget        tSuperClass;
    typedef  SMainWindow    tSelf;

public:
    // Construction / Destruction
    virtual  ~SMainWindow();
    SMainWindow( QWidget* iParent = nullptr );

protected:
    // Protected Qt event overrides
    virtual  void  resizeEvent( QResizeEvent*  event )  override;

private:
    // GUI utilities
    void  Init();
    void  Compose();
    void  Destroy();

public slots:
    // Slots
    void  ProcessSliderValueChanged( float iValue );

private:
    // Private data members
    nApplication::nWidgets::SCanvas* mCanvasA;
    nApplication::nWidgets::SCanvas* mCanvasB;
    nApplication::nWidgets::SCanvas* mCanvasResult;
    nApplication::nWidgets::SSlider* mSlider;

};


} // namespace  nWindow
} // namespace  nApplication

