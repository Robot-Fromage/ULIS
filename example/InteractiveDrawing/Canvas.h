// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Canvas.h
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <QWidget>
#include <QPoint>
#include <ULIS>
#include "ToolBar.h"
namespace ULIS { class FBlock; }
class QImage;
class QLabel;
class QPixmap;
class QTimer;
class FULISLoader;
using namespace ::ULIS;

class SCanvas
    : public QWidget
{
    Q_OBJECT

public:
    ~SCanvas();
    SCanvas( QWidget* iParent, FULISLoader& iHandle );

public slots:
    void SetFormat( int iIndex );
    void SetRaster( eRasterOp iRasterOp, eRasterMode iRasterMode );
    void SetBlend( int iIndex );
    void SetAlpha( int iIndex );
    void SetFilled( int iState );

protected:
    virtual void resizeEvent( QResizeEvent* event ) override;
    virtual void mousePressEvent( QMouseEvent* event ) override;
    virtual void mouseMoveEvent( QMouseEvent* event ) override;
    virtual void mouseReleaseEvent( QMouseEvent* event ) override;
    virtual void keyPressEvent( QKeyEvent* event ) override;

private:
    Q_SLOT void tickEvent();
    void ReallocInternalBuffers( const QSize& iNewSize );
    void Update();
    void Refresh();
    void Commit();

private:
    FULISLoader& mHandle;
    FBlock* mTemp;
    FBlock* mLive;
    FBlock* mCanvas;
    FBlock* mProxy;
    eRasterOp mRasterOp;
    eRasterMode mRasterMode;
    eBlendMode mBlendMode;
    eAlphaMode mAlphaMode;
    bool mFilled;
    QImage* mImage;
    QPixmap* mPixmap;
    QLabel* mLabel;
    QTimer* mTimer;
    QVector< QPoint > mPoints;
    int mPtIndex;
    int mOpMaxPoints;
};

