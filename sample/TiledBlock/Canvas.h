// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Canvas.h
* @author       Clement Berthaud
* @brief        TiledBlock application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>
#include <QWidget>

class QImage;
class QLabel;
class QPixmap;
class QTimer;

using namespace ::ul3;

class SCanvas : public QWidget
{
    Q_OBJECT

public:
    ~SCanvas();
    SCanvas();

protected:
    virtual void mouseMoveEvent( QMouseEvent* event ) override;
    virtual void keyPressEvent( QKeyEvent* event ) override;

private:
    Q_SLOT void tickEvent();

private:
    FHardwareMetrics                     mHost;
    FThreadPool*                        mPool;
    FBlock*                             mCanvas;
    FBlock*                             mRAMUSAGEBLOCK1;
    FBlock*                             mRAMUSAGEBLOCK2;
    FBlock*                             mRAMUSAGESWAPBUFFER;
    FTextEngine                         mTextEngine;
    FFontRegistry                       mFontReg;
    FFont                               mFont;

    ITilePool*                          mTilePool;
    ITiledBlock*                        mTiledBlock;

    QImage*                     mImage;
    QPixmap*                    mPixmap;
    QLabel*                     mLabel;
    QTimer*                     mTimer;
};

