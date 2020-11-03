// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Widgets.GraphView.h
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the declaration for the SGraphView class.
 */
#pragma once

#include <QGraphicsView>

class  QScroller;
class  QTimer;
class  QCursor;

namespace  nApplication {
namespace  nWidgets {


class SGraphView : public QGraphicsView
{
    Q_OBJECT

    typedef  QGraphicsView  tSuperClass;
    typedef  SGraphView     tSelf;

public:
    // Construction / Destruction
    virtual  ~SGraphView();
    SGraphView( QWidget* iParent = nullptr );

protected:
    // Protected Painting Behaviour Overrides
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
    virtual void drawForeground(QPainter *painter, const QRectF &rect) override;

protected:
    // Protected Event Overrids
    virtual  void  keyPressEvent( QKeyEvent*  event )           override;
    virtual  void  keyReleaseEvent( QKeyEvent*  event )         override;
    virtual  void  mouseMoveEvent( QMouseEvent*  event )        override;
    virtual  void  mousePressEvent( QMouseEvent*  event )       override;
    virtual  void  mouseReleaseEvent( QMouseEvent*  event )     override;
    virtual  void  wheelEvent( QWheelEvent*  event )            override;

private:
    // Tick slot
    Q_SLOT void tickEvent();

private:
    // Delegate Drawing Functions
    void  drawGrid( QPainter* painter, const QRectF &rect, const QColor& iGridColor, const QColor& iSubGridColor, int iGridSize, int iSubGridCount);

private:
    // Custom Interaction Behaviours
    qreal   Scale();
    void    DeltaScale( qreal iDelta );
    void    SetScale( qreal iValue );

private:
    // GUI utilities
    void  Init();
    void  Build();
    void  Compose();
    void  Destroy();

public:
    // Scroller Patch
    void  InitScroller();

private:
    // Private Data Members
    QScroller*  mScroller;
    QTimer*     mTimer;
    QCursor*    mLensCursor;

    bool        mMoveModifierOn;
    bool        mZoomModifierOn;
    bool        mRightButtonMoveOn;
    bool        mMiddleButtonMoveOn;

    bool        mMouseMovedWhileMouseZoom;

    QString     mInteractionState;
    QString     mLastInteractionState;
    QPoint      mLastMousePos;
    QPoint      mTargetViewPos;
    QPointF     mTargetScenePos;

    QImage*     mVignette_TopLeft;
    QImage*     mVignette_Top;
    QImage*     mVignette_TopRight;
    QImage*     mVignette_Right;
    QImage*     mVignette_BotRight;
    QImage*     mVignette_Bot;
    QImage*     mVignette_BotLeft;
    QImage*     mVignette_Left;
};


} // namespace  nWidgets
} // namespace  nApplication

