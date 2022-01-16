// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterRotatedEllipse.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementation of non template Rotated Ellipse functions for the raster API
* @license      Please refer to LICENSE.md
*/

#include "Process/Raster/RasterRotatedEllipse.h"

ULIS_NAMESPACE_BEGIN

void DrawRotatedEllipse(        FBlock&                  iBlock
                              , const FVec2I&            iCenter
                              , const int                iA
                              , const int                iB
                              , const int                iRotationDegrees
                              , const FColor&            iColor
                              , const bool               iFilled
                              , const FRectI&            iClippingRect )
{
    if (iA <= 0 || iB <= 0)
        return;

    if(iRotationDegrees % 180 == 0)
    {
        DrawEllipse(iBlock,iCenter,iA,iB,iColor,iFilled,iClippingRect); //Don't bother to use the rotated ellipse algorithm if the ellipse is not rotated
        return;
    }
    if(iRotationDegrees % 90 == 0)
    {
        DrawEllipse(iBlock,iCenter,iB,iA,iColor,iFilled,iClippingRect); //Don't bother to use the rotated ellipse algorithm if the ellipse is not rotated
        return;
    }

    //               x  y
    std::map< int,std::vector< int > > storagePoints; // storagePoints[x][0]  We have two points for each x on the ellipse: p1(x, y0), p2(x, y1)
                                                      //                 [1]
    storagePoints[0].push_back(iCenter.x);           // In order to only pass storagePoints in parameter to InternalDrawQuadRationalBezierSeg
    storagePoints[0].push_back(iCenter.y);           // we store the center (useful in this function) at index 0 (we'll know it's there)
        
    int a = iA;
    int b = iB;

    float dx = (float)iA*iA;
    float dy = (float)iB*iB;
    float s = std::sin(FMath::DegToRadF(float(iRotationDegrees)));
    float dz = (dx - dy) * s;
    dx = std::sqrt(dx - dz * s);
    dy = std::sqrt(dy + dz * s);
    a = int(dx + 0.5f);
    b = int(dy + 0.5f);
    dz = dz * a * b / (dx * dy);

    int x0 = iCenter.x - a;
    int y0 = iCenter.y - b;
    int x1 = iCenter.x + a;
    int y1 = iCenter.y + b;
    dz = (4.f * dz * std::cos(FMath::DegToRadF(float(iRotationDegrees))));


    dx = float(x1 - x0);
    dy = float(y1 - y0);
    float w = dx * dy;
    if(w != 0.0)
        w = (w - dz) / (w + w);

    if(w > 1 || w < 0)
        return;

    dx = std::floor(dx * w + 0.5f);
    dy = std::floor(dy * w + 0.5f);

    InternalDrawQuadRationalBezierSeg(iBlock,x0,int(y0 + dy),x0,y0,int(x0 + dx),y0,1.f - w,iColor,iClippingRect,&storagePoints);
    InternalDrawQuadRationalBezierSeg(iBlock,x0,int(y0 + dy),x0,y1,int(x1 - dx),y1,w,iColor,iClippingRect,&storagePoints);
    InternalDrawQuadRationalBezierSeg(iBlock,x1,int(y1 - dy),x1,y1,int(x1 - dx),y1,1.f - w,iColor,iClippingRect,&storagePoints);
    InternalDrawQuadRationalBezierSeg(iBlock,x1,int(y1 - dy),x1,y0,int(x0 + dx),y0,w,iColor,iClippingRect,&storagePoints);

    if(iFilled) //We fill the ellipse by drawing vertical lines
    {
        //We delete the values we stored for the center position (two times pop_front)
        storagePoints[0].erase(storagePoints[0].begin());
        storagePoints[0].erase(storagePoints[0].begin());

        for(std::map< int,std::vector< int > >::iterator it=storagePoints.begin(); it!=storagePoints.end(); ++it)
        {
            if(it->second.size() == 2)
            {
                DrawLine(iBlock,FVec2I(iCenter.x + it->first,iCenter.y + it->second[0]),FVec2I(iCenter.x + it->first,iCenter.y + it->second[1]),iColor,iClippingRect);
            }
            else if(it->second.size() > 2) // where we draw more than 2 pixels for a said y
            {
                int minY = it->second[0];
                int maxY = it->second[0];
                for(int i = 1; i < it->second.size(); i++)
                {
                    if(minY > it->second[i])
                        minY = it->second[i];

                    if(maxY < it->second[i])
                        maxY = it->second[i];
                }
                DrawLine(iBlock,FVec2I(iCenter.x + it->first,iCenter.y + minY),FVec2I(iCenter.x + it->first,iCenter.y + maxY),iColor,iClippingRect);
            }
        }
    }
}

ULIS_NAMESPACE_END

