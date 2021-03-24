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
#include <ULIS>
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

enum eDocumentFormat {
      kRGBA8
    , kRGBA16
    , kRGBA32
    , kRGBAF
    , kABGR8
    , kBGRA16
    , kHSVAF
    , kCMYKA16
    , kLabA32
    , kLabAF
    , kALab8
    , kNumDocumentFormat
};

static const char* kwDocumentFormat[] = {
      "RGBA8"
    , "RGBA16"
    , "RGBA32"
    , "RGBAF"
    , "ABGR8"
    , "BGRA16"
    , "HSVAF"
    , "CMYKA16"
    , "LabA32"
    , "LabAF"
    , "ALab8"
    , "Invalid"
};

static const eFormat eDocumentFormaMatchingTable[] = {
      Format_RGBA8
    , Format_RGBA16
    , Format_RGBA32
    , Format_RGBAF
    , Format_ABGR8
    , Format_BGRA16
    , Format_HSVAF
    , Format_CMYKA16
    , Format_LabA32
    , Format_LabAF
    , Format_ALab8
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
    void FormatChanged( int iIndex );
    void RasterChanged( eRasterOp iRasterOp, eRasterMode iRasterMode );
    void BlendChanged( int iIndex );
    void AlphaChanged( int iIndex );
    void FilledChanged( int iState );

private:
    void BuildButton( QPushButton* ioButton, eRasterOp iRasterOp, eRasterMode iRasterMode, const FBlock& iAA, const FBlock& iSP );

private:
    FULISLoader& mHandle;
    QVector< QPushButton* > mButtons;
    QComboBox* mFormats;
    QComboBox* mBlendModes;
    QComboBox* mAlphaModes;
    QCheckBox* mFilled;
};

