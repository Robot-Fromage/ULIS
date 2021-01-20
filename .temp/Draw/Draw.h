// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Draw.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the raster draw entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
void DrawDot( FBlock* iDst, const FColor& iColor, const FVec2I iPos );
void DrawHorizontalLine(  FBlock* iDst, const FColor& iColor, int iX1, int iX2, int iY );
void DrawVerticalLine(    FBlock* iDst, const FColor& iColor, int iY1, int iY2, int iX );
void DrawRectOutline( FBlock* iDst, const FColor& iColor, const FRectI& iRect );
void DrawUniformGridOutline( FBlock* iDst, const FColor& iColor, const FRectI& iRect, int iNumSubdiv );
void DrawDot();
void DrawDotAA();
void DrawRect();
void DrawRectAA();
void DrawLine();
void DrawLineAA();
void DrawPolyLine();
void DrawPolyLineAA();
void DrawPolygon();
void DrawPolygonAA();
void DrawCircleAndres();
void DrawCircleAndresAA();
void DrawCircleBresenham();
void DrawCircleBresenhamAA();
void DrawCircleParametric();
void DrawCircleParametricAA();
void DrawDiskAndres();
void DrawDiskAndresAA();
void DrawDiskBresenham();
void DrawDiskBresenhamAA();
void DrawDiskParametric();
void DrawDiskParametricAA();
void DrawEllipseRational();
void DrawEllipseRationalAA();
void DrawEllipseParametric();
void DrawEllipseParametricAA();
void DrawFilledEllipseRational();
void DrawFilledEllipseRationalAA();
void DrawFilledEllipseParametric();
void DrawFilledEllipseParametricAA();
void DrawBezierQuadraticParametric();
void DrawBezierQuadraticParametricAA();
void DrawBezierQuadraticRational();
void DrawBezierQuadraticRationalAA();
void DrawBezierCubicRational();
void DrawBezierCubicRationalAA();
void DrawTriangleBarycentric();
void DrawTriangleBarycentricAA();
void DrawTrianglePoly();
void DrawTrianglePolyAA();
*/
ULIS_NAMESPACE_END

