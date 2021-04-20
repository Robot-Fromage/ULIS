// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterPolygon.h
* @author       Thomas Schmitt
* @brief        This file provides the actual functions for the raster of Polygon
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Image/Block.h"
#include "Process/Raster/RasterLine.h"

ULIS_NAMESPACE_BEGIN

void DrawPolygon(        FBlock&                      iBlock
                       , const std::vector< FVec2I >& iPoints
                       , const FColor&                iColor
                       , const bool                   iFilled
                       , const FRectI&                iClippingRect );

template< typename T >
void DrawPolygonAA( FBlock&                      iBlock
                         , const std::vector< FVec2I >& iPoints
                         , const FColor&                iColor
                         , const bool                   iFilled
                         , const FRectI&                iClippingRect )
{
    if(iPoints.size() < 3)
        return;

    int j = int(iPoints.size() - 1);
    for(int i = 0; i < iPoints.size(); i++)  //TODO ? Drawing can be cleaner by blending the lines together instead of drawing them one by one
    {
        DrawLineAA<T>(iBlock,iPoints.at(i),iPoints.at(j),iColor,iClippingRect);
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
        minX++;
        maxX--;
        minY++;
        maxY--;

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

                    DrawLine(iBlock,FVec2I(nodesX[i] + 1,y),FVec2I(nodesX[i+1],y),iColor,iClippingRect);
                }
            }
        }
    }
}

template< typename T >
void DrawPolygonSP( FBlock&                      iBlock
                         , const std::vector< FVec2F >& iPoints
                         , const FColor&                iColor
                         , const bool                   iFilled
                         , const FRectI&                iClippingRect )
{
    if(iPoints.size() < 3)
        return;

    int j = int(iPoints.size() - 1);
    for(int i = 0; i < iPoints.size(); i++)  //TODO ? Drawing can be cleaner by blending the lines together instead of drawing them one by one
    {
        DrawLineSP<T>(iBlock,iPoints.at(i),iPoints.at(j),iColor,iClippingRect);
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
                maxX = int(iPoints[i].x);
            if(maxY < iPoints[i].y)
                maxY = int(iPoints[i].y);
            if(minX > iPoints[i].x)
                minX = int(iPoints[i].x);
            if(minY > iPoints[i].y)
                minY = int(iPoints[i].y);
        }
        minX++;
        maxX--;
        minY++;
        maxY--;

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

                    DrawLine(iBlock,FVec2I(nodesX[i] + 1,y),FVec2I(nodesX[i+1],y),iColor,iClippingRect);
                }
            }
        }
    }
}

ULIS_NAMESPACE_END

