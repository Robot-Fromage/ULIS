// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedBezier.h
* @author       Clement Berthaud
* @brief        AnimatedBezier application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>

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
    FHardwareMetrics             mHost;
    FThreadPool*                mPool;
    FBlock*                     mSRC;
    FBlock*                     mDST;
    std::vector< FCubicBezierControlPoint > mCtrlPts;
    float                       mEvolutiveAngle;

    QImage*                     mImage;
    QPixmap*                    mPixmap;
    QLabel*                     mLabel;
    QTimer*                     mTimer;
    bool                        mLeftButtonDown;
};

