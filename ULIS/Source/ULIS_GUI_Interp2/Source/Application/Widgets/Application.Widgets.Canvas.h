// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Widgets.Canvas.h
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the declaration for the SCanvas class.
 */
#pragma once

#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QWidget>
#include <ULIS_FDECL>

namespace  nApplication {
namespace  nWidgets {


class SCanvas : public QWidget
{
    Q_OBJECT

    typedef  QWidget    tSuperClass;
    typedef  SCanvas    tSelf;

public:
    // Construction / Destruction
    virtual  ~SCanvas();
    SCanvas( QWidget* iParent = nullptr );

protected:
    // Protected Qt event overrides
    virtual  void  resizeEvent( QResizeEvent*  event )  override;
    virtual  void  mousePressEvent( QMouseEvent*  event )  override;
    virtual  void  mouseMoveEvent( QMouseEvent*  event )  override;
    virtual  void  mouseReleaseEvent( QMouseEvent*  event )  override;
    virtual  void  keyPressEvent( QKeyEvent*  event )  override;

private:
    // GUI utilities
    void  Init();
    void  Compose();
    void  Destroy();

public:
    // Public API
    ::ULIS::IBlock* GetBlock();
    void RefreshBlock();

private:
    // Private data members
    ::ULIS::IBlock* mBlock;
    QImage* mImage;
    QPixmap mPixmap;
    QLabel* mLabel;

    bool bIsLeftMouseDown;
    bool bIsRightMouseDown;
};


} // namespace  nWidgets
} // namespace  nApplication

