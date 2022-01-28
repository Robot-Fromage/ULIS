// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Canvas.h
* @author       Clement Berthaud
* @brief        TiledBlock application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "ULISLoader.h"
#include <ULIS>
#include <QWidget>

class QImage;
class QLabel;
class QPixmap;
class QTimer;

using namespace ::ULIS;

class SCanvas : public QWidget
{
    Q_OBJECT

public:
    ~SCanvas();
    SCanvas( FULISLoader& iHandle );

protected:
    virtual void mouseMoveEvent( QMouseEvent* event ) override;
    virtual void keyPressEvent( QKeyEvent* event ) override;

private:
    Q_SLOT void tickEvent();

private:
    FULISLoader& mHandle;
    FBlock mCanvas;
    FFont mFont;
    FBlock* mRAMUSAGEBLOCK1;
    FBlock* mRAMUSAGEBLOCK2;
    FBlock* mRAMUSAGESWAPBUFFER;

    // 1, 5
    TTilePool< Micro_64, Macro_1024 >* mTilePool;
    TTiledBlock< Micro_64, Macro_1024 >* mTiledBlock;

    QImage* mImage;
    QPixmap* mPixmap;
    QLabel* mLabel;
    QTimer* mTimer;
};

