// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Draw.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the raster draw entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Maths/Geometry.h"

ULIS2_NAMESPACE_BEGIN
ULIS2_API void DrawDotNoAA( FBlock* iDst, const FPixelValue& iColor, const FVec2I iPos );
ULIS2_API void DrawHorizontalLineNoAA(  FBlock* iDst, const FPixelValue& iColor, int iX1, int iX2, int iY );
ULIS2_API void DrawVerticalLineNoAA(    FBlock* iDst, const FPixelValue& iColor, int iY1, int iY2, int iX );
ULIS2_API void DrawRectOutlineNoAA( FBlock* iDst, const FPixelValue& iColor, const FRect& iRect );
/*
ULIS2_API void DrawDot();
ULIS2_API void DrawDotNoAA();
ULIS2_API void DrawDotAA();
ULIS2_API void DrawRect();
ULIS2_API void DrawRectNoAA();
ULIS2_API void DrawRectAA();
ULIS2_API void DrawLine();
ULIS2_API void DrawLineNoAA();
ULIS2_API void DrawLineAA();
ULIS2_API void DrawPolyLine();
ULIS2_API void DrawPolyLineNoAA();
ULIS2_API void DrawPolyLineAA();
ULIS2_API void DrawPolygon();
ULIS2_API void DrawPolygonNoAA();
ULIS2_API void DrawPolygonAA();
ULIS2_API void DrawCircleAndres();
ULIS2_API void DrawCircleAndresNoAA();
ULIS2_API void DrawCircleAndresAA();
ULIS2_API void DrawCircleBresenham();
ULIS2_API void DrawCircleBresenhamNoAA();
ULIS2_API void DrawCircleBresenhamAA();
ULIS2_API void DrawCircleParametric();
ULIS2_API void DrawCircleParametricNoAA();
ULIS2_API void DrawCircleParametricAA();
ULIS2_API void DrawDiskAndres();
ULIS2_API void DrawDiskAndresNoAA();
ULIS2_API void DrawDiskAndresAA();
ULIS2_API void DrawDiskBresenham();
ULIS2_API void DrawDiskBresenhamNoAA();
ULIS2_API void DrawDiskBresenhamAA();
ULIS2_API void DrawDiskParametric();
ULIS2_API void DrawDiskParametricNoAA();
ULIS2_API void DrawDiskParametricAA();
ULIS2_API void DrawEllipseRational();
ULIS2_API void DrawEllipseRationalNoAA();
ULIS2_API void DrawEllipseRationalAA();
ULIS2_API void DrawEllipseParametric();
ULIS2_API void DrawEllipseParametricNoAA();
ULIS2_API void DrawEllipseParametricAA();
ULIS2_API void DrawFilledEllipseRational();
ULIS2_API void DrawFilledEllipseRationalNoAA();
ULIS2_API void DrawFilledEllipseRationalAA();
ULIS2_API void DrawFilledEllipseParametric();
ULIS2_API void DrawFilledEllipseParametricNoAA();
ULIS2_API void DrawFilledEllipseParametricAA();
ULIS2_API void DrawBezierQuadraticParametric();
ULIS2_API void DrawBezierQuadraticParametricNoAA();
ULIS2_API void DrawBezierQuadraticParametricAA();
ULIS2_API void DrawBezierQuadraticRational();
ULIS2_API void DrawBezierQuadraticRationalNoAA();
ULIS2_API void DrawBezierQuadraticRationalAA();
ULIS2_API void DrawBezierCubicRational();
ULIS2_API void DrawBezierCubicRationalNoAA();
ULIS2_API void DrawBezierCubicRationalAA();
ULIS2_API void DrawTriangleBarycentric();
ULIS2_API void DrawTriangleBarycentricNoAA();
ULIS2_API void DrawTriangleBarycentricAA();
ULIS2_API void DrawTrianglePoly();
ULIS2_API void DrawTrianglePolyNoAA();
ULIS2_API void DrawTrianglePolyAA();
*/
ULIS2_NAMESPACE_END

