// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterPolygon.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementation of non template Polygon functions for the raster API
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/

#include "Process/Raster/RasterPolygon.h"
#include "Process/Raster/RasterLine.h"

ULIS_NAMESPACE_BEGIN

void DrawPolygon(        FBlock&                      iBlock
                       , const std::vector< FVec2I >& iPoints
                       , const FColor&                iColor
                       , const bool                   iFilled
                       , const FRectI&                iClippingRect )
{
    if(iPoints.size() < 3)
        return;

    int j = int(iPoints.size() - 1);
    for(int i = 0; i < iPoints.size(); i++)
    {
        DrawLine(iBlock,iPoints.at(i),iPoints.at(j),iColor,iClippingRect);
        j = i;
    }


    if(iFilled)
    {
        int maxX = 0;
        int maxY = 0;
        int minX = INT_MAX;
        int minY = INT_MAX;

        //Initialization of useful variables
        for(int i = 0; i < iPoints.size(); i++)
        {
            if(maxX < iPoints[i].x)
                maxX = iPoints[i].x;
            if(maxY < iPoints[i].y)
                maxY = iPoints[i].y;
            if(minX > iPoints[i].x)
                minX = iPoints[i].x;
            if(minY > iPoints[i].y)
                minY = iPoints[i].y;
        }

        //We go through the polygon by scanning it top to bottom
        for(int y = minY; y <= maxY; y++)
        {
            std::vector< int > nodesX;
            int j = int(iPoints.size() - 1);

            for(int i = 0; i < iPoints.size(); i++)
            {
                if((iPoints[i].y < y && iPoints[j].y >= y) || (iPoints[j].y < y && iPoints[i].y >= y))
                {
                    nodesX.push_back(int(iPoints[i].x  + double(y - iPoints[i].y) / double(iPoints[j].y - iPoints[i].y) * (iPoints[j].x - iPoints[i].x)));
                }
                j = i;
            }

            //Sorting the nodes on X
            int i = 0;
            int size = int(nodesX.size() - 1);
            while(i < size)
            {
                if(nodesX[i] > nodesX[i+1])
                {
                    int temp = nodesX[i];
                    nodesX[i]=nodesX[i+1];
                    nodesX[i+1] = temp;
                    if(i > 0)
                        i--;
                } else
                {
                    i++;
                }
            }

            //Filling the polygon on line y
            for(i = 0; i < nodesX.size(); i+= 2)
            {
                if(nodesX[i] > maxX) break;
                if(nodesX[i+1] > minX)
                {
                    if(nodesX[i] < minX)
                        nodesX[i] = minX;
                    if(nodesX[i+1] > maxX)
                        nodesX[i+1] = maxX;

                    DrawLine(iBlock,FVec2I(nodesX[i],y),FVec2I(nodesX[i+1],y),iColor,iClippingRect);
                }
            }
        }
    }
}

ULIS_NAMESPACE_END

