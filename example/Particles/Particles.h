// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Particles.h
* @author       Clement Berthaud
* @brief        Particles application for ULIS.
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

struct FParticle {
    FVec2F p;
    FVec2F v;
};

class SWindow : public QWidget
{
    Q_OBJECT

public:
    ~SWindow();
    SWindow();

private:
    virtual void mousePressEvent( QMouseEvent* event ) override;
    virtual void mouseMoveEvent( QMouseEvent* event ) override;
    virtual void mouseReleaseEvent( QMouseEvent* event ) override;
    Q_SLOT void tickEvent();

private:
    std::vector< FParticle >    mParticles;

    FHardwareMetrics             mHost;
    FThreadPool*                mPool;
    FBlock*                     mCanvas;
    FBlock*                     mParticle;

    QPoint                      mPos;
    QImage*                     mImage;
    QPixmap*                    mPixmap;
    QLabel*                     mLabel;
    QTimer*                     mTimer;
    bool                        mLeftButtonDown;
};

