// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Canvas.h
* @author       Clement Berthaud
* @brief        TiledBlock application for ULIS2.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS2>
#include <QWidget>

class QImage;
class QLabel;
class QPixmap;
class QTimer;

using namespace ::ul2;

class SCanvas : public QWidget
{
    Q_OBJECT

public:
    ~SCanvas();
    SCanvas();

protected:
    virtual void mousePressEvent( QMouseEvent* event ) override;
    virtual void mouseMoveEvent( QMouseEvent* event ) override;
    virtual void mouseReleaseEvent( QMouseEvent* event ) override;
    virtual void keyPressEvent( QKeyEvent* event ) override;

private:
    Q_SLOT void tickEvent();

private:
    FHostDeviceInfo                     mHost;
    FThreadPool                         mPool;
    FBlock*                             mCanvas;
    FBlock*                             mRAMUSAGEBLOCK1;
    FBlock*                             mRAMUSAGEBLOCK2;
    FBlock*                             mRAMUSAGESWAPBUFFER;
    FFontEngine                         mFontEngine;
    FFontRegistry                       mFontReg;
    FFont                               mFont;

    TTilePool< MICRO_16, MACRO_16 >*    mTilePool;
    ITiledBlock*                        mTiledBlock;

    QImage*                     mImage;
    QPixmap*                    mPixmap;
    QLabel*                     mLabel;
    QTimer*                     mTimer;
};

