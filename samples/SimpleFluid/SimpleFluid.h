// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS3
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
    FPixelValue*                mCurrentParticleColor;
    FPixelValue*                mBGParticleColor;
    FPixelValue*                mDarkParticleColor;
    FPixelValue*                mLightParticleColor;
    eBlendingMode               mCurrentBlendingMode;
    float                       mCurrentOpacity;
    float                       mMul;
};

