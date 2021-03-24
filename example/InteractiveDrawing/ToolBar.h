// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ToolBar.h
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <QWidget>
namespace ULIS { class FBlock; }
class QPushButton;
class QComboBox;
class QLabel;
class QCheckBox;
class FULISLoader;
using namespace ::ULIS;

enum eRasterOp {
      kLine
    , kCircleAndres
    , kCircleBresenham
    , kArcAndres
    , kArcBresenham
    , kEllipse
    , kRotatedEllipse
    , kRectangle
    , kPolygon
    , kQuadraticBezier
    , kNumRasterOP
};

static const char* kwRasterOP[] = {
      "Line"
    , "CircleAndres"
    , "CircleBresenham"
    , "ArcAndres"
    , "ArcBresenham"
    , "Ellipse"
    , "RotatedEllipse"
    , "Rectangle"
    , "Polygon"
    , "QuadraticBezier"
    , "Invalid"
};

enum eRasterMode {
      kNone
    , kAA
    , kSP
    , kNumRasterMode
};

static const char* kwRasterMode[] = {
      ""
    , "AA"
    , "SP"
    , "Invalid"
};

class SToolBar
    : public QWidget
{
    Q_OBJECT

public:
    ~SToolBar();
    SToolBar( QWidget* iParent, FULISLoader& iHandle );

public slots:
    void SetRaster( eRasterOp iRasterOp, eRasterMode iRasterMode );

signals:
    void RasterChanged( eRasterOp iRasterOp, eRasterMode iRasterMode );
    void BlendChanged( int iIndex );
    void AlphaChanged( int iIndex );
    void FilledChanged( int iState );

private:
    void BuildButton( QPushButton* ioButton, eRasterOp iRasterOp, eRasterMode iRasterMode, const FBlock& iAA, const FBlock& iSP );

private:
    FULISLoader& mHandle;
    QVector< QPushButton* > mButtons;
    QComboBox* mBlendModes;
    QComboBox* mAlphaModes;
    QCheckBox* mFilled;
};

