// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Editor.h
* @author       Clement Berthaud
* @brief        AnimatedLayerStackBasicDisplay application for ULIS.
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
#include "VReader.h"

class SEditor
    : public QWidget
{
    Q_OBJECT

public:
    ~SEditor();
    SEditor( FContext& iCtx, FAnimatedLayerStack& iDocument );

private:
    Q_SLOT void tickEvent();

private:
    FContext& mCtx;
    FAnimatedLayerStack& mDocument;
    FBlock* mView;

    ufloat mElapsed;
    QImage* mImage;
    QPixmap* mPixmap;
    QLabel* mLabel;
    QTimer* mTimer;

    FVideoReaderState mVideoReader;
    uint8_t* mFrameData;
};

