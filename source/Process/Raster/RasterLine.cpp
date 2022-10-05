// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterLine.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementation of non template Line functions for the raster API
* @license      Please refer to LICENSE.md
*/
#include "Process/Raster/RasterLine.h"

ULIS_NAMESPACE_BEGIN

void DrawLine(
      FBlock& iBlock
    , const FVec2I& iP0
    , const FVec2I& iP1
    , const FColor& iColor
    , const FRectI& iClippingRect)
{
    //Clipping ----

    FVec2I p0 = iP0;
    FVec2I p1 = iP1;

    FRectI clippingRect = iClippingRect;
    clippingRect.w--;
    clippingRect.h--;

    if (clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0, 0, iBlock.Width() - 1, iBlock.Height() - 1);
    }

    if (!InternalCropLineToRectangle(p0, p1, clippingRect))
        return; //Nothing to draw

    //Drawing ----
    FColor val = iColor;

    if (::ULIS::FMath::Abs(p1.y - p0.y) < ::ULIS::FMath::Abs(p1.x - p0.x)) // x slope > y slope
    {
        if (p1.x < p0.x)
        {
            FVec2I temp = p0;
            p0 = p1;
            p1 = temp;
        }

        int dx = p1.x - p0.x;
        int dy = p1.y - p0.y;
        int yStep = 1;

        if (dy < 0)
        {
            yStep = -1;
            dy = -dy;
        }

        int slopeDifferential = 2 * dy - dx;
        int y = p0.y;

        for (int x = p0.x; x <= p1.x; x++)
        {
            iBlock.SetPixel(x, y, val);

            if (slopeDifferential > 0)
            {
                y += yStep;
                slopeDifferential -= (2 * dx);
            }
            slopeDifferential += (2 * dy);
        }
    }
    else //y slope > x slope
    {
        if (p1.y < p0.y)
        {
            FVec2I temp = p0;
            p0 = p1;
            p1 = temp;
        }

        int dx = p1.x - p0.x;
        int dy = p1.y - p0.y;
        int xStep = 1;

        if (dx < 0)
        {
            xStep = -1;
            dx = -dx;
        }

        int slopeDifferential = 2 * dx - dy;
        int x = p0.x;

        for (int y = p0.y; y <= p1.y; y++)
        {
            iBlock.SetPixel(x, y, val);

            if (slopeDifferential > 0)
            {
                x += xStep;
                slopeDifferential -= (2 * dy);
            }
            slopeDifferential += (2 * dx);
        }
    }
}

ULIS_NAMESPACE_END

