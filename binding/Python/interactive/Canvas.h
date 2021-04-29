// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Canvas.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "ULISLoader.h"
#include <ULIS>

#include <pybind11/embed.h>
namespace py = pybind11;
using namespace py::literals;

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
    QImage* mImage;
    QPixmap* mPixmap;
    QLabel* mLabel;
    QTimer* mTimer;
};

