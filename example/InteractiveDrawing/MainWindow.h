// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MainWindow.h
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QWidget>
class QImage;
class QIcon;
class QLabel;
class QPixmap;
class QPushButton;
class QTimer;
using namespace ::ULIS;

enum eRasterOp {
      kLine
    , kCircleAndres
    , kCircleBresenham
    , kArcAndres
    , kArcBresenham
    , kEllipse
    , kRotatedEllipse
    , kRectangle
    , kPolygon
    , kQuadraticBezier
    , kNumRasterOP
};

static const char* kwRasterOP[] = {
      "Line"
    , "CircleAndres"
    , "CircleBresenham"
    , "ArcAndres"
    , "ArcBresenham"
    , "Ellipse"
    , "RotatedEllipse"
    , "Rectangle"
    , "Polygon"
    , "QuadraticBezier"
    , "Invalid"
};

enum eRasterMode {
      kNone
    , kAA
    , kSP
    , kNumRasterMode
};

static const char* kwRasterMode[] = {
      ""
    , "AA"
    , "SP"
    , "Invalid"
};

class SMainWindow
    : public QWidget
{
    Q_OBJECT

public:
    ~SMainWindow();
    SMainWindow();

private:
    virtual void mousePressEvent( QMouseEvent* event ) override;
    virtual void mouseMoveEvent( QMouseEvent* event ) override;
    virtual void mouseReleaseEvent( QMouseEvent* event ) override;
    virtual void keyPressEvent( QKeyEvent* event ) override;

private slots:
    void tickEvent();
    void SetRaster( eRasterOp iRasterOp, eRasterMode iRasterMode );

private:
    void BuildButton( QPushButton* ioButton, eRasterOp iRasterOp, eRasterMode iRasterMode );

private:
    FThreadPool mPool;
    FCommandQueue mQueue;
    eFormat mFmt;
    FContext mCtx;
    FHardwareMetrics mHw;
    FSchedulePolicy mPolicyCacheEfficient;
    FSchedulePolicy mPolicyMultiScanlines;
    FBlock mTemp;
    FBlock mCanvas;
    FBlock mMiniBlockAA;
    FBlock mMiniBlockSP;
    eRasterOp mCurrentRasterOp;
    eRasterMode mCurrentRasterMode;
    QImage* mImage;
    QPixmap* mPixmap;
    QLabel* mLabel;
    QTimer* mTimer;
    QVector< QPushButton* > buttons;
};

