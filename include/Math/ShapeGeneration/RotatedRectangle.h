// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Rectangle.h
* @author       Thomas Schmitt
* @brief        This file provides the Rectangle points generation methods
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN

static inline void GenerateCircleInscribedRectanglePoints(
    const FVec2I& iTopLeft
    , const FVec2I& iBottomRight
    , const int iRotationDegrees
    , TArray<FVec2I>& ioRectanglePoints )
{
    if( iRotationDegrees % 180 == 0 )
    {
        GenerateRectanglePoints( iTopLeft, iBottomRight, ioRectanglePoints );
        return;
    }

    FVec2I center = iTopLeft + (iBottomRight - iTopLeft) / 2;
    FVec2I delta = center - iTopLeft;
    int angle = -FMath::RadToDeg( atan2( delta.y, delta.x ) );
    int radius = FMath::Dist( iTopLeft.x, iTopLeft.y, iBottomRight.x, iBottomRight.y ) / 2;

    FVec2I topRight = center + FVec2I(radius * cos(FMath::DegToRadF(float(angle + iRotationDegrees))), radius * sin(FMath::DegToRadF(float(angle + iRotationDegrees))));
    FVec2I bottomLeft = center + FVec2I(radius * cos(FMath::DegToRadF(float(angle + iRotationDegrees + 180))), radius * sin(FMath::DegToRadF(float(angle + iRotationDegrees + 180))));

    GenerateLinePoints( iTopLeft, topRight, ioRectanglePoints ); // Top
    GenerateLinePoints( topRight, iBottomRight, ioRectanglePoints); // Right
    GenerateLinePoints( iBottomRight, bottomLeft, ioRectanglePoints); // Bot
    GenerateLinePoints( bottomLeft, iTopLeft, ioRectanglePoints); // Left
}

ULIS_NAMESPACE_END

