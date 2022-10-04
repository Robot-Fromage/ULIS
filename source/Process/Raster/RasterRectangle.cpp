// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterRectangle.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementation of non template Rectangle functions for the raster API
* @license      Please refer to LICENSE.md
*/

#include "Process/Raster/RasterRectangle.h"
#include "Process/Raster/RasterLine.h"

ULIS_NAMESPACE_BEGIN

void DrawRectangle(        FBlock&                        iBlock
                         , const FVec2I&                  iTopLeft
                         , const FVec2I&                  iBottomRight
                         , const FColor&                  iColor
                         //, const bool                     iFilled // Done in invocation
                         , const FRectI&                  iClippingRect )
{
    FRectI clippingRect = iClippingRect;
    clippingRect.w--;
    clippingRect.h--;

    if(clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0,0,iBlock.Width() - 1,iBlock.Height() - 1);
    }

    const int xmin = FMath::Min3(iTopLeft.x,iBottomRight.x,clippingRect.x + clippingRect.w);
    const int ymin = FMath::Min3(iTopLeft.y,iBottomRight.y,clippingRect.y + clippingRect.h);
    const int xmax = FMath::Max3(iTopLeft.x,iBottomRight.x,clippingRect.x);
    const int ymax = FMath::Max3(iTopLeft.y,iBottomRight.y,clippingRect.y);
    //const int width = FMath::Max(xmax - xmin,0);
    //const int height = FMath::Max(ymax - ymin,0);
    /*if(iFilled)
    {
        const FRectI rect = FRectI(xmin,ymin,width,height);

    } else
    {*/
        DrawLine(iBlock,FVec2I(xmin,ymin),FVec2I(xmax,ymin),iColor,iClippingRect); // Top
        DrawLine(iBlock,FVec2I(xmax,ymin),FVec2I(xmax,ymax),iColor,iClippingRect); // Right
        DrawLine(iBlock,FVec2I(xmin,ymax),FVec2I(xmax,ymax),iColor,iClippingRect); // Bot
        DrawLine(iBlock,FVec2I(xmin,ymin),FVec2I(xmin,ymax),iColor,iClippingRect); // Left
    //}
}

ULIS_NAMESPACE_END

