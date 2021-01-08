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
ULIS_API void DrawDotNoAA( FBlock* iDst, const FColor& iColor, const FVec2I iPos );
ULIS_API void DrawHorizontalLineNoAA(  FBlock* iDst, const FColor& iColor, int iX1, int iX2, int iY );
ULIS_API void DrawVerticalLineNoAA(    FBlock* iDst, const FColor& iColor, int iY1, int iY2, int iX );
ULIS_API void DrawRectOutlineNoAA( FBlock* iDst, const FColor& iColor, const FRectI& iRect );
ULIS_API void DrawUniformGridOutlineNoAA( FBlock* iDst, const FColor& iColor, const FRectI& iRect, int iNumSubdiv );
/*
ULIS_API void DrawDot();
ULIS_API void DrawDotNoAA();
ULIS_API void DrawDotAA();
ULIS_API void DrawRect();
ULIS_API void DrawRectNoAA();
ULIS_API void DrawRectAA();
ULIS_API void DrawLine();
ULIS_API void DrawLineNoAA();
ULIS_API void DrawLineAA();
ULIS_API void DrawPolyLine();
ULIS_API void DrawPolyLineNoAA();
ULIS_API void DrawPolyLineAA();
ULIS_API void DrawPolygon();
ULIS_API void DrawPolygonNoAA();
ULIS_API void DrawPolygonAA();
ULIS_API void DrawCircleAndres();
ULIS_API void DrawCircleAndresNoAA();
ULIS_API void DrawCircleAndresAA();
ULIS_API void DrawCircleBresenham();
ULIS_API void DrawCircleBresenhamNoAA();
ULIS_API void DrawCircleBresenhamAA();
ULIS_API void DrawCircleParametric();
ULIS_API void DrawCircleParametricNoAA();
ULIS_API void DrawCircleParametricAA();
ULIS_API void DrawDiskAndres();
ULIS_API void DrawDiskAndresNoAA();
ULIS_API void DrawDiskAndresAA();
ULIS_API void DrawDiskBresenham();
ULIS_API void DrawDiskBresenhamNoAA();
ULIS_API void DrawDiskBresenhamAA();
ULIS_API void DrawDiskParametric();
ULIS_API void DrawDiskParametricNoAA();
ULIS_API void DrawDiskParametricAA();
ULIS_API void DrawEllipseRational();
ULIS_API void DrawEllipseRationalNoAA();
ULIS_API void DrawEllipseRationalAA();
ULIS_API void DrawEllipseParametric();
ULIS_API void DrawEllipseParametricNoAA();
ULIS_API void DrawEllipseParametricAA();
ULIS_API void DrawFilledEllipseRational();
ULIS_API void DrawFilledEllipseRationalNoAA();
ULIS_API void DrawFilledEllipseRationalAA();
ULIS_API void DrawFilledEllipseParametric();
ULIS_API void DrawFilledEllipseParametricNoAA();
ULIS_API void DrawFilledEllipseParametricAA();
ULIS_API void DrawBezierQuadraticParametric();
ULIS_API void DrawBezierQuadraticParametricNoAA();
ULIS_API void DrawBezierQuadraticParametricAA();
ULIS_API void DrawBezierQuadraticRational();
ULIS_API void DrawBezierQuadraticRationalNoAA();
ULIS_API void DrawBezierQuadraticRationalAA();
ULIS_API void DrawBezierCubicRational();
ULIS_API void DrawBezierCubicRationalNoAA();
ULIS_API void DrawBezierCubicRationalAA();
ULIS_API void DrawTriangleBarycentric();
ULIS_API void DrawTriangleBarycentricNoAA();
ULIS_API void DrawTriangleBarycentricAA();
ULIS_API void DrawTrianglePoly();
ULIS_API void DrawTrianglePolyNoAA();
ULIS_API void DrawTrianglePolyAA();
*/
ULIS_NAMESPACE_END

