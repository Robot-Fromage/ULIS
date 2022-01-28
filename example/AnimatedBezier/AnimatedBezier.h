// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedBezier.h
* @author       Clement Berthaud
* @brief        AnimatedBezier application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QWidget>
#include <QPoint>
class QImage;
class QLabel;
class QPixmap;
class QTimer;
using namespace ::ULIS;

class SWindow
    : public QWidget
{
    Q_OBJECT

public:
    ~SWindow();
    SWindow();

private:
    Q_SLOT void tickEvent();

private:
    FThreadPool mPool;
    FCommandQueue mQueue;
    eFormat mFmt;
    FContext mCtx;
    FHardwareMetrics mHw;
    FSchedulePolicy mPolicyCacheEfficient;
    FSchedulePolicy mPolicyMultiScanlines;
    FBlock mSrc;
    FBlock mDst;
    TArray< FCubicBezierControlPoint > mCtrlPts;
    float mAngle;

    QImage* mImage;
    QPixmap* mPixmap;
    QLabel* mLabel;
    QTimer* mTimer;
};

