// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/**
*
*   ULIS
*__________________
*
* @file         SimpleFluid.h
* @author       Clement Berthaud
* @brief        SimpleFluid application for ULIS3.
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

protected:
    virtual void mousePressEvent( QMouseEvent* event ) override;
    virtual void mouseMoveEvent( QMouseEvent* event ) override;
    virtual void mouseReleaseEvent( QMouseEvent* event ) override;
    virtual void keyPressEvent( QKeyEvent* event ) override;

private:
    Q_SLOT void tickEvent();
    void RedrawParticle();

private:
    std::vector< FParticle >    mParticles;

    FHostDeviceInfo             mHost;
    FThreadPool*                mPool;
    FBlock*                     mWetCanvas;
    FBlock*                     mDryCanvas;
    FBlock*                     mParticle;

    QPoint                      mPos;
    QImage*                     mImage;
    QPixmap*                    mPixmap;
    QLabel*                     mLabel;
    QTimer*                     mTimer;
    bool                        mLeftButtonDown;
    int                         mParticleSize;
    FColor*                mCurrentParticleColor;
    FColor*                mBGParticleColor;
    FColor*                mDarkParticleColor;
    FColor*                mLightParticleColor;
    eBlendMode               mCurrentBlendingMode;
    float                       mCurrentOpacity;
    float                       mMul;
};

