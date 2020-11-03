// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS3
*__________________
*
* @file         AnimatedBezier.h
* @author       Clement Berthaud
* @brief        AnimatedBezier application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>

#include <QWidget>
#include <QPoint>

#include <vector>

class QImage;
class QLabel;
class QPixmap;
class QTimer;

using namespace ::ul3;

class SWindow : public QWidget
{
    Q_OBJECT

public:
    ~SWindow();
    SWindow();

protected:
    virtual void mousePressEvent( QMouseEvent* event ) override;
    virtual void mouseMoveEvent( QMouseEvent* event ) override;
    virtual void mouseReleaseEvent( QMouseEvent* event ) override;
    virtual void keyPressEvent( QKeyEvent* event ) override;

private:
    Q_SLOT void tickEvent();

private:
    FHostDeviceInfo             mHost;
    FThreadPool*                mPool;
    FBlock*                     mSRC;
    FBlock*                     mDST;
    std::vector< FBezierCubicControlPoint > mCtrlPts;
    float                       mEvolutiveAngle;

    QImage*                     mImage;
    QPixmap*                    mPixmap;
    QLabel*                     mLabel;
    QTimer*                     mTimer;
    bool                        mLeftButtonDown;
};

